
#include <system.h>
#include <conio.h>

void INIT_GRAPHICS(void)
{
    set_text();
    charsetlc();
    textcolor(COLOR_WHITE);
    bgcolor(COLOR_DKBLUE);
    
    gotoxy(2,2);
    cprintf("Hello world\n");
    
    while(1){};
}

