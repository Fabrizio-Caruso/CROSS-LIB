#ifndef _7X8_CHARS
#define _7X8_CHARS


    // A - Z
	#define _TILE_A { 0x08, 0x1C, 0x22, 0x22, 0x3E, 0x22, 0x22, 0x00 }
	#define _TILE_B { 0x3C, 0x12, 0x12, 0x1C, 0x12, 0x12, 0x3C, 0x00 }
	#define _TILE_C { 0x1C, 0x22, 0x20, 0x20, 0x20, 0x22, 0x1C, 0x00 }
	#define _TILE_D { 0x38, 0x24, 0x22, 0x22, 0x22, 0x24, 0x38, 0x00 }
	#define _TILE_E { 0x3E, 0x20, 0x20, 0x3C, 0x20, 0x20, 0x3E, 0x00 }
	#define _TILE_F { 0x3E, 0x20, 0x20, 0x3C, 0x20, 0x20, 0x20, 0x00 }
	#define _TILE_G { 0x1C, 0x20, 0x20, 0x2E, 0x22, 0x22, 0x1C, 0x00 }
	#define _TILE_H { 0x22, 0x22, 0x22, 0x3E, 0x22, 0x22, 0x22, 0x00 }
	#define _TILE_I { 0x1C, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00 }
	#define _TILE_J { 0x0E, 0x04, 0x04, 0x04, 0x04, 0x24, 0x18, 0x00 }
	#define _TILE_K { 0x22, 0x24, 0x28, 0x30, 0x28, 0x24, 0x22, 0x00 }
	#define _TILE_L { 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3E, 0x00 }
	#define _TILE_M { 0x22, 0x36, 0x2A, 0x2A, 0x22, 0x22, 0x22, 0x00 }
	#define _TILE_N { 0x22, 0x22, 0x32, 0x2A, 0x26, 0x22, 0x22, 0x00 }
	#define _TILE_O { 0x1C, 0x22, 0x22, 0x22, 0x22, 0x22, 0x1C, 0x00 }
	#define _TILE_P { 0x3C, 0x22, 0x22, 0x3C, 0x20, 0x20, 0x20, 0x00 }
	#define _TILE_Q { 0x1C, 0x22, 0x22, 0x22, 0x2A, 0x24, 0x1A, 0x00 }
	#define _TILE_R { 0x3C, 0x22, 0x22, 0x3C, 0x28, 0x24, 0x22, 0x00 }
	#define _TILE_S { 0x1C, 0x22, 0x20, 0x1C, 0x02, 0x22, 0x1C, 0x00 }
	#define _TILE_T { 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00 }
	#define _TILE_U { 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x1C, 0x00 }
	#define _TILE_V { 0x22, 0x22, 0x22, 0x14, 0x14, 0x08, 0x08, 0x00 }
	#define _TILE_W { 0x22, 0x22, 0x22, 0x2A, 0x2A, 0x36, 0x22, 0x00 }
	#define _TILE_X { 0x22, 0x22, 0x14, 0x08, 0x14, 0x22, 0x22, 0x00 }
	#define _TILE_Y { 0x22, 0x22, 0x14, 0x08, 0x08, 0x08, 0x08, 0x00 }
	#define _TILE_Z { 0x3E, 0x02, 0x04, 0x08, 0x10, 0x20, 0x3E, 0x00 }
    
    // 0 - 9
	#define _TILE_DIGIT_0 { 0x1C, 0x22, 0x26, 0x2A, 0x32, 0x22, 0x1C, 0x00 }
	#define _TILE_DIGIT_1 { 0x08, 0x18, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00 }
	#define _TILE_DIGIT_2 { 0x1C, 0x22, 0x22, 0x0C, 0x10, 0x20, 0x3E, 0x00 }
	#define _TILE_DIGIT_3 { 0x1C, 0x22, 0x02, 0x0C, 0x02, 0x22, 0x1C, 0x00 }
	#define _TILE_DIGIT_4 { 0x04, 0x0C, 0x14, 0x24, 0x3E, 0x04, 0x04, 0x00 }
	#define _TILE_DIGIT_5 { 0x3E, 0x30, 0x3C, 0x02, 0x02, 0x22, 0x1C, 0x00 }
	#define _TILE_DIGIT_6 { 0x0C, 0x10, 0x20, 0x3C, 0x22, 0x22, 0x1C, 0x00 }
	#define _TILE_DIGIT_7 { 0x3E, 0x02, 0x04, 0x08, 0x10, 0x10, 0x10, 0x00 }
	#define _TILE_DIGIT_8 { 0x1C, 0x22, 0x22, 0x1C, 0x22, 0x22, 0x1C, 0x00 }
	#define _TILE_DIGIT_9 { 0x1C, 0x22, 0x22, 0x1E, 0x02, 0x04, 0x08, 0x00 }



	#define _TILE_0_UDG     {28,20,8,62,85,8,20,20}
	#define _TILE_1_UDG     {28,28,8,62,85,8,20,20}
	#define _TILE_2_UDG     {28,24,9,62,84,12,20,18}
	#define _TILE_3_UDG     {28,12,72,62,21,24,20,36}

	#define _TILE_4_UDG     {65,62,65,85,65,93,65,62}
	#define _TILE_7_UDG     {62,65,85,65,73,42,34,28}
	#define _TILE_9_UDG     {0,32,31,40,56,32, 0, 0}
	#define _TILE_8_UDG     {0,30,51,55,51,26,18,12}
	#define _TILE_11_UDG    {0, 0, 4,28,14, 8, 0, 0}
	#define _TILE_5_UDG     {62,65,107,85,85,107,65,62}

	#define _TILE_6_UDG     {34,62,85,85,65,93,65,62}
	
	#define _TILE_16_UDG    {12,30,30,30,63,45,33,33}
	#define _TILE_14_UDG    {0, 0,56,31,31,56, 0, 0}
	#define _TILE_15_UDG    {0, 0, 7,62,62, 7, 0, 0}
	#define _TILE_10_UDG    {28,20,0,62,0,8,0,20}
	#define _TILE_12_UDG    {12,12,28,12,12,14,12,12}
	#define _TILE_13_UDG    {0, 0, 2,127,127,32,00,00}
	#define _TILE_17_UDG    {0,30,51,55,51,22,20,12}
	#define _TILE_18_UDG    {0,30,43,43,35,42,30,12}

	#define _TILE_19_UDG    {63,63,0,24,24,0,63,63}
	#define _TILE_20_UDG    {63,63,0,24,24,0,63,63}
	#define _TILE_21_UDG    {63,63,0,24,24,0,63,63}
	#define _TILE_22_UDG    {63,63,0,24,24,0,63,63}
	#define _TILE_23_UDG    {63,63,0,24,24,0,63,63}
	#define _TILE_24_UDG    {63,63,0,24,24,0,63,63}
	#define _TILE_25_UDG    {0x18,0x3e,0x60,0x3c,0x06,0x7c,0x18,0x00}
    #define _TILE_26_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_27_UDG    {0, 0, 0, 0, 0, 0, 0, 0}

    #define _TILE_28_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_29_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_30_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_31_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_32_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_33_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_34_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_35_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_36_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_37_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_38_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_39_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_40_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_41_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_42_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_43_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_44_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_45_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_46_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_47_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_48_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_49_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_50_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_51_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_52_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_53_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_54_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_55_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_56_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_57_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_58_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_59_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_60_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_61_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_62_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_63_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_64_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_65_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_66_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_67_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_68_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_69_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_70_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_71_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_72_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_73_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_74_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_75_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_76_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_77_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_78_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_79_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_80_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_81_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_82_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_83_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_84_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_85_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_86_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_87_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_88_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_89_UDG    {0, 0, 0, 0, 0, 0, 0, 0}
    #define _TILE_90_UDG    {0, 0, 0, 0, 0, 0, 0, 0}

#endif // _7x8_CHARS