.globl write_matrix

.text
# ==============================================================================
# FUNCTION: Writes a matrix of integers into a binary file
# FILE FORMAT:
#   The first 8 bytes of the file will be two 4 byte ints representing the
#   numbers of rows and columns respectively. Every 4 bytes thereafter is an
#   element of the matrix in row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is the pointer to the start of the matrix in memory
#   a2 (int)   is the number of rows in the matrix
#   a3 (int)   is the number of columns in the matrix
# Returns:
#   None
# Exceptions:
#   - If you receive an fopen error or eof,
#     this function terminates the program with error code 27
#   - If you receive an fclose error or eof,
#     this function terminates the program with error code 28
#   - If you receive an fwrite error or eof,
#     this function terminates the program with error code 30
# ==============================================================================
write_matrix:
	#prologue 
	addi sp, sp, -28
    sw s0, 0(sp) # # pointer to a string representing the filename
    sw s1, 4(sp) # pointer to matrix in memory
    sw s2, 8(sp) # number of rows
    sw s3, 12(sp) # number of columns
    sw s4, 16(sp) # file descriptor
    sw s5, 20(sp) # pointer to a buffer
    sw ra, 24(sp)
	
    mv s0, a0
    mv s1, a1
    mv s2, a2
    mv s3, a3
    ebreak
	li a1, 1
	call fopen
	li t0, -1
    mv s4, a0
    beq s4, t0, nullcase
    jal start
    nullcase:
    li a0, 27
    j exit
	
    
	start:
    li a0, 4 #start mallocing row
    call malloc
    mv s5, a0
    
    sw s2, 0(s5) #putting in row int into file
    mv a0, s4
    mv a1, s5
    li a2, 1
    li a3, 4
    call fwrite

    li t0, 1
    lw t1, 0(s5) #check value
    bne a0, t0, failwrite
    
    mv a0, s5
    call free
   
   
   	li a0, 4 #start mallocing column
    call malloc
    mv s5, a0
    
    sw s3, 0(s5) #putting in col int into file
    mv a0, s4
    mv a1, s5
    li a2, 1
    li a3, 4
    call fwrite
	
    li t0, 1
    bne a0, t0, failwrite
    
    mv a0, s5
    call free
    
    mv a0, s4 # writing matrix
    mv a1, s1
    mul a2, s2, s3
    li a3, 4
    call fwrite
    mul t0, s2, s3
    bne a0, t0, failwrite
    
    
    end:
    
    mv a0, s4
    call fclose
    li t0, -1
    beq a0, t0, failclose
    j epilogue
    
    failclose:
    li a0, 28
    j exit
    
    failwrite:
    li a0, 30
    j exit
    
    epilogue:
    mv a0, s3
	# Epilogue
    lw s0, 0(sp) # pointer to a string representing the filename
    lw s1, 4(sp) # pointer to an integer (# of rows)
    lw s2, 8(sp) # pointer to an integer (# of columns)
    lw s3, 12(sp) # pointer to malloced space
    lw s4, 16(sp) # file descriptor
    lw s5, 20(sp) # number of bytes to read
    lw ra, 24(sp)
    addi sp, sp, 28
	ret
