//#include <nstdlib.h>

#include "standard_libs.h"

// COMX
void gotoxycomx(unsigned int vidmem, unsigned char column){
	asm(
	" ldireg R8, 0x419A\n"
	" sex R8\n"
	" glo R13\n"
	" stxd\n"
	" dec R8\n"
	" glo R12\n"
	" stxd\n"
	" ghi R12\n"
	" stxd\n"
	" dec R8\n"
	" dec R8\n"
	" glo R12\n"
	" stxd\n"
	" ghi R12\n"
	" adi 0xf8\n"
	" stxd\n");
}


void vidstrcpy(uint16_t vidmem, char * text){ //write to video memory
	asm(
#if defined(__PECOM__)
	" sex R3\n"
	" out 1\n"
	" db  2\n"
    " sex R2\n"
#endif
	"$$cpy:\n"
	" lda R13 ;pick up input pointer\n"
	" bz $$end\n"
	" b1  $	;wait til video is quiet\n"
	" str R12 ;move the byte\n"
	" inc R12 ;++\n"
	" br $$cpy\n"
	"$$end:\n");
}


void PRINT(uint8_t x, uint8_t y, char * str)
{
    vidstrcpy((uint16_t) 0xf800+x+y*40,str);

}


#define _DISPLAY(x,y,ch) vidchar((uint16_t)0xf800+x+y*40, ch+48)

void print_05u0(uint8_t x, uint8_t y, uint16_t val)
{
	uint8_t i;
	uint8_t digits[6];
	
	digits[0] = 0;
	for(i=1;i<6;++i)
	{
		digits[i] = (uint8_t) ((val)%10);
		val-= digits[i];
		val/=10;
	}
	
	for(i=0;i<6;++i)
	{
		_DISPLAY(x+i,y, (uint8_t) (digits[5-i])+48);
	}
}	

void print_02u(uint8_t x, uint8_t y, uint16_t val)
{
	_DISPLAY(x,y, ((uint8_t) val)/10+48);
	_DISPLAY(1+x,y, ((uint8_t) val)%10+48);
}	


void print_u(uint8_t x, uint8_t y, uint16_t val)
{
	_DISPLAY(x,y, (uint8_t) (val+48));
}

unsigned char strlen(const char *s)
{
    unsigned char count = 0;
    
    while(*s)
    {
        ++count;
        ++s;
    };
    return count;
}

void PRINTF(uint8_t x, uint8_t y, char * str, uint16_t val)
{
	if(strlen(str)==5)
	{	
		print_05u0(x,y,val);
	}
	else if(strlen(str)==4)
	{
		print_02u(x,y,val);		
	}
	else
	{
		print_u(x,y,val);		
	}
}


//#include <nstdlib.c>

