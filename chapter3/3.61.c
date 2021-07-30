.cread_alt
	testq	%rdi, %rdi
	je	.L3
	movl	(%rdi), %eax
	ret
.L3:
	movl	$0, %eax
	ret