#include <env.h>
#include <pmap.h>
#include <printf.h>

/* Overview:
 *  Implement simple round-robin scheduling.
 *
 *
 * Hints:
 *  1. The variable which is for counting should be defined as 'static'.
 *  2. Use variable 'env_sched_list', which is a pointer array.
 *  3. CANNOT use `return` statement!
 */
/*** exercise 3.14 ***/
void sched_yield(void)
{
	   static int count = 1; // remaining time slices of current env
   static int point = 0; // current env_sched_list index
   static struct Env *e;
    /*  hint:
     *  1. if (count==0), insert `e` into `env_sched_list[1-point]`
     *     using LIST_REMOVE and LIST_INSERT_TAIL.
     *  2. if (env_sched_list[point] is empty), point = 1 - point;
     *     then search through `env_sched_list[point]` for a runnable env `e`,
     *     and set count = e->env_pri
     *  3. count--
     *  4. env_run()
     *
     *  functions or macros below may be used (not all):
     *  LIST_INSERT_TAIL, LIST_REMOVE, LIST_FIRST, LIST_EMPTY
     */
    count--;
    if (count <= 0||curenv == NULL||curenv->env_status != ENV_RUNNABLE){
        int find = 0;
        while(1) {
            if (LIST_EMPTY(&env_sched_list[point])){
                point = 1 - point;
                break;
            }
            e = LIST_FIRST(&env_sched_list[point]);
            if (e->env_status == ENV_RUNNABLE) {
                find = 1;
                break;}
            LIST_REMOVE(e,env_sched_link);
            LIST_INSERT_TAIL(&env_sched_list[1-point],e,env_sched_link);
        }
        if (!find){
            while(1) {
                if (LIST_EMPTY(&env_sched_list[point])){
                    panic("^^^no runnable env^^^");
                }
                e = LIST_FIRST(&env_sched_list[point]);
                if (e->env_status == ENV_RUNNABLE) {
                    find = 1;
                    break;}
                LIST_REMOVE(e,env_sched_link);
                LIST_INSERT_TAIL(&env_sched_list[1-point],e,env_sched_link);
            }
        }
        LIST_REMOVE(e,env_sched_link);
        LIST_INSERT_TAIL(&env_sched_list[1-point],e,env_sched_link);
        count = e->env_pri;
        env_run(e);
        panic("^^^end of sched yield^^^");
    }
    env_run(curenv);
    panic("^^^enc of sched yield^^^");
    //env_run(LIST_FIRST(env_sched_list));

    //panic("end of ????");
}

