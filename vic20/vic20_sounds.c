
#include<vic20.h>

	void EXPLOSION_SOUND(void) 
	{ 
		unsigned int i; 
		unsigned int j;
		VIC.noise = 0x80; 
		VIC.volume_color |= 0x08; 
		for(i=0;i<200;++i) 
		{ 
		} 
		for(j=0;j<8;++j) 
		{ \
			for(i=0;i<300;++i) 
			{ 
			} 
			VIC.volume_color &= 8-j; 
		} 
		VIC.noise = 0x00; 
		VIC.volume_color &= 0x00; 
	};
	
	
	void PING_SOUND(void)
	{ 
		unsigned int i; 
		VIC.voice1 = 0xF0; 
		VIC.volume_color |= 0x08; 
		for(i=0;i<800;++i) 
		{ 
		} 
		VIC.voice1 = 0x00; 
		VIC.volume_color &= 0x00; 
	};	
	
	
	void SHOOT_SOUND(void) 
	{ 
		unsigned int i; 
		unsigned int j; 
		VIC.noise = 0xF0; 
		VIC.volume_color |= 0x0A; 
		for(i=0;i<600;++i) 
		{ 
		} 
		for(j=0;j<10;++j) 
		{ 
			for(i=0;i<100;++i) 
			{ 
			} 
			VIC.volume_color &= 10-j; 
		} 
		VIC.noise = 0x00; 
		VIC.volume_color &= 0x00; 
	};	
	
	
	void TICK_SOUND() 
	{ 
		unsigned int i; 
		VIC.voice1 = 0x60; 
		VIC.volume_color |= 0x08; 
		for(i=0;i<800;++i) 
		{ 
		} 
		VIC.voice1 = 0x00; 
		VIC.volume_color &= 0x00; 
	};	
	
	void TOCK_SOUND() 
	{ 
		unsigned int i;
		VIC.voice1 = 0x30; 
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
