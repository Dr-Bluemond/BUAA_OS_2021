#include <env.h>
#include <pmap.h>
#include <printf.h>

u_int get_cp0_status();

void handle_cons_ir(void) {
	static char mybuf[256];
	static u_int len = 0;
	static u_int t = 0;
	char ch;

	u_int temp = 0;
	bcopy(&temp, 0xB5000000, 4);
	bcopy(0xB5000010, &temp, 4);
	while (1) {
		bcopy(0xB0000000, &ch, 1);
		if (ch == 0) break;
		mybuf[len++] = ch;
	}
	if (t == 0) {
		t = temp;
		u_int status = get_cp0_status();
		printf("CP0 STATUS: %x, 1st interrupt: %d\n", status, t);
	} else {
		printf("interval: %d\n", temp - t);
		if (temp - t > 5) {
			mybuf[len] = '\0';
			printf("length=%d, string=%s\n", len, mybuf);
			temp = 0;
			bcopy(&temp, 0xB0000010, 4);
		}
		t = temp;
	}
	return;

}
