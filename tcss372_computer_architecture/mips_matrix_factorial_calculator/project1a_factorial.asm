#=======================================================#
# Author:	Jesse Bannon				#
# Class:	TCSS 372 B				#
# Desc:		Project #1a - Factorial Calculator	#
#=======================================================#

	.data
int_space: .space 16
intro: .asciiz "**** FACTORIAL CALCULATOR ****"
prompt: .asciiz "\nEnter Integer (-1 to quit): "
factorial: .asciiz "!"
equals: .asciiz " = "

	.text
	.globl main
main:
	li $v0, 4	# System call output string
	la $a0, intro	# Load string intro
	syscall		# Print string
	
	
factorial_start:
	li $v0, 4	# System call output string
	la $a0, prompt	# Load string intro
	syscall		# Print string
	
	li $v0, 5	# System Call input integer
	la $a1, int_space # Load register with bit-value of integer
	syscall 	# Input integer
	
	add $t0, $v0, $zero # Store input into $s0 (iterator)
	add $t2, $t0, $zero # Make copy of input into $s2 (for final answer)
	bltz $t0, end	# Branch to end if less than 0
	
	addi $t1, $zero, 1 # Sum initialized to 1
	beqz $t0, print_sum # If input = 0, print sum (1)

factorial_loop:
	mul $t1, $t0, $t1 # Multiplies sum by iterator (n-1)
	addi $t0, $t0, -1 # Decrements iterator
	blez $t0, print_sum # If iterator is less than or equal to zero, branch to print_sum
	b factorial_loop # Iterates again
	
print_sum:
	li $v0, 4	
	la $a0, factorial
	syscall		# Prints "!"
	
	li $v0, 1
	la $a0, ($t2)
	syscall		# Prints factorial input "<n>"
	
	li $v0, 4
	la $a0, equals
	syscall		# Prints " = "
	
	li $v0, 1
	la $a0, ($t1)
	syscall		# Prints factorial sum "<n>"
	
	b factorial_start # Repeats 
end: 
	jr $ra
