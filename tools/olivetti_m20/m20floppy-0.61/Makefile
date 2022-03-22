# $Id: Makefile,v 1.22 2006/10/30 20:18:25 chris Exp $
#
# Makefile for
# Olivetti M20 PCOS diskette access utility
# Copyright (C) 2000,2001,2004,2006 by Christian Groessler
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING.  If not, write
# to the Free Software Foundation, 51 Franklin Street - Fifth Floor,
# Boston, MA 02110-1301, USA.

CC = cc
#CC = i386-go32-gcc
#E = .exe
# currently, the -DLINUX, -DNETBSD, etc. defines aren't really used in the code
# these switches are for Linux/x86
CFLAGS = -O2 -DUNIX -DLINUX -D__32BIT__ -DCPG_LITTLE_ENDIAN -Wall -Wstrict-prototypes # -DDEBUG
# these switches are for Linux/ia64 or Linux/alpha
#CFLAGS = -O2 -DUNIX -DLINUX -D__64BIT__ -DCPG_LITTLE_ENDIAN -Wall -Wstrict-prototypes
# these switches are for Linux/ppc
#CFLAGS = -O2 -DUNIX -DLINUX -D__32BIT__ -DCPG_BIG_ENDIAN -Wall -Wstrict-prototypes
# these switches are for Darwin/ppc
#CFLAGS = -O2 -DUNIX -DMACOSX -D__32BIT__ -DCPG_BIG_ENDIAN -Wall -Wstrict-prototypes # -DDEBUG
# these switches are for NetBSD/i386
#CFLAGS = -O2 -DUNIX -DNETBSD -D__32BIT__ -DCPG_LITTLE_ENDIAN -Wall -Wstrict-prototypes
# these switches are for FreeBSD/i386
#CFLAGS = -O2 -DUNIX -DFREEBSD -D__32BIT__ -DCPG_LITTLE_ENDIAN -Wall -Wstrict-prototypes
# these switches are for DJGPP
#CFLAGS = -O2 -DDOS -D__32BIT__ -DCPG_LITTLE_ENDIAN -Wall -Wstrict-prototypes # -DDEBUG

all: m20$E

clean:
	rm -f m20 m20$E main.o core roserv.o imgopen.o blockop.o util.o rwserv.o

m20$E: main.o util.o roserv.o imgopen.o blockop.o rwserv.o
	$(CC) $(LDFLAGS) -o $@ main.o util.o roserv.o imgopen.o blockop.o rwserv.o

main.o: main.c cpgtypes.h pcosdir.h fhdr.h cpgdefs.h packend.h packstrt.h cmdopts.h \
	roprotos.h imgopen.h version.h rwprotos.h
	$(CC) $(CFLAGS) -c -o $@ main.c

roserv.o: roserv.c roprotos.h pcosdir.h blockop.h cpgtypes.h util.h cpgdefs.h util.h
	$(CC) $(CFLAGS) -c -o $@ roserv.c

rwserv.o: rwserv.c rwprotos.h pcosdir.h blockop.h cpgtypes.h util.h cpgdefs.h fhdr.h util.h
	$(CC) $(CFLAGS) -c -o $@ rwserv.c

imgopen.o: imgopen.c imgopen.h
	$(CC) $(CFLAGS) -c -o $@ imgopen.c

blockop.o: blockop.c blockop.h cpgtypes.h cpgdefs.h
	$(CC) $(CFLAGS) -c -o $@ $*.c

util.o: util.c util.h cpgtypes.h pcosdir.h blockop.h cpgdefs.h fhdr.h
	$(CC) $(CFLAGS) -c -o $@ $*.c

