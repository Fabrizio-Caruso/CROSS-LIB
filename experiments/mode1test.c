#include <cc65.h>
#include <conio.h>
#include <tgi.h>

int main(void)
{

	_graphics(1+16);
	_setcolor_low(0, TGI_COLOR_RED);
	_setcolor_low(1, TGI_COLOR_WHITE);
	_setcolor_low(2, TGI_COLOR_CYAN);
	_setcolor_low(3, TGI_COLOR_BROWN);
	_setcolor_low(4, TGI_COLOR_BLACK);


	while(1)
	{

	  char i;
	  
	  gotoxy(0,0);
	  for(i=0;i<8;++i)
	  {
		cputc('a'+64);
		cputc('b'+160);
	  }
	  
	  cgetc();


	  gotoxy(0,0);
	  
	  cprintf("4444");
	  // cprintf("06%d0",12345);
	  cgetc();
	  

	}

	return 0;
}
