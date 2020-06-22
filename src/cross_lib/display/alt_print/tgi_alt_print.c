#include <tgi.h>

#include "standard_libs.h"


void PRINT(uint8_t x, uint8_t y, char * str)
{
	tgi_outtextxy(x*8,y*8,str);
}



void PRINTD(uint8_t x, uint8_t y, uint8_t length, uint16_t val)
{

}



