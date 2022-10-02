.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
#   d = matmul(m0, m1)
# Arguments:
#   a0 (int*)  is the pointer to the start of m0
#   a1 (int)   is the # of rows (height) of m0
#   a2 (int)   is the # of columns (width) of m0
#   a3 (int*)  is the pointer to the start of m1
#   a4 (int)   is the # of rows (height) of m1
#   a5 (int)   is the # of columns (width) of m1
#   a6 (int*)  is the pointer to the the start of d
# Returns:
#   None (void), sets d = matmul(m0, m1)
# Exceptions:
#   Make sure to check in top to bottom order!
#   - If the dimensions of m0 do not make sense,
#     this function terminates the program with exit code 38
#   - If the dimensions of m1 do not make sense,
#     this function terminates the program with exit code 38
#   - If the dimensions of m0 and m1 don't match,
#     this function terminates the program with exit code 38
# =======================================================
matmul:

	# Error checks
    li t1, 1
    blt a1, t1, measurement
    blt a2, t1, measurement
    blt a4, t1, measurement
    blt a5, t1, measurement
    bne a2, a4, measurement
    j prologue

measurement:
    li a0, 38
    j exit

prologue:
    # Prologue
    addi sp, sp, -52
    sw s0, 0(sp) #pointer to first matrix
    sw s1, 4(sp) # first matrix's rows (height) 
    sw s2, 8(sp) # first matrix's columns (width) // number of elements for dot product
    sw s3, 12(sp) # pointer to second matrix
    sw s4, 16(sp) # second matrix's rows (height) 
    sw s5, 20(sp) # second matrix's columns (width) // stride of matrix 2
    sw s6, 24(sp) # resulting matrix
    sw s7, 28(sp)
    sw s8, 32(sp) # column of first x 4 // increment matrix 1 by it
    sw s9, 36(sp) # 1 times 4 = 4 // increment matrix 2 by it
    sw s10, 40(sp) # column saver
    sw s11, 44(sp) # save pointer of matrix 2
    sw ra, 48(sp)

    mv s0, a0
    mv s1, a1
    mv s2, a2
    mv s3, a3
    mv s4, a4
    mv s5, a5
    mv s6, a6
    mv s8, s2
    mv s11, s3
    slli s8, s8, 2
    li s9, 4
    li s7, 1 #stride of 1
    li s10, 0
    li t3, 0

outer_loop_start:
    li t4, 0 #start of outer loopx
    blt t3, s1, inner_loop_start
    j outer_loop_end 

inner_loop_start:
    beq s5, t4, inner_loop_end #start of inner loop
    addi sp, sp, -8
    sw t3, 0(sp)
    sw t4, 4(sp)

    mv a0, s0 #pointer of 1st matrix
    mv a1, s3 #pointer of 2nd matrix
    mv a2, s2 #number of elements
    mv a3, s7 #stride of 1
    mv a4, s5 #stride of 2
    jal ra, dot
    sw a0, 0(s6)
    lw t5, 0(s6) #to look at value
    lw t3, 0(sp)
    lw t4, 4(sp)
    addi sp, sp, 8

    addi t4, t4, 1
    addi s6, s6, 4
    add s3, s3, s9
    j inner_loop_start

inner_loop_end:
    mv s3, s11 # restore original matrix 2 pointer
    add s0, s0, s8 # point to next row of matrix 1
    addi t3, t3, 1 #end of inner loop
    j outer_loop_start

outer_loop_end:

    # Epilogue
    lw s0, 0(sp) #pointer to first matrix
    lw s1, 4(sp) # first matrix's rows (height)
    lw s2, 8(sp) # first matrix's columns (width)
    lw s3, 12(sp) # pointer to second matrix
    lw s4, 16(sp) # second matrix's rows (height)
    lw s5, 20(sp) # second matrix's columns (width)
    lw s6, 24(sp) # resulting matrix
    lw s7, 28(sp)
    lw s8, 32(sp)
    lw s9, 36(sp)
    lw s10, 40(sp) # column saver
    lw s11, 44(sp) # save pointer of matrix 2
    lw ra, 48(sp)
    addi sp, sp, 52

    ret