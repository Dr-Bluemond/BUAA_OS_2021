#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>

/*** exercise 4.1 ***/
LEAF(msyscall)
    // TODO: you JUST need to execute a `syscall` instruction and return from msyscall
	syscall
	jr ra
	nop

END(msyscall)


LEAF(uget_sp_asm)

	move v0, sp
	jr ra
	nop

END(uget_sp_asm)



