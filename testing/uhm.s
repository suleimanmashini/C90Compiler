	.file	1 "uhm.c"
	.section .mdebug.abi32
	.previous
	.nan	legacy
	.module	fp=xx
	.module	nooddspreg
	.abicalls
	.text
	.align	2
	.globl	ugh
	.set	nomips16
	.set	nomicromips
	.ent	ugh
	.type	ugh, @function
ugh:
	.frame	$fp,24,$31		# vars= 16, regs= 1/0, args= 0, gp= 0
	.mask	0x40000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$fp,20($sp)
	move	$fp,$sp
	li	$2,5			# 0x5
	sw	$2,4($fp)
	li	$2,17			# 0x11
	sw	$2,8($fp)
	lw	$3,4($fp)
	lw	$2,8($fp)
	addu	$2,$3,$2
	sw	$2,12($fp)
	nop
	move	$sp,$fp
	lw	$fp,20($sp)
	addiu	$sp,$sp,24
	jr	$31
	nop

	.set	macro
	.set	reorder
	.end	ugh
	.size	ugh, .-ugh
	.ident	"GCC: (Ubuntu 7.2.0-6ubuntu1) 7.2.0"
