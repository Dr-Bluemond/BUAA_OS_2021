#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>
#include <kcons.h>



.macro	setup_c0_status set clr
	.set	push
	mfc0	t0, CP0_STATUS
	or	t0, \set|\clr
	xor	t0, \clr
	mtc0	t0, CP0_STATUS			
	.set	pop
.endm

	.text
LEAF(set_cons)

	sw	sp, KERNEL_SP
setup_c0_status STATUS_CU0|0x401 0
	jr ra

	nop
END(set_cons)

LEAF(get_cp0_status)

	mfc0 v0, CP0_STATUS
	jr ra
	nop

END(get_cp0_status)
