
#ifndef _MO5_BIT_MAPPED_GRAPHICS
#define _MO5_BIT_MAPPED_GRAPHICS

#if defined(__MO5__)
	#define SV_VIDEO  ((uint8_t*)0x0000)
#elif defined(__TO7__)
	#define SV_VIDEO  ((uint8_t*)0x4000)
#endif

extern uint8_t udgs[];




#if defined(__MO5__)
	#include "conio_patch.h"
	#define __DRAW(x,y,image) \
	{ \
		uint8_t __i; \
		uint16_t __base = (x)+(XSize)*8*(y); \
		uint8_t __delta = 0; \
		uint8_t __offset = (8*(uint8_t)(image)->_imageData) ; \
		\
		SWITCH_COLOR_BANK_OFF(); \
		for(__i=0;__i<7;++__i) \
		{ \
			SV_VIDEO[__base+__delta]  = udgs[__offset+__i]; \
			__delta+=XSize; \
		} \
		SV_VIDEO[__base+(XSize)*7] = udgs[__offset+7]; \
		\
		__delta = 0; \
		SWITCH_COLOR_BANK_ON(); \
		for(__i=0;__i<7;++__i) \
		{ \
			SV_VIDEO[__base+__delta]  = (image)->_color; \
			__delta+=XSize; \
		} \
		SV_VIDEO[__base+(XSize)*(uint16_t)7] = (image)->_color; \
		SWITCH_COLOR_BANK_OFF(); \
	}


	#define __DELETE(x,y) \
	{ \
		uint8_t __i; \
		uint16_t __base = (x)+(XSize)*8*(y); \
		uint8_t __delta = 0; \
		\
		SWITCH_COLOR_BANK_OFF(); \
		for(__i=0;__i<7;++__i) \
		{ \
			SV_VIDEO[(uint16_t) __base+__delta] = 0; \
			__delta+=XSize; \
		} \
		SV_VIDEO[__base+(XSize)*(uint16_t)7] = 0; \
	}
#elif defined(__TO7__)
	#include "conio_patch.h"
	#define __DRAW(x,y,image) \
	{ \
		uint8_t __i; \
		uint16_t __base = (x)+(XSize)*8*(y); \
		uint8_t __delta = 0; \
		\
		SWITCH_COLOR_BANK_OFF(); \
		for(__i=0;__i<7;++__i) \
		{ \
			SV_VIDEO[__base+__delta]  = 255; \
			__delta+=XSize; \
		} \
		SV_VIDEO[__base+(XSize)*7] = 255; \
		\
	}


	#define __DELETE(x,y) \
	{ \
		uint8_t __i; \
		uint16_t __base = (x)+(XSize)*8*(y); \
		uint8_t __delta = 0; \
		\
		SWITCH_COLOR_BANK_OFF(); \
		for(__i=0;__i<7;++__i) \
		{ \
			SV_VIDEO[(uint16_t) __base+__delta] = 0; \
			__delta+=XSize; \
		} \
		SV_VIDEO[__base+(XSize)*(uint16_t)7] = 0; \
	}
    
#endif
	
#endif // _MO5_BIT_MAPPED_GRAPHICS

