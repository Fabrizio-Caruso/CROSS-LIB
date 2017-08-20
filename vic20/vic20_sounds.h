#ifndef _VIC_SOUNDS
#define _VIC_SOUNDS


// struct __vic {
    // unsigned char       leftborder;
    // unsigned char       upperborder;
    // unsigned char       charsperline;   /* Characters per line */
    // unsigned char       linecount;      /* Number of lines */
    // unsigned char       rasterline;     /* Current raster line */
    // unsigned char       addr;           /* Address of chargen and video ram */
    // unsigned char       strobe_x;       /* Light pen, X position */
    // unsigned char       strobe_y;       /* Light pen, Y position */
    // unsigned char       analog_x;       /* Analog input X */
    // unsigned char       analog_y;       /* Analog input Y */
    // unsigned char       voice1;         /* Sound generator #1 */
    // unsigned char       voice2;         /* Sound generator #2 */
    // unsigned char       voice3;         /* Sound generator #3 */
    // unsigned char       noise;          /* Noise generator */
    // unsigned char       volume_color;   /* Bits 0..3: volume, 4..7: color */
    // unsigned char       bg_border_color;/* Background and border color */
// };


#include<vic20.h>

	#define EXPLOSION_SOUND() \
	{ \
		unsigned int i; \
		unsigned int j; \
		VIC.noise = 0x80; \
		VIC.volume_color |= 0x08; \
		for(i=0;i<200;++i) \
		{ \
		} \
		for(j=0;j<8;++j) \
		{ \
			for(i=0;i<300;++i) \
			{ \
			} \
			VIC.volume_color &= 8-j; \
		} \
		VIC.noise = 0x00; \
		VIC.volume_color &= 0x00; \
	};
	
	
	#define PING_SOUND() \
	{ \
		unsigned int i; \
		VIC.voice1 = 0xF0; \
		VIC.volume_color |= 0x08; \
		for(i=0;i<800;++i) \
		{ \
		} \
		VIC.voice1 = 0x00; \
		VIC.volume_color &= 0x00; \
	};	
	
	
	#define SHOOT_SOUND() \
	{ \
		unsigned int i; \
		unsigned int j; \
		VIC.noise = 0xF0; \
		VIC.volume_color |= 0x0A; \
		for(i=0;i<600;++i) \
		{ \
		} \
		for(j=0;j<10;++j) \
		{ \
			for(i=0;i<100;++i) \
			{ \
			} \
			VIC.volume_color &= 10-j; \
		} \
		VIC.noise = 0x00; \
		VIC.volume_color &= 0x00; \
	};	
	
	
	#define TICK_SOUND() \
	{ \
		unsigned int i; \
		VIC.voice1 = 0x60; \
		VIC.volume_color |= 0x08; \
		for(i=0;i<800;++i) \
		{ \
		} \
		VIC.voice1 = 0x00; \
		VIC.volume_color &= 0x00; \
	};	
	
	#define TOCK_SOUND() \
	{ \
		unsigned int i; \
		VIC.voice1 = 0x30; \
		VIC.volume_color |= 0x08; \
		for(i=0;i<800;++i) \
		{ \
		} \
		VIC.voice1 = 0x00; \
		VIC.volume_color &= 0x00; \
	};		
	

	#define ZAP_SOUND() \
	{ \
		unsigned int i; \
		unsigned int j; \
		VIC.volume_color |= 0x0B; \
		for(j=0;j<15;++j) \
		{ \
			VIC.voice1 = j*15;\
			for(i=0;i<300;++i) \
			{ \
			} \
		} \
		VIC.voice1 = 0x00; \
		VIC.volume_color &= 0x00; \
	};		
	
#endif // _VIC_SOUNDS