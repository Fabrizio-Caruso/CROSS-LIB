/*
If compiled with gcc ./debug/ncurses_test.c -lncurses you get the correct behavior, i.e., it detects i and k key presses with auto-repeat. For example this works with gcc targeting the native machine or cygwin.

If the code above is cross-compiled with m68k-atari-mint-gcc ./debug/ncurses_test.c -lncurses -D__ATARI_ST__ then no key press is detected (tested on an emulated Atari ST with Hatari).
*/


#include <ncurses.h>

#define _NCURSES_BACKGROUND_COLOR COLOR_BLACK

#define INPUT_LOOPS 10


int main(void)
{
    volatile unsigned char ch;
    
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
    
    
    nodelay(stdscr,TRUE);    
    refresh();
    
    move(0,0);
    while(1)
    {
        ch = getch();
        
        if(ch=='i' || ch=='k')
        {
            printw("%c", ch);
        }
        refresh();
    }
    return 0;
}
```
