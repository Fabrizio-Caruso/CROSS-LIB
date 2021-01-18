#include <stdint.h>
#if defined(__ATARI_ST__)
    #include <ncurses/curses.h>
#else
    #include <ncurses.h>
#endif


void _XL_PRINT(uint8_t x, uint8_t y, char * str)
{
	move(y,x); 
	printw(str); 
	refresh();
}

