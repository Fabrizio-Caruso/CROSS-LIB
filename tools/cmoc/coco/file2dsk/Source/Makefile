AS = gcc

file2dsk:	file2dsk.c
	$(AS) $< -o$@.exe

file2dsk40:	file2dsk.c
	$(AS) $< -o$@.exe -DTRACKS40

file2dsk80:	file2dsk.c
	$(AS) $< -o$@.exe -DTRACKS80

all:	file2dsk file2dsk40 file2dsk80

clean:
	rm -f *.exe
