#include<conio.h>

#define cgetc() getch()

void wait_for_key(void)
{
    while(kbhit())
        (void) cgetc();
    while(!kbhit())
    { 
    }; 
    (void) cgetc();
}


int main(void)
{

    clrscr();
    
    while(1)
    {
        printf("press a key\n");
        wait_for_key();
    }

    return 0;
}
