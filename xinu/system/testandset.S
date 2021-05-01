	.text
	.globl	test_and_set

/**
 * test_and_set: assume it's called with test_and_set(&lock);
 * @param lock pointer to a mutex_t variable
 *
 * add a comment each line describing its action
 */
test_and_set:
	pushl	%ebx		# preserve contents of the %ebx register, which is about to be modified
	movl	8(%esp), %ebx	# Move 32-bits from the memory address of the stack pointer + (8 bits -- below) into general register %ebx (move value stored in that spot in memory into %eax)
	movl	$1, %eax	# Set general register %eax to the 32-bit integer representation of 1 -- %eax is now 1
	xchg	%eax, (%ebx)	# Swap the value of %eax (1) with value at memory address of %ebx -- (%ebx) points to (1) 
	popl	%ebx		# Restore %ebx to its original value
	ret			# Transfers control back to return address located on the stack with %ebx the same as before the CALL

