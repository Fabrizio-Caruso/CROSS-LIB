ifneq ($(COMSPEC),)
DO_WIN:=1
endif
ifneq ($(ComSpec),)
DO_WIN:=1
endif 

ifeq ($(DO_WIN),1)
EXEEXT = .exe
endif

ifeq ($(DO_WIN),1)
COMPILEDEXT = .exe
else
COMPILEDEXT = .out
endif

AS = gcc

file2dsk:	file2dsk.c
	$(AS) $< -o$@$(COMPILEDEXT)

file2dsk40:	file2dsk.c
	$(AS) $< -o$@$(COMPILEDEXT) -DTRACKS40

file2dsk80:	file2dsk.c
	$(AS) $< -o$@$(COMPILEDEXT) -DTRACKS80

all:	file2dsk file2dsk40 file2dsk80

clean:
	rm -f *.$(COMPILEDEXT)
