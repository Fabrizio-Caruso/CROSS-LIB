#include "atari_sounds.h"
	#include<peekpoke.h>
	
	
	void _explosion_sound(unsigned char freq)
	{
		unsigned short i; 
		POKEY_WRITE.audctl = 0x00; 
		POKE(53775u,3); 
		POKEY_WRITE.audf1 = freq; 
		POKEY_WRITE.audc1 = 0x07; 
		for(i=150;i>0;--i) 
		{ 
			POKEY_WRITE.audc1 = i/10u; 
		} 
	}

	
	void _ping_sound(unsigned char freq)
	{ 
		unsigned short i; 
		POKEY_WRITE.audctl = 0x00; 
		POKE(53775u,3); 
		POKEY_WRITE.audf1 = freq; 
		POKEY_WRITE.audc1 = 0xA7; 
		for(i=700;i>0;--i) 
		{ 
		} 
		POKEY_WRITE.audc1 = 0x00; 
	};			
	

	void ZAP_SOUND(void) 
	{ 
		unsigned short i; 
		unsigned short j; 
		POKEY_WRITE.audctl = 0x00; 
		POKE(53775u,3); 
		POKEY_WRITE.audf1 = 0x02;
		POKEY_WRITE.audc1 = 0xA7; 
		for(i=0;i<100;++i) 
		{ 
			for(j=0;j<50;++j) { } 
			POKEY_WRITE.audf1 = 100+i; 
		} 
		POKEY_WRITE.audc1 = 0x00; 
	};	