SHELL = /bin/sh

# LIBBBCIM: Acorn BBC computer disk image manipulation and more
#
# This file is released to the public domain, by W.H.Scholten 1996,1997,1999,2001,2003,2009,2014
#


SHELL=/bin/sh

INCLUDES=-Iinclude -Ishared -Ishared/archive_format
WARN	= -Wall -Wfatal-errors -Wstrict-prototypes -Wmissing-prototypes
ALL_CFLAGS=$(WARN) $(INCLUDES) -g -O2 -std=c99

all: static

static: libbbcim.a

shared: libbbcim.so.1.0

libbbcim.so.1.0: *.c include/*.h
	$(MAKE) libbbcim.o.shared

libbbcim.a: *.c include/*.h
	$(MAKE) libbbcim.o.static

libbbcim.o.shared: *.c include/*.h
	$(CC) $(ALL_CFLAGS) -shared -fPIC -o libbbcim.o -c bbcimlib.c
	$(CC) -O2 -Wl,-shared,-soname,libbbcim.so.1.0 -o libbbcim.so.1.0 libbbcim.o
	ln -s libbbcim.so.1.0 libbbcim.so

libbbcim.o.static: bbcimlib.c include/bbcim.h
	$(CC) $(ALL_CFLAGS) -o libbbcim.o -c bbcimlib.c
	ar cru libbbcim.a libbbcim.o
	ranlib libbbcim.a


clean:
	rm -f libbbcim.so.1.0 libbbcim.so libbbcim.a *.o *~ */*~


install: all

#install:	static


install.lib: libbbcim.so.1.0
	cp include/bbcim.h /usr/local/include
	cp -f libbbcim.so.1.0 /usr/local/lib
	rm -f /usr/local/lib/libbbcim.so
	ln -s /usr/local/lib/libbbcim.so.1.0 /usr/local/lib/libbbcim.so
#	ldconfig -m /usr/local/lib/	# merge new entries

#install.shared: shared
#	cp bbcim /usr/local/bin


uninstall.lib:
	rm -f /usr/local/lib/libbbcim.so* /usr/local/include/bbcim.h


#Depend
libbbcim.o	: *.c *.h

# // Compile C files to object files.
.c.o:
	$(CC) $(ALL_CFLAGS) -c -o $@ $<

# EOF
