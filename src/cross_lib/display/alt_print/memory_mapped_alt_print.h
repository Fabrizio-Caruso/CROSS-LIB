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

void _XL_PRINT(uint8_t x, uint8_t y, char * str);

void print_05u0(uint8_t x, uint8_t y, uint16_t val);

void print_02u(uint8_t x, uint8_t y, uint16_t val);

void print_u(uint8_t x, uint8_t y, uint16_t val);

void _XL_PRINTD(uint8_t x, uint8_t y, uint8_t length, uint16_t val);

// void _XL_CHAR(uint8_t x, uint8_t y, char ch);

// #if defined(CBM_SCREEN_CODES) || defined(__COCO__) || defined(__DRAGON__) || defined(__SUPERVISION__) \
    // || ((defined(__APPLE2__) || defined(__APPLE2ENH__)) && defined(APPLE2_HGR)) \
    // || (defined(__ZX81__) && !(defined(_API_VERSION) && _API_VERSION>=2))  \
    // || (defined(__C64__) && (defined(_API_VERSION) && _API_VERSION>=2))
    // #define _XL_CHAR(x,y,ch) \
        // _DISPLAY(x,y, screenCode(ch))
// #else
    // #define _XL_CHAR(x,y,ch) \
        // _DISPLAY(x,y, ch)
// #endif

#endif // _MEMORY_MAPPED_ALT_PRINT

