#include <stdint.h>
#if defined(__ATARI_ST__)
    #include <ncurses/curses.h>
#else
    #include <ncurses.h>
#endif


void PRINT(uint8_t x, uint8_t y, char * str)
{
	move(y,x); 
	printw(str); 
	refresh();
}

void PRINTD(uint8_t x, uint8_t y, char * str, uint16_t val)
{
	move(y,x); 
	printw(str,val); 
	refresh();
}
