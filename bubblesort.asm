	.word 1024 64
	.word 1025 34
	.word 1026 25
	.word 1027 12
	.word 1028 22
	.word 1029 11
	.word 1030 90
	add $t0, $zero, $imm, 0		# 0: set i=0
	add $s0, $zero, $imm, 15		# 2: set s0 = n-1 (15)
Test1:	
	blt $imm, $t0, $s0, Li		# 4: enter loop if i<n-1
	halt $zero, $zero, $zero, 0	# 6: halt
Li:
	add $t1, $zero, $zero, 0		# 7: set j=0
Test2:	
	sub $s2, $s0, $t0, 0		# 8: set s2 = n-i-1
	bge $imm, $t1, $s2, iplus		# a: don't enter loop if j>=n-1-i
	lw $a0, $t1, $imm, 1024	# c: get array[1024+j]
	lw $a1, $t1, $imm, 1025	# e: get array[1024+j+1]
	bgt $imm, $a0, $a1, SWAP	# 10: jump to SWAP
jplus:	
	add $t1, $t1, $imm, 1		# 12: j++
	beq $imm, $zero, $zero, Test2	# 14: jump back to n-i-1
iplus:
	add $t0, $t0, $imm, 1		# 16: i++
	beq $imm, $zero, $zero, Test1 	# 18: jump back to enter loop i
SWAP:
	sw $a0, $t1, $imm, 1025	# 1a: store bigger num in array[1024+j+1]
	sw $a1, $t1, $imm, 1024	# 1c: store smaller num in array[1024+j]
	beq $imm, $zero, $zero, jplus 	# 1e: jump back to enter loop i