.globl argmax

.text
# =================================================================
# FUNCTION: Given a int vector, return the index of the largest
#	element. If there are multiple, return the one
#	with the smallest index.
# Arguments:
# 	a0 (int*) is the pointer to the start of the vector
#	a1 (int)  is the # of elements in the vector
# Returns:
#	a0 (int)  is the first index of the largest element
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 77.
# =================================================================
argmax:
    # Prologue
    
	li t1, 1
    blt a1, t1, length_check
    
    li t0, 0 # current index
    li t1, 0 # index of the largest
    lw t2, 0(a0) # largest value

loop_start:
    bge t0, a1, loop_end
    lw t3, 0(a0) # current value
    bge t2, t3, loop_continue
    mv t2, t3
    mv t1, t0
    
loop_continue:
	addi a0, a0, 4
    addi t0, t0, 1
    j loop_start
                
loop_end:
	mv a0, t1
	ret

length_check:
	li a1, 77
	jal exit2