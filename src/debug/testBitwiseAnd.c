#include <conio.h>

int main()
{
   unsigned char i;
   unsigned char j;
   
   for(i=0;i<=10;++i)
   {
	gotoxy(0,i);
	
	cprintf("%d %d", i, i&3);
	
	j=i;
	j&=3;
	gotoxy(8,i);
	cprintf("%d", j);
   }

   while(1){};

   return 0;
}
