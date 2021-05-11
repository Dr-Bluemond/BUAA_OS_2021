#include "lib.h"
#include <unistd.h>
#include <mmu.h>
#include <env.h>
#include <trap.h>

void syscall_putchar(char ch)
{
	msyscall(SYS_putchar, 1, (int)ch);
}


u_int
syscall_getenvid(void)
{
	return msyscall(SYS_getenvid, 0);
}

void
syscall_yield(void)
{
	msyscall(SYS_yield, 0);
}


int
syscall_env_destroy(u_int envid)
{
	return msyscall(SYS_env_destroy, 1, envid);
}
int
syscall_set_pgfault_handler(u_int envid, void (*func)(void), u_int xstacktop)
{
	return msyscall(SYS_set_pgfault_handler, 3, envid, (int)func, xstacktop);
}

int
syscall_mem_alloc(u_int envid, u_int va, u_int perm)
{
	return msyscall(SYS_mem_alloc, 3, envid, va, perm);
}

int
syscall_mem_map(u_int srcid, u_int srcva, u_int dstid, u_int dstva, u_int perm)
{
	return msyscall(SYS_mem_map, 5, srcid, srcva, dstid, dstva, perm);
}

int
syscall_mem_unmap(u_int envid, u_int va)
{
	return msyscall(SYS_mem_unmap, 2, envid, va);
}

int
syscall_set_env_status(u_int envid, u_int status)
{
	return msyscall(SYS_set_env_status, 2, envid, status);
}

int
syscall_set_trapframe(u_int envid, struct Trapframe *tf)
{
	return msyscall(SYS_set_trapframe, 2, envid, (int)tf);
}

void
syscall_panic(char *msg)
{
	msyscall(SYS_panic, 1, (int)msg);
}

int
syscall_ipc_can_send(u_int envid, u_int value, u_int srcva, u_int perm)
{
	return msyscall(SYS_ipc_can_send, 4, envid, value, srcva, perm);
}

void
syscall_ipc_recv(u_int dstva)
{
	msyscall(SYS_ipc_recv, 1, dstva);
}

int
syscall_cgetc()
{
	return msyscall(SYS_cgetc, 0);
}
