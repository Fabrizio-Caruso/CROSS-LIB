#include <peekpoke.h>
#include <vic20.h>
#include <stdint.h>

void _XL_INIT_GRAPHICS(void)
{
	#if defined(REDEFINED_CHARS)
		#if defined(VIC20_EXP_8K) || defined(VIC20_EXP_16K)
			uint8_t tmp;
		
			tmp = ~0x0F & PEEK(&(VIC.addr));
			POKE(&(VIC.addr), tmp | 0x0F);
		#elif defined(VIC20_EXP_3K) || defined(VIC20_UNEXPANDED) 
			POKE(0x9005,0xFF);
		#endif
	#endif
	
	POKE(646,1);
    #if !defined(VIC20_WHITE_BORDER)
        POKE(36879L,8);
    #else
        POKE(36879L,9);
	#endif 
    
	#if defined(DEBUG)
		#if defined(VIC20_EXP_8K) || defined(VIC20_EXP_16K)
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
	
}

