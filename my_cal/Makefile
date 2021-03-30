# Main makefile
#
# Copyright (C) 2007 Beihang University
# Written by Zhu Like ( zlike@cse.buaa.edu.cn )
#

# ========= Configuration =========

INCLUDES	  := -I../include/

# ======== End of configuration ===

all: my_driver.o my_cal.o

%.o: %.S
	$(CC) $(CFLAGS) $(INCLUDES) -c $< 
%.o: %.c
	$(CC) $(DEFS) $(CFLAGS) $(INCLUDES) -c $<

.PHONY: clean

clean:
	rm -rf *~ *.o

include ../include.mk
