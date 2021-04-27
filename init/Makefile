INCLUDES := -I../include

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

.PHONY: clean

all: init.o main.o code_a.o code_b.o

clean:
	rm -rf *~ *.o
	rm -rf decode out.bin out.dec

decode:
	gcc decode.c -c -o decode.o
	gcc code_a.c -c -o code_a_x86.o
	gcc decode.o code_a_x86.o -o decode
	./decode
	$(CROSS_COMPILE)objdump -D out.bin > code_a.des
	rm decode.o code_a_x86.o decode out.bin

include ../include.mk

