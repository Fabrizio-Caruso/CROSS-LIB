# Makefile for CPCXFS
#
# Modified by Kev 27-Oct-2000 to compile with linux and dos-DJGPP

CC	= gcc

#uncomment this line to compile unix/linux version
#LINUX_VER = 1

#uncomment this line to compile dos version
DOS_VER = 1


CCOPT = -Wall -O3

ifdef LINUX_VER
CCOPT += -DLINUX=1 -DDOS=0
endif

ifdef DOS_VER
CCOPT += -DLINUX=0 -DDOS=1
endif

LDOPT	=

CFLAGS	= $(CCOPT)
LDARGS  = $(LDOPT)

ifdef LINUX_VER
LDARGS += -lreadline -lhistory -ltermcap
endif

HEADER=cpcfs.h 

ifdef LINUX_VER
HEADER+=unix.h
endif

ifdef DOS_VER
HEADER+=dos.h
endif

OBJECTS=match.o tools.o fs.o ui.o cpcfs.o readdef.o

ifdef LINUX_VER
OBJECTS+=unix.o
endif

ifdef DOS_VER
OBJECTS+=dos.o
endif

default: makedoc cpcxfs 

cpcxfs: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o cpcxfs $(LDARGS)

makedoc: makedoc.c
	$(CC) $(CFLAGS) makedoc.c -o makedoc

unix.o: unix.c unix.h
match.o: match.c match.h

tools.o: tools.c $(HEADER)
fs.o: fs.c $(HEADER)
ui.o: ui.c $(HEADER)
cpcfs.o: cpcfs.c $(HEADER)


strip: cpcfs makedoc
	strip cpcfs makedoc

new:
	touch $(HEADER)
	make

