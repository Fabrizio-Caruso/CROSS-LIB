#if defined(__ATARI_ST__)
    #include <ncurses/curses.h>
#else
    #include <ncurses.h>
#endif

#include "display_macros.h"

#if defined(_BACKGROUND_COLOR) && BACKGROUND_COLOR==_XL_WHITE
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
	init_pair(COLOR_YELLOW, COLOR_YELLOW, _NCURSES_BACKGROUND_COLOR);
	init_pair(COLOR_CYAN, COLOR_CYAN, _NCURSES_BACKGROUND_COLOR);
	init_pair(COLOR_RED, COLOR_RED, _NCURSES_BACKGROUND_COLOR);
	init_pair(COLOR_GREEN, COLOR_GREEN, _NCURSES_BACKGROUND_COLOR);
	init_pair(COLOR_BLUE, COLOR_BLUE, _NCURSES_BACKGROUND_COLOR);
	init_pair(COLOR_WHITE, COLOR_WHITE, _NCURSES_BACKGROUND_COLOR);
	init_pair(COLOR_MAGENTA, COLOR_MAGENTA, _NCURSES_BACKGROUND_COLOR);
	init_pair(COLOR_BLACK, COLOR_BLACK, _NCURSES_BACKGROUND_COLOR);
    wbkgd(stdscr, COLOR_PAIR(1));
    refresh();
    setScreenColors();
}

