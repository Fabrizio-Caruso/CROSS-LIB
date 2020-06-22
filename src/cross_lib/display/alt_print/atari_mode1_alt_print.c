
#include <stdint.h>
#include <conio.h>

void _GOTOXY(uint8_t x, uint8_t y)
{ 
	if((y)&1) 
	{ 
		gotoxy(x+20,(y)/2);
	} 
	else 
	{ 
		gotoxy(x, (y)/2);
	} 
};

void PRINT(uint8_t x, uint8_t y, char * str)
{ 
    _GOTOXY(x,y);
    cprintf(str); 
};




