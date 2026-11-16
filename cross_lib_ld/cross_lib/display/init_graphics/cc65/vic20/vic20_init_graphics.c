
#if !defined(PEEK)
    #include <peekpoke.h>
#endif
#include <vic20.h>
#include <stdint.h>

#include "display_macros.h"

void _XL_INIT_GRAPHICS(void)
{
	#if !defined(_XL_NO_UDG)
		#if defined(__VIC20_EXP_8K) || defined(__VIC20_EXP_16K)
            #if !defined(VIC20_USE_0X0F) && !defined(__MEMORY_MAPPED_GRAPHICS)
                uint8_t tmp;

                tmp = ~0x0F & PEEK(&(VIC.addr));
                POKE(&(VIC.addr), tmp | 0x0E); // 1110 -> character map at $1800
            #else
			uint8_t tmp;

			tmp = ~0x0F & PEEK(&(VIC.addr));
			POKE(&(VIC.addr), tmp | 0x0F);
            #endif
		#elif defined(__VIC20_EXP_3K) || defined(__VIC20_UNEXPANDED) 
			POKE(0x9005,0xFF);
		#endif
	#endif
	
	POKE(646,1);
    
    #if defined(__BACKGROUND_COLOR) && __BACKGROUND_COLOR==1
        POKE(36879L,25);
    #else
        POKE(36879L,8);
    #endif
    
	#if defined(DEBUG)
		#if defined(__VIC20_EXP_8K) || defined(__VIC20_EXP_16K)
			#define BASE_ADDR 0x1000
		#else
			#define BASE_ADDR 0x1E00
		#endif
		{
			uint8_t i;
			
			i=0;
			do
			{
				POKE(BASE_ADDR+i,i);
				++i;
			} while(i!=0);
			while(1);
		}
	#endif

    _setScreenColors();
}

