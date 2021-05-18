#include "lib.h"
void umain() {
    *((volatile int *)(0x45432100)) = 1;
    *((volatile int *)(0x45532100)) = 1;
}
