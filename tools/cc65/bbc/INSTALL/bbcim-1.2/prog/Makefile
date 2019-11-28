SHELL = /bin/sh

# bbcim (Acorn BBC computer disk image manipulation and more)
#
# This file is released to the public domain, by W.H.Scholten 1996-2001,2009,2014
#


WARN	= -Wall -Wfatal-errors -Wstrict-prototypes -Wmissing-prototypes

INCLUDES=-I../lib -I../lib/include -I../lib/shared -I../lib/shared/archive_format -I../lib
ALL_CFLAGS = $(WARN) $(INCLUDES) -O2 -g

all:	bbcim

bbcim:	bbcim.o utils.o
	$(CC) -L../lib -o bbcim bbcim.o utils.o -lbbcim

bbcim.o: *.c _config.h

#	$(CC) $(WARN) -I../lib/include -O2 -o bbcim.o -c bbcim.c


clean:
	rm -f bbcim *.o *~ */*~ core *.core


install:	bbcim
	cp -f bbcim $(INSTALL)/bin/
#	mkdir -p $(HOME)/bin/
#	cp -f bbcim $(HOME)/bin/



install.shared:
	cp -f bbcim $(INSTALL)/bin/


uninstall:
	rm -f $(INSTALL)/bin/bbcim


# // Compile C files to object files.
.c.o:
	$(CC) $(ALL_CFLAGS) -c -o $@ $<
