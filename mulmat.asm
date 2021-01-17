	add $a0, $zero, $imm, 0x100 	# 0: set $a0 = A11 address
	add $a1, $zero, $imm, 0x110	# 2: set $a1 = B11 address
	add $sp, $zero, $imm, 0x120	# 4: set $sp = C11 address
	add $t0, $zero, $zero, 0	# 6: set $t0 = 0 (i)
iloop:
	add $t1, $zero, $zero, 0	# 7: set $t1 = 0 (j)
jloop:
	add $t2, $zero, $zero, 0	# 8: set $t2 = 0 (k)
kloop:
	add $s0, $t2, $t0, 0		# 9: set $s0 = $t2 + $t0 (k+i)
	add $s0, $s0, $a0, 0		# A: set $s0 = address Aik
	lw $s0, $s0, $zero, 0		# B: load Aik into $s0
	sll $s1, $t2, $imm, 2		# C: set $s1 = k*4
	add $s1, $s1, $t1, 0		# E: set $s1 = $s1 + $t1 (k*4+j)
	add $s1, $s1, $a1, 0		# F: set $s1 = address Bkj
	lw $s1, $s1, $zero, 0		# 10: load Bkj into $s1
	mul $t3, $s0, $s1, 0		# 11: set $t3 = Aik*Bkj
	add $v0, $v0, $t3, 0		# 12: set $v0 = $v0 + $t3
	add $t2, $t2, $imm, 1		# 13: set $t2++ (k++)
	add $t3, $zero, $imm, 4		# 15: set $t3 = 4
	blt $imm, $t2, $t3, kloop	# 17: jump to kloop if k<4
	add $t2, $zero, $zero, 0	# 19: set $t2 = 0 (k)
	add $t3, $t0, $t1, 0		# 1A: set $t3 = $t0 + $t1 (i+j)
	add $t3, $t3, $sp, 0		# 1B: set $t3 = address Cij
	sw $v0, $t3, $zero, 0		# 1C: store Cij
	add $v0, $zero, $zero, 0	# 1D: set $v0 = 0
	add $t1, $t1, $imm, 1		# 1E: set $t1++ (j++)
	add $t3, $zero, $imm, 4		# 20: set $t3 = 4
	blt $imm, $t1, $t3, jloop	# 22: jump to jloop if j<4
	add $t0, $t0, $imm, 4		# 24: set $t0 += 4 (i+=4)
	add $t3, $zero, $imm, 16	# 26: set $t3 = 16
	blt $imm, $t0, $t3, iloop	# 28: jump to iloop if i<16
	halt $zero, $zero, $zero, 0	# 2A: terminate program