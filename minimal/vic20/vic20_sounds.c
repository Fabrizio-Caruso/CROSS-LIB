
#include<vic20.h>

	void _explosion_sound(unsigned char freq, unsigned char vol)
	{ 
		unsigned int i; 
		unsigned int j;
		VIC.noise = freq; 
		VIC.volume_color |= vol; 
		for(i=0;i<200;++i) 
		{ 
		} 
		for(j=0;j<8;++j) 
		{ \
			for(i=0;i<250;++i) 
			{ 
			} 
			VIC.volume_color &= 8-j; 
		} 
		VIC.noise = 0x00; 
		VIC.volume_color &= 0x00; 
	};	

	void _ping_sound(unsigned char freq)
	{ 
		unsigned int i; 
		VIC.voice1 = freq; 
		VIC.volume_color |= 0x08; 
		for(i=0;i<800;++i) 
		{ 
		} 
		VIC.voice1 = 0x00; 
		VIC.volume_color &= 0x00; 
	};
	

	void ZAP_SOUND() 
	{ 
		unsigned int i; 
		unsigned int j; 
		VIC.volume_color |= 0x0B; 
		for(j=0;j<15;++j) 
		{ \
			VIC.voice1 = j*15;
			for(i=0;i<300;++i) 
			{ 
			} 
		} \
		VIC.voice1 = 0x00; 
		VIC.volume_color &= 0x00; 
	};		

		
	// TODO: Save more memory in order to activate TICK
	
	// void TICK_SOUND() 
	// { 
		// unsigned int i; 
		// VIC.voice1 = 0x60; 
		// VIC.volume_color |= 0x08; 
		// for(i=0;i<800;++i) 
		// { 
		// } 
		// VIC.voice1 = 0x00; 
		// VIC.volume_color &= 0x00; 
	// };	
	
	