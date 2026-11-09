#ifndef _CONIO_GRAPHICS_H
#define _CONIO_GRAPHICS_H

#if defined(__C128__) && defined(__80COL_UDG)
    #include "display_macros.h"
    #define VDC_DATA_REGISTER     0x1F
    #define HIGH_ADDRESS_REGISTER 0x12
    #define LOW_ADDRESS_REGISTER  0x13

#endif


#if !defined(_XL_NO_COLOR)
    #if defined(__NO_BOTTOM)
        #define _XL_DRAW(x,y,tile,color) \
        do \
        { \
            if(y<YSize-1) \
            { \
                gotoxy((X_OFFSET+(x)),(Y_OFFSET+(y))); \
                _XL_SET_TEXT_COLOR(color); \
                cputc(tile); \
            } \
        } \
        while(0)
    #elif defined(__CX16__)
        #define _XL_DRAW(x,y,tile,color) \
        do \
        { \
            vpoke(tile, 0x1b000 + 256U * (y) + 2U * (x)); \
            vpoke(color, 0x1b001 + 256U * (y) + 2U * (x)); \
        } \
        while(0)
    #elif defined(__VIC20__) && defined(__VIC20_EXP_8K)
        #define _XL_DRAW(x,y,tile,color) \
        do \
        { \
            vic20_tile_write(x,y,tile,color); \
        } \
        while(0)
    #elif defined(__80COL_UDG)
        #define _XL_DRAW(x,y,tile,color) \
        do \
        { \
            vdc_tile_write(x, y, tile, color); \
        } \
        while(0)
    #else
        #define _XL_DRAW(x,y,tile,color) \
        do \
        { \
            gotoxy((X_OFFSET+(x)),(Y_OFFSET+(y))); \
            _XL_SET_TEXT_COLOR(color); \
            cputc(tile); \
        } \
        while(0)
        
    #endif

#elif defined(__KIM1__) || defined(__SYM1__)

		#define _XL_DRAW(x,y,tile,color) \
		do \
		{ \
			gotoxy((X_OFFSET+x),(Y_OFFSET+y)); \
			putchar(tile); \
		} \
		while(0)
#else

	#if defined(__NO_BOTTOM)
		#define _XL_DRAW(x,y,tile,color) \
		do \
		{ \
			if(y<YSize-1) \
			{ \
				gotoxy((X_OFFSET+x),(Y_OFFSET+y)); \
				cputc(tile); \
			} \
		} \
		while(0)
    #elif defined(__ATARI__) && !defined(__ATARI_ANTIC_15)
        #include<peekpoke.h>
        extern uint16_t BASE_ADDR;
        #define _XL_DRAW(x,y,tile,color) \
        do \
        { \
            POKE(BASE_ADDR+(x)+(y)*(XSize),tile); \
        } \
        while(0)
	#else

		#define _XL_DRAW(x,y,tile,color) \
		do \
		{ \
			gotoxy((X_OFFSET+x),(Y_OFFSET+y)); \
			cputc(tile); \
		} \
		while(0)
    #endif

#endif

#if defined(__NO_BOTTOM)
	#define _XL_DELETE(x,y) \
		do \
		{ \
			if(y<YSize-1) \
			{ \
				gotoxy((X_OFFSET+x),(Y_OFFSET+y)); \
				cputc(_SPACE); \
			} \
		} \
		while(0)
#elif defined(__KIM1__) || defined(__SYM1__)
	#define _XL_DELETE(x,y) \
		do \
		{ \
			gotoxy((X_OFFSET+x),(Y_OFFSET+y)); \
			putchar(_SPACE); \
		} \
		while(0)
#else
	
	#define _XL_DELETE(x,y) \
		do \
		{ \
			gotoxy((X_OFFSET+x),(Y_OFFSET+y)); \
			cputc(_SPACE); \
		} \
		while(0)

#endif
		
#endif // _CONIO_GRAPHICS_H



