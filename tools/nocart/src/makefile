# Makefile for NOCART utility

.PHONY: clean

CC = gcc
BIND = gcc
RM = rm

#   CFLAGS    flags for C compile
#   LFLAGS1   flags after output file spec, before obj file list
#   LFLAGS2   flags after obj file list (libraries, etc)

CFLAGS = -O2 -O3
LFLAGS1 =
LFLAGS2 = -s 

NOCART_O=	opth.o main.o 

NOCART: $(NOCART_O)
	$(BIND)  $(NOCART_O) -o nocart $(LFLAGS1) $(LFLAGS2) $(LIBS)

clean:
	rm -rf *.o
	rm -rf *.exe
	rm -rf nocart
	