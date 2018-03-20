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

	.frame $fp,96,$31
	.mask 0x40000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set nomacro
	addiu $sp, $sp, -96
	sw $31,92($sp)
	sw $25,88($sp)
	sw $24,84($sp)
	sw $23,80($sp)
	sw $22,76($sp)
	sw $21,72($sp)
	sw $20,68($sp)
	sw $19,64($sp)
	sw $18,60($sp)
	sw $17,56($sp)
	sw $16,52($sp)
	sw $15,48($sp)
	sw $14,44($sp)
	sw $13,40($sp)
	sw $12,36($sp)
	sw $11,32($sp)
	sw $10,30($sp)
	sw $9,24($sp)
	sw $8,20($sp)
	sw $fp,16($sp)
	move $fp, $sp
	li $t0, 1
	move $v0, $t0
	nop 
	nop
	move $sp, $fp
	lw $31,92($sp)
	lw $25,88($sp)
	lw $24,84($sp)
	lw $23,80($sp)
	lw $22,76($sp)
	lw $21,72($sp)
	lw $20,68($sp)
	lw $19,64($sp)
	lw $18,60($sp)
	lw $17,56($sp)
	lw $16,52($sp)
	lw $15,48($sp)
	lw $14,44($sp)
	lw $13,40($sp)
	lw $12,36($sp)
	lw $11,32($sp)
	lw $10,30($sp)
	lw $9,24($sp)
	lw $8,20($sp)
	lw $fp,16($sp)
	addiu $sp, $sp,96
	jr $31
	nop
	.set  macro
	.set  reorder
	.end  what
	.size what, .-what

