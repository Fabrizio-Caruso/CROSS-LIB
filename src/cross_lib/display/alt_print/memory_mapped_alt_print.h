#ifndef _MEMORY_MAPPED_ALT_PRINT
#define _MEMORY_MAPPED_ALT_PRINT

#include "standard_libs.h"

#if !defined(CC65)
	#define POKE(addr,val)     (*(uint8_t*) (addr) = (val))
	#define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
	#define PEEK(addr)         (*(uint8_t*) (addr))
	#define PEEKW(addr) (*(unsigned*) (addr))
#endif

#if defined(CBM_SCREEN_CODES)
	char screenCode(char ch);
#endif

void PRINT(uint8_t x, uint8_t y, char * str);

void print_05u0(uint8_t x, uint8_t y, uint16_t val);

void print_02u(uint8_t x, uint8_t y, uint16_t val);

void print_u(uint8_t x, uint8_t y, uint16_t val);

void PRINTD(uint8_t x, uint8_t y, char * str, uint16_t val);

#endif // _MEMORY_MAPPED_ALT_PRINT

