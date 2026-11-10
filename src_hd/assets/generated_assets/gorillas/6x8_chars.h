#ifndef _6X8_CHARS
#define _6X8_CHARS

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

	#define _SPACE_TILE   {0,0,0,0,0,0,0,0}

	#define _TILE_0_UDG     {0,17,42,42,42,42,17,0}
	#define _TILE_1_UDG     {0,17,43,41,41,41,19,0}
	#define _TILE_2_UDG     {0,17,42,40,41,42,19,0}
	#define _TILE_3_UDG     {0,19,40,40,43,40,19,0}

	#define _TILE_4_UDG     {0,18,42,42,43,40,16,0}
	#define _TILE_7_UDG     {0,19,40,40,41,41,17,0}
	#define _TILE_9_UDG     {0,19,42,42,43,40,16,0}
	#define _TILE_8_UDG     {0,19,42,42,43,42,19,0}
	#define _TILE_11_UDG    {0,17,51,17,17,17,59,0}
	#define _TILE_5_UDG     {0,19,42,42,43,40,19,0}

	#define _TILE_6_UDG     {0,19,42,42,43,42,19,0}
	
	#define _TILE_16_UDG    {0,19,50,18,19,18,59,0}
	#define _TILE_14_UDG    {0,18,50,18,19,16,56,0}
	#define _TILE_15_UDG    {0,19,50,18,19,16,59,0}
	#define _TILE_10_UDG    {0,17,50,18,18,18,57,0}
	#define _TILE_12_UDG    {0,17,50,16,17,18,59,0}
	#define _TILE_13_UDG    {0,19,48,16,19,16,59,0}
	#define _TILE_17_UDG    {0,19,48,16,17,17,57,0}
	#define _TILE_18_UDG    {0,19,50,18,19,18,59,0}

	#define _TILE_19_UDG    {0,19,50,18,19,16,56,0}
	#define _TILE_20_UDG    {0,25,42,10,18,34,57,0}
	#define _TILE_21_UDG    {0,25,43,9,17,33,59,0}
	#define _TILE_22_UDG    {0,25,42,8,17,34,59,0}
	#define _TILE_23_UDG    {0,27,40,8,19,32,59,0}
	#define _TILE_24_UDG    {0,26,42,10,19,32,56,0}
    #define _TILE_25_UDG    {0,27,42,10,19,32,59,0}
    #define _TILE_26_UDG    {0,27,42,10,19,34,59,0}
    
    #define _TILE_27_UDG    {0,27,40,8,17,33,57,0}
    #define _TILE_28_UDG    {0,27,42,10,19,34,59,0}
    #define _TILE_29_UDG    {0,27,42,10,19,32,56,0}
    #define _TILE_30_UDG    {0,57,10,10,58,10,57,0}
    #define _TILE_31_UDG    {0,57,11,9,57,9,59,0}
    #define _TILE_32_UDG    {0,57,10,8,57,10,59,0}
    #define _TILE_33_UDG    {0,59,8,8,59,8,59,0}
    #define _TILE_34_UDG    {0,58,10,10,59,8,56,0}
    #define _TILE_35_UDG    {0,59,10,10,59,8,59,0}
    #define _TILE_36_UDG    {0,59,10,10,59,10,59,0}
    #define _TILE_37_UDG    {0,59,8,8,57,9,57,0}
    #define _TILE_38_UDG    {0,59,10,10,59,10,59,0}
    #define _TILE_39_UDG    {0,59,10,10,59,8,56,0}
    #define _TILE_40_UDG    {0,41,42,42,58,10,9,0}
    #define _TILE_41_UDG    {0,41,43,41,57,9,11,0}
    #define _TILE_42_UDG    {0,41,42,40,57,10,11,0}
    #define _TILE_43_UDG    {0,43,40,40,59,8,11,0}
    #define _TILE_44_UDG    {0,42,42,42,59,8,8,0}
    #define _TILE_45_UDG    {0,43,42,42,59,8,11,0}
    #define _TILE_46_UDG    {0,43,42,42,59,10,11,0}
    #define _TILE_47_UDG    {0,43,40,40,57,9,9,0}
    #define _TILE_48_UDG    {0,43,42,42,59,10,11,0}
    #define _TILE_49_UDG    {0,43,42,42,59,8,8,0}
    #define _TILE_50_UDG    {0,57,34,34,58,10,57,0}
    #define _TILE_51_UDG    {0,57,35,33,57,9,59,0}
    #define _TILE_52_UDG    {0,57,34,32,57,10,59,0}
    #define _TILE_53_UDG    {0,59,32,32,59,8,59,0}
    #define _TILE_54_UDG    {0,58,34,34,59,8,56,0}
    #define _TILE_55_UDG    {0,59,34,34,59,8,59,0}
    #define _TILE_56_UDG    {0,59,34,34,59,10,59,0}
    #define _TILE_57_UDG    {0,59,32,32,57,9,57,0}
    #define _TILE_58_UDG    {0,59,34,34,59,10,59,0}
    #define _TILE_59_UDG    {0,59,34,34,59,8,56,0}
    #define _TILE_60_UDG    {0,57,34,34,58,42,57,0}
    #define _TILE_61_UDG    {0,57,35,33,57,41,59,0}
    #define _TILE_62_UDG    {0,57,34,32,57,42,59,0}
    #define _TILE_63_UDG    {0,59,32,32,59,40,59,0}
    #define _TILE_64_UDG    {0,58,34,34,59,40,56,0}
    #define _TILE_65_UDG    {0,59,34,34,59,40,59,0}
    #define _TILE_66_UDG    {0,59,34,34,59,42,59,0}
    #define _TILE_67_UDG    {0,59,32,32,57,41,57,0}
    #define _TILE_68_UDG    {0,59,34,34,59,42,59,0}
    #define _TILE_69_UDG    {0,59,34,34,59,40,56,0}
    #define _TILE_70_UDG    {0,57,10,10,18,18,17,0}
    #define _TILE_71_UDG    {0,57,11,9,17,17,19,0}
    #define _TILE_72_UDG    {0,57,10,8,17,18,19,0}
    #define _TILE_73_UDG    {0,59,8,8,19,16,19,0}
    #define _TILE_74_UDG    {0,58,10,10,19,16,16,0}
    #define _TILE_75_UDG    {0,59,10,10,19,16,19,0}
    #define _TILE_76_UDG    {0,59,10,10,19,18,19,0}
    #define _TILE_77_UDG    {0,59,8,8,17,17,17,0}
    #define _TILE_78_UDG    {0,59,10,10,19,18,19,0}
    #define _TILE_79_UDG    {0,59,10,10,19,16,16,0}
    #define _TILE_80_UDG    {0,57,42,42,58,42,57,0}
    #define _TILE_81_UDG    {0,57,43,41,57,41,59,0}
    #define _TILE_82_UDG    {0,57,42,40,57,42,59,0}
    #define _TILE_83_UDG    {0,59,40,40,59,40,59,0}
    #define _TILE_84_UDG    {0,58,42,42,59,40,56,0}
    #define _TILE_85_UDG    {0,59,42,42,59,40,59,0}
    #define _TILE_86_UDG    {0,59,42,42,59,42,59,0}
    #define _TILE_87_UDG    {0,59,40,40,57,41,57,0}
    #define _TILE_88_UDG    {0,59,42,42,59,42,59,0}
    #define _TILE_89_UDG    {0,59,42,42,59,40,56,0}
    #define _TILE_90_UDG    {0,57,42,42,58,10,9,0}
#endif // _6x8_CHARS

