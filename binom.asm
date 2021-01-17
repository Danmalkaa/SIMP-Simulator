	.word 0x100 8			# 0: set n = 8
	.word 0x101 3			# 2: set k = 3
	add $sp, $zero, $imm, 1		# 4: set $sp = 1
	sll $sp, $sp, $imm, 11		# 6: set $sp = 1 << 11 = 2048
	lw $a0, $imm, $zero, 0x100	# 8: get n from address 0x100
	lw $a1, $imm, $zero, 0x101	# a: get k from address 0x101
	jal $imm, $zero, $zero, bin	# c: calc $v0 = bin(n,k)
	sw $v0, $zero, $imm, 0x102	# e: store bin(n,k) in 0x102
	halt $zero, $zero, $zero, 0	# f: halt
bin:
	add $sp, $sp, $imm, -4		# 10: adjust stack for 4 items
	sw $s0, $sp, $imm, 3		# 12: save $s0
	sw $ra, $sp, $imm, 2		# 14: save return address
	sw $a0, $sp, $imm, 1		# 16: save argument n
	sw $a1, $sp, $imm, 0		# 18: save argument k
Test1:
	add $t0, $zero, $zero, 0		# 1a: t0=0
	bne $imm, $a0, $a1, Test2		# 1b: jump to L1 if n!=k
	add $t0, $zero, $imm, 1		# 1d: t0=1 if n=k
Test2:
	add $t1, $zero, $zero, 0		# 1f: t1=0
	bne $imm, $a1, $zero, Binom	# 20: jump to Binom if k=0
	add $t1, $zero, $imm, 1		# 22: t1=1 if n=k
Binom:
	or $t2, $t0, $t1, 0		# 24: t1=1 if n=k	
	beq $imm, $t2, $zero, L1		# 26: jump to L1 if none of the tests is true
	add $v0, $imm, $zero, 1		# 28: otherwise, bin(n,k) = 1, copy input
	beq $imm, $zero, $zero, L2	# 2a: jump to L2
L1:
	sub $a0, $a0, $imm, 1		# 2e: calculate n - 1
	jal $imm, $zero, $zero, bin	# 30: calc $v0=bin(n-1,k)
	add $s0, $v0, $imm, 0		# 32: $s0 = bin(n-1,k)
	sub $a1, $a1, $imm, 1		# 36: calculate k-1
	jal $imm, $zero, $zero, bin	# 38: calc bin(n-1,k-1)
	add $v0, $v0, $s0, 0		# 3a: $v0 = bin(n-1,k) + bin(n-1,k-1)
	lw $a1, $sp, $imm, 0		# 3b: restore $a1
	lw $a0, $sp, $imm, 1		# 3b: restore $a0
	lw $ra, $sp, $imm, 2		# 3d: restore $ra
	lw $s0, $sp, $imm, 3		# 3f: restore $s0
L2:
	add $sp, $sp, $imm, 4		# 41: pop 4 items from stack
	beq $ra, $zero, $zero, 0		# 43: and return

