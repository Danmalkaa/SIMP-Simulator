	add $sp, $zero, $imm, 1		# 0: set $sp = 1
	sll $sp, $sp, $imm, 10		# 2: set $sp = 1 << 10 = 1024
	add $t0, $zero, $imm, 6		# 4: set $t0 = 6
	out $imm, $t0, $zero, diskr	# 6: set irqhandler as diskr
	add $t0, $zero, $imm, 1		# 8: set $t0 = 1
	out $t0, $zero, $imm, 1		# A: enable irq1
	add $a0, $zero, $zero		# C: set $a0 = 0
	add $a1, $zero, $imm, 1		# D: set $a1 = 1
	add $t3, $zero, $zero		# F: set $t3 = 0
	add $s0, $zero, $imm, 128	# 10: set $s0 = 128
	add $t0, $imm, $zero, 6		# 12: set $t0 = 6
	add $t0, $imm, $t0, diskr	# 14: set $t0 = diskr +6 (first enter)
	beq $t0, $zero, $zero, 0	# 16: jump to diskr + 6
XOR:
	lw $t0, $sp, $t3		# 17: load word from first sector / last xor
	mul $t1, $a1, $imm, 128		# 18: set $t1 = $a1*128
	add $t1, $t1, $t3		# 1A: set $t1 = $t1 + $t3
	lw $t1, $sp, $t1		# 1B: load word from next sector
	xor $t0, $t0, $t1		# 1C: set $t0 = $t0 xor $t1
	sw $t0, $sp, $t3		# 1D: store resulte into first sector
	add $t3, $t3, $imm, 1		# 1E: set $t3++
	blt $imm, $t3, $s0, XOR		# 20: jump back to XOR loop
	add $t3, $zero, $zero		# 22: set $t3 = 0
	add $a1, $a1, $imm, 1		# 23: set $a1++
	add $t0, $zero, $imm, 4		# 25: set $t0 = 4
	beq $imm, $a1, $t0, diskw	# 27: jump to diskw if finish all xor loops
	bgt $imm, $a0, $a1, XOR		# 29: jump to XOR if $a0 > $a1
diskr:
	in $t2, $zero, $imm, 17		# 2B: read diskstatus into $t2
	bne $imm, $t2, $zero, waitr	# 2D: jump to waitr if disk is busy
	add $a0, $a0, $imm, 1		# 2F: set $a0 = $a0 + 1
	add $t2, $t2, $imm, 4		# 31: set $t2 = 4
	beq $imm, $a0, $t2, return	# 33: stop reading after 4 sectors
	out $a0, $zero, $imm, 15	# 35: set disksector = $a0
	mul $t2, $a0, $imm, 128		# 37: set $t2 = buffer offset
	add $t2, $t2, $sp		# 39: set $t2 = memmory address
	out $t2, $zero, $imm, 16	# 3A: set diskbuffer = $t2
	add $t2, $zero, $imm, 1		# 3C: set $t2 = 1
	out $t2, $zero, $imm, 14	# 3E: set diskcmd = 1 (read)
	beq $imm, $a0, $zero, waitr	# 40: jump to waitr in firstread
return:
	out $zero, $zero, $imm, 4	# 42: clear irq1 status
	reti $zero, $zero, $zero	# 44: jump back (set pc=iqreturn)
waitr:
	bgt $imm, $a0, $a1, XOR		# 45: jump to XOR if $a0 > $a1
	beq $imm, $zero, $zero, waitr	# 47: loop until finish reading	
diskw:
	add $t0, $zero, $imm, 6		# 49: set $t0 = 6
	out $imm, $t0, $zero, finish	# 4B: set irqhandler as finish
	in $t0, $zero, $imm, 17		# 4D: read diskstatus into $t0	
	bne $imm, $t0, $zero, diskw	# 4F: jump to diskw if disk is busy
	out $a1, $zero, $imm, 15	# 51: set disksector = $a1
	out $sp, $zero, $imm, 16	# 53: set diskbuffer = $sp
	add $t0, $zero, $imm, 2		# 55: set $t0 = 2
	out $t0, $zero, $imm, 14	# 57: set diskcmd = 2 (write)
waitw:
	beq $imm, $zero, $zero, waitw 	# 59: loop until finish writing
finish:
	halt $zero, $zero, $zero	# 5B: terminate program
	