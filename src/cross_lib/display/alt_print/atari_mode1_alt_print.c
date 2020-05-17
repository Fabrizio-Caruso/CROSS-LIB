
#include <stdint.h>
#include <conio.h>

	
void PRINT(uint8_t x, uint8_t y, char * str)
{ 
	if((y)&1)
	{ 
		gotoxy(x+20,(y)/2); 
	} 
	else 
	{ 
		gotoxy(x, (y)/2); 
	} 
	cprintf(str); 
};


void PRINTD(uint8_t x, uint8_t y, char * str, uint16_t val)
{ 
	if((y)&1) 
	{ 
		gotoxy(x+20,(y)/2);
	} 
	else 
	{ 
		gotoxy(x, (y)/2);
	} 
	cprintf(str, val); 
};

