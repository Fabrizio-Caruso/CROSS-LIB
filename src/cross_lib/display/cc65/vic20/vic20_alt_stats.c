#include "../../display_macros.h"

#if defined(ALT_DISPLAY_STATS) || defined(ALT_HISCORE)
	#include <peekpoke.h>
	void _displayShort(unsigned short value)
	{ 
		unsigned char i; 

		for(i=1;i<6;++i) 
		{ 
			value -= POKE(7686-i,(unsigned char) ((value)%10)); 
			value/=10; 
			POKE(7686-i,PEEK(7686-i)+48); 
		} 
		POKE(7686,48); 	
	}

	void PRINT(unsigned char x, unsigned char y, char * str)
	{
		unsigned char i = 0;
		while(str[i]!='\0')
		{
			#if defined(CBM_SCREEN_CODES) || (defined(__CMOC__) && !defined(__WINCMOC__))
			POKE(loc(x+i,y), screenCode(str[i])); 		
			#else
				
			POKE(loc(x+i,y), str[i]); 
			#endif
			++i;
		}
	}	
#endif
