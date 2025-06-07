#include <conio.h>



#define _XL_PRINTD(x,y,length,val) \
do \
{ \
    gotoxy(x,y); \
    cprintf("%0" #length "u",val); \
} while(0)

int main(void)
{
    // _XL_PRINTD(10,10,3,123);
    cprintf("%03u",123);

    while(1){};
    
    return 0;
}