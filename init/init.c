#include <asm/asm.h>
#include <pmap.h>
#include <env.h>
#include <printf.h>
#include <kclock.h>
#include <trap.h>

void my_test() {
	struct Env *a;
	ENV_CREATE_PRIORITY(user_A, 2);
	a = LIST_FIRST(env_sched_list);
	u_int b = fork(a);
	u_int e = fork(envs + ENVX(b));
	u_int f = fork(envs + ENVX(b));
	u_int c = fork(a);
	u_int d = fork(a);
	printf("a %08x\n", a->env_id);
	lab3_output(a->env_id);
	printf("b %08x\n", b);
	lab3_output(b);
	printf("c %08x\n", c);
	lab3_output(c);
	printf("d %08x\n", d);
	lab3_output(d);
	printf("e %08x\n", e);
	lab3_output(e);
	printf("f %08x\n", f);
	lab3_output(f);

	printf("================");
	printf("a %d\n", lab3_get_sum(a));
	printf("b %d\n", lab3_get_sum(b));
	printf("c %d\n", lab3_get_sum(c));
	printf("d %d\n", lab3_get_sum(d));
	printf("e %d\n", lab3_get_sum(e));
	printf("f %d\n", lab3_get_sum(f));
	
	
}

void mips_init()
{
	printf("init.c:\tmips_init() is called\n");
	mips_detect_memory();
	
	mips_vm_init();
	page_init();
	
	env_init();
	//env_check();
	my_test();
	panic("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
	while(1);
	panic("init.c:\tend of mips_init() reached!");

	/*you can create some processes(env) here. in terms of binary code, please refer current directory/code_a.c
	 * code_b.c*/
    /*** exercise 3.9 ***/
	/*you may want to create process by MACRO, please read env.h file, in which you will find it. this MACRO is very
	 * interesting, have fun please*/

	ENV_CREATE_PRIORITY(user_A, 2);

	ENV_CREATE_PRIORITY(user_B, 1);

	
	trap_init();
	kclock_init();
	//env_run(envs);
	panic("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
	while(1);
	panic("init.c:\tend of mips_init() reached!");
}

void bcopy(const void *src, void *dst, size_t len)
{
	void *max;

	max = dst + len;
	// copy machine words while possible
	while (dst + 3 < max)
	{
		*(int *)dst = *(int *)src;
		dst+=4;
		src+=4;
	}
	// finish remaining 0-3 bytes
	while (dst < max)
	{
		*(char *)dst = *(char *)src;
		dst+=1;
		src+=1;
	}
}

void bzero(void *b, size_t len)
{
	void *max;

	max = b + len;

	//printf("init.c:\tzero from %x to %x\n",(int)b,(int)max);
	
	// zero machine words while possible

	while (b + 3 < max)
	{
		*(int *)b = 0;
		b+=4;
	}
	
	// finish remaining 0-3 bytes
	while (b < max)
	{
		*(char *)b++ = 0;
	}		
	
}
