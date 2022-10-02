.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
#   a0 (int*) is the pointer to the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   None
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# ==============================================================================
relu:
	# Prologue
	addi sp, sp, -12
	sw s0, 0(sp)
	sw s1, 4(sp)
    sw ra, 8(sp)
	
	# code start
	mv s0, a0 # store addr of first elem
	mv s1, a1 # store len of arr
	addi t0, x0, 0
    
    # set counter
	li t3, 0

	# check null case
	addi t1, x0, 1
	blt s1, t1, zero
    j loop_start
	
zero:
	li a0, 36
	j exit 

loop_start:
	blt t3, s1, loop_continue
	j loop_end

loop_continue:
	lw t0, 0(s0) # get elem of arr & get ready to pass to abs
    
    blt t0, x0, change
    
	
	addi s0, s0, 4 # increment addr
	addi t3, t3, 1 # increment counter
	j loop_start

change:
	li t0, 0
    sw t0, 0(s0)
    addi s0, s0, 4 # increment addr
	addi t3, t3, 1 # increment counter
	j loop_start
    
loop_end:
	# Epilogue
	lw s0, 0(sp)
	lw s1, 4(sp)
    lw ra, 8(sp)
	addi sp, sp, 12
	ret
