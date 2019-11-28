SHELL = /bin/sh

# Test for libbbcim
#
# This file is public domain, by W.H.Scholten 1996-2003
#


WARN	= -Wall -Wstrict-prototypes -Wmissing-prototypes

ALL_CFLAGS = $(WARN) -I../lib/include -I../lib/archive_format -O2 -g

all:	libtest

libtest:	libtest.o
	$(CC) -L../lib -o libtest libtest.o -lbbcim

libtest.o: libtest.c

#	$(CC) $(WARN) -I../lib/include -O2 -o bbcim.o -c bbcim.c


clean:
	rm -f libtest *.o *~ core *.core


# // Compile C files to object files.
.c.o:
	cc $(ALL_CFLAGS) -c -o $@ $<
