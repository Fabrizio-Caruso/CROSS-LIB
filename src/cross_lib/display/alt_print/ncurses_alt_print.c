#include <stdint.h>
#include <ncurses.h>


void PRINT(uint8_t x, uint8_t y, char * str)
{
	move(y,x); 
	printw(str); 
	refresh();
}

void PRINTF(uint8_t x, uint8_t y, char * str, uint16_t val)
{
	move(y,x); 
	printw(str,val); 
	refresh();
}
