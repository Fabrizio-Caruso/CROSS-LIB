#include <gamate.h>
#include <peekpoke.h>

int main()
{
   POKE(LCD_X,4);
   POKE(LCD_Y,4);
   POKE(LCD_DATA,255);
   // POKE(LCD_X,5);
   POKE(LCD_DATA,255);
   POKE(LCD_X,6+0x80);
   POKE(LCD_DATA,255);
   

   while(1){};

   return 0;
}

