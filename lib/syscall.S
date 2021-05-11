#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>
#include <stackframe.h>
#include <unistd.h>

/*** exercise 4.2 ***/
NESTED(handle_sys,TF_SIZE, sp)
    SAVE_ALL                            // Macro used to save trapframe
    CLI                                 // Clean Interrupt Mask
    nop
    .set at                             // Resume use of $at

    // TODO: Fetch EPC from Trapframe, calculate a proper value and store it back to trapframe.

	lw		t0, TF_EPC(sp)
//	lw		t1, TF_CAUSE(sp)
//	lui		t2, 0x8000
//	and		t1, t1, t2
//	bnez	t1, IS_BD
//	nop
	addiu	t0, t0, 4
//IS_BD:
	sw		t0, TF_EPC(sp)



    // TODO: Copy the syscall number into $a0.

	lw a0, TF_REG4(sp)
    
    addiu   a0, a0, -__SYSCALL_BASE     // a0 <- relative syscall number
    sll     t0, a0, 2                   // t0 <- relative syscall number times 4
    la      t1, sys_call_table          // t1 <- syscall table base
    addu    t1, t1, t0                  // t1 <- table entry of specific syscall
    lw      t2, 0(t1)                   // t2 <- function entry of specific syscall

	lw s0, TF_REG5(sp) // s0 is the size of stack
	addiu s0, s0, 1
	sll s0, s0, 2

    lw      t0, TF_REG29(sp)            // t0 <- user's stack pointer
	lw      a0, TF_REG4(sp)
	lw      a1, TF_REG6(sp)
	lw      a2, TF_REG7(sp)
	lw      a3, 16(t0)

	subu sp, sp, s0

 	li t9, 0
 foo:
 	beq t9, s0, bar
 	addu t3, t0, t9
 	addiu t3, t3, 4
 	lw t5, 0(t3)
 	addu t3, sp, t9
 	sw t5, 0(t3)
 	addu t9, t9, 4
	j foo
 bar:

//    lw      t3, 20(t0)                  // t3 <- the 4th argument of msyscall
//    lw      t4, 24(t0)                  // t4 <- the 5th argument of msyscall
//    lw      t5, 28(t0)                  // t5 <- the 6th argument of msyscall
//    lw      t6, 32(t0)                  // t6 <- the 7th argument of msyscall
//    lw      t7, 36(t0)                  // t7 <- the 8th argument of msyscall
//    lw      t8, 40(t0)                  // t8 <- the 9th argument of msyscall
//
//    // TODO: Allocate a space of six arguments on current kernel stack and copy the six arguments to proper location
//	sw t3, 16(sp)
//	sw t4, 20(sp)
//	sw t5, 24(sp)
//	sw t6, 28(sp)
//	sw t7, 32(sp)
//	sw t8, 36(sp)
    
    
    jalr    t2                          // Invoke sys_* function
    nop
    
    // TODO: Resume current kernel stack

	addu sp, sp, s0
    
    sw      v0, TF_REG2(sp)             // Store return value of function sys_* (in $v0) into trapframe

    j       ret_from_exception          // Return from exeception
    nop
END(handle_sys)

sys_call_table:                         // Syscall Table
.align 2
    .word sys_putchar
    .word sys_getenvid
    .word sys_yield
    .word sys_env_destroy
    .word sys_set_pgfault_handler
    .word sys_mem_alloc
    .word sys_mem_map
    .word sys_mem_unmap
    .word sys_env_alloc
    .word sys_set_env_status
    .word sys_set_trapframe
    .word sys_panic
    .word sys_ipc_can_send
    .word sys_ipc_recv
    .word sys_cgetc
	.word sys_ipc_can_multi_send
