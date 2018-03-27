	.nan  legacy
	.module fp=xx
	.module nooddspreg
	.abicalls
	.comm a,4,4

	.text
	.align  2
	.global test
	.ent  test
	.type  test, @function
test:

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
	lui	$28,%hi(__gnu_local_gp)
	addiu	$28,$28,%lo(__gnu_local_gp)
	li $t0, 5
	lw $t1,%got(a)($28)
	sw $t0,0($t1)
	li $t0, 7
	sw $t0,20($fp)
	li $t0, 8
	sw $t0,20($fp)
	nop
$test:
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
	.end  test
	.size test, .-test

