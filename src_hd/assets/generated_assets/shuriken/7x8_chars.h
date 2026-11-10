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



	#define _TILE_0_UDG     {79,93,69,71,102,34,56,15}
	#define _TILE_1_UDG     {8,8,104,72,24,16,112,64}
	#define _TILE_2_UDG     {0,0,0,0,15,56,34,99}
	#define _TILE_3_UDG     {0,0,0,0,64,112,16,24}

	#define _TILE_4_UDG     {8,8,9,11,12,4,7,1}
	#define _TILE_7_UDG     {0,0,0,0,120,14,66,99}
	#define _TILE_9_UDG     {24,16,112,64,0,0,0,0}
	#define _TILE_8_UDG     {99,33,56,15,0,0,0,0}
	#define _TILE_11_UDG    {64,112,16,24,104,72,8,8}
	#define _TILE_5_UDG     {125,89,81,113,51,18,14,120}

	#define _TILE_6_UDG     {0,0,0,0,1,7,4,12}
	
	#define _TILE_16_UDG    {16,24,31,22,52,124,12,4}
	#define _TILE_14_UDG    {1,7,4,12,9,11,8,8}
	#define _TILE_15_UDG    {120,14,34,51,113,81,93,121}
	#define _TILE_10_UDG    {15,56,36,102,71,69,77,95}
	#define _TILE_12_UDG    {12,4,7,1,0,0,0,0}
	#define _TILE_13_UDG    {99,34,14,120,0,0,0,0}
	#define _TILE_17_UDG    {23,30,24,8,0,0,0,0}
	#define _TILE_18_UDG    {0,0,0,0,8,12,60,116}

	#define _TILE_19_UDG    {0,0,3,7,1,1,1,0}
	#define _TILE_20_UDG    {64,96,96,32,56,112,64,64}
	#define _TILE_21_UDG    {127,127,119,113,99,123,127,127}
	#define _TILE_22_UDG    {127,71,95,95,125,125,113,127}
	#define _TILE_23_UDG    {8,28,58,125,58,28,8,0}
	#define _TILE_24_UDG    {0,0,8,14,28,4,0,0}
	#define _TILE_25_UDG    {0,28,54,99,99,54,28,0}
    #define _TILE_26_UDG    {0,12,26,61,26,12,0,0}
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