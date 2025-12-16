#include <stdint.h>
#if defined(WIN32)
    #include <ncurses/curses.h>
#else
    #include <ncurses.h>
#endif
#include "display_macros.h"

void _XL_PRINT(uint8_t x, uint8_t y, const char * str)
{
	move(y,x); 
	printw("%s",str); 
	refresh();
}


// void _XL_PRINTD(uint8_t x, uint8_t y, uint8_t len, uint16_t value)
// {
// }


void _XL_CHAR(uint8_t x, uint8_t y, char ch)
{
    move(y,x);
    cputc(ch);
}
