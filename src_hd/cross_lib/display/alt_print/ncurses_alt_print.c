#include <stdint.h>
#include <ncurses.h>
#include "display_macros.h"

void _XL_PRINT(uint8_t x, uint8_t y, const char * str)
{
	move(y,x); 
	printw("%s",str); 
	refresh();
}


void _XL_CHAR(uint8_t x, uint8_t y, char ch)
{
    move(y,x);
    putchar(ch);
}
