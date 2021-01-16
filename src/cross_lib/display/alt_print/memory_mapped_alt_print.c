
#if !defined(Z88DK_SPRITES)
    #include "memory_mapped_graphics.h"
#endif
#include "memory_mapped_alt_print.h"

#include "standard_libs.h"


#if defined(BUFFERED)	

	#include "display_target_geometry.h"
	
	extern uint8_t video_buffer[YSize+Y_OFFSET][XSize];
#endif


#if defined(CBM_SCREEN_CODES) || defined(__SUPERVISION__)
	char screenCode(char ch)
	{
		if(ch<64)
		{
			return ch;
		}
		else
		{
			return ch-64;
		}
		return ch;
	}
#elif (defined(__COCO__) || defined(__DRAGON__)) && defined(MEMORY_MAPPED)
	char screenCode(char ch)
	{
		if(ch==32) 
		{
			return 32+64;
		}
		else
		{
			return ch-32;
		}	
	}
#elif (defined(__COCO__) || defined(__DRAGON__)) && defined(BIT_MAPPED)
	char screenCode(char ch)
	{

			return ch-32;

	}
#elif (defined(__VIC20__) && defined(VIC20_EXP_8K)) 
	char screenCode(char ch)
	{
		if(ch<64)
		{
			return 128+ch;
		}
		else
		{
			return 64+ch;
		}	
	}
#elif ((defined(__C16__) && defined(C16_UNEXPANDED))) 
	char screenCode(char ch)
	{
		if(ch<64)
		{
			return 64+ch;
		}
		else
		{
			return ch;
		}	
	}
#endif

#  if defined(Z88DK_SPRITES)
	#include <graphics.h>
	#include <games.h>

	#include "display_macros.h"
	
	#define _DISPLAY(x,y,ch) \
		_draw_ch(x,y,ch);
	
#elif defined(BUFFERED)
	#define _DISPLAY(x,y,ch) \
		video_buffer[(y)][(x)] = (ch)
		
#elif defined(NO_GRAPHICS)
	#define _DISPLAY(x,y,ch)
		
#elif (defined(__C16__) && defined(C16_UNEXPANDED))  && defined(NO_SCREEN_CODES)
	#define _DISPLAY(x,y,ch) \
		do \
		{ \
			DISPLAY_POKE((loc(x,y)), ch); \
			DISPLAY_POKE((loc(x,y)-1024), PEEK(0x053B)); \
		} while(0)
#elif (defined(__C16__) && defined(C16_UNEXPANDED)) 
	#define _DISPLAY(x,y,ch) \
		do \
		{ \
			DISPLAY_POKE((loc(x,y)), screenCode(ch)); \
			DISPLAY_POKE((loc(x,y)-1024), PEEK(0x053B)); \
		} while(0)
#elif (defined(__VIC20__) && defined(VIC20_EXP_8K)) && defined(REDEFINED_CHARS)
	#define _DISPLAY(x,y,ch) \
		do \
		{ \
			DISPLAY_POKE((loc(x,y)), screenCode(ch)); \
			DISPLAY_POKE((0x8400+loc(x,y)), PEEK(0x0286)); \
		} while(0)
#elif defined(__C64__)
	#define _DISPLAY(x,y,ch) \
		do \
		{ \
			DISPLAY_POKE((loc(x,y)), screenCode(ch)); \
			DISPLAY_POKE((0x1800+loc(x,y)), PEEK(0x0286)); \
		} while(0)
#elif defined(__SUPERVISION__)
    #include "bit_mapped_4_graphics.h"
    #include "cross_lib.h"
    #define _DISPLAY(x,y,ch) \
		do \
		{ \
            uint8_t __k; \
            uint16_t __base = (x)+(XSize)*8*(y); \
            uint16_t __offset = (8*(uint8_t)(ch)) ; \
            \
            for(__k=0;__k<8;++__k) \
            { \
                SV_VIDEO[2*(x)+BYTES_PER_LINE*__k+BYTES_PER_LINE*8*(y)]    = left_map_one_to_two(udgs[__offset+__k]); \
                SV_VIDEO[2*(x)+BYTES_PER_LINE*__k+BYTES_PER_LINE*8*(y)+1]  = right_map_one_to_two(udgs[__offset+__k]); \
            } \
		} while(0)
#elif (defined(__COCO__) || defined(__DRAGON__)) && defined(BIT_MAPPED)
    #include "bit_mapped_graphics.h"
    #include "cross_lib.h"
    #define _DISPLAY(x,y,ch) \
        if(ch==0) \
        { \
            _XLIB_DELETE(x,y); \
        } \
        else \
        { \
            _XL_DRAW(x,y,(ch-13),0); \
        }

#else
	#define _DISPLAY(x,y,ch) \
		DISPLAY_POKE((loc(x,y)), (ch))
#endif


void PRINT(uint8_t x, uint8_t y, char * str)
{
	uint8_t i = 0;

	while(str[i]!='\0')
	{
		#if defined(CBM_SCREEN_CODES) || defined(__COCO__) || defined(__DRAGON__) || defined(__SUPERVISION__)
			_DISPLAY(x+i,y, screenCode(str[i]));
		#else
			_DISPLAY(x+i,y, str[i]);
		#endif
		++i;
	}
}


void PRINTD(uint8_t x, uint8_t y, uint8_t length, uint16_t val)
{
	uint8_t i;
	uint8_t digit;
	
	for(i=0;i<length;++i)
	{
		digit = (uint8_t) ((val)%10);
		val-= digit;
		val/=10;
		_DISPLAY(x+length-1-i,y, (uint8_t) (digit+(uint8_t) 48u));
	}
}


