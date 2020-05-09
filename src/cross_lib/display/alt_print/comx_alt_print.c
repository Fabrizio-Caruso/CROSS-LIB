#include <nstdlib.h>

#include "standard_libs.h"

void gotoxycomx(unsigned int vidmem, unsigned char column){
	asm(
	" ldireg R8, 0x419A\n"
	" sex R8\n"
	" glo R13\n"
	" stxd\n"
	" dec R8\n"
	" glo R12\n"
	" stxd\n"
	" ghi R12\n"
	" stxd\n"
	" dec R8\n"
	" dec R8\n"
	" glo R12\n"
	" stxd\n"
	" ghi R12\n"
	" adi 0xf8\n"
	" stxd\n");
}

// void gotoxy(unsigned char x, unsigned char y){
	// unsigned int vidmem;
	// vidmem = y*40 + x;
    // gotoxycomx(vidmem, x);
// }

#define gotoxy(x,y) gotoxycomx((y)*40 + (x), (x))

// #define cprintf 

void PRINT(uint8_t x, uint8_t y, char * str)
{
	gotoxy(x,y);
    // cprintf(str);
}

void PRINTF(uint8_t x, uint8_t y, char * str, uint16_t val)
{
	gotoxy(x,y);
    // cprintf(str);
}


// #include <nstdlib.c>

