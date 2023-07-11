#include <conio.h>

int main()
{
   gotoxy(10,10);
   
   cprintf("hello world\n");

   gotoxy(10,13);

   cprintf("au revoir\n");

   while(1){};

   return 0;
}
