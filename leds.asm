	add $s1, $zero, $imm, 1		# 0: set $s1 = 1
	add $s2, $zero, $imm, 11		# 2: set $s2 = 11 address of timer enable
	sll $s0, $s1, $imm, 31		# a: set $s0 = 1 << 31
	out $s0, $imm, $zero, 13		# 4: set timermax to max
	out $s1, $zero, $zero, 0		# 4: set irq0 enable
	out $s1, $s2, $zero, 0		# 4: set timer enable
	out $s1, $imm, $zero, 9		# 5: set led on
check:
	in $t1, $zero, $imm, 12		# 7: t1 = current timer
 	add $t1, $t1, $imm, 20		# 9: add to t1 amount of commands till led on
	sll $t1, $t1, $imm, 22		# a: set $t1 << 22
	or $t2, $t1, $imm, 0		# c: only after 1024 clocks (imm for 2 clocks)
	bne $imm, $t2, $zero, check	# d: jump to check if not 1024
	in $s1, $zero, $imm, 9		# f: set s1 to current led
	sra $t0, $s1, $imm, 31		# 11: set $t0 = leds >> 31
	beq $imm, $t0, $s1, end		# 13: jump to halt if reached 32 lit leds
	sll $s1, $s1, $imm, 1		# 15: set $s1 = leds << 1
	or $s1, $s1, $imm, 1		# 17: prepare led[right] = 1
	out $s1, $imm, $zero, 9		# 19: set led
	beq $imm, $zero, $zero, check	# 1a: jump to check
end:
	out $zero, $s2, $zero, 0		# 1c: set timer disable	
	out $zero, $imm, $zero, 3		# 4: clear irq0 status
	halt $zero, $zero, $zero, 0	# 1e: halt