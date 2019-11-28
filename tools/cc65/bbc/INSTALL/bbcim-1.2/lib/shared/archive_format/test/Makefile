SHELL=/bin/sh

WARN	= -Wall -Wstrict-prototypes -Wmissing-prototypes

INCLUDES=-I.. -I/usr/local/include -I../../shared

ALL_CFLAGS=$(WARN) $(INCLUDES) -O2 -g

all: archive_test

archive_test: archive_test.o _bbc_archive_read_attributes.o ../_bbc_archive.h
	$(CC) $(WARN) -o archive_test archive_test.o _bbc_archive_read_attributes.o 

_bbc_archive_read_attributes.o: ../_bbc_archive_read_attributes.c

clean:
	rm -f archive_test *.o *~ core *.core


# // Compile C files to object files.
.c.o:
	$(CC) $(ALL_CFLAGS) -c -o $@ $<
