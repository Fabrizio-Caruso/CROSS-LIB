#ifndef _MEMORY_MAPPED_ALT_PRINT
#define _MEMORY_MAPPED_ALT_PRINT

#include "cross_lib.h"


#if !defined(__CC65__) && !defined(POKE) && !defined(PEEK)
	#define POKE(addr,val)     (*(uint8_t*) (addr) = (val))
	#define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
	#define PEEK(addr)         (*(uint8_t*) (addr))
	#define PEEKW(addr) (*(unsigned*) (addr))
#endif

#if defined(CBM_SCREEN_CODES)
	char screenCode(char ch);
#endif

// void _XL_PRINT(uint8_t x, uint8_t y, const char * str);

// void _XL_PRINTD(uint8_t x, uint8_t y, uint8_t length, uint16_t val);

#endif // _MEMORY_MAPPED_ALT_PRINT

