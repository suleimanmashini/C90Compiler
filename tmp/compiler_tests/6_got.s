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
	lui	$28,%hi(__gnu_local_gp)
	addiu	$28,$28,%lo(__gnu_local_gp)
	li $t0, 7
	sw $t0,20($fp)
	li $t0, 5
	lw $t1,%got(a)($28)
	sw $t0,0($t1)
	lw $t0,%got(a)($28)
	lw $t0,                              0($t0)
	move $v0, $t0
	b $test
	nop 
	nop
$test:
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
	.end  test
	.size test, .-test
