.globl classify

.text
classify:
    # =====================================
    # COMMAND LINE ARGUMENTS
    # =====================================
    # Args:
    #   a0 (int)    argc
    #   a1 (char**) argv
    #   a2 (int)    print_classification, if this is zero, 
    #               you should print the classification. Otherwise,
    #               this function should not print ANYTHING.
    # Returns:
    #   a0 (int)    Classification
    # Exceptions:
    # - If there are an incorrect number of command line args,
    #   this function terminates the program with exit code 89.
    # - If malloc fails, this function terminats the program with exit code 88.
    #
    # Usage:
    #   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>


	li t0, 5
    bne a0, t0, args_err
	
    # prologue
    addi sp, sp, -52
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw s4, 16(sp)
    sw s5, 20(sp)
    sw s6, 24(sp)
    sw s7, 28(sp)
    sw s8, 32(sp)
    sw s9, 36(sp)
    sw s10, 40(sp)
    sw s11, 44(sp)
    sw ra, 48(sp)
    
    mv s0, a0 # s0 is argc
    mv s1, a1 # s1 is argv
    mv s2, a2 # s2 is print_classification
    
    # save s2 because s registers are not enough in my program
    addi sp, sp, -4
    sw s2, 0(sp)


	# =====================================
    # LOAD MATRICES
    # =====================================


    # Load pretrained m0
	li a0, 4
	jal malloc
	beq a0, x0, malloc_err
	mv s3, a0 # s3 is rows of m0
    
    li a0, 4
	jal malloc
	beq a0, x0, malloc_err
	mv s4, a0 # s4 is cols of m0
    
    lw a0, 4(s1)
    mv a1, s3
    mv a2, s4
    jal read_matrix
    mv s5, a0 # s5 is the starting address of m0


    # Load pretrained m1
	li a0, 4
	jal malloc
	beq a0, x0, malloc_err
	mv s6, a0 # s6 is rows of m1
    
    li a0, 4
	jal malloc
	beq a0, x0, malloc_err
	mv s7, a0 # s7 is cols of m1
    
    lw a0, 8(s1)
    mv a1, s6
    mv a2, s7
    jal read_matrix
    mv s8, a0 # s8 is the starting address of m1


    # Load input matrix
	li a0, 4
	jal malloc
	beq a0, x0, malloc_err
	mv s9, a0 # s9 is rows of input
    
    li a0, 4
	jal malloc
	beq a0, x0, malloc_err
	mv s10, a0 # s10 is cols of input
    
    lw a0, 12(s1)
    mv a1, s9
    mv a2, s10
    jal read_matrix
    mv s11, a0 # s11 is the starting address of input


    # =====================================
    # RUN LAYERS
    # =====================================
    # 1. LINEAR LAYER:    m0 * input
    # 2. NONLINEAR LAYER: ReLU(m0 * input)
    # 3. LINEAR LAYER:    m1 * ReLU(m0 * input)
    
	# 1
    # malloc for result of 1
    lw t0, 0(s3)
    lw t1, 0(s10)
    mul a0, t0, t1
    slli a0, a0, 2
	jal malloc
	beq a0, x0, malloc_err
	mv s0, a0
    
    lw t0, 0(s3)
    lw t1, 0(s4)
    lw t2, 0(s9)
    lw t3, 0(s10)
	mv a0, s5
    mv a1, t0
	mv a2, t1
	mv a3, s11
	mv a4, t2
	mv a5, t3
    mv a6, s0
	jal matmul
	
	# 2
	mv a0, s0 # s0 holds the intermidiate result
    lw t0, 0(s3)
    lw t1, 0(s10)
	mul a1, t0, t1
	jal relu
    
    # 3
    # malloc for result of 3
    lw t0, 0(s6)
    lw t1, 0(s10)
    mul a0, t0, t1
    slli a0, a0, 2
	jal malloc
	beq a0, x0, malloc_err
    mv s2, a0 # s2 holds the final result
    
    lw t0, 0(s6)
    lw t1, 0(s7)
    lw t2, 0(s3)
    lw t3, 0(s10)
    mv a0, s8
    mv a1, t0
    mv a2, t1
    mv a3, s0
    mv a4, t2
    mv a5, t3
    mv a6, s2
    jal matmul

    # =====================================
    # WRITE OUTPUT
    # =====================================
    # Write output matrix
	lw a0, 16(s1)
    lw t0, 0(s6)
    lw t1, 0(s10)
	mv a1, s2
    mv a2, t0
    mv a3, t1
    jal write_matrix


    # =====================================
    # CALCULATE CLASSIFICATION/LABEL
    # =====================================
    # Not print then jump to end
    lw t0, 0(sp)
    addi sp, sp, 4
    bne t0, x0, end
    # Call argmax
	mv a0, s2
    lw t0, 0(s6)
    lw t1, 0(s10)
	mul a1, t0, t1
    jal argmax


    # Print classification
    mv a1, a0
	jal print_int


    # Print newline afterwards for clarity
	li a1 '\n'
	jal print_char

end:
	# free final result
    mv a0, s0
    jal free
    # free intermediate result
    mv a0, s2
    jal free
    # free rows and cols
    mv a0, s3
    jal free
    mv a0, s4
    jal free
    mv a0, s6
    jal free
    mv a0, s7
    jal free
    mv a0, s9
    jal free
    mv a0, s10
    jal free

    # prologue
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw s4, 16(sp)
    lw s5, 20(sp)
    lw s6, 24(sp)
    lw s7, 28(sp)
    lw s8, 32(sp)
    lw s9, 36(sp)
    lw s10, 40(sp)
    lw s11, 44(sp)
    lw ra, 48(sp)
    addi sp, sp, 52

    ret


malloc_err:
	li a1 88
    jal exit2

args_err:
	li a1 89
    jal exit2