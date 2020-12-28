
#ifndef _UDG_MAP
#define _UDG_MAP

#if !defined(USE_MSX_BIOS)
    #include "display_macros.h"
    #include "tiles.h"
#endif

const struct redefine_struct
{
   const uint8_t ascii;
   const uint8_t bitmap[8];
} ;

#ifndef _TILE_0_OFFSET
	#define _TILE_0_OFFSET_ _TILE_0
#else
	#define _TILE_0_OFFSET_ _TILE_0_OFFSET
#endif
#ifndef _TILE_1_OFFSET
	#define _TILE_1_OFFSET_ _TILE_1
#else
	#define _TILE_1_OFFSET_ _TILE_1_OFFSET
#endif
#ifndef _TILE_3_OFFSET
	#define _TILE_3_OFFSET_ _TILE_3
#else
	#define _TILE_3_OFFSET_ _TILE_3_OFFSET	
#endif
#ifndef _TILE_2_OFFSET
	#define _TILE_2_OFFSET_ _TILE_2
#else 
	#define _TILE_2_OFFSET_ _TILE_2_OFFSET	
#endif

#ifndef _TILE_4_OFFSET
	#define _TILE_4_OFFSET_ _TILE_4
#else
	#define _TILE_4_OFFSET_ _TILE_4_OFFSET
#endif
#ifndef _TILE_5_OFFSET
	#define _TILE_5_OFFSET_ _TILE_5
#else
	#define _TILE_5_OFFSET_ _TILE_5_OFFSET
#endif


#ifndef _TILE_6_OFFSET
	#define _TILE_6_OFFSET_ _TILE_6
#else
	#define _TILE_6_OFFSET_ _TILE_6_OFFSET
#endif


#ifndef _TILE_7_OFFSET
	#define _TILE_7_OFFSET_ _TILE_7
#else
	#define _TILE_7_OFFSET_ _TILE_7_OFFSET
#endif
#ifndef _TILE_9_OFFSET
	#define _TILE_9_OFFSET_ _TILE_9
#else
	#define _TILE_9_OFFSET_ _TILE_9_OFFSET
#endif
#ifndef _TILE_8_OFFSET
	#define _TILE_8_OFFSET_ _TILE_8
#else
	#define _TILE_8_OFFSET_ _TILE_8_OFFSET
#endif
#ifndef _TILE_11_OFFSET
	#define _TILE_11_OFFSET_ _TILE_11
#else
	#define _TILE_11_OFFSET_ _TILE_11_OFFSET
#endif
#ifndef _TILE_12_OFFSET 
	#define _TILE_12_OFFSET_ _TILE_12
#else
	#define _TILE_12_OFFSET_ _TILE_12_OFFSET
#endif
#ifndef _TILE_13_OFFSET
	#define _TILE_13_OFFSET_ _TILE_13
#else
	#define _TILE_13_OFFSET_ _TILE_13_OFFSET
#endif

#ifndef _TILE_16_OFFSET
	#define _TILE_16_OFFSET_ _TILE_16
#else
	#define _TILE_16_OFFSET_ _TILE_16_OFFSET
#endif

#ifndef _TILE_15_OFFSET
	#define _TILE_15_OFFSET_ _TILE_15
#else
	#define _TILE_15_OFFSET_ _TILE_15_OFFSET
#endif
#ifndef _TILE_14_OFFSET
	#define _TILE_14_OFFSET_ _TILE_14
#else
	#define _TILE_14_OFFSET_ _TILE_14_OFFSET
#endif

#ifndef _TILE_10_OFFSET
	#define _TILE_10_OFFSET_ _TILE_10
#else
	#define _TILE_10_OFFSET_ _TILE_10_OFFSET
#endif

#ifndef _TILE_0_B_OFFSET
	#define _TILE_0_B_OFFSET_ _TILE_0_B
#else
	#define _TILE_0_B_OFFSET_ _TILE_0_B_OFFSET
#endif

#ifndef _TILE_18_OFFSET
	#define _TILE_18_OFFSET_ _TILE_18
#else
	#define _TILE_18_OFFSET_ _TILE_18_OFFSET
#endif

#ifndef _TILE_17_OFFSET
	#define _TILE_17_OFFSET_ _TILE_17
#else
	#define _TILE_17_OFFSET_ _TILE_17_OFFSET
#endif

#ifndef _TILE_19_OFFSET
	#define _TILE_19_OFFSET_ _TILE_19
#else
	#define _TILE_19_OFFSET_ _TILE_19_OFFSET
#endif

#ifndef _TILE_20_OFFSET
	#define _TILE_20_OFFSET_ _TILE_20
#else
	#define _TILE_20_OFFSET_ _TILE_20_OFFSET
#endif

#ifndef _TILE_21_OFFSET
	#define _TILE_21_OFFSET_ _TILE_21
#else
	#define _TILE_21_OFFSET_ _TILE_21_OFFSET
#endif

#ifndef _TILE_22_OFFSET
	#define _TILE_22_OFFSET_ _TILE_22
#else
	#define _TILE_22_OFFSET_ _TILE_22_OFFSET
#endif

#ifndef _TILE_23_OFFSET
	#define _TILE_23_OFFSET_ _TILE_23
#else
	#define _TILE_23_OFFSET_ _TILE_23_OFFSET
#endif

#ifndef _TILE_24_OFFSET
	#define _TILE_24_OFFSET_ _TILE_24
#else
	#define _TILE_24_OFFSET_ _TILE_24_OFFSET
#endif

#if defined(__C16__) && defined(CONIO)
	char screenCode(char ch)
	{
		if(ch<64)
		{
			return ch;
		}
		else
		{
			return ch-64;
		}
		return ch;
	}
#else
    #define screenCode(ch) ch
#endif

const struct redefine_struct redefine_map[] =
{
	{_TILE_0_OFFSET_, _TILE_0_UDG},
	{_TILE_1_OFFSET_, _TILE_1_UDG},
	{_TILE_2_OFFSET_, _TILE_2_UDG},	
	{_TILE_3_OFFSET_, _TILE_3_UDG},
	{_TILE_4_OFFSET_, _TILE_4_UDG},
	{_TILE_5_OFFSET_, _TILE_5_UDG},
	
	#if !defined(NO_TILE_6)
		{_TILE_6_OFFSET_, _TILE_6_UDG},
	#endif
	
    
    
	#if !defined(NO_TILE_7)
		{_TILE_7_OFFSET_, _TILE_7_UDG},
	#endif
	#if !defined(NO_TILE_9)
		{_TILE_9_OFFSET_, _TILE_9_UDG},
	#endif
	#if !defined(NO_TILE_8)
		{_TILE_8_OFFSET_, _TILE_8_UDG},
	#endif
	#if !defined(NO_TILE_11)
		{_TILE_11_OFFSET_, _TILE_11_UDG},
	#endif
	#if !defined(NO_TILE_12)
		{_TILE_12_OFFSET_, _TILE_12_UDG},
	#endif
	#if !defined(NO_TILE_13)
		{_TILE_13_OFFSET_, _TILE_13_UDG},
	#endif	
	
    
    
	#if !defined(NO_TILE_16)
		{_TILE_16_OFFSET_, _TILE_16_UDG},
	#endif
	#if !defined(NO_TILE_10)
		{_TILE_10_OFFSET_, _TILE_10_UDG},
	#endif
    
    #if !defined(NO_TILE_15)
		{_TILE_15_OFFSET_, _TILE_15_UDG},
    #endif
    #if !defined(NO_TILE_14)
		{_TILE_14_OFFSET_, _TILE_14_UDG},
	#endif
	
    #if !defined(NO_TILE_19)
		{_TILE_19_OFFSET_, _TILE_19_UDG},
    #endif
    
    #if !defined(NO_TILE_20)
		{_TILE_20_OFFSET_, _TILE_20_UDG},
    #endif
    
    #if !defined(NO_TILE_21)
		{_TILE_21_OFFSET_, _TILE_21_UDG},
    #endif
    
    #if !defined(NO_TILE_22)
		{_TILE_22_OFFSET_, _TILE_22_UDG},
    #endif
    
    #if !defined(NO_TILE_23)
		{_TILE_23_OFFSET_, _TILE_23_UDG},
    #endif
    
    #if !defined(NO_TILE_24)
		{_TILE_24_OFFSET_, _TILE_24_UDG},
    #endif

    {_TILE_18_OFFSET_, _TILE_18_UDG},
    {_TILE_17_OFFSET_, _TILE_17_UDG},

};


#define REDEFINE_AT(addr) \
{ \
	uint8_t i; \
	\
	for (i = 0; i < (sizeof(redefine_map)/sizeof(*redefine_map)); ++i) \
	{ \
		memcpy((uint8_t *)(addr) + screenCode((redefine_map[i].ascii))*8, redefine_map[i].bitmap, 8); \
	} \
}


#endif // _UDG_MAP

