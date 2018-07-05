
#include <conio.h>

	
void PRINT(unsigned char x, unsigned char y, char * str)
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


void PRINTF(unsigned char x, unsigned char y, char * str, unsigned short val)
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

