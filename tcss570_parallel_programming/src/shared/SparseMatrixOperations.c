#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include "Matrix.h"
#include "error.h"

int SparseMatrix_mult(SparseMatrix *a, SparseMatrix *b, SparseMatrix *c) {
	CHECK_ERROR_RETURN(a->nr_cols != b->nr_rows || a->nr_rows != c->nr_rows || b->nr_cols != c->nr_cols, "Invalid dimensions for matrix multiply", 1);

	int res;
	int max_nr_elems = a->nr_elems > b->nr_elems ? a->nr_elems : b->nr_elems;

	CHECK_ERROR_RETURN(c->size < max_nr_elems, "Matrix C does not have a sufficient numer of elements", 1);

	// Zero elements in C
	c->nr_elems = 0;

	bool is_zero;
	int k;
	for (int i = 0; i < a->nr_elems; i++) {
		for (int j = 0; j < b->nr_elems; j++) {

			if (a->col[i] == b->row[j]) {
				is_zero = true;
				for (k = 0; k < c->nr_elems; c++) {
					if (c->row[k] == a->row[i] && c->col[k] == b->col[j]) {
						c->data[k] += a->data[i] * b->data[i];
						is_zero = false;
						break;
					}
				}
				if (is_zero) {
					c->row[k] = a->row[i];
					c->col[k] = b->col[j];
					c->data[k] = a->data[i] * b->data[i];
					c->nr_elems++;
				}
			}

		}
	}

	return 0;
}