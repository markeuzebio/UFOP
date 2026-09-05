add $s0, $zero, $sp

li $t0, 2
sb $t0, ($s0)
li $t0, 7
sb $t0, -4($s0)
li $t0, 8
sb $t0, -8($s0)
li $t0, 3
sb $t0, -12($s0)
li $t0, 4
sb $t0, -16($s0)
li $t0, 1
sb $t0, -20($s0)
li $t0, 6
sb $t0, -24($s0)
li $t0, 5
sb $t0, -28($s0)

li $t3, 8	# n = 8
li $t2, 1	# i = 1

# t1 = j
# t2 = i
# t3 = n
# t4 utilizado como flag

LOOP_E:
	beq $t2, $t3, END_LOOP_E	# if i == n, sai do loop
	add $t1, $zero, $t2		# j == i inicialmente
	add $s1, $zero, $s0		# $s1 eh um registro auxiliar para andar na memoria no loop interno
	LOOP_I:
		beq $t1, $zero, END_LOOP_I	# if j == 0
		lw $s6, -4($s1)
		lw $s7, 0($s1)
		sgt $t4, $s6, $s7
		beq $t4, $zero, SWITCH		# if v[j] < v[j - 1]
		bne $t4, $zero, END_LOOP_I	# if v[j] > v[j - 1] quebra o loop interno
		AFTER_SWTICH:
			addi $s1, $s1, 4
			addi $t1, $t1, -1
			j LOOP_I
	END_LOOP_I:
	# as linhas abaixo equivalem aa i++
	addi $s0, $s0, -4
	addi $t2, $t2, 1
	j LOOP_E

SWITCH:
	sb $s6, 0($s1)
	sb $s7, -4($s1)
	j AFTER_SWTICH
	
END_LOOP_E: