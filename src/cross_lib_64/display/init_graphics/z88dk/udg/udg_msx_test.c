
#include <graphics.h>

#include "display_macros.h"

#include <sys/ioctl.h>

#include <conio.h>

void _XL_INIT_GRAPHICS(void)
{
	
    int mode = mode_2;
    console_ioctl(IOCTL_GENCON_SET_MODE, &mode);
    
    gotoxy(2,2);
    printf("hello world!\n");
    
    gotoxy(0,0);
    printf("a");
    
    gotoxy(31,0);
    printf("b");
    
    gotoxy(0,23);
    printf("c");
    
    gotoxy(31,23);
    printf("d");
    
    while(1){};
    _setScreenColors();
}


