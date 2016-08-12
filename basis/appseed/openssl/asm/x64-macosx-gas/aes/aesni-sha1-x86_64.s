.text



.globl	_aesni_cbc_sha1_enc

.p2align	4
_aesni_cbc_sha1_enc:

	movl	_OPENSSL_ia32cap_P+0(%rip),%r10d
	movl	_OPENSSL_ia32cap_P+4(%rip),%r11d
	jmp	aesni_cbc_sha1_enc_ssse3
	.byte	0xf3,0xc3


.p2align	4
aesni_cbc_sha1_enc_ssse3:
	movq	8(%rsp),%r10


	pushq	%rbx
	pushq	%rbp
	pushq	%r12
	pushq	%r13
	pushq	%r14
	pushq	%r15
	leaq	-104(%rsp),%rsp


	movq	%rdi,%r12
	movq	%rsi,%r13
	movq	%rdx,%r14
	movq	%rcx,%r15
	movdqu	(%r8),%xmm11
	movq	%r8,88(%rsp)
	shlq	$6,%r14
	subq	%r12,%r13
	movl	240(%r15),%r8d
	addq	%r10,%r14

	leaq	K_XX_XX(%rip),%r11
	movl	0(%r9),%eax
	movl	4(%r9),%ebx
	movl	8(%r9),%ecx
	movl	12(%r9),%edx
	movl	%ebx,%esi
	movl	16(%r9),%ebp

	movdqa	64(%r11),%xmm6
	movdqa	0(%r11),%xmm9
	movdqu	0(%r10),%xmm0
	movdqu	16(%r10),%xmm1
	movdqu	32(%r10),%xmm2
	movdqu	48(%r10),%xmm3
.byte	102,15,56,0,198
	addq	$64,%r10
.byte	102,15,56,0,206
.byte	102,15,56,0,214
.byte	102,15,56,0,222
	paddd	%xmm9,%xmm0
	paddd	%xmm9,%xmm1
	paddd	%xmm9,%xmm2
	movdqa	%xmm0,0(%rsp)
	psubd	%xmm9,%xmm0
	movdqa	%xmm1,16(%rsp)
	psubd	%xmm9,%xmm1
	movdqa	%xmm2,32(%rsp)
	psubd	%xmm9,%xmm2
	movups	(%r15),%xmm13
	movups	16(%r15),%xmm14
	jmp	L$oop_ssse3
.p2align	4
L$oop_ssse3:
	movdqa	%xmm1,%xmm4
	addl	0(%rsp),%ebp
	movups	0(%r12),%xmm12
	xorps	%xmm13,%xmm12
	xorps	%xmm12,%xmm11
.byte	102,69,15,56,220,222
	movups	32(%r15),%xmm15
	xorl	%edx,%ecx
	movdqa	%xmm3,%xmm8
.byte	102,15,58,15,224,8
	movl	%eax,%edi
	roll	$5,%eax
	paddd	%xmm3,%xmm9
	andl	%ecx,%esi
	xorl	%edx,%ecx
	psrldq	$4,%xmm8
	xorl	%edx,%esi
	addl	%eax,%ebp
	pxor	%xmm0,%xmm4
	rorl	$2,%ebx
	addl	%esi,%ebp
	pxor	%xmm2,%xmm8
	addl	4(%rsp),%edx
	xorl	%ecx,%ebx
	movl	%ebp,%esi
	roll	$5,%ebp
	pxor	%xmm8,%xmm4
	andl	%ebx,%edi
	xorl	%ecx,%ebx
	movdqa	%xmm9,48(%rsp)
	xorl	%ecx,%edi
.byte	102,69,15,56,220,223
	movups	48(%r15),%xmm14
	addl	%ebp,%edx
	movdqa	%xmm4,%xmm10
	movdqa	%xmm4,%xmm8
	rorl	$7,%eax
	addl	%edi,%edx
	addl	8(%rsp),%ecx
	xorl	%ebx,%eax
	pslldq	$12,%xmm10
	paddd	%xmm4,%xmm4
	movl	%edx,%edi
	roll	$5,%edx
	andl	%eax,%esi
	xorl	%ebx,%eax
	psrld	$31,%xmm8
	xorl	%ebx,%esi
	addl	%edx,%ecx
	movdqa	%xmm10,%xmm9
	rorl	$7,%ebp
	addl	%esi,%ecx
	psrld	$30,%xmm10
	por	%xmm8,%xmm4
	addl	12(%rsp),%ebx
	xorl	%eax,%ebp
	movl	%ecx,%esi
	roll	$5,%ecx
.byte	102,69,15,56,220,222
	movups	64(%r15),%xmm15
	pslld	$2,%xmm9
	pxor	%xmm10,%xmm4
	andl	%ebp,%edi
	xorl	%eax,%ebp
	movdqa	0(%r11),%xmm10
	xorl	%eax,%edi
	addl	%ecx,%ebx
	pxor	%xmm9,%xmm4
	rorl	$7,%edx
	addl	%edi,%ebx
	movdqa	%xmm2,%xmm5
	addl	16(%rsp),%eax
	xorl	%ebp,%edx
	movdqa	%xmm4,%xmm9
.byte	102,15,58,15,233,8
	movl	%ebx,%edi
	roll	$5,%ebx
	paddd	%xmm4,%xmm10
	andl	%edx,%esi
	xorl	%ebp,%edx
	psrldq	$4,%xmm9
	xorl	%ebp,%esi
	addl	%ebx,%eax
	pxor	%xmm1,%xmm5
	rorl	$7,%ecx
	addl	%esi,%eax
	pxor	%xmm3,%xmm9
	addl	20(%rsp),%ebp
.byte	102,69,15,56,220,223
	movups	80(%r15),%xmm14
	xorl	%edx,%ecx
	movl	%eax,%esi
	roll	$5,%eax
	pxor	%xmm9,%xmm5
	andl	%ecx,%edi
	xorl	%edx,%ecx
	movdqa	%xmm10,0(%rsp)
	xorl	%edx,%edi
	addl	%eax,%ebp
	movdqa	%xmm5,%xmm8
	movdqa	%xmm5,%xmm9
	rorl	$7,%ebx
	addl	%edi,%ebp
	addl	24(%rsp),%edx
	xorl	%ecx,%ebx
	pslldq	$12,%xmm8
	paddd	%xmm5,%xmm5
	movl	%ebp,%edi
	roll	$5,%ebp
	andl	%ebx,%esi
	xorl	%ecx,%ebx
	psrld	$31,%xmm9
	xorl	%ecx,%esi
.byte	102,69,15,56,220,222
	movups	96(%r15),%xmm15
	addl	%ebp,%edx
	movdqa	%xmm8,%xmm10
	rorl	$7,%eax
	addl	%esi,%edx
	psrld	$30,%xmm8
	por	%xmm9,%xmm5
	addl	28(%rsp),%ecx
	xorl	%ebx,%eax
	movl	%edx,%esi
	roll	$5,%edx
	pslld	$2,%xmm10
	pxor	%xmm8,%xmm5
	andl	%eax,%edi
	xorl	%ebx,%eax
	movdqa	16(%r11),%xmm8
	xorl	%ebx,%edi
	addl	%edx,%ecx
	pxor	%xmm10,%xmm5
	rorl	$7,%ebp
	addl	%edi,%ecx
	movdqa	%xmm3,%xmm6
	addl	32(%rsp),%ebx
	xorl	%eax,%ebp
	movdqa	%xmm5,%xmm10
.byte	102,15,58,15,242,8
	movl	%ecx,%edi
	roll	$5,%ecx
.byte	102,69,15,56,220,223
	movups	112(%r15),%xmm14
	paddd	%xmm5,%xmm8
	andl	%ebp,%esi
	xorl	%eax,%ebp
	psrldq	$4,%xmm10
	xorl	%eax,%esi
	addl	%ecx,%ebx
	pxor	%xmm2,%xmm6
	rorl	$7,%edx
	addl	%esi,%ebx
	pxor	%xmm4,%xmm10
	addl	36(%rsp),%eax
	xorl	%ebp,%edx
	movl	%ebx,%esi
	roll	$5,%ebx
	pxor	%xmm10,%xmm6
	andl	%edx,%edi
	xorl	%ebp,%edx
	movdqa	%xmm8,16(%rsp)
	xorl	%ebp,%edi
	addl	%ebx,%eax
	movdqa	%xmm6,%xmm9
	movdqa	%xmm6,%xmm10
	rorl	$7,%ecx
	addl	%edi,%eax
	addl	40(%rsp),%ebp
.byte	102,69,15,56,220,222
	movups	128(%r15),%xmm15
	xorl	%edx,%ecx
	pslldq	$12,%xmm9
	paddd	%xmm6,%xmm6
	movl	%eax,%edi
	roll	$5,%eax
	andl	%ecx,%esi
	xorl	%edx,%ecx
	psrld	$31,%xmm10
	xorl	%edx,%esi
	addl	%eax,%ebp
	movdqa	%xmm9,%xmm8
	rorl	$7,%ebx
	addl	%esi,%ebp
	psrld	$30,%xmm9
	por	%xmm10,%xmm6
	addl	44(%rsp),%edx
	xorl	%ecx,%ebx
	movl	%ebp,%esi
	roll	$5,%ebp
	pslld	$2,%xmm8
	pxor	%xmm9,%xmm6
	andl	%ebx,%edi
	xorl	%ecx,%ebx
	movdqa	16(%r11),%xmm9
	xorl	%ecx,%edi
.byte	102,69,15,56,220,223
	movups	144(%r15),%xmm14
	addl	%ebp,%edx
	pxor	%xmm8,%xmm6
	rorl	$7,%eax
	addl	%edi,%edx
	movdqa	%xmm4,%xmm7
	addl	48(%rsp),%ecx
	xorl	%ebx,%eax
	movdqa	%xmm6,%xmm8
.byte	102,15,58,15,251,8
	movl	%edx,%edi
	roll	$5,%edx
	paddd	%xmm6,%xmm9
	andl	%eax,%esi
	xorl	%ebx,%eax
	psrldq	$4,%xmm8
	xorl	%ebx,%esi
	addl	%edx,%ecx
	pxor	%xmm3,%xmm7
	rorl	$7,%ebp
	addl	%esi,%ecx
	pxor	%xmm5,%xmm8
	addl	52(%rsp),%ebx
	xorl	%eax,%ebp
	movl	%ecx,%esi
	roll	$5,%ecx
.byte	102,69,15,56,220,222
	movups	160(%r15),%xmm15
	pxor	%xmm8,%xmm7
	andl	%ebp,%edi
	xorl	%eax,%ebp
	movdqa	%xmm9,32(%rsp)
	xorl	%eax,%edi
	addl	%ecx,%ebx
	movdqa	%xmm7,%xmm10
	movdqa	%xmm7,%xmm8
	rorl	$7,%edx
	addl	%edi,%ebx
	addl	56(%rsp),%eax
	xorl	%ebp,%edx
	pslldq	$12,%xmm10
	paddd	%xmm7,%xmm7
	movl	%ebx,%edi
	roll	$5,%ebx
	andl	%edx,%esi
	xorl	%ebp,%edx
	psrld	$31,%xmm8
	xorl	%ebp,%esi
	addl	%ebx,%eax
	movdqa	%xmm10,%xmm9
	rorl	$7,%ecx
	addl	%esi,%eax
	psrld	$30,%xmm10
	por	%xmm8,%xmm7
	addl	60(%rsp),%ebp
	cmpl	$11,%r8d
	jb	L$aesenclast1
	movups	176(%r15),%xmm14
.byte	102,69,15,56,220,223
	movups	192(%r15),%xmm15
.byte	102,69,15,56,220,222
	je	L$aesenclast1
	movups	208(%r15),%xmm14
.byte	102,69,15,56,220,223
	movups	224(%r15),%xmm15
.byte	102,69,15,56,220,222
L$aesenclast1:
.byte	102,69,15,56,221,223
	movups	16(%r15),%xmm14
	xorl	%edx,%ecx
	movl	%eax,%esi
	roll	$5,%eax
	pslld	$2,%xmm9
	pxor	%xmm10,%xmm7
	andl	%ecx,%edi
	xorl	%edx,%ecx
	movdqa	16(%r11),%xmm10
	xorl	%edx,%edi
	addl	%eax,%ebp
	pxor	%xmm9,%xmm7
	rorl	$7,%ebx
	addl	%edi,%ebp
	movdqa	%xmm7,%xmm9
	addl	0(%rsp),%edx
	pxor	%xmm4,%xmm0
.byte	102,68,15,58,15,206,8
	xorl	%ecx,%ebx
	movl	%ebp,%edi
	roll	$5,%ebp
	pxor	%xmm1,%xmm0
	andl	%ebx,%esi
	xorl	%ecx,%ebx
	movdqa	%xmm10,%xmm8
	paddd	%xmm7,%xmm10
	xorl	%ecx,%esi
	movups	16(%r12),%xmm12
	xorps	%xmm13,%xmm12
	movups	%xmm11,0(%r13,%r12,1)
	xorps	%xmm12,%xmm11
.byte	102,69,15,56,220,222
	movups	32(%r15),%xmm15
	addl	%ebp,%edx
	pxor	%xmm9,%xmm0
	rorl	$7,%eax
	addl	%esi,%edx
	addl	4(%rsp),%ecx
	xorl	%ebx,%eax
	movdqa	%xmm0,%xmm9
	movdqa	%xmm10,48(%rsp)
	movl	%edx,%esi
	roll	$5,%edx
	andl	%eax,%edi
	xorl	%ebx,%eax
	pslld	$2,%xmm0
	xorl	%ebx,%edi
	addl	%edx,%ecx
	psrld	$30,%xmm9
	rorl	$7,%ebp
	addl	%edi,%ecx
	addl	8(%rsp),%ebx
	xorl	%eax,%ebp
	movl	%ecx,%edi
	roll	$5,%ecx
.byte	102,69,15,56,220,223
	movups	48(%r15),%xmm14
	por	%xmm9,%xmm0
	andl	%ebp,%esi
	xorl	%eax,%ebp
	movdqa	%xmm0,%xmm10
	xorl	%eax,%esi
	addl	%ecx,%ebx
	rorl	$7,%edx
	addl	%esi,%ebx
	addl	12(%rsp),%eax
	xorl	%ebp,%edx
	movl	%ebx,%esi
	roll	$5,%ebx
	andl	%edx,%edi
	xorl	%ebp,%edx
	xorl	%ebp,%edi
	addl	%ebx,%eax
	rorl	$7,%ecx
	addl	%edi,%eax
	addl	16(%rsp),%ebp
.byte	102,69,15,56,220,222
	movups	64(%r15),%xmm15
	pxor	%xmm5,%xmm1
.byte	102,68,15,58,15,215,8
	xorl	%edx,%esi
	movl	%eax,%edi
	roll	$5,%eax
	pxor	%xmm2,%xmm1
	xorl	%ecx,%esi
	addl	%eax,%ebp
	movdqa	%xmm8,%xmm9
	paddd	%xmm0,%xmm8
	rorl	$7,%ebx
	addl	%esi,%ebp
	pxor	%xmm10,%xmm1
	addl	20(%rsp),%edx
	xorl	%ecx,%edi
	movl	%ebp,%esi
	roll	$5,%ebp
	movdqa	%xmm1,%xmm10
	movdqa	%xmm8,0(%rsp)
	xorl	%ebx,%edi
	addl	%ebp,%edx
	rorl	$7,%eax
	addl	%edi,%edx
	pslld	$2,%xmm1
	addl	24(%rsp),%ecx
	xorl	%ebx,%esi
	psrld	$30,%xmm10
	movl	%edx,%edi
	roll	$5,%edx
	xorl	%eax,%esi
.byte	102,69,15,56,220,223
	movups	80(%r15),%xmm14
	addl	%edx,%ecx
	rorl	$7,%ebp
	addl	%esi,%ecx
	por	%xmm10,%xmm1
	addl	28(%rsp),%ebx
	xorl	%eax,%edi
	movdqa	%xmm1,%xmm8
	movl	%ecx,%esi
	roll	$5,%ecx
	xorl	%ebp,%edi
	addl	%ecx,%ebx
	rorl	$7,%edx
	addl	%edi,%ebx
	addl	32(%rsp),%eax
	pxor	%xmm6,%xmm2
.byte	102,68,15,58,15,192,8
	xorl	%ebp,%esi
	movl	%ebx,%edi
	roll	$5,%ebx
	pxor	%xmm3,%xmm2
	xorl	%edx,%esi
	addl	%ebx,%eax
	movdqa	32(%r11),%xmm10
	paddd	%xmm1,%xmm9
	rorl	$7,%ecx
	addl	%esi,%eax
	pxor	%xmm8,%xmm2
	addl	36(%rsp),%ebp
.byte	102,69,15,56,220,222
	movups	96(%r15),%xmm15
	xorl	%edx,%edi
	movl	%eax,%esi
	roll	$5,%eax
	movdqa	%xmm2,%xmm8
	movdqa	%xmm9,16(%rsp)
	xorl	%ecx,%edi
	addl	%eax,%ebp
	rorl	$7,%ebx
	addl	%edi,%ebp
	pslld	$2,%xmm2
	addl	40(%rsp),%edx
	xorl	%ecx,%esi
	psrld	$30,%xmm8
	movl	%ebp,%edi
	roll	$5,%ebp
	xorl	%ebx,%esi
	addl	%ebp,%edx
	rorl	$7,%eax
	addl	%esi,%edx
	por	%xmm8,%xmm2
	addl	44(%rsp),%ecx
	xorl	%ebx,%edi
	movdqa	%xmm2,%xmm9
	movl	%edx,%esi
	roll	$5,%edx
	xorl	%eax,%edi
.byte	102,69,15,56,220,223
	movups	112(%r15),%xmm14
	addl	%edx,%ecx
	rorl	$7,%ebp
	addl	%edi,%ecx
	addl	48(%rsp),%ebx
	pxor	%xmm7,%xmm3
.byte	102,68,15,58,15,201,8
	xorl	%eax,%esi
	movl	%ecx,%edi
	roll	$5,%ecx
	pxor	%xmm4,%xmm3
	xorl	%ebp,%esi
	addl	%ecx,%ebx
	movdqa	%xmm10,%xmm8
	paddd	%xmm2,%xmm10
	rorl	$7,%edx
	addl	%esi,%ebx
	pxor	%xmm9,%xmm3
	addl	52(%rsp),%eax
	xorl	%ebp,%edi
	movl	%ebx,%esi
	roll	$5,%ebx
	movdqa	%xmm3,%xmm9
	movdqa	%xmm10,32(%rsp)
	xorl	%edx,%edi
	addl	%ebx,%eax
	rorl	$7,%ecx
	addl	%edi,%eax
	pslld	$2,%xmm3
	addl	56(%rsp),%ebp
.byte	102,69,15,56,220,222
	movups	128(%r15),%xmm15
	xorl	%edx,%esi
	psrld	$30,%xmm9
	movl	%eax,%edi
	roll	$5,%eax
	xorl	%ecx,%esi
	addl	%eax,%ebp
	rorl	$7,%ebx
	addl	%esi,%ebp
	por	%xmm9,%xmm3
	addl	60(%rsp),%edx
	xorl	%ecx,%edi
	movdqa	%xmm3,%xmm10
	movl	%ebp,%esi
	roll	$5,%ebp
	xorl	%ebx,%edi
	addl	%ebp,%edx
	rorl	$7,%eax
	addl	%edi,%edx
	addl	0(%rsp),%ecx
	pxor	%xmm0,%xmm4
.byte	102,68,15,58,15,210,8
	xorl	%ebx,%esi
	movl	%edx,%edi
	roll	$5,%edx
	pxor	%xmm5,%xmm4
	xorl	%eax,%esi
.byte	102,69,15,56,220,223
	movups	144(%r15),%xmm14
	addl	%edx,%ecx
	movdqa	%xmm8,%xmm9
	paddd	%xmm3,%xmm8
	rorl	$7,%ebp
	addl	%esi,%ecx
	pxor	%xmm10,%xmm4
	addl	4(%rsp),%ebx
	xorl	%eax,%edi
	movl	%ecx,%esi
	roll	$5,%ecx
	movdqa	%xmm4,%xmm10
	movdqa	%xmm8,48(%rsp)
	xorl	%ebp,%edi
	addl	%ecx,%ebx
	rorl	$7,%edx
	addl	%edi,%ebx
	pslld	$2,%xmm4
	addl	8(%rsp),%eax
	xorl	%ebp,%esi
	psrld	$30,%xmm10
	movl	%ebx,%edi
	roll	$5,%ebx
	xorl	%edx,%esi
	addl	%ebx,%eax
	rorl	$7,%ecx
	addl	%esi,%eax
	por	%xmm10,%xmm4
	addl	12(%rsp),%ebp
.byte	102,69,15,56,220,222
	movups	160(%r15),%xmm15
	xorl	%edx,%edi
	movdqa	%xmm4,%xmm8
	movl	%eax,%esi
	roll	$5,%eax
	xorl	%ecx,%edi
	addl	%eax,%ebp
	rorl	$7,%ebx
	addl	%edi,%ebp
	addl	16(%rsp),%edx
	pxor	%xmm1,%xmm5
.byte	102,68,15,58,15,195,8
	xorl	%ecx,%esi
	movl	%ebp,%edi
	roll	$5,%ebp
	pxor	%xmm6,%xmm5
	xorl	%ebx,%esi
	addl	%ebp,%edx
	movdqa	%xmm9,%xmm10
	paddd	%xmm4,%xmm9
	rorl	$7,%eax
	addl	%esi,%edx
	pxor	%xmm8,%xmm5
	addl	20(%rsp),%ecx
	xorl	%ebx,%edi
	movl	%edx,%esi
	roll	$5,%edx
	movdqa	%xmm5,%xmm8
	movdqa	%xmm9,0(%rsp)
	xorl	%eax,%edi
	cmpl	$11,%r8d
	jb	L$aesenclast2
	movups	176(%r15),%xmm14
.byte	102,69,15,56,220,223
	movups	192(%r15),%xmm15
.byte	102,69,15,56,220,222
	je	L$aesenclast2
	movups	208(%r15),%xmm14
.byte	102,69,15,56,220,223
	movups	224(%r15),%xmm15
.byte	102,69,15,56,220,222
L$aesenclast2:
.byte	102,69,15,56,221,223
	movups	16(%r15),%xmm14
	addl	%edx,%ecx
	rorl	$7,%ebp
	addl	%edi,%ecx
	pslld	$2,%xmm5
	addl	24(%rsp),%ebx
	xorl	%eax,%esi
	psrld	$30,%xmm8
	movl	%ecx,%edi
	roll	$5,%ecx
	xorl	%ebp,%esi
	addl	%ecx,%ebx
	rorl	$7,%edx
	addl	%esi,%ebx
	por	%xmm8,%xmm5
	addl	28(%rsp),%eax
	xorl	%ebp,%edi
	movdqa	%xmm5,%xmm9
	movl	%ebx,%esi
	roll	$5,%ebx
	xorl	%edx,%edi
	addl	%ebx,%eax
	rorl	$7,%ecx
	addl	%edi,%eax
	movl	%ecx,%edi
	movups	32(%r12),%xmm12
	xorps	%xmm13,%xmm12
	movups	%xmm11,16(%r13,%r12,1)
	xorps	%xmm12,%xmm11
.byte	102,69,15,56,220,222
	movups	32(%r15),%xmm15
	pxor	%xmm2,%xmm6
.byte	102,68,15,58,15,204,8
	xorl	%edx,%ecx
	addl	32(%rsp),%ebp
	andl	%edx,%edi
	pxor	%xmm7,%xmm6
	andl	%ecx,%esi
	rorl	$7,%ebx
	movdqa	%xmm10,%xmm8
	paddd	%xmm5,%xmm10
	addl	%edi,%ebp
	movl	%eax,%edi
	pxor	%xmm9,%xmm6
	roll	$5,%eax
	addl	%esi,%ebp
	xorl	%edx,%ecx
	addl	%eax,%ebp
	movdqa	%xmm6,%xmm9
	movdqa	%xmm10,16(%rsp)
	movl	%ebx,%esi
	xorl	%ecx,%ebx
	addl	36(%rsp),%edx
	andl	%ecx,%esi
	pslld	$2,%xmm6
	andl	%ebx,%edi
	rorl	$7,%eax
	psrld	$30,%xmm9
	addl	%esi,%edx
	movl	%ebp,%esi
	roll	$5,%ebp
.byte	102,69,15,56,220,223
	movups	48(%r15),%xmm14
	addl	%edi,%edx
	xorl	%ecx,%ebx
	addl	%ebp,%edx
	por	%xmm9,%xmm6
	movl	%eax,%edi
	xorl	%ebx,%eax
	movdqa	%xmm6,%xmm10
	addl	40(%rsp),%ecx
	andl	%ebx,%edi
	andl	%eax,%esi
	rorl	$7,%ebp
	addl	%edi,%ecx
	movl	%edx,%edi
	roll	$5,%edx
	addl	%esi,%ecx
	xorl	%ebx,%eax
	addl	%edx,%ecx
	movl	%ebp,%esi
	xorl	%eax,%ebp
	addl	44(%rsp),%ebx
	andl	%eax,%esi
	andl	%ebp,%edi
.byte	102,69,15,56,220,222
	movups	64(%r15),%xmm15
	rorl	$7,%edx
	addl	%esi,%ebx
	movl	%ecx,%esi
	roll	$5,%ecx
	addl	%edi,%ebx
	xorl	%eax,%ebp
	addl	%ecx,%ebx
	movl	%edx,%edi
	pxor	%xmm3,%xmm7
.byte	102,68,15,58,15,213,8
	xorl	%ebp,%edx
	addl	48(%rsp),%eax
	andl	%ebp,%edi
	pxor	%xmm0,%xmm7
	andl	%edx,%esi
	rorl	$7,%ecx
	movdqa	48(%r11),%xmm9
	paddd	%xmm6,%xmm8
	addl	%edi,%eax
	movl	%ebx,%edi
	pxor	%xmm10,%xmm7
	roll	$5,%ebx
	addl	%esi,%eax
	xorl	%ebp,%edx
	addl	%ebx,%eax
	movdqa	%xmm7,%xmm10
	movdqa	%xmm8,32(%rsp)
	movl	%ecx,%esi
.byte	102,69,15,56,220,223
	movups	80(%r15),%xmm14
	xorl	%edx,%ecx
	addl	52(%rsp),%ebp
	andl	%edx,%esi
	pslld	$2,%xmm7
	andl	%ecx,%edi
	rorl	$7,%ebx
	psrld	$30,%xmm10
	addl	%esi,%ebp
	movl	%eax,%esi
	roll	$5,%eax
	addl	%edi,%ebp
	xorl	%edx,%ecx
	addl	%eax,%ebp
	por	%xmm10,%xmm7
	movl	%ebx,%edi
	xorl	%ecx,%ebx
	movdqa	%xmm7,%xmm8
	addl	56(%rsp),%edx
	andl	%ecx,%edi
	andl	%ebx,%esi
	rorl	$7,%eax
	addl	%edi,%edx
	movl	%ebp,%edi
	roll	$5,%ebp
.byte	102,69,15,56,220,222
	movups	96(%r15),%xmm15
	addl	%esi,%edx
	xorl	%ecx,%ebx
	addl	%ebp,%edx
	movl	%eax,%esi
	xorl	%ebx,%eax
	addl	60(%rsp),%ecx
	andl	%ebx,%esi
	andl	%eax,%edi
	rorl	$7,%ebp
	addl	%esi,%ecx
	movl	%edx,%esi
	roll	$5,%edx
	addl	%edi,%ecx
	xorl	%ebx,%eax
	addl	%edx,%ecx
	movl	%ebp,%edi
	pxor	%xmm4,%xmm0
.byte	102,68,15,58,15,198,8
	xorl	%eax,%ebp
	addl	0(%rsp),%ebx
	andl	%eax,%edi
	pxor	%xmm1,%xmm0
	andl	%ebp,%esi
.byte	102,69,15,56,220,223
	movups	112(%r15),%xmm14
	rorl	$7,%edx
	movdqa	%xmm9,%xmm10
	paddd	%xmm7,%xmm9
	addl	%edi,%ebx
	movl	%ecx,%edi
	pxor	%xmm8,%xmm0
	roll	$5,%ecx
	addl	%esi,%ebx
	xorl	%eax,%ebp
	addl	%ecx,%ebx
	movdqa	%xmm0,%xmm8
	movdqa	%xmm9,48(%rsp)
	movl	%edx,%esi
	xorl	%ebp,%edx
	addl	4(%rsp),%eax
	andl	%ebp,%esi
	pslld	$2,%xmm0
	andl	%edx,%edi
	rorl	$7,%ecx
	psrld	$30,%xmm8
	addl	%esi,%eax
	movl	%ebx,%esi
	roll	$5,%ebx
	addl	%edi,%eax
	xorl	%ebp,%edx
	addl	%ebx,%eax
	por	%xmm8,%xmm0
	movl	%ecx,%edi
.byte	102,69,15,56,220,222
	movups	128(%r15),%xmm15
	xorl	%edx,%ecx
	movdqa	%xmm0,%xmm9
	addl	8(%rsp),%ebp
	andl	%edx,%edi
	andl	%ecx,%esi
	rorl	$7,%ebx
	addl	%edi,%ebp
	movl	%eax,%edi
	roll	$5,%eax
	addl	%esi,%ebp
	xorl	%edx,%ecx
	addl	%eax,%ebp
	movl	%ebx,%esi
	xorl	%ecx,%ebx
	addl	12(%rsp),%edx
	andl	%ecx,%esi
	andl	%ebx,%edi
	rorl	$7,%eax
	addl	%esi,%edx
	movl	%ebp,%esi
	roll	$5,%ebp
.byte	102,69,15,56,220,223
	movups	144(%r15),%xmm14
	addl	%edi,%edx
	xorl	%ecx,%ebx
	addl	%ebp,%edx
	movl	%eax,%edi
	pxor	%xmm5,%xmm1
.byte	102,68,15,58,15,207,8
	xorl	%ebx,%eax
	addl	16(%rsp),%ecx
	andl	%ebx,%edi
	pxor	%xmm2,%xmm1
	andl	%eax,%esi
	rorl	$7,%ebp
	movdqa	%xmm10,%xmm8
	paddd	%xmm0,%xmm10
	addl	%edi,%ecx
	movl	%edx,%edi
	pxor	%xmm9,%xmm1
	roll	$5,%edx
	addl	%esi,%ecx
	xorl	%ebx,%eax
	addl	%edx,%ecx
	movdqa	%xmm1,%xmm9
	movdqa	%xmm10,0(%rsp)
	movl	%ebp,%esi
	xorl	%eax,%ebp
	addl	20(%rsp),%ebx
	andl	%eax,%esi
	pslld	$2,%xmm1
	andl	%ebp,%edi
.byte	102,69,15,56,220,222
	movups	160(%r15),%xmm15
	rorl	$7,%edx
	psrld	$30,%xmm9
	addl	%esi,%ebx
	movl	%ecx,%esi
	roll	$5,%ecx
	addl	%edi,%ebx
	xorl	%eax,%ebp
	addl	%ecx,%ebx
	por	%xmm9,%xmm1
	movl	%edx,%edi
	xorl	%ebp,%edx
	movdqa	%xmm1,%xmm10
	addl	24(%rsp),%eax
	andl	%ebp,%edi
	andl	%edx,%esi
	rorl	$7,%ecx
	addl	%edi,%eax
	movl	%ebx,%edi
	roll	$5,%ebx
	addl	%esi,%eax
	xorl	%ebp,%edx
	addl	%ebx,%eax
	movl	%ecx,%esi
	cmpl	$11,%r8d
	jb	L$aesenclast3
	movups	176(%r15),%xmm14
.byte	102,69,15,56,220,223
	movups	192(%r15),%xmm15
.byte	102,69,15,56,220,222
	je	L$aesenclast3
	movups	208(%r15),%xmm14
.byte	102,69,15,56,220,223
	movups	224(%r15),%xmm15
.byte	102,69,15,56,220,222
L$aesenclast3:
.byte	102,69,15,56,221,223
	movups	16(%r15),%xmm14
	xorl	%edx,%ecx
	addl	28(%rsp),%ebp
	andl	%edx,%esi
	andl	%ecx,%edi
	rorl	$7,%ebx
	addl	%esi,%ebp
	movl	%eax,%esi
	roll	$5,%eax
	addl	%edi,%ebp
	xorl	%edx,%ecx
	addl	%eax,%ebp
	movl	%ebx,%edi
	pxor	%xmm6,%xmm2
.byte	102,68,15,58,15,208,8
	xorl	%ecx,%ebx
	addl	32(%rsp),%edx
	andl	%ecx,%edi
	pxor	%xmm3,%xmm2
	andl	%ebx,%esi
	rorl	$7,%eax
	movdqa	%xmm8,%xmm9
	paddd	%xmm1,%xmm8
	addl	%edi,%edx
	movl	%ebp,%edi
	pxor	%xmm10,%xmm2
	roll	$5,%ebp
	movups	48(%r12),%xmm12
	xorps	%xmm13,%xmm12
	movups	%xmm11,32(%r13,%r12,1)
	xorps	%xmm12,%xmm11
.byte	102,69,15,56,220,222
	movups	32(%r15),%xmm15
	addl	%esi,%edx
	xorl	%ecx,%ebx
	addl	%ebp,%edx
	movdqa	%xmm2,%xmm10
	movdqa	%xmm8,16(%rsp)
	movl	%eax,%esi
	xorl	%ebx,%eax
	addl	36(%rsp),%ecx
	andl	%ebx,%esi
	pslld	$2,%xmm2
	andl	%eax,%edi
	rorl	$7,%ebp
	psrld	$30,%xmm10
	addl	%esi,%ecx
	movl	%edx,%esi
	roll	$5,%edx
	addl	%edi,%ecx
	xorl	%ebx,%eax
	addl	%edx,%ecx
	por	%xmm10,%xmm2
	movl	%ebp,%edi
	xorl	%eax,%ebp
	movdqa	%xmm2,%xmm8
	addl	40(%rsp),%ebx
	andl	%eax,%edi
	andl	%ebp,%esi
.byte	102,69,15,56,220,223
	movups	48(%r15),%xmm14
	rorl	$7,%edx
	addl	%edi,%ebx
	movl	%ecx,%edi
	roll	$5,%ecx
	addl	%esi,%ebx
	xorl	%eax,%ebp
	addl	%ecx,%ebx
	movl	%edx,%esi
	xorl	%ebp,%edx
	addl	44(%rsp),%eax
	andl	%ebp,%esi
	andl	%edx,%edi
	rorl	$7,%ecx
	addl	%esi,%eax
	movl	%ebx,%esi
	roll	$5,%ebx
	addl	%edi,%eax
	xorl	%ebp,%edx
	addl	%ebx,%eax
	addl	48(%rsp),%ebp
.byte	102,69,15,56,220,222
	movups	64(%r15),%xmm15
	pxor	%xmm7,%xmm3
.byte	102,68,15,58,15,193,8
	xorl	%edx,%esi
	movl	%eax,%edi
	roll	$5,%eax
	pxor	%xmm4,%xmm3
	xorl	%ecx,%esi
	addl	%eax,%ebp
	movdqa	%xmm9,%xmm10
	paddd	%xmm2,%xmm9
	rorl	$7,%ebx
	addl	%esi,%ebp
	pxor	%xmm8,%xmm3
	addl	52(%rsp),%edx
	xorl	%ecx,%edi
	movl	%ebp,%esi
	roll	$5,%ebp
	movdqa	%xmm3,%xmm8
	movdqa	%xmm9,32(%rsp)
	xorl	%ebx,%edi
	addl	%ebp,%edx
	rorl	$7,%eax
	addl	%edi,%edx
	pslld	$2,%xmm3
	addl	56(%rsp),%ecx
	xorl	%ebx,%esi
	psrld	$30,%xmm8
	movl	%edx,%edi
	roll	$5,%edx
	xorl	%eax,%esi
.byte	102,69,15,56,220,223
	movups	80(%r15),%xmm14
	addl	%edx,%ecx
	rorl	$7,%ebp
	addl	%esi,%ecx
	por	%xmm8,%xmm3
	addl	60(%rsp),%ebx
	xorl	%eax,%edi
	movl	%ecx,%esi
	roll	$5,%ecx
	xorl	%ebp,%edi
	addl	%ecx,%ebx
	rorl	$7,%edx
	addl	%edi,%ebx
	addl	0(%rsp),%eax
	paddd	%xmm3,%xmm10
	xorl	%ebp,%esi
	movl	%ebx,%edi
	roll	$5,%ebx
	xorl	%edx,%esi
	movdqa	%xmm10,48(%rsp)
	addl	%ebx,%eax
	rorl	$7,%ecx
	addl	%esi,%eax
	addl	4(%rsp),%ebp
.byte	102,69,15,56,220,222
	movups	96(%r15),%xmm15
	xorl	%edx,%edi
	movl	%eax,%esi
	roll	$5,%eax
	xorl	%ecx,%edi
	addl	%eax,%ebp
	rorl	$7,%ebx
	addl	%edi,%ebp
	addl	8(%rsp),%edx
	xorl	%ecx,%esi
	movl	%ebp,%edi
	roll	$5,%ebp
	xorl	%ebx,%esi
	addl	%ebp,%edx
	rorl	$7,%eax
	addl	%esi,%edx
	addl	12(%rsp),%ecx
	xorl	%ebx,%edi
	movl	%edx,%esi
	roll	$5,%edx
	xorl	%eax,%edi
.byte	102,69,15,56,220,223
	movups	112(%r15),%xmm14
	addl	%edx,%ecx
	rorl	$7,%ebp
	addl	%edi,%ecx
	cmpq	%r14,%r10
	je	L$done_ssse3
	movdqa	64(%r11),%xmm6
	movdqa	0(%r11),%xmm9
	movdqu	0(%r10),%xmm0
	movdqu	16(%r10),%xmm1
	movdqu	32(%r10),%xmm2
	movdqu	48(%r10),%xmm3
.byte	102,15,56,0,198
	addq	$64,%r10
	addl	16(%rsp),%ebx
	xorl	%eax,%esi
.byte	102,15,56,0,206
	movl	%ecx,%edi
	roll	$5,%ecx
	paddd	%xmm9,%xmm0
	xorl	%ebp,%esi
	addl	%ecx,%ebx
	rorl	$7,%edx
	addl	%esi,%ebx
	movdqa	%xmm0,0(%rsp)
	addl	20(%rsp),%eax
	xorl	%ebp,%edi
	psubd	%xmm9,%xmm0
	movl	%ebx,%esi
	roll	$5,%ebx
	xorl	%edx,%edi
	addl	%ebx,%eax
	rorl	$7,%ecx
	addl	%edi,%eax
	addl	24(%rsp),%ebp
.byte	102,69,15,56,220,222
	movups	128(%r15),%xmm15
	xorl	%edx,%esi
	movl	%eax,%edi
	roll	$5,%eax
	xorl	%ecx,%esi
	addl	%eax,%ebp
	rorl	$7,%ebx
	addl	%esi,%ebp
	addl	28(%rsp),%edx
	xorl	%ecx,%edi
	movl	%ebp,%esi
	roll	$5,%ebp
	xorl	%ebx,%edi
	addl	%ebp,%edx
	rorl	$7,%eax
	addl	%edi,%edx
	addl	32(%rsp),%ecx
	xorl	%ebx,%esi
.byte	102,15,56,0,214
	movl	%edx,%edi
	roll	$5,%edx
	paddd	%xmm9,%xmm1
	xorl	%eax,%esi
.byte	102,69,15,56,220,223
	movups	144(%r15),%xmm14
	addl	%edx,%ecx
	rorl	$7,%ebp
	addl	%esi,%ecx
	movdqa	%xmm1,16(%rsp)
	addl	36(%rsp),%ebx
	xorl	%eax,%edi
	psubd	%xmm9,%xmm1
	movl	%ecx,%esi
	roll	$5,%ecx
	xorl	%ebp,%edi
	addl	%ecx,%ebx
	rorl	$7,%edx
	addl	%edi,%ebx
	addl	40(%rsp),%eax
	xorl	%ebp,%esi
	movl	%ebx,%edi
	roll	$5,%ebx
	xorl	%edx,%esi
	addl	%ebx,%eax
	rorl	$7,%ecx
	addl	%esi,%eax
	addl	44(%rsp),%ebp
.byte	102,69,15,56,220,222
	movups	160(%r15),%xmm15
	xorl	%edx,%edi
	movl	%eax,%esi
	roll	$5,%eax
	xorl	%ecx,%edi
	addl	%eax,%ebp
	rorl	$7,%ebx
	addl	%edi,%ebp
	addl	48(%rsp),%edx
	xorl	%ecx,%esi
.byte	102,15,56,0,222
	movl	%ebp,%edi
	roll	$5,%ebp
	paddd	%xmm9,%xmm2
	xorl	%ebx,%esi
	addl	%ebp,%edx
	rorl	$7,%eax
	addl	%esi,%edx
	movdqa	%xmm2,32(%rsp)
	addl	52(%rsp),%ecx
	xorl	%ebx,%edi
	psubd	%xmm9,%xmm2
	movl	%edx,%esi
	roll	$5,%edx
	xorl	%eax,%edi
	cmpl	$11,%r8d
	jb	L$aesenclast4
	movups	176(%r15),%xmm14
.byte	102,69,15,56,220,223
	movups	192(%r15),%xmm15
.byte	102,69,15,56,220,222
	je	L$aesenclast4
	movups	208(%r15),%xmm14
.byte	102,69,15,56,220,223
	movups	224(%r15),%xmm15
.byte	102,69,15,56,220,222
L$aesenclast4:
.byte	102,69,15,56,221,223
	movups	16(%r15),%xmm14
	addl	%edx,%ecx
	rorl	$7,%ebp
	addl	%edi,%ecx
	addl	56(%rsp),%ebx
	xorl	%eax,%esi
	movl	%ecx,%edi
	roll	$5,%ecx
	xorl	%ebp,%esi
	addl	%ecx,%ebx
	rorl	$7,%edx
	addl	%esi,%ebx
	addl	60(%rsp),%eax
	xorl	%ebp,%edi
	movl	%ebx,%esi
	roll	$5,%ebx
	xorl	%edx,%edi
	addl	%ebx,%eax
	rorl	$7,%ecx
	addl	%edi,%eax
	movups	%xmm11,48(%r13,%r12,1)
	leaq	64(%r12),%r12

	addl	0(%r9),%eax
	addl	4(%r9),%esi
	addl	8(%r9),%ecx
	addl	12(%r9),%edx
	movl	%eax,0(%r9)
	addl	16(%r9),%ebp
	movl	%esi,4(%r9)
	movl	%esi,%ebx
	movl	%ecx,8(%r9)
	movl	%edx,12(%r9)
	movl	%ebp,16(%r9)
	jmp	L$oop_ssse3

.p2align	4
L$done_ssse3:
	addl	16(%rsp),%ebx
	xorl	%eax,%esi
	movl	%ecx,%edi
	roll	$5,%ecx
	xorl	%ebp,%esi
	addl	%ecx,%ebx
	rorl	$7,%edx
	addl	%esi,%ebx
	addl	20(%rsp),%eax
	xorl	%ebp,%edi
	movl	%ebx,%esi
	roll	$5,%ebx
	xorl	%edx,%edi
	addl	%ebx,%eax
	rorl	$7,%ecx
	addl	%edi,%eax
	addl	24(%rsp),%ebp
.byte	102,69,15,56,220,222
	movups	128(%r15),%xmm15
	xorl	%edx,%esi
	movl	%eax,%edi
	roll	$5,%eax
	xorl	%ecx,%esi
	addl	%eax,%ebp
	rorl	$7,%ebx
	addl	%esi,%ebp
	addl	28(%rsp),%edx
	xorl	%ecx,%edi
	movl	%ebp,%esi
	roll	$5,%ebp
	xorl	%ebx,%edi
	addl	%ebp,%edx
	rorl	$7,%eax
	addl	%edi,%edx
	addl	32(%rsp),%ecx
	xorl	%ebx,%esi
	movl	%edx,%edi
	roll	$5,%edx
	xorl	%eax,%esi
.byte	102,69,15,56,220,223
	movups	144(%r15),%xmm14
	addl	%edx,%ecx
	rorl	$7,%ebp
	addl	%esi,%ecx
	addl	36(%rsp),%ebx
	xorl	%eax,%edi
	movl	%ecx,%esi
	roll	$5,%ecx
	xorl	%ebp,%edi
	addl	%ecx,%ebx
	rorl	$7,%edx
	addl	%edi,%ebx
	addl	40(%rsp),%eax
	xorl	%ebp,%esi
	movl	%ebx,%edi
	roll	$5,%ebx
	xorl	%edx,%esi
	addl	%ebx,%eax
	rorl	$7,%ecx
	addl	%esi,%eax
	addl	44(%rsp),%ebp
.byte	102,69,15,56,220,222
	movups	160(%r15),%xmm15
	xorl	%edx,%edi
	movl	%eax,%esi
	roll	$5,%eax
	xorl	%ecx,%edi
	addl	%eax,%ebp
	rorl	$7,%ebx
	addl	%edi,%ebp
	addl	48(%rsp),%edx
	xorl	%ecx,%esi
	movl	%ebp,%edi
	roll	$5,%ebp
	xorl	%ebx,%esi
	addl	%ebp,%edx
	rorl	$7,%eax
	addl	%esi,%edx
	addl	52(%rsp),%ecx
	xorl	%ebx,%edi
	movl	%edx,%esi
	roll	$5,%edx
	xorl	%eax,%edi
	cmpl	$11,%r8d
	jb	L$aesenclast5
	movups	176(%r15),%xmm14
.byte	102,69,15,56,220,223
	movups	192(%r15),%xmm15
.byte	102,69,15,56,220,222
	je	L$aesenclast5
	movups	208(%r15),%xmm14
.byte	102,69,15,56,220,223
	movups	224(%r15),%xmm15
.byte	102,69,15,56,220,222
L$aesenclast5:
.byte	102,69,15,56,221,223
	movups	16(%r15),%xmm14
	addl	%edx,%ecx
	rorl	$7,%ebp
	addl	%edi,%ecx
	addl	56(%rsp),%ebx
	xorl	%eax,%esi
	movl	%ecx,%edi
	roll	$5,%ecx
	xorl	%ebp,%esi
	addl	%ecx,%ebx
	rorl	$7,%edx
	addl	%esi,%ebx
	addl	60(%rsp),%eax
	xorl	%ebp,%edi
	movl	%ebx,%esi
	roll	$5,%ebx
	xorl	%edx,%edi
	addl	%ebx,%eax
	rorl	$7,%ecx
	addl	%edi,%eax
	movups	%xmm11,48(%r13,%r12,1)
	movq	88(%rsp),%r8

	addl	0(%r9),%eax
	addl	4(%r9),%esi
	addl	8(%r9),%ecx
	movl	%eax,0(%r9)
	addl	12(%r9),%edx
	movl	%esi,4(%r9)
	addl	16(%r9),%ebp
	movl	%ecx,8(%r9)
	movl	%edx,12(%r9)
	movl	%ebp,16(%r9)
	movups	%xmm11,(%r8)
	leaq	104(%rsp),%rsi
	movq	0(%rsi),%r15
	movq	8(%rsi),%r14
	movq	16(%rsi),%r13
	movq	24(%rsi),%r12
	movq	32(%rsi),%rbp
	movq	40(%rsi),%rbx
	leaq	48(%rsi),%rsp
L$epilogue_ssse3:
	.byte	0xf3,0xc3

.p2align	6
K_XX_XX:
.long	0x5a827999,0x5a827999,0x5a827999,0x5a827999

.long	0x6ed9eba1,0x6ed9eba1,0x6ed9eba1,0x6ed9eba1

.long	0x8f1bbcdc,0x8f1bbcdc,0x8f1bbcdc,0x8f1bbcdc

.long	0xca62c1d6,0xca62c1d6,0xca62c1d6,0xca62c1d6

.long	0x00010203,0x04050607,0x08090a0b,0x0c0d0e0f


.byte	65,69,83,78,73,45,67,66,67,43,83,72,65,49,32,115,116,105,116,99,104,32,102,111,114,32,120,56,54,95,54,52,44,32,67,82,89,80,84,79,71,65,77,83,32,98,121,32,60,97,112,112,114,111,64,111,112,101,110,115,115,108,46,111,114,103,62,0
.p2align	6
