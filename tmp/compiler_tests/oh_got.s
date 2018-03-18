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

	.frame $fp,40,$31
	.mask 0x40000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set nomacro
	addiu $sp, $sp, -40
	sw $31,36($sp)
	sw $23,32($sp)
	sw $22,28($sp)
	sw $21,24($sp)
	sw $20,20($sp)
	sw $19,16($sp)
	sw $18,12($sp)
	sw $17,8($sp)
	sw $16,4($sp)
	sw $fp,0($sp)
	move $fp, $sp
	li $v0, 1
	nop 
	nop
	move $sp, $fp
	lw $31,36($sp)
	lw $23,32($sp)
	lw $22,28($sp)
	lw $21,24($sp)
	lw $20,20($sp)
	lw $19,16($sp)
	lw $18,12($sp)
	lw $17,8($sp)
	lw $16,4($sp)
	lw $fp,0($sp)
	addiu $sp, $sp,40
	jr $31
	nop
	.set  macro
	.set  reorder
	.end  what
	.size what, .-what

