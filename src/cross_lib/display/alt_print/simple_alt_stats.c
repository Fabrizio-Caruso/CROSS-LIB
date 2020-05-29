#include "standard_libs.h"

#include "display_macros.h"

#include "memory_mapped_graphics.h"

#if defined(ALT_DISPLAY_STATS) || defined(ALT_HISCORE)

	#if !defined(CC65)
		#define POKE(addr,val)     (*(uint8_t*) (addr) = (val))
		#define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
		#define PEEK(addr)         (*(uint8_t*) (addr))
		#define PEEKW(addr) (*(unsigned*) (addr))
	#else
		#include <peekpoke.h>
	#endif


	#if defined(__VIC20__) && defined(VIC20_UNEXPANDED) && defined(REDEFINED_CHARS)
		#define UDG_OFFSET 128
	#else
		#define UDG_OFFSET 0
	#endif
	
	void _displayShort(uint16_t value)
	{ 
		uint8_t i; 

		for(i=1;i<6;++i) 
		{ 
			value -= POKE(BASE_ADDR+6-i,(uint8_t) ((value)%10)); 
			value/=10; 
			POKE(BASE_ADDR+6-i,UDG_OFFSET+48+PEEK(BASE_ADDR+6-i)); 
		} 
		POKE(BASE_ADDR+6,48+UDG_OFFSET); 
	}

	void PRINT(uint8_t x, uint8_t y, char * str)
	{
		uint8_t i = 0;
		while(str[i]!='\0')
		{
			#if (!defined(ALT_DISPLAY_STATS) && (defined(CBM_SCREEN_CODES) || defined(__COCO__) || defined(__DRAGON__)))
			POKE(loc(x+i,y), screenCode(str[i]));
			#elif defined(__VIC20__) && defined(VIC20_UNEXPANDED) && defined(REDEFINED_CHARS)
			POKE(loc(x+i,y), 64+str[i]); 
			POKE((0x7800+loc(x+i,y)),0x01);
			#else
			POKE(loc(x+i,y), str[i]); 
			#endif
			++i;
		}
	}	
#endif
