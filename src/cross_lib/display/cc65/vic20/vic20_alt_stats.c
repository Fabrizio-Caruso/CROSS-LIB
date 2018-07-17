#include "../../display_macros.h"

#if defined(ALT_DISPLAY_STATS) || defined(ALT_HISCORE)
	#include <peekpoke.h>
	void PRINTF(unsigned char x, unsigned char y, char * str, unsigned short value)
	{ 
		unsigned char i; 
		unsigned short tmp; 
		
		tmp = value; 
		
		for(i=1;i<6;++i) 
		{ 
			tmp -= POKE(7686-i,(unsigned char) ((tmp)%10)); 
			tmp/=10; 
			POKE(7686-i,PEEK(7686-i)+48); 
		} 
		POKE(7686,48); 	
	}
#endif
