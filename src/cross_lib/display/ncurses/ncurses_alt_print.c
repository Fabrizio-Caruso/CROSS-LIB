#include <ncurses.h>
	
void PRINT(unsigned char x, unsigned char y, char * str)
{
	move(y,x); 
	printw(str); 
	refresh();
}

void PRINTF(unsigned char x, unsigned char y, char * str, unsigned short val)
{
	move(y,x); 
	printw(str,val); 
	refresh();
}
		