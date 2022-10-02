.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int arrays
# Arguments:
#   a0 (int*) is the pointer to the start of arr0
#   a1 (int*) is the pointer to the start of arr1
#   a2 (int)  is the number of elements to use
#   a3 (int)  is the stride of arr0
#   a4 (int)  is the stride of arr1
# Returns:
#   a0 (int)  is the dot product of arr0 and arr1
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
#   - If the stride of either array is less than 1,
#     this function terminates the program with error code 37
# =======================================================
dot:

	# Prologue
   addi sp, sp, -36
    sw s0, 0(sp) # pointer of first array
    sw s1, 4(sp) # pointer of second array
    sw s2, 8(sp) # number of elements
    sw s3, 12(sp) # stride of first
    sw s4, 16(sp) # stride of second
    sw s5, 20(sp) # sum
    sw s6, 24(sp) # stride 1 times 4
    sw s7, 28(sp) # stride 2 times 4
    sw ra, 32(sp)

    # Prologue
    mv s0, a0
    mv s1, a1
    mv s2, a2
    mv s3, a3
    mv s4, a4

    add s6, x0, s3
    slli s6, s6, 2
    add s7, x0, s4
    slli s7, s7, 2
    li s5, 0 # sum equals 0
    li t1, 1
    li t5, 1
    li t4, 1 # counter of number of elements
    blt s2, t1, zero
    blt s3, t1, stride
    blt s4, t1, stride
    j loop_start
zero:
    li a0, 36
    j exit

stride:
    li a0, 37
    j exit

loop_start:
    blt s2, t4, loop_end
    lw t2, 0(s0)
    lw t3, 0(s1)
    mul t5, t2, t3
    add s5, s5, t5

    add s0, s0, s6
    add s1, s1, s7
    addi t4, t4, 1
    j loop_start




loop_end:
    mv a0, s5

    # Epilogue
    lw s0, 0(sp) # pointer of first array
    lw s1, 4(sp) # pointer of second array
    lw s2, 8(sp) # number of elements
    lw s3, 12(sp) # stride of first
    lw s4, 16(sp) # stride of second
    lw s5, 20(sp) # sum
    lw s6, 24(sp) # index of array 1
    lw s7, 28(sp)
    lw ra, 32(sp)
    addi sp, sp 36
    ret