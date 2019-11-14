	.file	"linux_freestanding_number_output.cc"
	.text
	.section	.rodata._ZNK7fast_io3eof4whatEv.str1.1,"aMS",@progbits,1
.LC0:
	.string	"EOF"
	.section	.text._ZNK7fast_io3eof4whatEv,"axG",@progbits,_ZNK7fast_io3eof4whatEv,comdat
	.align 2
	.p2align 4
	.weak	_ZNK7fast_io3eof4whatEv
	.type	_ZNK7fast_io3eof4whatEv, @function
_ZNK7fast_io3eof4whatEv:
.LFB4071:
	.cfi_startproc
	movl	$.LC0, %eax
	ret
	.cfi_endproc
.LFE4071:
	.size	_ZNK7fast_io3eof4whatEv, .-_ZNK7fast_io3eof4whatEv
	.section	.text._ZN7fast_io3eofD2Ev,"axG",@progbits,_ZN7fast_io3eofD5Ev,comdat
	.align 2
	.p2align 4
	.weak	_ZN7fast_io3eofD2Ev
	.type	_ZN7fast_io3eofD2Ev, @function
_ZN7fast_io3eofD2Ev:
.LFB4784:
	.cfi_startproc
	movq	$_ZTVN7fast_io3eofE+16, (%rdi)
	jmp	_ZNSt9exceptionD2Ev
	.cfi_endproc
.LFE4784:
	.size	_ZN7fast_io3eofD2Ev, .-_ZN7fast_io3eofD2Ev
	.weak	_ZN7fast_io3eofD1Ev
	.set	_ZN7fast_io3eofD1Ev,_ZN7fast_io3eofD2Ev
	.section	.text._ZN7fast_io3eofD0Ev,"axG",@progbits,_ZN7fast_io3eofD5Ev,comdat
	.align 2
	.p2align 4
	.weak	_ZN7fast_io3eofD0Ev
	.type	_ZN7fast_io3eofD0Ev, @function
_ZN7fast_io3eofD0Ev:
.LFB4786:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rdi, %rbp
	movq	$_ZTVN7fast_io3eofE+16, (%rdi)
	call	_ZNSt9exceptionD2Ev
	movq	%rbp, %rdi
	movl	$8, %esi
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	_ZdlPvm
	.cfi_endproc
.LFE4786:
	.size	_ZN7fast_io3eofD0Ev, .-_ZN7fast_io3eofD0Ev
	.section	.text.unlikely,"ax",@progbits
.LCOLDB2:
	.text
.LHOTB2:
	.p2align 4
	.globl	_start
	.type	_start, @function
_start:
.LFB4339:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA4339
	subq	$10008, %rsp
	.cfi_def_cfa_offset 10016
	movdqa	.LC1(%rip), %xmm0
	movabsq	$753052655351312434, %rax
	movq	%rax, 16(%rsp)
	leaq	25(%rsp), %rcx
	movq	%rsp, %rax
	movb	$10, 24(%rsp)
	movups	%xmm0, (%rsp)
	jmp	.L7
	.p2align 4,,10
	.p2align 3
.L27:
	subl	$48, %edi
	cmpb	$9, %dil
	jbe	.L26
	cmpq	%rcx, %rax
	je	.L24
.L7:
	movzbl	(%rax), %edi
	movq	%rcx, %rsi
	addq	$1, %rax
	movq	%rax, %rdx
	subq	%rax, %rsi
	cmpb	$45, %dil
	jne	.L27
	xorl	%edi, %edi
	testq	%rsi, %rsi
	je	.L12
	movl	$1, %r8d
	.p2align 4,,10
	.p2align 3
.L14:
	movzbl	(%rdx), %eax
	subl	$48, %eax
	cmpb	$9, %al
	ja	.L13
	leal	(%rdi,%rdi,4), %esi
	movzbl	%al, %eax
	addq	$1, %rdx
	leal	(%rax,%rsi,2), %edi
	cmpq	%rdx, %rcx
	jne	.L14
.L13:
	testb	%r8b, %r8b
	je	.L10
.L12:
	negl	%edi
.L10:
	call	_Exit
.L26:
	movzbl	%dil, %edi
	xorl	%r8d, %r8d
	testq	%rsi, %rsi
	jne	.L14
	jmp	.L10
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA4339:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE4339-.LLSDACSB4339
.LLSDACSB4339:
.LLSDACSE4339:
	.text
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDAC4339
	.type	_start.cold, @function
_start.cold:
.LFSB4339:
.L24:
	.cfi_def_cfa_offset 10016
	movl	$8, %edi
	call	__cxa_allocate_exception
	movl	$_ZN7fast_io3eofD1Ev, %edx
	movl	$_ZTIN7fast_io3eofE, %esi
	movq	$_ZTVN7fast_io3eofE+16, (%rax)
	movq	%rax, %rdi
	call	__cxa_throw
	.cfi_endproc
.LFE4339:
	.section	.gcc_except_table
.LLSDAC4339:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSEC4339-.LLSDACSBC4339
.LLSDACSBC4339:
.LLSDACSEC4339:
	.section	.text.unlikely
	.text
	.size	_start, .-_start
	.section	.text.unlikely
	.size	_start.cold, .-_start.cold
.LCOLDE2:
	.text
.LHOTE2:
	.weak	_ZTSN7fast_io3eofE
	.section	.rodata._ZTSN7fast_io3eofE,"aG",@progbits,_ZTSN7fast_io3eofE,comdat
	.align 8
	.type	_ZTSN7fast_io3eofE, @object
	.size	_ZTSN7fast_io3eofE, 15
_ZTSN7fast_io3eofE:
	.string	"N7fast_io3eofE"
	.weak	_ZTIN7fast_io3eofE
	.section	.rodata._ZTIN7fast_io3eofE,"aG",@progbits,_ZTIN7fast_io3eofE,comdat
	.align 8
	.type	_ZTIN7fast_io3eofE, @object
	.size	_ZTIN7fast_io3eofE, 24
_ZTIN7fast_io3eofE:
	.quad	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	_ZTSN7fast_io3eofE
	.quad	_ZTISt9exception
	.weak	_ZTVN7fast_io3eofE
	.section	.rodata._ZTVN7fast_io3eofE,"aG",@progbits,_ZTVN7fast_io3eofE,comdat
	.align 8
	.type	_ZTVN7fast_io3eofE, @object
	.size	_ZTVN7fast_io3eofE, 40
_ZTVN7fast_io3eofE:
	.quad	0
	.quad	_ZTIN7fast_io3eofE
	.quad	_ZN7fast_io3eofD1Ev
	.quad	_ZN7fast_io3eofD0Ev
	.quad	_ZNK7fast_io3eof4whatEv
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC1:
	.quad	7522264139816006497
	.quad	3560209233811170409
	.ident	"GCC: (cqwrteur) 10.0.0 20191105 (experimental)"
	.section	.note.GNU-stack,"",@progbits
