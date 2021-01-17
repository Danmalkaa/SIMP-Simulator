	lw $s0, $zero, $imm, 0x100 	# 0: load radios into $s0
	mul $s1, $s0, $s0, 0		# 2: set $s1 = $s0*$s0
	add $s2, $zero, $imm, 1		# 3: set $s2 = 1
	add $t0, $zero, $imm, 255	# 5: set $t0 = 255
	out $t0, $zero, $imm, 21	# 7: set monitor data = FF (white)
	add $a0, $zero, $imm, 175	# 9: set $a0 = 175 (x0)
	add $a1, $zero, $imm, 143	# B: set $a1 = 143 (y0)
	out $a0, $zero, $imm, 19	# D: set monitorx = 175
	out $a1, $zero, $imm, 20	# F: set monitory = 143
	add $t2, $zero, $zero, 0	# 11: set $t2 = 0
	add $t0, $zero, $imm, 0		# 12: set $t0 = 0
loop:
	mul $t1, $t0, $t0, 0		# 14: set $t1 = $t0*$t0
	sub $t1, $s1, $t1, 0		# 15: set $t1 = $s1 - $t1 (radios^2 - x^2)
	mul $t3, $t2, $t2, 0		# 16: set $t3 = $t2*$t2
	bgt $imm, $t3, $t1, next	# 17: jump if $t3 > $t1 (y^2 > radios^2)
	add $t1, $a0, $t0, 0		# 19: set $t1 = $a0 + $t0
	out $t1, $zero, $imm, 19	# 1A: set monitorx = $t1
	add $t3, $a1, $t2, 0		# 1C: set $t3 = $a1 + $t2
	out $t3, $zero, $imm, 20	# 1D: set monitory = $t3
	out $s2, $zero, $imm, 18	# 1F: illumanate pixel
	sub $t3, $a1, $t2, 0		# 21: set $t3 = $a1 - $t2
	out $t3, $zero, $imm, 20	# 22: set monitory = $t3
	out $s2, $zero, $imm, 18	# 24: illumanate pixel
	sub $t1, $a0, $t0, 0		# 26: set $t1 = $a0 - $t0
	out $t1, $zero, $imm, 19	# 27: set monitorx = $t1
	out $s2, $zero, $imm, 18	# 29: illumanate pixel
	add $t3, $a1, $t2, 0		# 2B: set $t3 = $a1 + $t2
	out $t3, $zero, $imm, 20	# 2C: set monitory = $t3
	out $s2, $zero, $imm, 18	# 2E: illumanate pixel
	add $t2, $t2, $imm, 1		# 30: $t2++
	bge $imm, $s0, $t2, loop	# 32: jump to loop if y < radios
next:
	add $t2, $zero, $zero, 0	# 34: set $t2 = 0
	add $t0, $t0, $imm, 1		# 35: set $t0++
	ble $imm, $t0, $s0, loop	# 37: strat loop if x <= radios
	halt $zero, $zero, $zero, 0	# 39: terminiate program