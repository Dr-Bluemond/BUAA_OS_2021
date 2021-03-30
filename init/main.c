/*
 * Copyright (C) 2001 MontaVista Software Inc.
 * Author: Jun Sun, jsun@mvista.com or jsun@junsun.net
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <printf.h>
#include <pmap.h>

struct s1 {
	int a;
	char b;
	char c;
	int d;
};

struct s2 {
	int size;
	int c[5];
};

int main()
{
	printf("main.c:\tmain is start ...\n");
	struct s1 t1; t1.a=1; t1.b='b'; t1.c='c'; t1.d=4;
	struct s2 t2; t2.size=3; t2.c[0]=0; t2.c[1]=1; t2.c[2]=2;t2.c[3]=3;t2.c[4]=4;

	printf("%$1T" ,&t1);
	printf("%04$1T" ,&t1);
	printf("%04$2T" ,&t2);


	mips_init();
	panic("main is over is error!");

	return 0;
}
