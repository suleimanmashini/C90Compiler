	.file	1 "MAIN_driver.c"
	.section .mdebug.abi32
	.previous
	.nan	legacy
	.module	fp=xx
	.module	nooddspreg
	.abicalls
	.text
	.align	2
	.globl	notmain
	.set	nomips16
	.set	nomicromips
	.ent	notmain
	.type	notmain, @function
notmain:
	.frame	$fp,8,$31		# vars= 0, regs= 1/0, args= 0, gp= 0
	.mask	0x40000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$fp,4($sp)
	move	$fp,$sp
	li	$2,1			# 0x1
	move	$sp,$fp
	lw	$fp,4($sp)
	addiu	$sp,$sp,8
	jr	$31
	nop

	.set	macro
	.set	reorder
	.end	notmain
	.size	notmain, .-notmain
	.ident	"GCC: (Ubuntu 7.2.0-6ubuntu1) 7.2.0"
