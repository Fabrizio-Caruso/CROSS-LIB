
#include <conio.h>
#include <unistd.h>
#include <joystick.h>
#include <stdint.h>


void waitForJoy(void)
{
    while(!(JOY_BTN_1(joy_read(0)))){};
}


void display_3_rows(uint8_t x_offset, uint8_t y_offset)
{
    uint8_t i;
    uint16_t j;
    
    for(i=0;i<3;++i)
    {
        gotoxy(x_offset,y_offset+i);
        textcolor(1+i);
        cprintf("hello");
        for(j=0;j<10000;++j){};
        waitForJoy();
    }
}


int main(void)
{
    uint8_t i;
    
    joy_install((void *)joy_static_stddrv);    

    while(1)
    {
        clrscr();
        
        for(i=0;i<3;++i)
        {
        display_3_rows(2+i,2+i);
        waitForJoy();
        }
        
        for(i=0;i<3;++i)
        {
        display_3_rows(2+5*i,9+i);
        waitForJoy();
        }
        sleep(1);
    }
    return 0;
}