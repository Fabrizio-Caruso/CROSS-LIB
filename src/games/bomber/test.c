#include <stdint.h>
#include <conio.h>
#include <unistd.h>

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


int main(void)
{
    uint16_t i;
    
	_graphics(1+16);
	
	_setcolor_low(0, TGI_COLOR_RED);
	_setcolor_low(1, TGI_COLOR_WHITE);
	_setcolor_low(2, TGI_COLOR_CYAN); 	
	_setcolor_low(3, TGI_COLOR_BROWN);
	_setcolor_low(4, TGI_COLOR_BLACK);
    
    while(1)
    {
        PRINT(2,7,"hi world");
        sleep(1);
        
        gotoxy(0,0);
        for(i=0;i<20*24;++i)
        {
            cprintf("x");
        }
        
        sleep(1);
        
        PRINT(2,9,"hello world");
        sleep(1);
        
        
    }
    return 0;
}