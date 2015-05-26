#include <minix/blockdriver.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
// Useful constants and global variables.
//-----------------------------------------------------------------------------

// Ramdisk size, 2 MB.
#define RAMDISK_SIZE 0x200000
// The encryption/decryption key.
static const char KEY[] = {0x28, 0xC4, 0x59, 0x4B, 0x81, 0x05, 0xF2, 0x7C};
static const unsigned int key_size = 8;
// Holds address of the array backing the ramdisk.
static vir_bytes m_vaddrs;

//-----------------------------------------------------------------------------

/* Holds device information, e.g., ramdisk size. */
static struct device m_geom;
/* Tracks how many open requests are currently active for the device. */
static int open_counter;

static void sef_local_startup(void);
static int sef_cb_init_fresh(int type, sef_init_info_t *info);
static struct device *m_block_part(dev_t minor);
static int m_block_open(dev_t minor, int access);
static int m_block_close(dev_t minor);
static int m_block_transfer(dev_t minor, int do_write, u64_t position, endpoint_t endpt, iovec_t *iov, unsigned int nr_req, int flags);
static void en_decrpt(char ** data, unsigned count);

/* Holds the callback functions for this device driver. */
static struct blockdriver m_bdtab = {
	BLOCKDRIVER_TYPE_DISK, /* handle partition requests */
	m_block_open,          /* open or mount */
	m_block_close,         /* close */
	m_block_transfer,      /* do the I/O */
	NULL,                  /* no I/O control */
	NULL,                  /* no need to clean up */
	m_block_part,          /* return partition information */
	NULL,                  /* no geometry */
	NULL,                  /* no interrupt processing */
	NULL,                  /* no alarm processing */
	NULL,                  /* no processing of other messages */
	NULL                   /* no threading support */
};


int main(void) {
	sef_local_startup();
	blockdriver_task(&m_bdtab);
	return OK;
}

static void sef_local_startup() {
	sef_setcb_init_fresh(sef_cb_init_fresh);
	sef_setcb_init_lu(sef_cb_init_fresh);
	sef_setcb_init_restart(sef_cb_init_fresh);
	sef_setcb_lu_prepare(sef_cb_lu_prepare_always_ready);
	sef_setcb_lu_state_isvalid(sef_cb_lu_state_isvalid_standard);
	sef_startup();
}

static int sef_cb_init_fresh(int UNUSED(type), sef_init_info_t *UNUSED(info)) {
	m_geom.dv_base = cvul64(0);
	m_geom.dv_size = cvul64(RAMDISK_SIZE);
	m_vaddrs = (vir_bytes)calloc(RAMDISK_SIZE, 1);
	if (m_vaddrs == (vir_bytes)NULL)
		panic("ENCRD: Couldn't allocate memory for ramdisk.");
	open_counter = 0;
	return OK;
}

static struct device *m_block_part(dev_t UNUSED(minor)) {
	return &m_geom;
}

static int m_block_open(dev_t UNUSED(minor), int UNUSED(access)) {
	open_counter++;
	return OK;
}

static int m_block_close(dev_t UNUSED(minor)) {
	if(open_counter < 1) {
		printf("CRD: closing unopened device.\n");
		return(EINVAL);
	}
	open_counter--;
	return OK;
}

static void en_decrpt(
		char ** data,
		unsigned count
) {
	unsigned i;
	for (i = 0; i < count; i++)
		(*data)[i] ^= KEY[i % key_size];
}

static int m_block_transfer(
		dev_t UNUSED(minor), /* minor device number */
		int do_write,        /* read or write? */
		u64_t pos64,         /* offset on device to read or write */
		endpoint_t endpt,    /* process doing the request */
		iovec_t *iov,        /* pointer to read or write request vector */
		unsigned int nr_req, /* length of request vector */
		int UNUSED(flags)    /* transfer flags */
) {
	unsigned count;
	vir_bytes vir_offset = 0;
	int r;
	off_t position;
	ssize_t total = 0;

	if (ex64hi(pos64) != 0)
		return OK;	/* Beyond EOF */
	position = cv64ul(pos64);

	while (nr_req > 0) {
		count = iov->iov_size;

//-----------------------------------------------------------------------------
// At this point in the code, the meaningful data transfer parameters are held
// in two variables, "position" and "count".  Position holds the index into
// the ramdisk at which the transfer will begin.  Count holds the number of
// bytes to be transfered.
//-----------------------------------------------------------------------------

		// If the transfer position is invalid, stop the transfer here.
		if (position >= m_geom.dv_size)
			return total;
		// If the transfer would extend beyond the end of the ramdisk, lower
		// the number of bytes to transfer so that the transfer stops at the
		// end of the ramdisk.
		if (position + count > m_geom.dv_size)
			count = m_geom.dv_size - position;

		char * buffer = (char *)(m_vaddrs + position);
		if (!do_write) {
			// Reading from the ramdisk.
			// Transfer data from the ramdisk to another process.

			// Decrypts the data in the ram disk for process to copy
			en_decrpt(&buffer, count);
			r = sys_safecopyto(endpt, iov->iov_addr, vir_offset, m_vaddrs + position, count);

			// Encrypts the data that was previously decrypted
			en_decrpt(&buffer, count);
		} else {
			// Writing to the ramdisk.
			// Transfer data from another process to the ramdisk.
			r = sys_safecopyfrom(endpt, iov->iov_addr, vir_offset, m_vaddrs + position, count);

			// Encrypts the data in the ram disk
			en_decrpt(&buffer, count);
		}
//-----------------------------------------------------------------------------
		if (r != OK)
			panic("CRD: I/O copy failed: %d", r);

		/* Book the number of bytes transferred. */
		position += count;
		vir_offset += count;
		total += count;
		if ((iov->iov_size -= count) == 0) {
			iov++;
			nr_req--;
			vir_offset = 0;
		}
	}
	return total;
}
