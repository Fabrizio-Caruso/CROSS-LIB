#include <conio.h>
#include <graphics.h>
#include <sys/ioctl.h>


int main(void)
{

    int mode = 0;
    console_ioctl(IOCTL_GENCON_SET_MODE, &mode);
    
    textbackground(BLACK);
    
    printf("Hello world\n");
    getchar();
    
    bordercolor(BLACK);
    
    clrscr(); 
    textcolor(RED);
    gotoxy(2,2);
    printf("Hello world\n");
    
    while(1){};
    return 0;
}

