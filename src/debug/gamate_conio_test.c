
#include <conio.h>
#include <unistd.h>
#include <joystick.h>

void waitForJoy(void)
{
    while(!(JOY_BTN_1(joy_read(0)))){};
}


int main(void)
{    
    joy_install((void *)joy_static_stddrv);    

    while(1)
    {
        clrscr();
        
        gotoxy(2,2);
        textcolor(COLOR_BLACK);
        cprintf("hello");
        sleep(1);
        waitForJoy();
        
        textcolor(COLOR_WHITE);
        gotoxy(2,2);
        cprintf("ciao!");
        sleep(1);
        waitForJoy();
        
        gotoxy(2,2);
        textcolor(1);
        cprintf("hello");
        sleep(1);
        waitForJoy();
        
        textcolor(2);
        gotoxy(2,2);
        cprintf("ciao!");
        sleep(1);
        waitForJoy();

        gotoxy(2,2);
        textcolor(2);
        cprintf("hello");
        sleep(1);
        waitForJoy();
        
        textcolor(1);
        gotoxy(2,2);
        cprintf("ciao!");
        sleep(1);
        waitForJoy();
        
        gotoxy(2,2);
        textcolor(COLOR_BLACK);
        cprintf("hello");
        sleep(1);
        waitForJoy();
        
        textcolor(COLOR_WHITE);
        gotoxy(2,2);
        cprintf("ciao!");    
    }
    return 0;
}