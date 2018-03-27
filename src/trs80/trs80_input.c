#include "trs80_input.h"

#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))
#define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
#define PEEK(addr)         (*(unsigned char*) (addr))
#define PEEKW(addr) (*(unsigned*) (addr))

char GET_CHAR(void)
{
	unsigned char ijkl = PEEK(0x3802);
	
	if(ijkl==2)
		return 'I';
	else if(ijkl==4)
		return 'J';
	else if(ijkl==8)
		return 'K';
	else if(ijkl==16)
		return 'L';
	
	if(PEEK(0x3840)==128)
		return ' ';
	else
		return '';
}