// #if defined(__ATARI_ST__)
    // #include <ncurses/curses.h>
// #else
    #include <ncurses.h>
// #endif

#include "display_macros.h"

#if defined(__BACKGROUND_COLOR) && __BACKGROUND_COLOR==1
    #define _NCURSES_BACKGROUND_COLOR COLOR_WHITE
#else
    #define _NCURSES_BACKGROUND_COLOR COLOR_BLACK
#endif

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
	init_pair(1, COLOR_YELLOW, _NCURSES_BACKGROUND_COLOR);
	init_pair(2, COLOR_CYAN, _NCURSES_BACKGROUND_COLOR);
	init_pair(3, COLOR_RED, _NCURSES_BACKGROUND_COLOR);
	init_pair(4, COLOR_GREEN, _NCURSES_BACKGROUND_COLOR);
	init_pair(5, COLOR_BLUE, _NCURSES_BACKGROUND_COLOR);
	init_pair(6, COLOR_WHITE, _NCURSES_BACKGROUND_COLOR);
	init_pair(7, COLOR_MAGENTA, _NCURSES_BACKGROUND_COLOR);
	init_pair(8, COLOR_BLACK, _NCURSES_BACKGROUND_COLOR);

    wbkgd(stdscr, COLOR_PAIR(1));
    
    refresh();
    _setScreenColors();
}

