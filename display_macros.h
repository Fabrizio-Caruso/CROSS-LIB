#ifndef _DISPLAY_MACROS
#define _DISPLAY_MACROS

#include <conio.h>

#define DRAW(x,y,ch) {gotoxy((x),(y)); cputc(ch);};

#define PRINT(x,y,str) {gotoxy(x,y); cputs(str); };

#define PRINTF(x,y,...) {gotoxy(x,y); cprintf(##__VA_ARGS__); };

#define CLEAR_SCREEN() clrscr();

#define DRAW_BORDERS()\
{ \
	cputc (CH_ULCORNER);\
    chline (XSize - 2);\
    cputc (CH_URCORNER);\
    cvlinexy (0, 1, YSize - 2);\
    cputc (CH_LLCORNER);\
    chline (XSize - 2);\
    cputc (CH_LRCORNER);\
    cvlinexy (XSize - 1, 1, YSize - 2); \
}

#define DRAW_VERTICAL_LINE(x,y,length) cvlinexy (x,y,length);
	
#define SET_TEXT_COLOR(c) (void) textcolor (c);

#define SET_BORDER_COLOR(c) (void) bordercolor (c);

#define SET_BACKGROUND_COLOR(c) (void) bgcolor (c);

	
#endif // _DISPLAY_MACROS