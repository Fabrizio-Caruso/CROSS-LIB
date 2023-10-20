
#ifndef _BIT_MAPPED_GRAPHICS
#define _BIT_MAPPED_GRAPHICS

#if defined(__MO5__)
	#define SV_VIDEO  ((uint8_t*)0x0000)
#elif defined(__TO7__)
	#define SV_VIDEO  ((uint8_t*)0x4000)
#elif defined(__COCO__) || defined(__DRAGON__)
	#define SV_VIDEO  ((uint8_t*)0x0E00)
#elif defined(__COCO3__)
	#define SV_VIDEO ((uint8_t*)0x8000)
#endif

extern uint8_t udgs[];

#if defined(__MO5__) || defined(__TO7__) 
    #define _BITMAP_XSize 40
#else
    #define _BITMAP_XSize (XSize)
#endif

#define BIT_MAP_DRAW() \
    for(__i=0;__i<7;++__i) \
    { \
        SV_VIDEO[__base+__delta]  = udgs[__offset+__i]; \
        __delta+=_BITMAP_XSize; \
    } \
    SV_VIDEO[__base+(_BITMAP_XSize)*7] = udgs[__offset+7]; \


#define BIT_MAP_DELETE() \
    for(__i=0;__i<7;++__i) \
    { \
        SV_VIDEO[(uint16_t) __base+__delta] = 0; \
        __delta+=_BITMAP_XSize; \
    } \
    SV_VIDEO[__base+(_BITMAP_XSize)*(uint16_t)7] = 0; \

#if defined(__MO5__) || defined(__TO7__) 
	#include "conio_patch.h"
    
	#define _XL_DRAW(x,y,tile,color) \
	{ \
		uint8_t __i; \
		uint16_t __base = (x)+(_BITMAP_XSize)*8*(y); \
		uint8_t __delta = 0; \
		uint8_t __offset = (8U*(uint8_t)(tile)) ; \
		\
		SWITCH_COLOR_BANK_OFF(); \
		BIT_MAP_DRAW(); \
		\
		__delta = 0; \
		SWITCH_COLOR_BANK_ON(); \
		for(__i=0;__i<7;++__i) \
		{ \
			SV_VIDEO[__base+__delta]  = (color); \
			__delta+=_BITMAP_XSize; \
		} \
		SV_VIDEO[__base+(_BITMAP_XSize)*(uint16_t)7] = (color); \
		SWITCH_COLOR_BANK_OFF(); \
	}
	#define _XL_DELETE(x,y) \
	{ \
		uint8_t __i; \
		uint16_t __base = (x)+(_BITMAP_XSize)*8*(y); \
		uint8_t __delta = 0; \
		\
		SWITCH_COLOR_BANK_OFF(); \
		BIT_MAP_DELETE(); \
	}

#elif defined(__COCO__) || defined(__DRAGON__)

	#define _XL_DRAW(x,y,tile,color) \
	{ \
		uint16_t __i; \
		uint16_t __base = (x)+(_BITMAP_XSize)*8*(y); \
		uint8_t __delta = 0; \
		uint16_t __offset = (8U*((uint16_t) tile)) ; \
		\
		BIT_MAP_DRAW(); \
	}
	


	#define _XL_DELETE(x,y) \
	{ \
		uint8_t __i; \
		uint16_t __base = (x)+(_BITMAP_XSize)*8*(y); \
		uint8_t __delta = 0; \
		\
		BIT_MAP_DELETE(); \
	}

#endif
	
#endif // _BIT_MAPPED_GRAPHICS

