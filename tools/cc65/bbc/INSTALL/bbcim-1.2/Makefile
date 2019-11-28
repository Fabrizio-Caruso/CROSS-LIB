SHELL=/bin/sh

# bbcim (Acorn BBC computer disk image manipulation)
#
# This file is public domain, by W.H.Scholten 1996 - 2009
#

INSTALL=$(HOME)

all:	libbbcim bbcim

bbcim:
	cd prog; $(MAKE) all

libbbcim:
	cd lib; $(MAKE) all

postscript: doc/bbcim.ps

libtest:
	cd test; $(MAKE) all


clean:
	rm -f *~ doc/*~
	cd lib; $(MAKE) clean
	cd prog; $(MAKE) clean
	rm -f doc/bbcim.ps
	rm -f core *.core
	cd test; $(MAKE) clean

install.lib:
	cd lib; $(MAKE) INSTALL=$(HOME) install.lib

install: all
	cd prog; $(MAKE) INSTALL=$(HOME) install
	mkdir -p $(INSTALL)/man/man1/
	cp -fp doc/bbcim.1 $(INSTALL)/man/man1/

uninstall:
	cd prog; $(MAKE) INSTALL=$(HOME) uninstall
	rm -f $(INSTALL)/man/man1/bbcim.1


# // Compile man pages to postscript
.SUFFIXES: .1 .2 .3 .4 .5 .6 .7 .8 .9 .ps
.1.ps:
	groff -t -e -mandoc -Tps $< > $@


# EOF
