extern char _my_getchar();
extern void _myputchar(char ch);
extern void _my_exit();

void my_cal() {
	int a, b, sum;
	a = 0;
	b = 0;
	while (1) {
		char c = _my_getchar();
		if (c == '\0') {
			continue;
		} 
		if (c >= '0' && c <= '9') {
			a = a * 10 + (c - '0');
		} else { // '\n'
			break; 
		}
	}
	while (1) {
		char c = _my_getchar();
		if (c == '\0') {
			continue;
		}
		if (c >= '0' && c <= '9') {
			b = b * 10 + (c - '0');
		} else {
			break; 
		}
	}
	
	sum = a + b;
	print_num(sum);
	return;
}

void print_num(int num) {

	char buf[64];
	int p = 0;
	while (num != 0) {
		buf[p++] = num % 10 + '0';
		num /= 10;
	}
	while (p) {
		_my_putchar(buf[--p]);
	}

}
