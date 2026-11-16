
/*
 * conio API test program
 *
 * keys:
 *
 * 1...0        change text color
 * F5/F6        change border color
 * F7/F8        change background color
 *
 */


#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <joystick.h>

int main()
{
    int i;
    int j;
    
    for(i=0;i<10;++i)
    {
        for(j=0;j<10;++j)
        {
            gotoxy(i,j);
            cputc('x');
        }
    }
    
    return 0;
}

