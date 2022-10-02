.globl classify

.text
# =====================================
# COMMAND LINE ARGUMENTS
# =====================================
# Args:
#   a0 (int)        argc
#   a1 (char**)     argv
#   a1[1] (char*)   pointer to the filepath string of m0
#   a1[2] (char*)   pointer to the filepath string of m1
#   a1[3] (char*)   pointer to the filepath string of input matrix
#   a1[4] (char*)   pointer to the filepath string of output file
#   a2 (int)        silent mode, if this is 1, you should not print
#                   anything. Otherwise, you should print the
#                   classification and a newline.
# Returns:
#   a0 (int)        Classification
# Exceptions:
#   - If there are an incorrect number of command line args,
#     this function terminates the program with exit code 31
#   - If malloc fails, this function terminates the program with exit code 26
#
# Usage:
#   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>
classify:
	# prologue
    addi sp, sp, -48
    sw s0, 0(sp) # the number of arguments provided
    sw s1, 4(sp) # a pointer to an array of argument strings 
    sw s2, 8(sp) # A pointer to the filepath string of the first matrix file m0
    sw s3, 12(sp) # A pointer to the filepath string of the second matrix file m1
    sw s4, 16(sp) # A pointer to the filepath string of the input matrix file input
    sw s5, 20(sp) # A pointer to the filepath string of the output file
    sw s6, 24(sp) # whether or not to print out the classification
    sw s7, 28(sp) # matrix h
    sw s8, 32(sp) # size of h then becomes size of o
    sw s9, 36(sp) # matrix o
    sw s10, 40(sp) # save the clarification here
    sw ra, 44(sp)
    
    mv s0, a0
    
    li t0, 5
    bne t0, s0, nullerror
    j startloadingargs
    
nullerror:
	li a0, 31
    j exit

mallocfn:
	addi sp, sp -4
    sw ra, 0(sp)
	li a0, 4
    call malloc
    beq x0, a0, mallocerror
    lw ra, 0(sp)
    addi sp, sp 4
    ret
    
mallocsizeints: 
	addi sp, sp -4
    sw ra, 0(sp)
	call mallocfn
    mv t0, a0 # pointer to space alloc'd for row
    addi sp, sp, -8
    sw t0, 0(sp)
    
    call mallocfn
    mv t1, a0 # pointer to space alloc'd for col
    sw t1, 4(sp)
    lw ra, 0(sp)
    addi sp, sp 4
 
    ret

freesizeints:
	addi sp, sp -4
    sw ra, 0(sp)
	lw a0, 0(sp) # row
    call free
    
    lw a0, 4(sp) # col
    call free
    
    addi sp, sp, 8
    
    lw ra, 0(sp)
    addi sp, sp 4
    ret

mallocerror:
	li a0, 26
    j exit
	
startloadingargs: 
	mv s1, a1
    lw s2, 4(s1)
    lw s3, 8(s1) 
    lw s4, 12(s1)
    lw s5, 16(s1)
    mv s6, a2

	# Read pretrained m0
# 	call mallocfn
#     lw t0, 0(s2) # keep string val
#     mv s2, a0 # set s2 to pointer malloc'd
#     sw t0, 0(s2) # store string val in pointer
    
	#call mallocsizeints
    addi sp, sp -24
	call mallocfn # allocate sizes for m0
    mv t0, a0 # pointer to space alloc'd for row
    sw t0, 0(sp)
    call mallocfn
    mv t1, a0 # pointer to space alloc'd for col
    sw t1, 4(sp)
    
    call mallocfn # allocate sizes for m1
    mv t0, a0 # pointer to space alloc'd for row
    sw t0, 8(sp)
    call mallocfn
    mv t1, a0 # pointer to space alloc'd for col
    sw t1, 12(sp)
    
    call mallocfn # allocate sizes for input
    mv t0, a0 # pointer to space alloc'd for row
    sw t0, 16(sp)
    call mallocfn
    mv t1, a0 # pointer to space alloc'd for col
    sw t1, 20(sp)
    
    
    
 
    
    mv a0, s2
    lw a1, 0(sp) # pointer space for row
    lw a2, 4(sp) # pointer space for col
    call read_matrix
    mv s2, a0 # store pointer to matrix read from m0 file into s2
    
	# Read pretrained m1
#     call mallocfn
#     mv t0, s3 # keep string val
#     mv s3, a0 # set s2 to pointer malloc'd
#     sw t0, 0(s3) # store string val in pointer
    
	#call mallocsizeints
    
    mv a0, s3
    lw a1, 8(sp) # pointer space for row
    lw a2, 12(sp) # pointer space for col
    call read_matrix
    mv s3, a0 # store pointer to matrix read from m1 file into s3

	# Read input matrix
# 	call mallocfn
#     mv t0, s4 # keep string val
#     mv s4, a0 # set s2 to pointer malloc'd
#     sw t0, 0(s4) # store string val in pointer
    
	#call mallocsizeints
    
    mv a0, s4
    lw a1, 16(sp) # pointer space for row
    lw a2, 20(sp) # pointer space for col
    call read_matrix
    mv s4, a0 # store pointer to matrix read from input file into s4

	# Compute h = matmul(m0, input)
    lw t1, 0(sp)
    lw t1, 0(t1)
    lw t2, 20(sp)
    lw t2, 0(t2)
    mul s8, t1, t2 # size of h
    slli t0, s8, 2
    mv a0, t0
    call malloc
    ebreak #check malloc
    beq a0, x0, mallocerror
    mv s7, a0 # now holds a6 which is where the output of the matmul will be 
    
    mv a0, s2
    lw a1, 0(sp) # m0's row which is at 0(sp)
    lw a1, 0(a1) # load in value of row
    lw a2, 4(sp) # m0's col
    lw a2, 0(a2) #check
    mv a3, s4
    lw a4, 16(sp) # input's row
    lw a4, 0(a4) # load in value of input row
    lw a5, 20(sp) # input's col
    lw a5, 0(a5) # load in value of input col
    mv a6, s7 # store matrix mul result output alloc lmao
    
	call matmul

	# Compute h = relu(h)
	mv a0, s7
    mv a1, s8
    call relu # now s7 holds the result of result(s7)
    
	# Compute o = matmul(m1, h)
    lw t1, 8(sp)
    lw t1, 0(t1)
    lw t2, 20(sp)
    lw t2, 0(t2)
    mul s8, t1, t2 # size of h
    slli t0, s8, 2
    mv a0, t0
    call malloc
    ebreak #check malloc
    beq a0, x0, mallocerror
    mv s9, a0 # now holds a6 which is where the output of the matmul will be 
    
	mv a0, s3
    lw a1, 8(sp) # m1's row which is at 8(sp)
    lw a1, 0(a1) # load in m1's row value
    lw a2, 12(sp) # m1's col
    lw a2, 0(a2) # load in m1's col value
    mv a3, s7
    lw a4, 0(sp) # m0's row
    lw a4, 0(a4) # load in h's row
    lw a5, 20(sp) # input's col
    lw a5, 0(a5) # load in h's col
    mv a6, s9 # store matrix mul result output alloc lmao
	
    call matmul
 
	# Write output matrix o
    mv a0, s5 # pointer to output
    mv a1, s9
    lw a2, 8(sp)
    lw a2, 0(a2) # load in int of the matrix row
    lw a3, 20(sp) 
    lw a3, 0(a3) # load in int of matrix col
	
    call write_matrix
    
	# Compute and return argmax(o)
	mv a0, s9 # pointer to start of integer array
    mv a1, s8 # number of integers at array
    call argmax
	# If enabled, print argmax(o) and newline
    mv s10, a0
	bne s6, x0, freeblock
    call print_int
    li a0, '\n'
    call print_char
	
    
freeblock:
    lw a0, 0(sp)
    call free
    
    lw a0, 4(sp)
    call free
    
    lw a0, 8(sp)
    call free
    
    lw t0, 12(sp)
    mv a0, t0
    call free
    
    lw t0, 16(sp)
    mv a0, t0
    call free
    
    lw t0, 20(sp)
    mv a0, t0
    call free
    
    addi sp, sp 24
    
    
    mv a0, s2
    call free
    mv a0, s3
    call free
    mv a0, s4
    call free
    mv a0, s7
    call free
    mv a0, s9
    call free
    
    # epilogue
    mv a0, s10
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw s4, 16(sp)
    lw s5, 20(sp)
    lw s6, 24(sp)
    lw s7, 28(sp) # matrix h
    lw s8, 32(sp) # size of h then becomes size of o
    lw s9, 36(sp) # matrix o
    lw s10, 40(sp)
    lw ra, 44(sp)
	addi sp, sp, 48
    
	ret
