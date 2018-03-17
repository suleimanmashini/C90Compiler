	.nan  legacy
	.module fp=xx
	.module nooddspreg
	.abicalls
	.text
	.align  2
	.global what
	.ent  what
	.type  what, @function
what:

	.frame $fp,8,$31
	.mask 0x40000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set nomacro
	addiu $sp, $sp, -8
	sw $fp,4($sp)
	move $fp, $sp
	li $v0, 1
	nop 
	nop
	move $sp, $fp
	lw $fp,4($sp)
	addiu $sp, $sp,8
	jr $31
	nop
	.set  macro
	.set  reorder
	.end  what
	.size what, .-what

