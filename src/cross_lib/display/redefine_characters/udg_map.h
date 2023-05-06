
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


#if defined(__C16__) && defined(__CONIO_GRAPHICS)
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
	{_TILE_0, _TILE_0_UDG},
	{_TILE_1, _TILE_1_UDG},
	{_TILE_2, _TILE_2_UDG},	
	{_TILE_3, _TILE_3_UDG},
	{_TILE_4, _TILE_4_UDG},
	{_TILE_5, _TILE_5_UDG},
	{_TILE_6, _TILE_6_UDG},
    {_TILE_7, _TILE_7_UDG},
    {_TILE_8, _TILE_8_UDG},
    {_TILE_9, _TILE_9_UDG},
    {_TILE_10, _TILE_10_UDG},
    {_TILE_11, _TILE_11_UDG},
    {_TILE_12, _TILE_12_UDG},
    {_TILE_13, _TILE_13_UDG},
    {_TILE_14, _TILE_14_UDG},
    {_TILE_15, _TILE_15_UDG},
    {_TILE_16, _TILE_16_UDG},
    {_TILE_17, _TILE_17_UDG},
    {_TILE_18, _TILE_18_UDG},
    {_TILE_19, _TILE_19_UDG},
    {_TILE_20, _TILE_20_UDG},
    {_TILE_21, _TILE_21_UDG},
    {_TILE_22, _TILE_22_UDG},
    {_TILE_23, _TILE_23_UDG},
    {_TILE_24, _TILE_24_UDG},
    {_TILE_25, _TILE_25_UDG},
    {_TILE_26, _TILE_26_UDG},
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

