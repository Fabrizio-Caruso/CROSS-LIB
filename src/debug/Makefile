
EXEEXT?=.exe

gamate_conio_test:
	cl65$(EXEEXT) -tgamate gamate_conio_test.c
	../../tools/cc65/gamate/gamate-fixcart$(EXEEXT) gamate_conio_test


all: gamate_conio_test

clean: 
	rm -rf gamate_conio_test

