
#include <graphics.h>


#include <sys/ioctl.h>

#include <conio.h>

int main(void)
{


    int mode = mode_2;
    console_ioctl(IOCTL_GENCON_SET_MODE, &mode);
        
    
    gotoxy(2,2);
    
    printf("hello world!\n");
    
    
    textcolor(RED);
    gotoxy(0,0);
    printf("a");
    
    gotoxy(31,0);
    printf("b");
    
    textcolor(BLUE);
    gotoxy(0,23);
    printf("c");
    
    gotoxy(31,23);
    printf("d");
    
    while(1){};
    
    return 0;
}
