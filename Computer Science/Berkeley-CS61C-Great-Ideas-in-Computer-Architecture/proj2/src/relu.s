.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
# 	a0 (int*) is the pointer to the array
#	a1 (int)  is the # of elements in the array
# Returns:
#	None
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 78.
# ==============================================================================
relu:
    # Prologue
    
	li t1, 1
    blt a1, t1, length_check
    
    li t0, 0 # current index

loop_start:
    bge t0, a1, loop_end
    lw t1, 0(a0)
    bge t1, x0, loop_continue
    sw x0, 0(a0)
    
loop_continue:
	addi a0, a0, 4
    addi t0, t0, 1
    j loop_start
                
loop_end:
	ret

length_check:
	li a1, 78
	jal exit2


