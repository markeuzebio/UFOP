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
li $t2, 0	# i = 1

LOOP_E:
	beq $t2, $t3, END_LOOP_E	# if i == n quebra o loop
	lw $t9, 0($s0)			# t9 armazena o valor contido na primeira posicao daquela iteracao
	lw $t7, 0($s0)			# t7 representa o menor valor
	addi $t1, $t2, 1		# j = i + 1
	addi $s1, $s0, -4		# s1 eh um registrador auxiliar que anda na memoria
	add $s3, $zero, $s0		# s3 eh outro registrador auxiliar para armazenar uma posicao na memoria (funciona como o k)
	LOOP_I:
		beq $t1, $t3, END_LOOP_I	# if j == n quebra o loop
		lw $t8, 0($s1)			# t8 representa valores concorrentes para menor valor
		bge $t8, $t7, CONTINUE
		add $s3, $zero, $s1		# grava no registrador $s3 o indice de maior valor na memoria
		add $t7, $zero, $t8
		CONTINUE:
		addi $s1, $s1, -4
		addi $t1, $t1, 1
		j LOOP_I
	END_LOOP_I:
	j SWITCH				# realiza a troca necessaria
	AFTER_SWITCH:
	# as linhas abaixo equivalem aa i++
	addi $s0, $s0, -4
	addi $t2, $t2, 1
	j LOOP_E
SWITCH:
	sb $t7, 0($s0)
	sb $t9, 0($s3)
	j AFTER_SWITCH
END_LOOP_E: