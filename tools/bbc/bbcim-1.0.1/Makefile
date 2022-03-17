SHELL=/bin/sh

# bbcim (Acorn BBC computer disk image manipulation)
#
# This file is public domain, by W.H.Scholten 1996 - 2001, 2014, 2017
#

ALL_CFLAGS=-O2 -Wall -Wfatal-errors -Wstrict-prototypes -Wmissing-prototypes -g

INSTALL=$(HOME)

all:	bbcim

bbcim:	src/*.c src/*.h
	$(CC) $(ALL_CFLAGS) -o bbcim src/bbcim.c

clean:
	rm -f bbcim src/*.o doc/bbcim.ps doc/bbcim.pdf
	rm -f *~ src/*~ doc/*~ core *.core

install: all
	mkdir -p $(INSTALL)/bin/
	cp -f bbcim $(INSTALL)/bin/
	mkdir -p $(INSTALL)/man/man1/
	cp -fp doc/bbcim.1 $(INSTALL)/man/man1/


uninstall:
	rm -f $(INSTALL)/bin/bbcim $(INSTALL)/man/man1/bbcim.1

postscript: doc/bbcim.ps

pdf: doc/bbcim.pdf


# // Compile C files to object files.
.c.o:
	$(CC) $(ALL_CFLAGS) -c -o $@ $<


# // Compile man pages to postscript/pdf
.SUFFIXES: .1 .2 .3 .4 .5 .6 .7 .8 .9 .ps .pdf
.1.ps:
	groff -t -e -mandoc -Tps $< > $@

.ps.pdf: doc/bbcim.ps
	ps2pdf $< $@

# EOF
