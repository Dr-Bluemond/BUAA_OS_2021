#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>
	.text
LEAF(_my_putchar)
	andi	$4,$4,0x00ff
	li	$8,0xb0000000
	sb	$4,0($8)
	j	$31
	nop
END(_my_putchar)

LEAF(_my_getchar)
	li	$8,0xb0000000
	lb	$2,0($8)
	sb	$2,0($8)
	li  $9,13

	bne $2, $9,foo
	nop
	li $9,10
	sb $9, 0($8)

	foo:
	j	$31
	nop
END(_my_getchar)

LEAF(_my_exit)
	li	$8,0xb0000010
	sb	$0,0($8)
	j	$31
	nop
END(_my_exit)
