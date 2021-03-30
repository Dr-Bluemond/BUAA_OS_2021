	.text
	.globl _my_putchar
	.ent _my_putchar
	.type _my_putchar, @function
_my_putchar:
	andi	$4,$4,0x00ff
	li	$2,-1342177280			# 0xffffffffb0000000
	sb	$4,0($2)
	j	$31
	nop
	.end printcharc

	.globl _my_getchar
	.ent _my_getchar
	.type _my_getchar @function
_my_getchar:
	li	$8,-1342177280			# 0xffffffffb0000000
	lb	$2,0($8)
	j	$31
	nop
	.end _my_getchar

	.globl _my_exit
	.ent _my_exit
	.type _my_exit, @function
_my_exit:
	li	$2,-1342177280			# 0xffffffffb0000000
	ori	$2,$2,0x10
	sb	$0,0($2)
	j	$31
	nop
	.end _my_exit
