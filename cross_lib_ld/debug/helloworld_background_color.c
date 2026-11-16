#include<conio.h>
#include <sys/ioctl.h>

#if defined(__MSX__)
    #define MODE 2
#else
    #define MODE 0
#endif

int main()
{
    int mode = MODE;
    console_ioctl(IOCTL_GENCON_SET_MODE, &mode);

    textcolor(WHITE);    
    bordercolor(YELLOW);
    textbackground(RED);
    clrscr();

    gotoxy(2,2);
    cprintf("hello world\n");
    while(1){};
    return 0;
}

