	.file	"toupper.cpp"
	.text
	.section	.text._ZNKSt5ctypeIcE8do_widenEc,"axG",@progbits,_ZNKSt5ctypeIcE8do_widenEc,comdat
	.align 2
	.p2align 4
	.weak	_ZNKSt5ctypeIcE8do_widenEc
	.type	_ZNKSt5ctypeIcE8do_widenEc, @function
_ZNKSt5ctypeIcE8do_widenEc:
.LFB3308:
	.cfi_startproc
	movl	%esi, %eax
	ret
	.cfi_endproc
.LFE3308:
	.size	_ZNKSt5ctypeIcE8do_widenEc, .-_ZNKSt5ctypeIcE8do_widenEc
	.text
	.p2align 4
	.globl	_Z16toupper_standardPci
	.type	_Z16toupper_standardPci, @function
_Z16toupper_standardPci:
.LFB3555:
	.cfi_startproc
	testl	%esi, %esi
	jle	.L3
	leal	-1(%rsi), %eax
	leaq	1(%rdi,%rax), %rcx
	.p2align 4,,10
	.p2align 3
.L6:
	movzbl	(%rdi), %eax
	leal	-97(%rax), %edx
	cmpb	$25, %dl
	ja	.L5
	subl	$32, %eax
	movb	%al, (%rdi)
.L5:
	addq	$1, %rdi
	cmpq	%rcx, %rdi
	jne	.L6
.L3:
	ret
	.cfi_endproc
.LFE3555:
	.size	_Z16toupper_standardPci, .-_Z16toupper_standardPci
	.p2align 4
	.globl	_Z18toupper_branchlessPci
	.type	_Z18toupper_branchlessPci, @function
_Z18toupper_branchlessPci:
.LFB3556:
	.cfi_startproc
	movq	%rdi, %rcx
	movl	%esi, %edx
	testl	%esi, %esi
	jle	.L8
	leal	-1(%rsi), %eax
	cmpl	$14, %eax
	jbe	.L13
	shrl	$4, %esi
	movq	%rdi, %rax
	pxor	%xmm2, %xmm2
	movdqa	.LC0(%rip), %xmm5
	subl	$1, %esi
	movdqa	.LC1(%rip), %xmm4
	movdqa	.LC2(%rip), %xmm3
	salq	$4, %rsi
	leaq	16(%rdi,%rsi), %rsi
	.p2align 4,,10
	.p2align 3
.L11:
	movdqu	(%rax), %xmm0
	movdqu	(%rax), %xmm1
	addq	$16, %rax
	paddb	%xmm5, %xmm0
	psubusb	%xmm4, %xmm0
	pcmpeqb	%xmm2, %xmm0
	pand	%xmm3, %xmm0
	psubb	%xmm0, %xmm1
	movups	%xmm1, -16(%rax)
	cmpq	%rsi, %rax
	jne	.L11
	movl	%edx, %eax
	andl	$-16, %eax
	testb	$15, %dl
	je	.L15
.L10:
	movslq	%eax, %rsi
	addq	%rcx, %rsi
	movzbl	(%rsi), %edi
	leal	-97(%rdi), %r8d
	cmpb	$25, %r8b
	setbe	%r8b
	sall	$5, %r8d
	subl	%r8d, %edi
	movb	%dil, (%rsi)
	leal	1(%rax), %esi
	cmpl	%esi, %edx
	jle	.L8
	movslq	%esi, %rsi
	addq	%rcx, %rsi
	movzbl	(%rsi), %edi
	leal	-97(%rdi), %r8d
	cmpb	$25, %r8b
	setbe	%r8b
	sall	$5, %r8d
	subl	%r8d, %edi
	movb	%dil, (%rsi)
	leal	2(%rax), %esi
	cmpl	%esi, %edx
	jle	.L8
	movslq	%esi, %rsi
	addq	%rcx, %rsi
	movzbl	(%rsi), %edi
	leal	-97(%rdi), %r8d
	cmpb	$25, %r8b
	setbe	%r8b
	sall	$5, %r8d
	subl	%r8d, %edi
	movb	%dil, (%rsi)
	leal	3(%rax), %esi
	cmpl	%esi, %edx
	jle	.L8
	movslq	%esi, %rsi
	addq	%rcx, %rsi
	movzbl	(%rsi), %edi
	leal	-97(%rdi), %r8d
	cmpb	$25, %r8b
	setbe	%r8b
	sall	$5, %r8d
	subl	%r8d, %edi
	movb	%dil, (%rsi)
	leal	4(%rax), %esi
	cmpl	%esi, %edx
	jle	.L8
	movslq	%esi, %rsi
	addq	%rcx, %rsi
	movzbl	(%rsi), %edi
	leal	-97(%rdi), %r8d
	cmpb	$25, %r8b
	setbe	%r8b
	sall	$5, %r8d
	subl	%r8d, %edi
	movb	%dil, (%rsi)
	leal	5(%rax), %esi
	cmpl	%esi, %edx
	jle	.L8
	movslq	%esi, %rsi
	addq	%rcx, %rsi
	movzbl	(%rsi), %edi
	leal	-97(%rdi), %r8d
	cmpb	$25, %r8b
	setbe	%r8b
	sall	$5, %r8d
	subl	%r8d, %edi
	movb	%dil, (%rsi)
	leal	6(%rax), %esi
	cmpl	%esi, %edx
	jle	.L8
	movslq	%esi, %rsi
	addq	%rcx, %rsi
	movzbl	(%rsi), %edi
	leal	-97(%rdi), %r8d
	cmpb	$25, %r8b
	setbe	%r8b
	sall	$5, %r8d
	subl	%r8d, %edi
	movb	%dil, (%rsi)
	leal	7(%rax), %esi
	cmpl	%esi, %edx
	jle	.L8
	movslq	%esi, %rsi
	addq	%rcx, %rsi
	movzbl	(%rsi), %edi
	leal	-97(%rdi), %r8d
	cmpb	$25, %r8b
	setbe	%r8b
	sall	$5, %r8d
	subl	%r8d, %edi
	movb	%dil, (%rsi)
	leal	8(%rax), %esi
	cmpl	%esi, %edx
	jle	.L8
	movslq	%esi, %rsi
	addq	%rcx, %rsi
	movzbl	(%rsi), %edi
	leal	-97(%rdi), %r8d
	cmpb	$25, %r8b
	setbe	%r8b
	sall	$5, %r8d
	subl	%r8d, %edi
	movb	%dil, (%rsi)
	leal	9(%rax), %esi
	cmpl	%esi, %edx
	jle	.L8
	movslq	%esi, %rsi
	addq	%rcx, %rsi
	movzbl	(%rsi), %edi
	leal	-97(%rdi), %r8d
	cmpb	$25, %r8b
	setbe	%r8b
	sall	$5, %r8d
	subl	%r8d, %edi
	movb	%dil, (%rsi)
	leal	10(%rax), %esi
	cmpl	%esi, %edx
	jle	.L8
	movslq	%esi, %rsi
	addq	%rcx, %rsi
	movzbl	(%rsi), %edi
	leal	-97(%rdi), %r8d
	cmpb	$25, %r8b
	setbe	%r8b
	sall	$5, %r8d
	subl	%r8d, %edi
	movb	%dil, (%rsi)
	leal	11(%rax), %esi
	cmpl	%esi, %edx
	jle	.L8
	movslq	%esi, %rsi
	addq	%rcx, %rsi
	movzbl	(%rsi), %edi
	leal	-97(%rdi), %r8d
	cmpb	$25, %r8b
	setbe	%r8b
	sall	$5, %r8d
	subl	%r8d, %edi
	movb	%dil, (%rsi)
	leal	12(%rax), %esi
	cmpl	%esi, %edx
	jle	.L8
	movslq	%esi, %rsi
	addq	%rcx, %rsi
	movzbl	(%rsi), %edi
	leal	-97(%rdi), %r8d
	cmpb	$25, %r8b
	setbe	%r8b
	sall	$5, %r8d
	subl	%r8d, %edi
	movb	%dil, (%rsi)
	leal	13(%rax), %esi
	cmpl	%esi, %edx
	jle	.L8
	movslq	%esi, %rsi
	addq	%rcx, %rsi
	movzbl	(%rsi), %edi
	leal	-97(%rdi), %r8d
	cmpb	$25, %r8b
	setbe	%r8b
	addl	$14, %eax
	sall	$5, %r8d
	subl	%r8d, %edi
	movb	%dil, (%rsi)
	cmpl	%eax, %edx
	jle	.L8
	cltq
	addq	%rax, %rcx
	movzbl	(%rcx), %eax
	leal	-97(%rax), %edx
	cmpb	$25, %dl
	setbe	%dl
	sall	$5, %edx
	subl	%edx, %eax
	movb	%al, (%rcx)
.L8:
	ret
.L13:
	xorl	%eax, %eax
	jmp	.L10
.L15:
	ret
	.cfi_endproc
.LFE3556:
	.size	_Z18toupper_branchlessPci, .-_Z18toupper_branchlessPci
	.section	.text._ZNSt13random_deviceC2Ev,"axG",@progbits,_ZNSt13random_deviceC5Ev,comdat
	.align 2
	.p2align 4
	.weak	_ZNSt13random_deviceC2Ev
	.type	_ZNSt13random_deviceC2Ev, @function
_ZNSt13random_deviceC2Ev:
.LFB2242:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA2242
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	$27765, %eax
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$40, %rsp
	.cfi_def_cfa_offset 64
	leaq	16(%rsp), %rbx
	movq	%rsp, %rsi
	movw	%ax, 20(%rsp)
	movq	%rbx, (%rsp)
	movl	$1634100580, 16(%rsp)
	movb	$116, 22(%rsp)
	movq	$7, 8(%rsp)
	movb	$0, 23(%rsp)
.LEHB0:
	call	_ZNSt13random_device7_M_initERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
.LEHE0:
	movq	(%rsp), %rdi
	cmpq	%rbx, %rdi
	je	.L16
	movq	16(%rsp), %rax
	leaq	1(%rax), %rsi
	call	_ZdlPvm
.L16:
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L20:
	.cfi_restore_state
	movq	%rax, %rbp
.L18:
	movq	(%rsp), %rdi
	cmpq	%rbx, %rdi
	je	.L19
	movq	16(%rsp), %rax
	leaq	1(%rax), %rsi
	call	_ZdlPvm
.L19:
	movq	%rbp, %rdi
.LEHB1:
	call	_Unwind_Resume
.LEHE1:
	.cfi_endproc
.LFE2242:
	.globl	__gxx_personality_v0
	.section	.gcc_except_table._ZNSt13random_deviceC2Ev,"aG",@progbits,_ZNSt13random_deviceC5Ev,comdat
.LLSDA2242:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE2242-.LLSDACSB2242
.LLSDACSB2242:
	.uleb128 .LEHB0-.LFB2242
	.uleb128 .LEHE0-.LEHB0
	.uleb128 .L20-.LFB2242
	.uleb128 0
	.uleb128 .LEHB1-.LFB2242
	.uleb128 .LEHE1-.LEHB1
	.uleb128 0
	.uleb128 0
.LLSDACSE2242:
	.section	.text._ZNSt13random_deviceC2Ev,"axG",@progbits,_ZNSt13random_deviceC5Ev,comdat
	.size	_ZNSt13random_deviceC2Ev, .-_ZNSt13random_deviceC2Ev
	.weak	_ZNSt13random_deviceC1Ev
	.set	_ZNSt13random_deviceC1Ev,_ZNSt13random_deviceC2Ev
	.section	.text._ZNSt6vectorIS_IcSaIcEESaIS1_EED2Ev,"axG",@progbits,_ZNSt6vectorIS_IcSaIcEESaIS1_EED5Ev,comdat
	.align 2
	.p2align 4
	.weak	_ZNSt6vectorIS_IcSaIcEESaIS1_EED2Ev
	.type	_ZNSt6vectorIS_IcSaIcEESaIS1_EED2Ev, @function
_ZNSt6vectorIS_IcSaIcEESaIS1_EED2Ev:
.LFB3946:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	%rdi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	8(%rdi), %rbx
	movq	(%rdi), %rbp
	cmpq	%rbp, %rbx
	je	.L23
	.p2align 4,,10
	.p2align 3
.L27:
	movq	0(%rbp), %rdi
	testq	%rdi, %rdi
	je	.L24
	movq	16(%rbp), %rsi
	addq	$24, %rbp
	subq	%rdi, %rsi
	call	_ZdlPvm
	cmpq	%rbp, %rbx
	jne	.L27
.L26:
	movq	(%r12), %rbp
.L23:
	testq	%rbp, %rbp
	je	.L22
	movq	16(%r12), %rsi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rbp, %rdi
	subq	%rbp, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	_ZdlPvm
	.p2align 4,,10
	.p2align 3
.L24:
	.cfi_restore_state
	addq	$24, %rbp
	cmpq	%rbp, %rbx
	jne	.L27
	jmp	.L26
	.p2align 4,,10
	.p2align 3
.L22:
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE3946:
	.size	_ZNSt6vectorIS_IcSaIcEESaIS1_EED2Ev, .-_ZNSt6vectorIS_IcSaIcEESaIS1_EED2Ev
	.weak	_ZNSt6vectorIS_IcSaIcEESaIS1_EED1Ev
	.set	_ZNSt6vectorIS_IcSaIcEESaIS1_EED1Ev,_ZNSt6vectorIS_IcSaIcEESaIS1_EED2Ev
	.section	.rodata._ZNSt6vectorIS_IcSaIcEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_.str1.1,"aMS",@progbits,1
.LC3:
	.string	"vector::_M_realloc_insert"
	.section	.text._ZNSt6vectorIS_IcSaIcEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_,"axG",@progbits,_ZNSt6vectorIS_IcSaIcEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_,comdat
	.align 2
	.p2align 4
	.weak	_ZNSt6vectorIS_IcSaIcEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_
	.type	_ZNSt6vectorIS_IcSaIcEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_, @function
_ZNSt6vectorIS_IcSaIcEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_:
.LFB4170:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA4170
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$56, %rsp
	.cfi_def_cfa_offset 112
	movq	8(%rdi), %rax
	movq	(%rdi), %r13
	movq	%rdx, 16(%rsp)
	movabsq	$-6148914691236517205, %rdx
	movq	%rax, 40(%rsp)
	subq	%r13, %rax
	sarq	$3, %rax
	imulq	%rdx, %rax
	movabsq	$384307168202282325, %rdx
	cmpq	%rdx, %rax
	je	.L82
	testq	%rax, %rax
	movl	$1, %edx
	movq	%rsi, %r15
	movq	%rdi, %r14
	cmovne	%rax, %rdx
	movq	%rsi, %rbp
	movq	%rsi, %rbx
	addq	%rdx, %rax
	setc	%dl
	movq	%rax, 8(%rsp)
	subq	%r13, %r15
	movzbl	%dl, %edx
	testq	%rdx, %rdx
	jne	.L55
	testq	%rax, %rax
	jne	.L35
	xorl	%r12d, %r12d
.L54:
	movq	16(%rsp), %rdi
	addq	%r12, %r15
	pxor	%xmm0, %xmm0
	movq	$0, 16(%r15)
	movq	8(%rdi), %rax
	subq	(%rdi), %rax
	movups	%xmm0, (%r15)
	movq	%rax, 32(%rsp)
	je	.L36
	js	.L83
	movq	%rax, %rdi
.LEHB2:
	call	_Znwm
.LEHE2:
	movq	16(%rsp), %rdx
	movq	%rax, %rcx
	movq	%rax, %xmm0
	movq	32(%rsp), %rax
	punpcklqdq	%xmm0, %xmm0
	movq	(%rdx), %rsi
	movq	8(%rdx), %rdx
	addq	%rcx, %rax
	movups	%xmm0, (%r15)
	movq	%rax, 16(%r15)
	subq	%rsi, %rdx
	jne	.L84
.L38:
	addq	%rdx, %rcx
	movq	%rcx, 8(%r15)
	cmpq	%r13, %rbp
	je	.L57
	leaq	-24(%rbp), %rcx
	movq	%r13, %rax
	movabsq	$768614336404564651, %rdx
	subq	%r13, %rcx
	shrq	$3, %rcx
	imulq	%rdx, %rcx
	movabsq	$2305843009213693951, %rdx
	andq	%rdx, %rcx
	movq	%r12, %rdx
	subq	%r13, %rdx
	subq	$8, %rdx
	cmpq	$32, %rdx
	jbe	.L58
	testq	%rcx, %rcx
	je	.L58
	addq	$1, %rcx
	movq	%r12, %rdx
	movq	%rcx, %rsi
	shrq	%rsi
	leaq	(%rsi,%rsi,2), %rsi
	salq	$4, %rsi
	addq	%r13, %rsi
	.p2align 4,,10
	.p2align 3
.L41:
	movdqu	(%rax), %xmm6
	movdqu	16(%rax), %xmm7
	addq	$48, %rax
	addq	$48, %rdx
	movdqu	-16(%rax), %xmm0
	movups	%xmm6, -48(%rdx)
	movups	%xmm7, -32(%rdx)
	movups	%xmm0, -16(%rdx)
	cmpq	%rsi, %rax
	jne	.L41
	movq	%rcx, %rsi
	andq	$-2, %rsi
	leaq	(%rsi,%rsi,2), %rax
	salq	$3, %rax
	leaq	(%r12,%rax), %rdx
	addq	%r13, %rax
	cmpq	%rsi, %rcx
	je	.L43
	movdqu	(%rax), %xmm5
	movq	16(%rax), %rax
	movq	%rax, 16(%rdx)
	movups	%xmm5, (%rdx)
.L43:
	leaq	(%rcx,%rcx,2), %rax
	leaq	(%r12,%rax,8), %rsi
.L39:
	movq	40(%rsp), %rdi
	addq	$24, %rsi
	cmpq	%rdi, %rbp
	je	.L44
	movabsq	$768614336404564651, %rcx
	subq	%rbp, %rdi
	movq	%rbp, %rax
	movq	%rdi, %rdx
	subq	$24, %rdx
	shrq	$3, %rdx
	imulq	%rcx, %rdx
	movabsq	$2305843009213693951, %rcx
	andq	%rcx, %rdx
	leaq	1(%rdx), %rdi
	je	.L59
	movq	%rdi, %rcx
	movq	%rsi, %rdx
	shrq	%rcx
	leaq	(%rcx,%rcx,2), %rcx
	salq	$4, %rcx
	addq	%rbp, %rcx
	.p2align 4,,10
	.p2align 3
.L46:
	movdqu	(%rax), %xmm2
	movdqu	16(%rax), %xmm3
	addq	$48, %rax
	addq	$48, %rdx
	movdqu	-16(%rax), %xmm4
	movups	%xmm2, -48(%rdx)
	movups	%xmm3, -32(%rdx)
	movups	%xmm4, -16(%rdx)
	cmpq	%rax, %rcx
	jne	.L46
	movq	%rdi, %rdx
	andq	$-2, %rdx
	leaq	(%rdx,%rdx,2), %rbx
	salq	$3, %rbx
	leaq	(%rsi,%rbx), %rax
	addq	%rbp, %rbx
	cmpq	%rdx, %rdi
	je	.L47
.L45:
	movq	16(%rbx), %rdx
	movdqu	(%rbx), %xmm1
	movq	%rdx, 16(%rax)
	movups	%xmm1, (%rax)
.L47:
	leaq	(%rdi,%rdi,2), %rax
	leaq	(%rsi,%rax,8), %rsi
.L44:
	movq	%r12, %xmm0
	movq	%rsi, %xmm7
	punpcklqdq	%xmm7, %xmm0
	testq	%r13, %r13
	je	.L48
	movq	16(%r14), %rsi
	movq	%r13, %rdi
	movaps	%xmm0, 16(%rsp)
	subq	%r13, %rsi
	call	_ZdlPvm
	movdqa	16(%rsp), %xmm0
.L48:
	addq	8(%rsp), %r12
	movups	%xmm0, (%r14)
	movq	%r12, 16(%r14)
	addq	$56, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L84:
	.cfi_restore_state
	movq	%rcx, %rdi
	movq	%rdx, 16(%rsp)
	call	memmove
	movq	16(%rsp), %rdx
	movq	%rax, %rcx
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L55:
	movabsq	$9223372036854775800, %rax
	movq	%rax, 8(%rsp)
	movq	%rax, %rdi
.L34:
.LEHB3:
	call	_Znwm
.LEHE3:
	movq	%rax, %r12
	jmp	.L54
	.p2align 4,,10
	.p2align 3
.L36:
	movq	$0, 16(%r15)
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L58:
	movq	%r13, %rax
	movq	%r12, %rdx
	.p2align 4,,10
	.p2align 3
.L40:
	movdqu	(%rax), %xmm6
	movq	16(%rax), %rsi
	addq	$24, %rax
	addq	$24, %rdx
	movups	%xmm6, -24(%rdx)
	movq	%rsi, -8(%rdx)
	cmpq	%rax, %rbp
	jne	.L40
	addq	$1, %rcx
	jmp	.L43
	.p2align 4,,10
	.p2align 3
.L57:
	movq	%r12, %rsi
	jmp	.L39
.L59:
	movq	%rsi, %rax
	jmp	.L45
.L83:
.LEHB4:
	call	_ZSt17__throw_bad_allocv
.LEHE4:
.L35:
	movq	%rax, %rdi
	movabsq	$384307168202282325, %rax
	cmpq	%rax, %rdi
	cmovbe	%rdi, %rax
	leaq	(%rax,%rax,2), %rax
	salq	$3, %rax
	movq	%rax, 8(%rsp)
	movq	%rax, %rdi
	jmp	.L34
.L82:
	movl	$.LC3, %edi
.LEHB5:
	call	_ZSt20__throw_length_errorPKc
.LEHE5:
.L60:
	movq	%rax, %rdi
.L49:
	call	__cxa_begin_catch
	testq	%r12, %r12
	je	.L85
	movq	8(%rsp), %rsi
	movq	%r12, %rdi
	call	_ZdlPvm
.L53:
.LEHB6:
	call	__cxa_rethrow
.LEHE6:
.L85:
	movq	(%r15), %rdi
	movq	16(%r15), %rsi
	subq	%rdi, %rsi
	testq	%rdi, %rdi
	je	.L53
	call	_ZdlPvm
	jmp	.L53
.L61:
	movq	%rax, %rbp
.L52:
	call	__cxa_end_catch
	movq	%rbp, %rdi
.LEHB7:
	call	_Unwind_Resume
.LEHE7:
	.cfi_endproc
.LFE4170:
	.section	.gcc_except_table._ZNSt6vectorIS_IcSaIcEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_,"aG",@progbits,_ZNSt6vectorIS_IcSaIcEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_,comdat
	.align 4
.LLSDA4170:
	.byte	0xff
	.byte	0x3
	.uleb128 .LLSDATT4170-.LLSDATTD4170
.LLSDATTD4170:
	.byte	0x1
	.uleb128 .LLSDACSE4170-.LLSDACSB4170
.LLSDACSB4170:
	.uleb128 .LEHB2-.LFB4170
	.uleb128 .LEHE2-.LEHB2
	.uleb128 .L60-.LFB4170
	.uleb128 0x1
	.uleb128 .LEHB3-.LFB4170
	.uleb128 .LEHE3-.LEHB3
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB4-.LFB4170
	.uleb128 .LEHE4-.LEHB4
	.uleb128 .L60-.LFB4170
	.uleb128 0x1
	.uleb128 .LEHB5-.LFB4170
	.uleb128 .LEHE5-.LEHB5
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB6-.LFB4170
	.uleb128 .LEHE6-.LEHB6
	.uleb128 .L61-.LFB4170
	.uleb128 0
	.uleb128 .LEHB7-.LFB4170
	.uleb128 .LEHE7-.LEHB7
	.uleb128 0
	.uleb128 0
.LLSDACSE4170:
	.byte	0x1
	.byte	0
	.align 4
	.long	0

.LLSDATT4170:
	.section	.text._ZNSt6vectorIS_IcSaIcEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_,"axG",@progbits,_ZNSt6vectorIS_IcSaIcEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_,comdat
	.size	_ZNSt6vectorIS_IcSaIcEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_, .-_ZNSt6vectorIS_IcSaIcEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_
	.section	.text._ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EE11_M_gen_randEv,"axG",@progbits,_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EE11_M_gen_randEv,comdat
	.align 2
	.p2align 4
	.weak	_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EE11_M_gen_randEv
	.type	_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EE11_M_gen_randEv, @function
_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EE11_M_gen_randEv:
.LFB4379:
	.cfi_startproc
	movq	%rdi, %r8
	movq	(%rdi), %rdi
	movl	$2567483615, %r10d
	movq	%r8, %rax
	leaq	1816(%r8), %r9
	movq	%r8, %rdx
	.p2align 4,,10
	.p2align 3
.L88:
	andq	$-2147483648, %rdi
	movq	%rdi, %rcx
	movq	8(%rdx), %rdi
	movq	%rdi, %rsi
	andl	$2147483647, %esi
	orq	%rcx, %rsi
	movq	%rsi, %rcx
	shrq	%rcx
	xorq	3176(%rdx), %rcx
	andl	$1, %esi
	je	.L87
	xorq	%r10, %rcx
.L87:
	movq	%rcx, (%rdx)
	addq	$8, %rdx
	cmpq	%r9, %rdx
	jne	.L88
	movq	1816(%r8), %rsi
	leaq	3168(%r8), %rdi
	movl	$2567483615, %r9d
	.p2align 4,,10
	.p2align 3
.L90:
	andq	$-2147483648, %rsi
	movq	%rsi, %rdx
	movq	1824(%rax), %rsi
	movq	%rsi, %rcx
	andl	$2147483647, %ecx
	orq	%rdx, %rcx
	movq	%rcx, %rdx
	shrq	%rdx
	xorq	(%rax), %rdx
	andl	$1, %ecx
	je	.L89
	xorq	%r9, %rdx
.L89:
	movq	%rdx, 1816(%rax)
	addq	$8, %rax
	cmpq	%rdi, %rax
	jne	.L90
	movq	4984(%r8), %rax
	movq	(%r8), %rdx
	andq	$-2147483648, %rax
	andl	$2147483647, %edx
	orq	%rdx, %rax
	movq	%rax, %rdx
	shrq	%rdx
	xorq	3168(%r8), %rdx
	testb	$1, %al
	je	.L91
	movl	$2567483615, %eax
	xorq	%rax, %rdx
.L91:
	movq	%rdx, 4984(%r8)
	movq	$0, 4992(%r8)
	ret
	.cfi_endproc
.LFE4379:
	.size	_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EE11_M_gen_randEv, .-_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EE11_M_gen_randEv
	.text
	.align 2
	.p2align 4
	.type	_ZNSt24uniform_int_distributionIiEclISt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EEEEiRT_RKNS0_10param_typeE.isra.0, @function
_ZNSt24uniform_int_distributionIiEclISt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EEEEiRT_RKNS0_10param_typeE.isra.0:
.LFB4499:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	movslq	%esi, %rax
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	movslq	%edx, %r13
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	subq	%rax, %r13
	movq	%rdi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	movq	%rax, %rbp
	movl	$4294967294, %eax
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$8, %rsp
	.cfi_def_cfa_offset 64
	cmpq	%rax, %r13
	jbe	.L120
	movl	$4294967295, %eax
	cmpq	%rax, %r13
	je	.L110
.L118:
	movl	$-1, %edx
	xorl	%esi, %esi
	movq	%r12, %rdi
	call	_ZNSt24uniform_int_distributionIiEclISt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EEEEiRT_RKNS0_10param_typeE.isra.0
	movq	%rax, %rbx
	movq	4992(%r12), %rax
	salq	$32, %rbx
	cmpq	$623, %rax
	jbe	.L111
	movq	%r12, %rdi
	call	_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EE11_M_gen_randEv
	movq	4992(%r12), %rax
.L111:
	leaq	1(%rax), %rdx
	movq	(%r12,%rax,8), %rax
	movq	%rdx, 4992(%r12)
	movq	%rax, %rdx
	shrq	$11, %rdx
	movl	%edx, %edx
	xorq	%rdx, %rax
	movq	%rax, %rdx
	salq	$7, %rdx
	andl	$2636928640, %edx
	xorq	%rdx, %rax
	movq	%rax, %rdx
	salq	$15, %rdx
	andl	$4022730752, %edx
	xorq	%rdx, %rax
	movq	%rax, %rdx
	shrq	$18, %rdx
	xorq	%rdx, %rax
	addq	%rax, %rbx
	setc	%al
	movzbl	%al, %eax
	cmpq	%rbx, %r13
	jb	.L118
	testq	%rax, %rax
	jne	.L118
.L109:
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	leal	0(%rbp,%rbx), %eax
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L120:
	.cfi_restore_state
	movq	4992(%rdi), %rax
	addq	$1, %r13
	movl	%r13d, %r15d
	cmpq	$623, %rax
	ja	.L121
.L105:
	movq	(%r12,%rax,8), %rbx
	leaq	1(%rax), %rcx
	movq	%rcx, 4992(%r12)
	movq	%rbx, %rax
	shrq	$11, %rax
	movl	%eax, %eax
	xorq	%rax, %rbx
	movq	%rbx, %rax
	salq	$7, %rax
	andl	$2636928640, %eax
	xorq	%rax, %rbx
	movq	%rbx, %rax
	salq	$15, %rax
	andl	$4022730752, %eax
	xorq	%rax, %rbx
	movq	%rbx, %rax
	shrq	$18, %rax
	xorq	%rax, %rbx
	imulq	%r13, %rbx
	cmpl	%ebx, %r13d
	jbe	.L106
	movl	%r13d, %eax
	xorl	%edx, %edx
	negl	%eax
	divl	%r15d
	movl	%edx, %r14d
	cmpl	%edx, %ebx
	jb	.L108
	jmp	.L106
	.p2align 4,,10
	.p2align 3
.L107:
	movq	(%r12,%rax,8), %rbx
	leaq	1(%rax), %rcx
	movq	%rcx, 4992(%r12)
	movq	%rbx, %rax
	shrq	$11, %rax
	movl	%eax, %eax
	xorq	%rax, %rbx
	movq	%rbx, %rax
	salq	$7, %rax
	andl	$2636928640, %eax
	xorq	%rax, %rbx
	movq	%rbx, %rax
	salq	$15, %rax
	andl	$4022730752, %eax
	xorq	%rax, %rbx
	movq	%rbx, %rax
	shrq	$18, %rax
	xorq	%rax, %rbx
	imulq	%r13, %rbx
	cmpl	%ebx, %r14d
	jbe	.L106
.L108:
	movq	%rcx, %rax
	cmpq	$623, %rcx
	jbe	.L107
	movq	%r12, %rdi
	call	_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EE11_M_gen_randEv
	movq	4992(%r12), %rax
	jmp	.L107
	.p2align 4,,10
	.p2align 3
.L106:
	shrq	$32, %rbx
	jmp	.L109
	.p2align 4,,10
	.p2align 3
.L110:
	movq	4992(%rdi), %rax
	cmpq	$623, %rax
	ja	.L122
.L115:
	movq	(%r12,%rax,8), %rbx
	leaq	1(%rax), %rdx
	movq	%rdx, 4992(%r12)
	movq	%rbx, %rax
	shrq	$11, %rax
	movl	%eax, %eax
	xorq	%rax, %rbx
	movq	%rbx, %rax
	salq	$7, %rax
	andl	$2636928640, %eax
	xorq	%rax, %rbx
	movq	%rbx, %rax
	salq	$15, %rax
	andl	$4022730752, %eax
	xorq	%rax, %rbx
	movq	%rbx, %rax
	shrq	$18, %rax
	xorq	%rax, %rbx
	jmp	.L109
	.p2align 4,,10
	.p2align 3
.L121:
	call	_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EE11_M_gen_randEv
	movq	4992(%r12), %rax
	jmp	.L105
	.p2align 4,,10
	.p2align 3
.L122:
	call	_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EE11_M_gen_randEv
	movq	4992(%r12), %rax
	jmp	.L115
	.cfi_endproc
.LFE4499:
	.size	_ZNSt24uniform_int_distributionIiEclISt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EEEEiRT_RKNS0_10param_typeE.isra.0, .-_ZNSt24uniform_int_distributionIiEclISt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EEEEiRT_RKNS0_10param_typeE.isra.0
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC4:
	.string	"vector::reserve"
.LC5:
	.string	"Elapsed: "
.LC6:
	.string	" [\316\274s]"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB7:
	.section	.text.startup,"ax",@progbits
.LHOTB7:
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB3557:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA3557
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$10136, %rsp
	.cfi_def_cfa_offset 10192
	leaq	112(%rsp), %rdi
.LEHB8:
	call	_ZNSt13random_deviceC1Ev
.LEHE8:
	leaq	112(%rsp), %rdi
.LEHB9:
	call	_ZNSt13random_device9_M_getvalEv
.LEHE9:
	movl	%eax, %ecx
	movl	$1, %edx
	movq	%rcx, 5120(%rsp)
	.p2align 4,,10
	.p2align 3
.L124:
	movq	%rcx, %rax
	shrq	$30, %rax
	xorq	%rcx, %rax
	imulq	$1812433253, %rax, %rax
	leal	(%rax,%rdx), %ecx
	movq	%rcx, 5120(%rsp,%rdx,8)
	addq	$1, %rdx
	cmpq	$624, %rdx
	jne	.L124
	pxor	%xmm0, %xmm0
	leaq	48(%rsp), %rax
	movl	$2400000, %edi
	movq	$624, 10112(%rsp)
	movq	$0, 64(%rsp)
	movq	%rax, 40(%rsp)
	movaps	%xmm0, 48(%rsp)
.LEHB10:
	call	_Znwm
.LEHE10:
	movq	%rax, %xmm0
	addq	$2400000, %rax
	movl	$100000, 36(%rsp)
	movq	%rax, 64(%rsp)
	leaq	48(%rsp), %rax
	punpcklqdq	%xmm0, %xmm0
	movq	%rax, 40(%rsp)
	movaps	%xmm0, 48(%rsp)
	.p2align 4,,10
	.p2align 3
.L150:
	pxor	%xmm0, %xmm0
	movl	$50, %edx
	movl	$5, %esi
	movq	$0, 96(%rsp)
	leaq	5120(%rsp), %rdi
	movaps	%xmm0, 80(%rsp)
	call	_ZNSt24uniform_int_distributionIiEclISt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EEEEiRT_RKNS0_10param_typeE.isra.0
	movslq	%eax, %r12
	movq	%r12, %r14
	testl	%r12d, %r12d
	js	.L210
	xorl	%r15d, %r15d
	testq	%r12, %r12
	jne	.L211
	movq	56(%rsp), %rbp
	cmpq	64(%rsp), %rbp
	je	.L142
	.p2align 4,,10
	.p2align 3
.L215:
	movq	80(%rsp), %r12
	pxor	%xmm0, %xmm0
	movq	$0, 16(%rbp)
	movups	%xmm0, 0(%rbp)
	subq	%r12, %r15
	je	.L143
	js	.L212
	movq	%r15, %rdi
.LEHB11:
	call	_Znwm
.LEHE11:
	movq	%rax, %xmm0
	leaq	(%rax,%r15), %rbx
	movq	%rax, %rdi
	punpcklqdq	%xmm0, %xmm0
	movq	%rbx, 16(%rbp)
	movups	%xmm0, 0(%rbp)
	testq	%r15, %r15
	je	.L145
	movq	%r15, %rdx
	movq	%r12, %rsi
	call	memmove
.L145:
	movq	%rbx, 8(%rbp)
	addq	$24, %rbp
	movq	%rbp, 56(%rsp)
.L146:
	testq	%r12, %r12
	je	.L147
	movq	96(%rsp), %rsi
	movq	%r12, %rdi
	subq	%r12, %rsi
	call	_ZdlPvm
	subl	$1, 36(%rsp)
	jne	.L150
.L149:
	xorl	%r13d, %r13d
.L160:
	call	_ZNSt6chrono3_V212system_clock3nowEv
	movq	56(%rsp), %r14
	movq	48(%rsp), %rbx
	movq	%rax, %r15
	jmp	.L151
.L154:
	movl	%edx, %esi
	movq	%rbp, %rdi
.LEHB12:
	call	*C.1.0(%r13)
.LEHE12:
	testq	%rbp, %rbp
	je	.L155
	movq	%r12, %rsi
	movq	%rbp, %rdi
	call	_ZdlPvm
.L155:
	addq	$24, %rbx
.L151:
	cmpq	%rbx, %r14
	je	.L213
	movq	8(%rbx), %r12
	subq	(%rbx), %r12
	je	.L171
	js	.L214
	movq	%r12, %rdi
.LEHB13:
	call	_Znwm
.LEHE13:
	movq	%rax, %rbp
.L152:
	movq	(%rbx), %rsi
	movq	8(%rbx), %rdx
	subq	%rsi, %rdx
	je	.L154
	movq	%rbp, %rdi
	movq	%rdx, 16(%rsp)
	call	memmove
	movq	16(%rsp), %rdx
	jmp	.L154
	.p2align 4,,10
	.p2align 3
.L147:
	subl	$1, 36(%rsp)
	jne	.L150
	jmp	.L149
	.p2align 4,,10
	.p2align 3
.L211:
	movq	%r12, %rdi
.LEHB14:
	call	_Znwm
	addq	%rax, %r12
	movq	%rax, %xmm0
	movq	%rax, %r15
	xorl	%ebp, %ebp
	movq	%r12, 96(%rsp)
	punpcklqdq	%xmm0, %xmm0
	movaps	%xmm0, 80(%rsp)
	.p2align 4,,10
	.p2align 3
.L141:
	movq	10112(%rsp), %rax
	cmpq	$623, %rax
	ja	.L209
	.p2align 4,,10
	.p2align 3
.L127:
	leaq	1(%rax), %rdx
	movq	%rdx, 10112(%rsp)
	movq	5120(%rsp,%rax,8), %rax
	movq	%rax, %rsi
	shrq	$11, %rsi
	movl	%esi, %esi
	xorq	%rsi, %rax
	movq	%rax, %rsi
	salq	$7, %rsi
	andl	$2636928640, %esi
	xorq	%rsi, %rax
	movq	%rax, %rsi
	salq	$15, %rsi
	andl	$4022730752, %esi
	xorq	%rsi, %rax
	movq	%rax, %rsi
	shrq	$18, %rsi
	xorq	%rsi, %rax
	movq	%rax, %rbx
	salq	$6, %rbx
	subq	%rax, %rbx
	cmpl	$3, %ebx
	ja	.L128
	movq	%rdx, %rax
	cmpq	$623, %rdx
	jbe	.L127
.L209:
	leaq	5120(%rsp), %rdi
	call	_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EE11_M_gen_randEv
	movq	10112(%rsp), %rax
	jmp	.L127
	.p2align 4,,10
	.p2align 3
.L128:
	shrq	$32, %rbx
	cmpq	%r15, %r12
	je	.L131
	movslq	%ebx, %rbx
	addq	$1, %r15
	movzbl	_ZL8alphanum(%rbx), %eax
	movb	%al, -1(%r15)
	movq	%r15, 88(%rsp)
.L132:
	addl	$1, %ebp
	cmpl	%r14d, %ebp
	jl	.L141
	movq	56(%rsp), %rbp
	cmpq	64(%rsp), %rbp
	jne	.L215
.L142:
	movq	40(%rsp), %rdi
	leaq	80(%rsp), %rdx
	movq	%rbp, %rsi
	call	_ZNSt6vectorIS_IcSaIcEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_
	movq	80(%rsp), %r12
	jmp	.L146
	.p2align 4,,10
	.p2align 3
.L131:
	movq	80(%rsp), %rax
	movq	%r12, %r13
	movq	%rax, 16(%rsp)
	subq	%rax, %r13
	movabsq	$9223372036854775807, %rax
	cmpq	%rax, %r13
	je	.L216
	testq	%r13, %r13
	movl	$1, %eax
	cmovne	%r13, %rax
	addq	%r13, %rax
	jc	.L135
	xorl	%r8d, %r8d
	xorl	%edi, %edi
	testq	%rax, %rax
	jne	.L217
.L137:
	movslq	%ebx, %rbx
	leaq	1(%rdi,%r13), %r15
	movq	%rdi, %xmm0
	movzbl	_ZL8alphanum(%rbx), %eax
	movq	%r15, %xmm1
	punpcklqdq	%xmm1, %xmm0
	movb	%al, (%rdi,%r13)
	testq	%r13, %r13
	jg	.L218
	cmpq	$0, 16(%rsp)
	jne	.L139
.L140:
	movq	%r8, 96(%rsp)
	movq	%r8, %r12
	movaps	%xmm0, 80(%rsp)
	jmp	.L132
	.p2align 4,,10
	.p2align 3
.L218:
	movq	16(%rsp), %rsi
	movq	%r13, %rdx
	movq	%r8, 24(%rsp)
	movaps	%xmm0, (%rsp)
	call	memmove
	movdqa	(%rsp), %xmm0
	movq	24(%rsp), %r8
.L139:
	movq	16(%rsp), %rdi
	movq	%r12, %rsi
	movq	%r8, 24(%rsp)
	movaps	%xmm0, (%rsp)
	subq	%rdi, %rsi
	call	_ZdlPvm
	movq	24(%rsp), %r8
	movdqa	(%rsp), %xmm0
	jmp	.L140
	.p2align 4,,10
	.p2align 3
.L217:
	movabsq	$9223372036854775807, %rdx
	cmpq	%rdx, %rax
	cmovbe	%rax, %rdx
	movq	%rdx, %rax
.L136:
	movq	%rax, %rdi
	movq	%rax, %r15
	call	_Znwm
	movq	%r15, %r8
	movq	%rax, %rdi
	addq	%rax, %r8
	jmp	.L137
	.p2align 4,,10
	.p2align 3
.L143:
	movq	$0, 0(%rbp)
	xorl	%ebx, %ebx
	movq	$0, 16(%rbp)
	jmp	.L145
.L212:
	call	_ZSt17__throw_bad_allocv
.LEHE14:
.L171:
	xorl	%ebp, %ebp
	jmp	.L152
.L213:
	call	_ZNSt6chrono3_V212system_clock3nowEv
	movl	$1000, %ecx
	movl	$.LC5, %esi
	movl	$_ZSt4cout, %edi
	subq	%r15, %rax
	cqto
	idivq	%rcx
	movl	$9, %edx
	movq	%rax, %rbp
.LEHB15:
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	movq	%rbp, %rsi
	movl	$_ZSt4cout, %edi
	call	_ZNSo9_M_insertIlEERSoT_
	movl	$6, %edx
	movl	$.LC6, %esi
	movq	%rax, %rdi
	movq	%rax, %rbp
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	movq	0(%rbp), %rax
	movq	-24(%rax), %rax
	movq	240(%rbp,%rax), %r12
	testq	%r12, %r12
	je	.L219
	cmpb	$0, 56(%r12)
	je	.L158
	movzbl	67(%r12), %eax
.L159:
	movsbl	%al, %esi
	movq	%rbp, %rdi
	call	_ZNSo3putEc
	movq	%rax, %rdi
	call	_ZNSo5flushEv
.LEHE15:
	addq	$8, %r13
	cmpq	$16, %r13
	jne	.L160
	movq	40(%rsp), %rdi
	call	_ZNSt6vectorIS_IcSaIcEESaIS1_EED1Ev
	leaq	112(%rsp), %rdi
	call	_ZNSt13random_device7_M_finiEv
	addq	$10136, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
.L214:
	.cfi_restore_state
.LEHB16:
	call	_ZSt17__throw_bad_allocv
.L158:
	movq	%r12, %rdi
	call	_ZNKSt5ctypeIcE13_M_widen_initEv
	movq	(%r12), %rax
	movq	48(%rax), %rdx
	movl	$10, %eax
	cmpq	$_ZNKSt5ctypeIcE8do_widenEc, %rdx
	je	.L159
	movl	$10, %esi
	movq	%r12, %rdi
	call	*%rdx
	jmp	.L159
.L219:
	call	_ZSt16__throw_bad_castv
.LEHE16:
.L135:
	movabsq	$9223372036854775807, %rax
	jmp	.L136
.L216:
	movl	$.LC3, %edi
.LEHB17:
	call	_ZSt20__throw_length_errorPKc
.L210:
	movl	$.LC4, %edi
	call	_ZSt20__throw_length_errorPKc
.LEHE17:
.L176:
	movq	%rax, %rbx
	jmp	.L164
.L175:
	movq	%rax, %rbx
	jmp	.L161
.L174:
	movq	%rax, %rbx
	jmp	.L163
.L173:
	movq	%rax, %rbx
	jmp	.L166
	.section	.gcc_except_table,"a",@progbits
.LLSDA3557:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE3557-.LLSDACSB3557
.LLSDACSB3557:
	.uleb128 .LEHB8-.LFB3557
	.uleb128 .LEHE8-.LEHB8
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB9-.LFB3557
	.uleb128 .LEHE9-.LEHB9
	.uleb128 .L173-.LFB3557
	.uleb128 0
	.uleb128 .LEHB10-.LFB3557
	.uleb128 .LEHE10-.LEHB10
	.uleb128 .L174-.LFB3557
	.uleb128 0
	.uleb128 .LEHB11-.LFB3557
	.uleb128 .LEHE11-.LEHB11
	.uleb128 .L175-.LFB3557
	.uleb128 0
	.uleb128 .LEHB12-.LFB3557
	.uleb128 .LEHE12-.LEHB12
	.uleb128 .L176-.LFB3557
	.uleb128 0
	.uleb128 .LEHB13-.LFB3557
	.uleb128 .LEHE13-.LEHB13
	.uleb128 .L174-.LFB3557
	.uleb128 0
	.uleb128 .LEHB14-.LFB3557
	.uleb128 .LEHE14-.LEHB14
	.uleb128 .L175-.LFB3557
	.uleb128 0
	.uleb128 .LEHB15-.LFB3557
	.uleb128 .LEHE15-.LEHB15
	.uleb128 .L174-.LFB3557
	.uleb128 0
	.uleb128 .LEHB16-.LFB3557
	.uleb128 .LEHE16-.LEHB16
	.uleb128 .L174-.LFB3557
	.uleb128 0
	.uleb128 .LEHB17-.LFB3557
	.uleb128 .LEHE17-.LEHB17
	.uleb128 .L175-.LFB3557
	.uleb128 0
.LLSDACSE3557:
	.section	.text.startup
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDAC3557
	.type	main.cold, @function
main.cold:
.LFSB3557:
.L164:
	.cfi_def_cfa_offset 10192
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	testq	%rbp, %rbp
	jne	.L220
.L163:
	movq	40(%rsp), %rdi
	call	_ZNSt6vectorIS_IcSaIcEESaIS1_EED1Ev
.L166:
	leaq	112(%rsp), %rdi
	call	_ZNSt13random_device7_M_finiEv
	movq	%rbx, %rdi
.LEHB18:
	call	_Unwind_Resume
.LEHE18:
.L161:
	movq	80(%rsp), %rdi
	movq	96(%rsp), %rsi
	subq	%rdi, %rsi
	testq	%rdi, %rdi
	je	.L163
	call	_ZdlPvm
	jmp	.L163
.L220:
	movq	%r12, %rsi
	movq	%rbp, %rdi
	call	_ZdlPvm
	jmp	.L163
	.cfi_endproc
.LFE3557:
	.section	.gcc_except_table
.LLSDAC3557:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSEC3557-.LLSDACSBC3557
.LLSDACSBC3557:
	.uleb128 .LEHB18-.LCOLDB7
	.uleb128 .LEHE18-.LEHB18
	.uleb128 0
	.uleb128 0
.LLSDACSEC3557:
	.section	.text.unlikely
	.section	.text.startup
	.size	main, .-main
	.section	.text.unlikely
	.size	main.cold, .-main.cold
.LCOLDE7:
	.section	.text.startup
.LHOTE7:
	.p2align 4
	.type	_GLOBAL__sub_I__Z16toupper_standardPci, @function
_GLOBAL__sub_I__Z16toupper_standardPci:
.LFB4491:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$_ZStL8__ioinit, %edi
	call	_ZNSt8ios_base4InitC1Ev
	movl	$__dso_handle, %edx
	movl	$_ZStL8__ioinit, %esi
	movl	$_ZNSt8ios_base4InitD1Ev, %edi
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	jmp	__cxa_atexit
	.cfi_endproc
.LFE4491:
	.size	_GLOBAL__sub_I__Z16toupper_standardPci, .-_GLOBAL__sub_I__Z16toupper_standardPci
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I__Z16toupper_standardPci
	.section	.rodata
	.align 16
	.type	C.1.0, @object
	.size	C.1.0, 16
C.1.0:
	.quad	_Z18toupper_branchlessPci
	.quad	_Z16toupper_standardPci
	.align 32
	.type	_ZL8alphanum, @object
	.size	_ZL8alphanum, 63
_ZL8alphanum:
	.string	"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC0:
	.byte	-97
	.byte	-97
	.byte	-97
	.byte	-97
	.byte	-97
	.byte	-97
	.byte	-97
	.byte	-97
	.byte	-97
	.byte	-97
	.byte	-97
	.byte	-97
	.byte	-97
	.byte	-97
	.byte	-97
	.byte	-97
	.align 16
.LC1:
	.byte	25
	.byte	25
	.byte	25
	.byte	25
	.byte	25
	.byte	25
	.byte	25
	.byte	25
	.byte	25
	.byte	25
	.byte	25
	.byte	25
	.byte	25
	.byte	25
	.byte	25
	.byte	25
	.align 16
.LC2:
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.byte	32
	.hidden	__dso_handle
	.ident	"GCC: (mk-2022.0) 11.2.0"
	.section	.note.GNU-stack,"",@progbits
