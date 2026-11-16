
#ifndef _UDG_MAP
#define _UDG_MAP

#if !defined(USE_MSX_BIOS)
    #include "display_macros.h"
    #include "tiles.h"
#endif

#if (defined(__COMX__) || defined(__PECOM__) || defined(__MICRO__)) && !defined(NTSC)
    #define NUMBER_OF_LINES 9
#else
    #define NUMBER_OF_LINES 8
#endif

const struct redefine_struct
{
   const uint8_t ascii;
   const uint8_t bitmap[NUMBER_OF_LINES];
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
    #if _XL_NUMBER_OF_TILES>6
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
    #if _XL_NUMBER_OF_TILES>27

    {_TILE_27, _TILE_27_UDG},
    {_TILE_28, _TILE_28_UDG},
    {_TILE_29, _TILE_29_UDG},
    {_TILE_30, _TILE_30_UDG},
    {_TILE_31, _TILE_31_UDG},
    
    #if _XL_NUMBER_OF_TILES>32
    {_TILE_32, _TILE_32_UDG},
    {_TILE_33, _TILE_33_UDG},
    {_TILE_34, _TILE_34_UDG},
    {_TILE_35, _TILE_35_UDG},
    {_TILE_36, _TILE_36_UDG},
    {_TILE_37, _TILE_37_UDG},
    {_TILE_38, _TILE_38_UDG},
    {_TILE_39, _TILE_39_UDG},
    {_TILE_40, _TILE_40_UDG},
    {_TILE_41, _TILE_41_UDG},
    {_TILE_42, _TILE_42_UDG},
    {_TILE_43, _TILE_43_UDG},
    {_TILE_44, _TILE_44_UDG},
    {_TILE_45, _TILE_45_UDG},
    {_TILE_46, _TILE_46_UDG},
    {_TILE_47, _TILE_47_UDG},
    {_TILE_48, _TILE_48_UDG},
    {_TILE_49, _TILE_49_UDG},
    {_TILE_50, _TILE_50_UDG},
    {_TILE_51, _TILE_51_UDG},
    {_TILE_52, _TILE_52_UDG},
    {_TILE_53, _TILE_53_UDG},
    {_TILE_54, _TILE_54_UDG},
    {_TILE_55, _TILE_55_UDG},
    {_TILE_56, _TILE_56_UDG},
    {_TILE_57, _TILE_57_UDG},
    {_TILE_58, _TILE_58_UDG},
    {_TILE_59, _TILE_59_UDG},
    {_TILE_60, _TILE_60_UDG},
    {_TILE_61, _TILE_61_UDG},
    {_TILE_62, _TILE_62_UDG},
    {_TILE_63, _TILE_63_UDG},
    {_TILE_64, _TILE_64_UDG},
    {_TILE_65, _TILE_65_UDG},
    {_TILE_66, _TILE_66_UDG},
    {_TILE_67, _TILE_67_UDG},
    {_TILE_68, _TILE_68_UDG},
    {_TILE_69, _TILE_69_UDG},
    {_TILE_70, _TILE_70_UDG},
    {_TILE_71, _TILE_71_UDG},
    {_TILE_72, _TILE_72_UDG},
    {_TILE_73, _TILE_73_UDG},
    {_TILE_74, _TILE_74_UDG},
    {_TILE_75, _TILE_75_UDG},
    {_TILE_76, _TILE_76_UDG},
    {_TILE_77, _TILE_77_UDG},
    {_TILE_78, _TILE_78_UDG},
    {_TILE_79, _TILE_79_UDG},
    {_TILE_80, _TILE_80_UDG},
    {_TILE_81, _TILE_81_UDG},
    {_TILE_82, _TILE_82_UDG},
    {_TILE_83, _TILE_83_UDG},
    {_TILE_84, _TILE_84_UDG},
    {_TILE_85, _TILE_85_UDG},
    {_TILE_86, _TILE_86_UDG},
    {_TILE_87, _TILE_87_UDG},
    {_TILE_88, _TILE_88_UDG},
    {_TILE_89, _TILE_89_UDG},
    {_TILE_90, _TILE_90_UDG},
    #endif
    #endif
    #endif
    // #endif
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

