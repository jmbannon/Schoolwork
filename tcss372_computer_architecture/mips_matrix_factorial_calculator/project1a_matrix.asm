#=======================================================#
# Author:	Jesse Bannon				#
# Class:	TCSS 372 B				#
# Desc:		Project #1a - Matrix Calculator		#
#=======================================================#

	.data
int_space: .space 16
intro: .asciiz 			"\n#=======================================================#\n#                   MATRIX CALCULATOR                   #\n#                     Jesse  Bannon                     #\n#                       TCSS  372                       #\n#=======================================================#\n"
intro_text: .asciiz 		"\nMatrix A * Matrix B = Matrix C"
prompt_MA_rows: .asciiz		"\nMatrix A rows (-1 to quit): "
prompt_MA_columns: .asciiz 	"Matrix A columns: "
prompt_MB_rows: .asciiz		"Matrix B rows: "
prompt_MB_columns: .asciiz 	"\nMatrix B columns: "

A: .asciiz 	"A"
B: .asciiz     	"B"
comma: .asciiz  ","
open: .asciiz 	"("
close: .asciiz	"): "
tab: .asciiz	"\t"
endl: .asciiz 	"\n"
times: .asciiz 	"*"
equals: .asciiz "="

	.text
	.globl main
main:
	# Add registers used to stack
	sub $sp, $sp, 28
	sw $ra, 0($sp)
	sw $s0, 4($sp)
	sw $s1, 8($sp)
	sw $s2, 12($sp)
	sw $s3, 16($sp)
	sw $s4, 20($sp)
	sw $s5, 24($sp)

	li $v0, 4	# System call output string
	la $a0, intro # Load string intro
	syscall		# Print string	

start:
	li $v0, 4	# System call output string
	la $a0, intro_text # Load string intro
	syscall		# Print string

matrix_initialize:
	# Matrix A Rows:
	li $v0, 4	
	la $a0, prompt_MA_rows
	syscall		# Print string
	
	li $v0, 5	
	la $a1, int_space
	syscall 	# Read Matrix A rows
	
	add $s0, $v0, $zero # Store Matrix A rows to $s0
	bltz $s0, end	# Branch to end if less than 0
	
	# Matrix A Columns:
	li $v0, 4	
	la $a0, prompt_MA_columns 
	syscall		# Print string
	
	li $v0, 5	
	la $a1, int_space 
	syscall 	# Read Matrix A columns
	
	add $s1, $v0, $zero # Store Matrix A columns into $s1
	
	# Matrix B Rows: (equals Matrix A Columns)
	li $v0, 4	
	la $a0, prompt_MB_rows 
	syscall		
	
	li $v0, 1
	la $a0, ($s1)
	syscall # Print that Matrix B rows = Matrix A columns
	
	# Matrix B Columns:
	li $v0, 4	
	la $a0, prompt_MB_columns 
	syscall		# Print string
	
	li $v0, 5	
	la $a1, int_space 
	syscall 	# Input Matrix B Columns
	
	add $s2, $v0, $zero # Store Matrix B Columns into $s2
	
	
	# Dynamically allocate array for Matrix A
	li $t0, 4 # Number of bits in an integer
	
	mul $a0, $s0, $s1 # MA_Rows * MA_Columns
	mul $a0, $a0, $t0 # MA_Size * sizeofInteger(bytes)
	
	li $v0, 9
	syscall # Allocate memory for MA
	
	move $s3, $v0 # MA array address in $s3
	
	
	# Dynamically allocate array for Matrix B
	mul $a0, $s1, $s2 # MA_Rows * MB_Columns
	mul $a0, $a0, $t0 # MB_Size * sizeofInteger(bytes)
	
	li $v0, 9
	syscall # Allocate memory for MB
	
	move $s4, $v0 # MB array address in $s4
	
	
	# Dynamically allocate array for Matrix C
	mul $a0, $s0, $s2 # MA_Rows * MB_Columns
	mul $a0, $a0, $t0 # MC_Size * sizeofInteger(bytes)
	
	li $v0, 9
	syscall # Allocate memory for MC
	
	move $s5, $v0 # MC array address in $s5
	
	
	sub $sp, $sp, 8
	
	la $t0, ($s3)
	sw $t0, 28($sp) # Stores memory location of Matrix A array in stack
	
	la $t0, ($s4)
	sw $t0, 32($sp) # Stores memory location of Matrix B array in stack
	
	
	# Values for inputting integers into matrixes
	add $t0, $zero, $zero   # if ($t0 = 0) input into MA; else input into MB
	addi $t1, $zero, 1	# Row Iterator
	addi $t2, $zero, 1	# Column Iterator

#-----------------------------------------------------------------------------		
	
matrix_input:
	
	# Prints Matrix 
	li $v0, 4
	beqz $t0, print_A
	
	la $a0, B
	syscall
	j continue_print
	
	print_A:
	la $a0, A
	syscall
	
	continue_print:
	
	li $v0, 4	
	la $a0, open
	syscall		# '('
	
	li $v0, 1
	la $a0, ($t1)
	syscall		# '<row>'
	
	li $v0, 4
	la $a0, comma
	syscall		# ','
	
	li $v0, 1
	la $a0, ($t2)	# '<column>'
	syscall
	
	li $v0, 4
	la $a0, close
	syscall		# '): '
		# 'A(<row>,<column>): ' for Matrix A or B
	
	li $v0, 5	# System Call to input integer
	la $a1, int_space # Load register with bit-value of integer
	syscall 	# Input integer
	
	beqz $t0, save_to_A
	
	# Saves to B row major
	sw $v0, 0($s4)
	addi $s4, $s4, 4
	
	addi $t2, $t2, 1
	bgt $t2, $s2, new_row_B
	
	j matrix_input
	
	# Saves to A row major
	save_to_A:
	sw $v0, 0($s3)
	addi $s3, $s3, 4
	
	addi $t2, $t2, 1
	bgt $t2, $s1, new_row_A
	
	j matrix_input
	
	new_row_A:
		addi $t2, $zero, 1
		addi $t1, $t1, 1
		bgt $t1, $s0, next_matrix
		j matrix_input
		
	new_row_B:
		addi $t2, $zero, 1
		addi $t1, $t1, 1
		bgt $t1, $s1, multiply_initialize
		j matrix_input
	
	next_matrix:
		beq $t0, 1, multiply_initialize
		addi $t0, $zero, 1
		addi, $t1, $zero, 1
		addi $t2, $zero, 1
		j matrix_input
		
#-----------------------------------------------------------------------------	
		
multiply_initialize:
	lw $s3, 28($sp)
	lw $s4, 32($sp)
	
	add $t0, $s3, $zero	# $t0 = Base(A)
	add $t1, $s4, $zero	# $t1 = Base(B)
	add $t2, $s5, $zero	# $t2 = Base(C)
	
	add $t3, $zero, $zero	# $t3 = A(m,n) contents, then product
	add $t6, $zero, $zero   # $t6 = B(n,p) contents, then column(B) increment
	add $t4, $zero, $zero	# $t4 = sum
	
	add $t5, $zero, $zero	# Row/Column iterator (for multiplying)
	
	add $t7, $zero, $zero   # Row iterator (C)
	add $t8, $zero, $zero   # Column iterator (C)

multiply_loop:		
	lw $t3, ($s3)
	lw $t6, ($s4)	
	mul $t3, $t3, $t6	# A(m,n) * B(n,p)
	add $t4, $t4, $t3	# sum += A(m,n) * B(n,p)
	
        addi $t5, $t5, 1	# increments row(A) column(B) by 1
        beq $t5, $s1, store_to_MC # if it equals row(A) column(B) length, store to C
        
        
        addi $s3, $s3, 4	# otherwise increment row(A) by one integer
        
        mul $t6, $s2, 4
        add $s4, $s4, $t6	# increment column(B)
        j multiply_loop
        
        			
        store_to_MC:
        sw $t4, 0($s5)	# Saves sum to Matrix C in row major		
	
	addi $s5, $s5, 4	# Increments C by integer length																	
	addi $t8, $t8, 1	# Increments column iterator (C) by 1
	
	add $s3, $t0, $zero	# Restores current row (A)
	
	mul $t4, $t8, 4		# $t4 = offset from base to next column
        add $s4, $t1, $t4	# Restores current column, then adds 4*column to get to the next column
	
	add $t4, $zero, $zero	# Resets sum back to 0
	add $t5, $zero, $zero 	# Resets row(A) column(B) iterator to 0
	
	
	
        beq $t8, $s2, new_MC_row # If column iterator = $s2, new row for matrix C
        
        
        
        j multiply_loop													
																	
	new_MC_row:	
	
	addi $t7, $t7, 1	    # Increments row iterator (C) by 1
	beq $t7, $s0, print_initialize  # If row_count(C) = row_count(A), multiplication is finished, print answer
	
																																																																																																																																						
	mul $t8, $s1, 4		# Gets column length of A
	add $t0, $t0, $t8	# Adds it to current row base of A
	
	add $s3, $t0, $zero	# Sets base address of A to next row
	add $s4, $t1, $zero	# Sets base address of B to original column
	
	add $t8, $zero, $zero 	# Resets column iterator (C)																																												
	
        j multiply_loop																									
	
	#-----------------------------------------------------------------------------																												
																																																																																					
	print_initialize:
	
	lw $s3, 28($sp)		# Restores base addresses of matrixes
	lw $s4, 32($sp)
	add $s5, $t2, $zero
	
	add $t0, $zero, $zero
	add $t1, $zero, $zero
	
	li $v0, 4
	la $a0, endl
	syscall
	
	li $v0, 4
	la $a0, endl
	syscall
	
	j print_MA
		
	next_line:
	li $v0, 4
	la $a0, endl
	syscall
		
	addi $t1, $t1, 1
		
	blt $t1, $s0, print_MA
	blt $t1, $s1, print_MA
	
	li $v0, 4
	la $a0, endl
	syscall
	
	j start
	
	print_MA:
	add $t0, $zero, $zero
	bge $t1, $s0, empty_A_row
	
	print_A_row:
	lw $t5, 0($s3)
	  
	li $v0, 1	
	la $a0, ($t5)
	syscall	
		
	li $v0, 4
	la $a0, tab
	syscall
		
	addi $s3, $s3, 4
	addi $t0, $t0, 1
		
	beq $t0, $s1, print_MB
	j print_A_row
		
	empty_A_row:
		
	li $v0, 4
	la $a0, tab
	syscall
		
	addi $t0, $t0, 1
		
	beq $t0, $s1, print_MB
	j empty_A_row
		
		
		
	print_MB:
	add $t0, $zero, $zero
		
	beqz $t1, print_mult
	j printed_mult
		
	print_mult:
	li $v0, 4	
	la $a0, times
	syscall	
		
	printed_mult:
	li $v0, 4
	la $a0, tab
	syscall
		
	bge $t1, $s1, empty_B_row
		
	print_B_row:
	lw $t5, 0($s4)
		
	li $v0, 1	
	la $a0, ($t5)
	syscall	
		
	li $v0, 4
	la $a0, tab
	syscall
		
	addi $s4, $s4, 4
	addi $t0, $t0, 1
		
	beq $t0, $s2, print_C
	j print_B_row
		
	empty_B_row:
		
	li $v0, 4
	la $a0, tab
	syscall
		
	addi $t0, $t0, 1
		
	beq $t0, $s2, print_C
	j empty_B_row
	
	
	
	print_C:
	add $t0, $zero, $zero
		
	beqz $t1, print_equals
	j printed_equals
		
	print_equals:
	li $v0, 4	
	la $a0, equals
	syscall	
		
	printed_equals:
	li $v0, 4
	la $a0, tab
	syscall
		
	bge $t1, $s0, empty_C_row
		
	print_C_row:
	lw $t5, 0($s5)
		
	li $v0, 1	
	la $a0, ($t5)
	syscall	
		
	li $v0, 4
	la $a0, tab
	syscall
		
	addi $s5, $s5, 4
	addi $t0, $t0, 1
		
	beq $t0, $s2, next_line
	j print_C_row
		
	empty_C_row:
		
	li $v0, 4
	la $a0, tab
	syscall
		
	addi $t0, $t0, 1
		
	beq $t0, $s2, next_line
	j empty_C_row
																																			
end:
	lw $ra, 0($sp)
	lw $s0, 4($sp)
	lw $s1, 8($sp)
	lw $s2, 12($sp)
	lw $s3, 16($sp)
	lw $s4, 20($sp)
	lw $s5, 24($sp)
	addi $sp, $sp, 28
	jr $ra
