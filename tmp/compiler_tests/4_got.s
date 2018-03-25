	.nan  legacy
	.module fp=xx
	.module nooddspreg
	.abicalls
	.text
	.align  2
	.global add
	.ent  add
	.type  add, @function
add:

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
	sw $a2,112($sp)
	sw $a3,116($sp)
	lw $t0, 104($fp)
	lw $t1, 108($fp)
	lw $t2, 112($fp)
	lw $t3, 116($fp)
	lw $t4, 120($fp)
	lw $t5, 124($fp)
	addu $t4, $t4, $t5
	addu $t3, $t3, $t4
	addu $t2, $t2, $t3
	addu $t1, $t1, $t2
	addu $t0, $t0, $t1
	move $v0, $t0
	b $add
	nop 
	nop
$add:
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
	.end  add
	.size add, .-add

	.text
	.align  2
	.global driver
	.ent  driver
	.type  driver, @function
driver:

	.frame $fp,136,$31
	.mask 0x40000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set nomacro
	addiu $sp, $sp, -136
	sw $31,132($sp)
	sw $25,128($sp)
	sw $24,124($sp)
	sw $23,120($sp)
	sw $22,116($sp)
	sw $21,112($sp)
	sw $20,108($sp)
	sw $19,104($sp)
	sw $18,100($sp)
	sw $17,96($sp)
	sw $16,92($sp)
	sw $15,88($sp)
	sw $14,84($sp)
	sw $13,80($sp)
	sw $12,76($sp)
	sw $11,72($sp)
	sw $10,70($sp)
	sw $9,64($sp)
	sw $8,60($sp)
	sw $fp,56($sp)
	move $fp, $sp
	li $t0, 1
	sw $t0,48($fp)
	li $t0, 1
	sw $t0,44($fp)
	li $t0, 1
	sw $t0,40($fp)
	li $t0, 1
	sw $t0,36($fp)
	li $t0, 1
	sw $t0,32($fp)
	li $t0, 1
	sw $t0,28($fp)
	lw $t0, 48($fp)
	move $a0, $t0
	lw $t0, 44($fp)
	move $a1, $t0
	lw $t0, 40($fp)
	move $a2, $t0
	lw $t0, 36($fp)
	move $a3, $t0
	lw $t0, 32($fp)
	sw $t0,20($fp)
	lw $t0, 28($fp)
	sw $t0,24($fp)
	.option	pic0
	jal	add
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
	lw $31,132($sp)
	lw $25,128($sp)
	lw $24,124($sp)
	lw $23,120($sp)
	lw $22,116($sp)
	lw $21,112($sp)
	lw $20,108($sp)
	lw $19,104($sp)
	lw $18,100($sp)
	lw $17,96($sp)
	lw $16,92($sp)
	lw $15,88($sp)
	lw $14,84($sp)
	lw $13,80($sp)
	lw $12,76($sp)
	lw $11,72($sp)
	lw $10,70($sp)
	lw $9,64($sp)
	lw $8,60($sp)
	lw $fp,56($sp)
	addiu $sp, $sp,136
	jr $31
	nop
	.set  macro
	.set  reorder
	.end  driver
	.size driver, .-driver

