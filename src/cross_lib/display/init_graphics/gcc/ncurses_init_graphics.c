#if defined(__ATARI_ST__)
    #include <ncurses/curses.h>
#else
    #include <ncurses.h>
#endif

#include "display_macros.h"


void _XL_INIT_GRAPHICS(void)
{
    #if defined(__ATARI_ST__)
        putenv("TERM=st52");
    #endif
	initscr();   
	noecho();
	curs_set(0);
	start_color();
	cbreak();
	intrflush(stdscr, TRUE);
	init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
	init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
	init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
	init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
	init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
	init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
	init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    
    setScreenColors();
}

