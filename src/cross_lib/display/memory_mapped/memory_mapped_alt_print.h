#ifndef _MEMORY_MAPPED_ALT_PRINT
#define _MEMORY_MAPPED_ALT_PRINT

#include "memory_mapped_graphics.h"

#if defined(CBM_SCREEN_CODES)
	char screenCode(char ch);
#endif

void PRINT(unsigned char x, unsigned char y, char * str);

void print_05u0(unsigned char x, unsigned char y, unsigned short val);

void print_02u(unsigned char x, unsigned char y, unsigned short val);

void print_u(unsigned char x, unsigned char y, unsigned short val);

void PRINTF(unsigned char x, unsigned char y, char * str, unsigned short val);

#endif // _MEMORY_MAPPED_ALT_PRINT

