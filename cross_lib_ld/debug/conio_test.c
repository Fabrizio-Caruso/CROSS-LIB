#include <conio.h>

int main()
{
   gotoxy(0,1);
   
   cprintf("hello world");

   gotoxy(0,2);

   cprintf("au revoir");

   gotoxy(0,3);
   
   cprintf("%d", XSize);

   gotoxy(5,3);
   
   cprintf("%d", YSize);

   gotoxy(0,0);
   
   // cprintf("01234567890123456789012345678901234567890123456789012345678901234567890123456789");
   cprintf("0123456789012345678901234567890123456789");
   
   gotoxy(0,24);
   
   // cprintf("01234567890123456789012345678901234567890123456789012345678901234567890123456789");
   cprintf("0123456789012345678901234567890123456789");

   while(1){};

   return 0;
}
