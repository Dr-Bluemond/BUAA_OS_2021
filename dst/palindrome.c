#include<stdio.h>
int main()
{
	char n[10];
	scanf("%s",n);
	
	char *a, *b;
	a = b = n;
	while (*b != '\0') {
		b++;
	}
	b--;
	while (b > a) {
		if (*b != *a) {
			printf("N");
			return 0;
		}
		a++;
		b--;
	}
	printf("Y");
	return 1;
}
