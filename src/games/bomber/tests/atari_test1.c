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

#define BASE_ADDR_VALUE (PEEK(88)+PEEK(89)*256)

#define _delete(i,j) POKE(BASE_ADDR_VALUE+i+j*20,0)

#define _draw(i,j) POKE(BASE_ADDR_VALUE+i+j*20,'x'+192)

#include <peekpoke.h>

void CLEAR_SCREEN(void)
{
    uint8_t i;
    uint8_t j;
    
    for(i=0;i<20;++i)
    {

        for(j=0;j<24;++j)
        {
            _delete(i,j);
        }
    }
    *(unsigned char *)0x5d = 0;
}



int main(void)
{
    uint8_t i;
    uint8_t j;
    
	_graphics(1+16);
	
	_setcolor_low(0, TGI_COLOR_RED);
	_setcolor_low(1, TGI_COLOR_WHITE);
	_setcolor_low(2, TGI_COLOR_CYAN); 	
	_setcolor_low(3, TGI_COLOR_BROWN);
	_setcolor_low(4, TGI_COLOR_BLACK);
    
    while(1)
    {
        CLEAR_SCREEN();
        PRINT(2,7,"hello world 1");
        sleep(1);
        
        gotoxy(0,0);
        for(i=0;i<20;++i)
        {
            for(j=0;j<24;++j)
            {
                _draw(i,j);
            }
        }
        cgetc();
        sleep(1);
        CLEAR_SCREEN();
        sleep(1);
        
        PRINT(2,9,"hello world 2");
        sleep(1);
        CLEAR_SCREEN();
        PRINT(2,5,"hello world 3");
        sleep(2);
        
    }
    return 0;
}