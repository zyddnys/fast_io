	.file	"linux_freestanding_compile_test.cc"
	.text
	.p2align 4
	.globl	_start
	.type	_start, @function
_start:
.LFB4339:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	xorl	%edi, %edi
	call	_Exit
	.cfi_endproc
.LFE4339:
	.size	_start, .-_start
	.ident	"GCC: (cqwrteur) 10.0.0 20191105 (experimental)"
	.section	.note.GNU-stack,"",@progbits
