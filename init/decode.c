#include<stdio.h>

extern unsigned char binary_user_A_start[];
extern int binary_user_A_size;

int main() {
	FILE* f = fopen("out.bin", "wb");
	fwrite(binary_user_A_start, sizeof(char), binary_user_A_size, f);
	fclose(f);
	return 0;
}
