

#include <conio.h>
#include <unistd.h>

int main(void)
{
    clrscr();
    gotoxy(2,2);
    textcolor(COLOR_BLACK);
    cprintf("hello");
    sleep(1);
    textcolor(COLOR_WHITE);
    gotoxy(2,2);
    cprintf("ciao!");
    sleep(1);
    gotoxy(2,2);
    textcolor(1);
    cprintf("hello");
    sleep(1);
    textcolor(2);
    gotoxy(2,2);
    cprintf("ciao!");
    sleep(1);
    gotoxy(2,2);
    textcolor(COLOR_BLACK);
    cprintf("hello");
    sleep(1);
    textcolor(COLOR_WHITE);
    gotoxy(2,2);
    cprintf("ciao!");    
    while(1){};
    return 0;
}