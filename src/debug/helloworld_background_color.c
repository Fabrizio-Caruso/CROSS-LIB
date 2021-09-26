#include<conio.h>
#include <sys/ioctl.h>

int main()
{
    int mode = 0;
    console_ioctl(IOCTL_GENCON_SET_MODE, &mode);

    textcolor(WHITE);    
    bordercolor(YELLOW);
    textbackground(RED); \
    clrscr();

    gotoxy(2,2);
    cprintf("hello world\n");
    while(1){};
    return 0;
}

