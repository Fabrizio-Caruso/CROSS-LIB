CC1541_CFLAGS = -std=c99 -pipe -O2 -Wall -Wextra -pedantic

ALL_TARGETS = cc1541

ifneq ($(ENABLE_MAN),)
ALL_TARGETS += man
endif

ifneq ($(ENABLE_WERROR),)
CC1541_CFLAGS += -Werror
endif

override CFLAGS := $(CC1541_CFLAGS) $(CFLAGS)

prefix ?= /usr/local
bindir ?= $(prefix)/bin
mandir ?= $(prefix)/share/man

INSTALL ?= install

VERSION := $(shell grep 'define VERSION' cc1541.c | cut -d\" -f2)

CC1541_SRC := Makefile transwarp\ v0.84.prg transwarp\ v0.86.prg $(wildcard *.c *.h *.in *.sln *.vcxproj* LICENSE* README*)

all: $(ALL_TARGETS)

cc1541: cc1541.c

cc1541.1.txt: cc1541.1.txt.in
	sed -e "s/@@VERSION@@/$(VERSION)/g" < cc1541.1.txt.in > cc1541.1.txt

cc1541.1: cc1541.1.txt
	a2x -d manpage -f manpage cc1541.1.txt

man: cc1541.1

test_cc1541: test_cc1541.c

check: cc1541 test_cc1541
	./test_cc1541 ./cc1541

test: check

install: all
	$(INSTALL) -Dpm 0755 ./cc1541 $(DESTDIR)$(bindir)/cc1541
ifneq ($(ENABLE_MAN),)
	$(INSTALL) -Dpm 0644 ./cc1541.1 $(DESTDIR)$(mandir)/man1/cc1541.1
endif

cc1541-$(VERSION).tar: $(CC1541_SRC)
	rm -rf cc1541-$(VERSION)/ *~ README.md.T
	mkdir -p cc1541-$(VERSION)
	cp -a $(CC1541_SRC) cc1541-$(VERSION)/
	chmod 0644 cc1541-$(VERSION)/*
	tar cf cc1541-$(VERSION).tar cc1541-$(VERSION)/
	rm -rf cc1541-$(VERSION)/

cc1541-$(VERSION).tar.bz2: cc1541-$(VERSION).tar
	bzip2 -9cz < cc1541-$(VERSION).tar > cc1541-$(VERSION).tar.bz2

cc1541-$(VERSION).tar.gz: cc1541-$(VERSION).tar
	gzip -9c < cc1541-$(VERSION).tar > cc1541-$(VERSION).tar.gz

cc1541-$(VERSION).tar.xz: cc1541-$(VERSION).tar
	xz -ce < cc1541-$(VERSION).tar > cc1541-$(VERSION).tar.xz

cc1541-$(VERSION).zip: $(CC1541_SRC)
	rm -rf cc1541-$(VERSION)/ *~ README.md.T
	mkdir -p cc1541-$(VERSION)
	cp -a $(CC1541_SRC) cc1541-$(VERSION)/
	chmod 0644 cc1541-$(VERSION)/*
	zip -9r cc1541-$(VERSION).zip cc1541-$(VERSION)/
	rm -rf cc1541-$(VERSION)/

dist-bz2: cc1541-$(VERSION).tar.bz2
dist-gz:  cc1541-$(VERSION).tar.gz
dist-xz:  cc1541-$(VERSION).tar.xz
dist-zip: cc1541-$(VERSION).zip
dist-all: dist-bz2 dist-gz dist-xz dist-zip

dist: dist-gz dist-zip

dist-check: dist
	tar xf cc1541-$(VERSION).tar.gz
	$(MAKE) -C cc1541-$(VERSION)/ all check
	rm -rf cc1541-$(VERSION)/

codestyle: cc1541.c test_cc1541.c
	astyle --style=kr -n -s -z2 cc1541.c test_cc1541.c

wrap: cc1541.1.txt.in LICENSE.txt README.md
	for f in $^; do \
	  fold -s -w 70 < $$f | perl -pe 's/[\t\040]+$$//' > $$f.T; \
	  mv -f $$f.T $$f; \
	done

clean:
	rm -rf cc1541-$(VERSION)/ *~ README.md.T *.o *.orig cc1541 test_cc1541 cc1541-$(VERSION).* cc1541.1 cc1541.1.txt

.PHONY: all check clean codestyle dist dist-all dist-bz2 dist-check dist-gz dist-xz dist-zip install man test wrap

.NOTPARALLEL: cc1541-$(VERSION).tar cc1541-$(VERSION).zip
