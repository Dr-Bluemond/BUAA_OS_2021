#include"lib.h"
int global_a;
void umain()
{
	int a=0;
	int id=0;
	if((id=fork())==0){
		if((id=tfork())==0){
			global_a+=3;
			for(;;){
				writef("\t\tthisischild2:a:%d\n",global_a);
				for(;;){}
			}
		}
		global_a+=2;
		for(;;){
			writef("\tthisischild:a:%d\n",global_a);
			for(;;){
			}
		}
	}
	global_a++;
	for(;;){
		writef("thisisfather:a:%d\n",global_a);
		for(;;){}
	}
}
