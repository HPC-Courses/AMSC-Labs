	.file	"smaller.cpp"
	.text
	.p2align 4
	.globl	_Z16smaller_standardii
	.type	_Z16smaller_standardii, @function
_Z16smaller_standardii:
.LFB0:
	.cfi_startproc
	cmpl	%esi, %edi
	movl	%esi, %eax
	cmovle	%edi, %eax
	ret
	.cfi_endproc
.LFE0:
	.size	_Z16smaller_standardii, .-_Z16smaller_standardii
	.p2align 4
	.globl	_Z18smaller_branchlessii
	.type	_Z18smaller_branchlessii, @function
_Z18smaller_branchlessii:
.LFB1:
	.cfi_startproc
	xorl	%eax, %eax
	cmpl	%esi, %edi
	cmovge	%eax, %edi
	cmovl	%eax, %esi
	leal	(%rdi,%rsi), %eax
	ret
	.cfi_endproc
.LFE1:
	.size	_Z18smaller_branchlessii, .-_Z18smaller_branchlessii
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (mk-2022.0) 11.2.0"
	.section	.note.GNU-stack,"",@progbits
