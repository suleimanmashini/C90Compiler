	.file	1 "uhm.c"
	.section .mdebug.abi32
	.previous
	.nan	legacy
	.module	fp=xx
	.module	nooddspreg
	.abicalls
	.text
	.align	2
	.globl	uhm
	.set	nomips16
	.set	nomicromips
	.ent	uhm
	.type	uhm, @function
uhm:
	.frame	$fp,32,$31		# vars= 24, regs= 1/0, args= 0, gp= 0
	.mask	0x40000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$fp,28($sp)
	move	$fp,$sp
	li	$2,5			# 0x5
	sw	$2,4($fp)
	li	$2,4			# 0x4
	sw	$2,8($fp)
	li	$2,5			# 0x5
	sw	$2,12($fp)
	li	$2,2			# 0x2
	sw	$2,16($fp)
	li	$2,1			# 0x1
	sw	$2,20($fp)
	nop
	move	$sp,$fp
	lw	$fp,28($sp)
	addiu	$sp,$sp,32
	jr	$31
	nop

	.set	macro
	.set	reorder
	.end	uhm
	.size	uhm, .-uhm
	.align	2
	.globl	ok
	.set	nomips16
	.set	nomicromips
	.ent	ok
	.type	ok, @function
ok:
	.frame	$fp,24,$31		# vars= 16, regs= 1/0, args= 0, gp= 0
	.mask	0x40000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$fp,20($sp)
	move	$fp,$sp
	li	$2,2			# 0x2
	sw	$2,4($fp)
	li	$2,2			# 0x2
	sw	$2,8($fp)
	lw	$2,4($fp)
	beq	$2,$0,$L4
	nop

	lw	$2,8($fp)
	beq	$2,$0,$L4
	nop

	li	$2,1			# 0x1
	.option	pic0
	b	$L5
	nop

	.option	pic2
$L4:
	move	$2,$0
$L5:
	sw	$2,12($fp)
	nop
	move	$sp,$fp
	lw	$fp,20($sp)
	addiu	$sp,$sp,24
	jr	$31
	nop

	.set	macro
	.set	reorder
	.end	ok
	.size	ok, .-ok
	.ident	"GCC: (Ubuntu 7.2.0-6ubuntu1) 7.2.0"
