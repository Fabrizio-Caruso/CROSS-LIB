
EXEEXT?=.exe

CL65?=/home/fabri/CC65_fix/cc65/bin/cl65$(EXEEXT)

gamate_conio_test:
	$(CL65) -tgamate gamate_conio_test.c
	../../tools/cc65/gamate/gamate-fixcart$(EXEEXT) gamate_conio_test


gamate_conio_test2:
	$(CL65) -tgamate gamate_conio_test2.c
	../../tools/cc65/gamate/gamate-fixcart$(EXEEXT) gamate_conio_test2


all: gamate_conio_test gamate_conio_test2

clean: 
	rm -rf gamate_conio_test
	rm -rf gamate_conio_test2
