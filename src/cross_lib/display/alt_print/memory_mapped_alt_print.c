
#if !defined(__Z88DK_SPRITES_GRAPHICS)
    #if defined(__QUAD_MEMORY_MAPPED_GRAPHICS)
        #include "quad_memory_mapped_graphics.h"
    #elif defined(__DUAL_MEMORY_MAPPED_GRAPHICS)
        #include "dual_memory_mapped_graphics.h"
    #else
        #include "memory_mapped_graphics.h"
    #endif
#endif


#include "memory_mapped_alt_print.h"

#include "standard_libs.h"

#include "display_macros.h"

#if defined(__BUFFERED_GRAPHICS)	

	#include "display_target_geometry.h"
	
	extern uint8_t video_buffer[YSize+Y_OFFSET][XSize];
#endif


#  if defined(__C64__)
	char screenCode(char ch)
	{
        if(ch>=65)
        {
            return 64+ch;
        }
        else
        {
            return ch;
        }
	}
    
#elif defined(__QUAD_MEMORY_MAPPED_GRAPHICS)
	char screenCode(char ch)
	{
        return ch-(uint8_t) 32u;
	}  
#elif defined(CBM_SCREEN_CODES) || defined(__SUPERVISION__)
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


#elif (defined(__COCO__) || defined(__DRAGON__)) && !defined(__BIT_MAPPED_GRAPHICS) && !defined(__BIT_MAPPED_4_GRAPHICS)

	char screenCode(char ch)
	{
		if(ch==32)
		{
			return 96;
		}
		else if((ch>='0')&&(ch<='9'))
		{
			return 64+ch;
		}
		else
		{
			return ch;
		}        
	}

#elif ((defined(__APPLE2__) || defined(__APPLE2ENH__)) && defined(__APPLE2_HGR_GRAPHICS))
	char screenCode(char ch)
	{
		if(ch==32) 
		{
			return 0;
		}
		else if((ch>='0')&&(ch<='9'))
		{
			return ch-48+1;
		}
		else if(ch<58)
		{
			return ch-48+1-64;
		}	
		else
		{
			return ch+18-65;
		}
	}    
#elif (defined(__COCO3__) && defined(__BIT_MAPPED_16_GRAPHICS))

	#define _SPACE_OFFSET 13

	char screenCode(char ch)
	{
        if(ch==32)
        {
            return 0+_SPACE_OFFSET;
        }
        else
        {
			return ch;
        }
	}
#elif (defined(__COCO3__) || defined(__COCO__) || defined(__DRAGON__)) && (defined(__BIT_MAPPED_GRAPHICS) || defined(__BIT_MAPPED_4_GRAPHICS))

    #if defined(__BIT_MAPPED_4_GRAPHICS)
        #define _SPACE_OFFSET 13
    #else
        #define _SPACE_OFFSET 0
    #endif
	char screenCode(char ch)
	{
        if(ch==32)
        {
            return 0+_SPACE_OFFSET;
        }
        else
        {
			return ch;
        }
	}

#elif defined(__ZX81__) && defined(__Z88DK_SPRITES_GRAPHICS)
	char screenCode(char ch)
	{
        if(ch==32)
        {
            return 32;
        }
        else
        {
			return ch-32-64;
        }
	}

#elif (defined(__VIC20__) && (defined(__VIC20_EXP_3K) || defined(__VIC20_EXP_8K)))
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
#elif (defined(__VIC20__) && (defined(__VIC20_UNEXPANDED)))
	char screenCode(char ch)
	{
		if(ch<64)
		{
			return 128+ch;
		}
		else
		{
			return 192+ch;
		}	
	}
#elif defined(__C16__) && defined(__MEMORY_MAPPED_GRAPHICS)
	char screenCode(char ch)
	{
		if(ch&0x80)
        {
			return ch&0x7F;
		}
        else
		{
            if(ch<58) // TODO: Use bitwise operator <64?
            {
                return ch|64;
            }
            else
            {
                return ch;
            }
		}
	}  

#endif

#  if defined(__Z88DK_SPRITES_GRAPHICS)
	#include <graphics.h>
	#include <games.h>

	#include "display_macros.h"
	
	#define _DISPLAY(x,y,ch) \
		__draw_ch(x,y,ch);
	
#elif defined(__BUFFERED_GRAPHICS)
	#define _DISPLAY(x,y,ch) \
		video_buffer[(y)][(x)] = (ch)
		
#elif defined(__NO_GRAPHICS)
	#define _DISPLAY(x,y,ch)

#elif (defined(__APPLE2__)||defined(__APPLE2ENH__)) && defined(__APPLE2_HGR_GRAPHICS)
    #include "cross_lib.h"

    extern uint8_t _apple2_text_color;

    void _DISPLAY(uint8_t x, uint8_t y, uint8_t ch)
        {
            hgr_draw(x, y, ch, _apple2_text_color);
        }
	
#elif (defined(__C16__) && defined(__MEMORY_MAPPED_GRAPHICS)) && defined(__NO_SCREEN_CODES)
    void _DISPLAY(uint8_t x, uint8_t y, uint8_t ch)
        {
			DISPLAY_POKE((loc(x,y)), ch);
			DISPLAY_POKE((loc(x,y)-1024), PEEK(0x053B));
		}
#elif (defined(__C16__) && defined(__MEMORY_MAPPED_GRAPHICS)) && defined(__DOUBLE_BUFFER)
    void _DISPLAY(uint8_t x, uint8_t y, uint8_t ch)
        {
            DISPLAY_POKE(((uint16_t) BASE_ADDR)+(1)*x+(uint8_t)((1)*y)*((uint16_t) ((XSize) + X_OFFSET)), screenCode(ch));
            DISPLAY_POKE(((uint16_t) COLOR_ADDR)+(1)*x+(uint8_t)((1)*y)*((uint16_t) ((XSize) + X_OFFSET)), PEEK(0x053B));
		}
#elif (defined(__C16__) && defined(__MEMORY_MAPPED_GRAPHICS)) 
    void _DISPLAY(uint8_t x, uint8_t y, uint8_t ch)
        {
			DISPLAY_POKE((loc(x,y)), screenCode(ch));
			DISPLAY_POKE((loc(x,y)-1024), PEEK(0x053B));
		}
#elif (defined(__VIC20__) && defined(__VIC20_EXP_8K)) && !defined(_XL_NO_UDG)
    void _DISPLAY(uint8_t x, uint8_t y, uint8_t ch)
        {
        DISPLAY_POKE((loc(x,y)), screenCode(ch));
        DISPLAY_POKE((0x8400+loc(x,y)), PEEK(0x0286));
        }
#elif (defined(__VIC20__) && defined(__VIC20_UNEXPANDED)) && !defined(_XL_NO_UDG)
    #if !defined(_XL_NO_TEXT_COLOR)
        void _DISPLAY(uint8_t x, uint8_t y, uint8_t ch)
            {
                DISPLAY_POKE((loc(x,y)), screenCode(ch));
                DISPLAY_POKE((0x7800+loc(x,y)), PEEK(0x0286));
            }
    #else
        void _DISPLAY(uint8_t x, uint8_t y, uint8_t ch)
            {
                DISPLAY_POKE((loc(x,y)), screenCode(ch));
                DISPLAY_POKE((0x7800+loc(x,y)), 0x1);
            }
    #endif
#elif defined(__C64__)
    void _DISPLAY(uint8_t x, uint8_t y, uint8_t ch)
        {
        DISPLAY_POKE((loc(x,y)), ch);
        DISPLAY_POKE((0x1800+loc(x,y)), PEEK(0x0286));
        } 
#elif defined(__BIT_MAPPED_4_GRAPHICS)
    #include "bit_mapped_4_graphics.h"
    #include "cross_lib.h"
    
    #if defined(__COCO__) || defined(__DRAGON__)
        #define _CHAR_OFFSET 13
    #else
        #define _CHAR_OFFSET 0
    #endif
    
    extern uint8_t _bitmap4_text_color;

    void _DISPLAY(uint8_t x, uint8_t y, uint8_t ch)
        {
            _color_draw(x,y,ch-_CHAR_OFFSET,_bitmap4_text_color);
        }

#elif defined(__BIT_MAPPED_16_GRAPHICS)
    #include "bit_mapped_16_graphics.h"
    #include "cross_lib.h"
    
	#define _CHAR_OFFSET 13
 
    extern uint8_t _bitmap16_text_color;

    void _DISPLAY(uint8_t x, uint8_t y, uint8_t ch)
        {
        _color_draw(x,y,ch-_CHAR_OFFSET,_bitmap16_text_color);
        }
#elif (defined(__COCO__) || defined(__DRAGON__)) && defined(__BIT_MAPPED_GRAPHICS)
    #include "bit_mapped_graphics.h"
    #include "cross_lib.h"
    void _DISPLAY(uint8_t x, uint8_t y, uint8_t ch)
        {
            if(ch==0)
            {
                _XL_DELETE(x,y);
            }
            else
            {
                _XL_DRAW(x,y,(ch-13),0);
            }
        }
#elif defined(__AQUARIUS__) || defined(__MEMORY_MAPPED_GRAPHICS)
    void _DISPLAY(uint8_t x, uint8_t y, uint8_t ch)
        {
            if(ch==0)
            {
                _XL_DELETE(x,y);
            }
            else
            {
                _XL_DRAW(x,y,ch,_XL_WHITE);
            }
        }
#elif defined(__QUAD_MEMORY_MAPPED_GRAPHICS) || defined(__DUAL_MEMORY_MAPPED_GRAPHICS)
    void _DISPLAY(uint8_t x, uint8_t y, uint8_t ch)
        {
            _XL_DRAW(x,y,ch-32,_XL_WHITE);
        }
#else
    void _DISPLAY(uint8_t x, uint8_t y, uint8_t ch)
        {		
            DISPLAY_POKE((loc(x,y)), (ch));
        }
#endif


void _XL_PRINT(uint8_t x, uint8_t y, const char * str)
{
	uint8_t i = 0;

	while(str[i]!='\0')
	{
		#if defined(CBM_SCREEN_CODES) || defined(__COCO3__) || defined(__COCO__) || defined(__DRAGON__) || defined(__SUPERVISION__) \
            || ((defined(__APPLE2__) || defined(__APPLE2ENH__)) && defined(__APPLE2_HGR_GRAPHICS)) \
            || defined(__C64__) \
            || (defined(__VIC20__) && !defined(__VIC20_UNEXPANDED)) \
            || defined(__QUAD_MEMORY_MAPPED_GRAPHICS)
			_DISPLAY(x+i,y, screenCode(str[i]));
		#else
			_DISPLAY(x+i,y, str[i]);
		#endif
		++i;
	}
}


void _XL_PRINTD(uint8_t x, uint8_t y, uint8_t length, uint16_t val)
{
	uint8_t i;
	uint8_t digit;
	
	for(i=0;i<length;++i)
	{
		digit = (uint8_t) ((val)%10);
		val-= digit;
		val/=10;
        #if ((defined(__APPLE2__) || defined(__APPLE2ENH__)) && defined(__APPLE2_HGR_GRAPHICS))
        _DISPLAY(x+length-1-i,y, (uint8_t) (digit+(uint8_t) 1u));
        #elif ((defined(__COCO__) || defined(__DRAGON__))&&!defined(__BIT_MAPPED_GRAPHICS))
        _DISPLAY(x+length-1-i,y, (uint8_t) (digit+(uint8_t) 48u + 64u));
        // #elif defined(__QUAD_MEMORY_MAPPED_GRAPHICS)
        // DRAW_QUAD_CHAR(x+length-1-i,y,(digit+(uint8_t) 48u),_XL_WHITE);
        #elif defined(__QUAD_MEMORY_MAPPED_GRAPHICS)
		_DISPLAY(x+length-1-i,y, (uint8_t) (digit+(uint8_t) 48u-32u));
        #else
		_DISPLAY(x+length-1-i,y, (uint8_t) (digit+(uint8_t) 48u));
        #endif
	}
}

#if defined(CBM_SCREEN_CODES) || defined(__COCO3__) || defined(__COCO__) || defined(__DRAGON__) || defined(__SUPERVISION__) \
    || ((defined(__APPLE2__) || defined(__APPLE2ENH__)) && defined(__APPLE2_HGR_GRAPHICS)) \
    || defined(__C64__)
    void _XL_CHAR(uint8_t x, uint8_t y, char ch)
    {    
        _DISPLAY(x,y, screenCode(ch));
    }
#else
    void _XL_CHAR(uint8_t x, uint8_t y, char ch)
    {    
        _DISPLAY(x,y, ch);
    }
#endif
