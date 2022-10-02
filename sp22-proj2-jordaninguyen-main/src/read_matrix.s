.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
#   - If malloc returns an error,
#     this function terminates the program with error code 26
#   - If you receive an fopen error or eof,
#     this function terminates the program with error code 27
#   - If you receive an fclose error or eof,
#     this function terminates the program with error code 28
#   - If you receive an fread error or eof,
#     this function terminates the program with error code 29
# ==============================================================================
read_matrix:

	# Prologue
	addi sp, sp, -28
    sw s0, 0(sp) # # pointer to a string representing the filename
    sw s1, 4(sp) # pointer to an integer (# of rows)
    sw s2, 8(sp) # pointer to an integer (# of columns)
    sw s3, 12(sp) # pointer to malloced space
    sw s4, 16(sp) # file descriptor
    sw s5, 20(sp) # number of bytes to read
    sw ra, 24(sp)

    mv s0, a0
    mv s1, a1
    mv s2, a2

	li a1, 0
	call fopen
	li t0, -1
    mv s4, a0
    beq s4, t0, nullcase
    jal findsize
    nullcase:
    li a0, 27
    j exit

    findsize:
    mv a0, s4 #find the row int
    mv a1, s1 # pointer to the buffer where the read bytes are stored
    li a2, 4 
    call fread
    li t0, 4
    bne a0, t0, readerror

    mv a0, s4 #find the column int
    mv a1, s2
    li a2, 4
    call fread
    li t0, 4
    bne a0, t0, readerror

	start:
    #find size of matrix
    lw t0, 0(s1)
    lw t1, 0(s2)
    mul s5, t0, t1
    slli s5, s5, 2
    mv a0, s5
    call malloc
    mv s3, a0


    beq a0, x0, failmalloc # check malloced size
    j readmatrix

    failmalloc:
    li a0, 26
    j exit

    readmatrix:
    mv a0, s4
    mv a1, s3
    mv a2, s5
	call fread

    bne s5, a0, readerror
    j end


    readerror:
    li a0, 29
    j exit


    end:

    mv a0, s4
    call fclose
    li t0, -1
    beq a0, t0, failclose
    j epilogue

    failclose:
    li a0, 28
    j exit


    epilogue:
    mv a0, s3
	
    # Epilogue
    lw s0, 0(sp) # # pointer to a string representing the filename
    lw s1, 4(sp) # pointer to an integer (# of rows)
    lw s2, 8(sp) # pointer to an integer (# of columns)
    lw s3, 12(sp) # pointer to malloced space
    lw s4, 16(sp) # file descriptor
    lw s5, 20(sp) # number of bytes to read
    lw ra, 24(sp)
    addi sp, sp, 28

	ret
