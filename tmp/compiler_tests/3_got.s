	.nan  legacy
	.module fp=xx
	.module nooddspreg
	.abicalls
	.text
	.align  2
	.global sub
	.ent  sub
	.type  sub, @function
sub:

	.frame $fp,104,$31
	.mask 0x40000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set nomacro
	addiu $sp, $sp, -104
	sw $31,100($sp)
	sw $25,96($sp)
	sw $24,92($sp)
	sw $23,88($sp)
	sw $22,84($sp)
	sw $21,80($sp)
	sw $20,76($sp)
	sw $19,72($sp)
	sw $18,68($sp)
	sw $17,64($sp)
	sw $16,60($sp)
	sw $15,56($sp)
	sw $14,52($sp)
	sw $13,48($sp)
	sw $12,44($sp)
	sw $11,40($sp)
	sw $10,38($sp)
	sw $9,32($sp)
	sw $8,28($sp)
	sw $fp,24($sp)
	move $fp, $sp
	sw $a0,104($sp)
	sw $a1,108($sp)
	lw $t0, 104($fp)
	lw $t1, 108($fp)
	subu $t0, $t0, $t1
	move $v0, $t0
	b $sub
	nop 
	nop
$sub:
	move $sp, $fp
	lw $31,100($sp)
	lw $25,96($sp)
	lw $24,92($sp)
	lw $23,88($sp)
	lw $22,84($sp)
	lw $21,80($sp)
	lw $20,76($sp)
	lw $19,72($sp)
	lw $18,68($sp)
	lw $17,64($sp)
	lw $16,60($sp)
	lw $15,56($sp)
	lw $14,52($sp)
	lw $13,48($sp)
	lw $12,44($sp)
	lw $11,40($sp)
	lw $10,38($sp)
	lw $9,32($sp)
	lw $8,28($sp)
	lw $fp,24($sp)
	addiu $sp, $sp,104
	jr $31
	nop
	.set  macro
	.set  reorder
	.end  sub
	.size sub, .-sub

	.text
	.align  2
	.global driver
	.ent  driver
	.type  driver, @function
driver:

	.frame $fp,112,$31
	.mask 0x40000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set nomacro
	addiu $sp, $sp, -112
	sw $31,108($sp)
	sw $25,104($sp)
	sw $24,100($sp)
	sw $23,96($sp)
	sw $22,92($sp)
	sw $21,88($sp)
	sw $20,84($sp)
	sw $19,80($sp)
	sw $18,76($sp)
	sw $17,72($sp)
	sw $16,68($sp)
	sw $15,64($sp)
	sw $14,60($sp)
	sw $13,56($sp)
	sw $12,52($sp)
	sw $11,48($sp)
	sw $10,46($sp)
	sw $9,40($sp)
	sw $8,36($sp)
	sw $fp,32($sp)
	move $fp, $sp
	li $t0, 5
	sw $t0,24($fp)
	li $t0, 2
	sw $t0,20($fp)
	lw $t0, 24($fp)
	move $a0, $t0
	lw $t0, 20($fp)
	move $a1, $t0
	.option	pic0
	jal	sub
	nop	

	.option pic2

	nop
	move $t0,$v0
	move $v0, $t0
	b $driver
	nop 
	nop
$driver:
	move $sp, $fp
	lw $31,108($sp)
	lw $25,104($sp)
	lw $24,100($sp)
	lw $23,96($sp)
	lw $22,92($sp)
	lw $21,88($sp)
	lw $20,84($sp)
	lw $19,80($sp)
	lw $18,76($sp)
	lw $17,72($sp)
	lw $16,68($sp)
	lw $15,64($sp)
	lw $14,60($sp)
	lw $13,56($sp)
	lw $12,52($sp)
	lw $11,48($sp)
	lw $10,46($sp)
	lw $9,40($sp)
	lw $8,36($sp)
	lw $fp,32($sp)
	addiu $sp, $sp,112
	jr $31
	nop
	.set  macro
	.set  reorder
	.end  driver
	.size driver, .-driver

