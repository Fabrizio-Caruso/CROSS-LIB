
#include "aquarius/aquarius_input.h"

#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))
#define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
#define PEEK(addr)         (*(unsigned char*) (addr))
#define PEEKW(addr) (*(unsigned*) (addr))

unsigned char GET_CHAR(void)
{
	if(PEEK(14351) == 6)
		return PEEK(14346);
	else
		return '\0';
}
