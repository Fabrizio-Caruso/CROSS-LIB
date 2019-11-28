SHELL=/bin/sh

all:
	cd test; $(MAKE) all

clean:
	rm -f *~
	cd test; $(MAKE) clean


ALL_CFLAGS= -I/usr/local/include -O2 -g -Wall -Wfatal-errors

# // Compile C files to object files.
.c.o:
	$(CC) $(ALL_CFLAGS) -c -o $@ $<
