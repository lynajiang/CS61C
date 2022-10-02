.globl argmax

.text
# =================================================================
# FUNCTION: Given a int array, return the index of the largest
#   element. If there are multiple, return the one
#   with the smallest index.
# Arguments:
#   a0 (int*) is the pointer to the start of the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   a0 (int)  is the first index of the largest element
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# =================================================================
argmax:
	# Prologue
	addi sp, sp, -20
	sw s0, 0(sp)
	sw s1, 4(sp)
	sw s2, 8(sp) # largest elem seen so far
	sw s3, 12(sp) # index of largest elem
    sw ra, 16(sp)

	# code begins
	mv s0, a0 # store addr of first elem
	mv s1, a1 # store len of arr
	lw s2, 0(s0)
	li s3, 0

	# check null case
	addi t1, x0, 1
	addi t3, x0, -1 # counter
    
    blt s1, t1, zero
    j loop_start

zero:
	li a0, 36
	j exit

loop_start:
	addi t3, t3, 1 # increment counter
	blt t3, s1, loop_continue
	j loop_end

loop_continue:
	lw t0, 0(s0) # get first elem out
	blt s2, t0, store
	addi s0, s0, 4 # increment addr
	j loop_start

store:
	mv s2, t0
	mv s3, t3
	addi s0, s0, 4 # increment addr
	j loop_start

loop_end:
	mv a0, s3 # set return val
	# Epilogue
	lw s0, 0(sp)
	lw s1, 4(sp)
	lw s2, 8(sp) # largest elem seen so far
	lw s3, 12(sp) # index of largest elem
    lw ra, 16(sp)
	addi sp, sp, 20
	ret
