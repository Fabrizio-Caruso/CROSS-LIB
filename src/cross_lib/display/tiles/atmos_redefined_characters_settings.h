
#define _INVERTED 128u


#define _TILE_2_OFFSET '*'
#define _TILE_3_OFFSET ','
#define _TILE_1_OFFSET '.'
#define _TILE_0_OFFSET '/'

#define _TILE_2 _TILE_3_OFFSET
#define _TILE_3 _TILE_2_OFFSET
#define _TILE_1 _TILE_1_OFFSET
#define _TILE_0 _TILE_0_OFFSET

	
#define _TILE_4_OFFSET 0x60
#define _TILE_7_OFFSET 0x7b
#define _TILE_5_OFFSET 0x5b
#define _TILE_8_OFFSET 0x5d
#define _TILE_9_OFFSET 0x5e
#define _TILE_11_OFFSET 0x5f	
	
#define _TILE_4 _TILE_4_OFFSET
#define _TILE_7 _TILE_7_OFFSET
#define _TILE_5 (_TILE_5_OFFSET + _INVERTED)
#define _TILE_8 ( _TILE_8_OFFSET + _INVERTED)
#define _TILE_9 (_TILE_9_OFFSET + _INVERTED)
#define _TILE_10 '$'
	
#define _TILE_11 _TILE_11_OFFSET

#if !defined(NO_DEAD_GHOST)
	#define _TILE_6_OFFSET 0x5a
	#define _TILE_6 _TILE_6_OFFSET
#endif

#if !defined(TINY_GAME)
	#define _TILE_12_OFFSET '|'
	#define _TILE_13_OFFSET '-'
	
	#define _TILE_12 _TILE_12_OFFSET
	#define _TILE_13 _TILE_13_OFFSET	
#endif

#if defined(FULL_GAME)
	// #define _TILE_17_OFFSET 'F'
	#define _TILE_19_OFFSET _TILE_0
	#define _TILE_20_OFFSET 0x3B  
	// #define _TILE_21_OFFSET 'C'
	// #define _TILE_18_OFFSET 'H'		
	// #define _TILE_22_OFFSET 'Z' 

	#define _TILE_17 ('F' +_INVERTED)
	#define _TILE_19 (_TILE_0 + _INVERTED)
	#define _TILE_20 (_TILE_20_OFFSET + _INVERTED) 
	#define _TILE_21 (_TILE_7 + _INVERTED)
	#define _TILE_18 ('H' + _INVERTED)		
	#define _TILE_22 (_TILE_4 + _INVERTED)

	// #define _CHASE_OFFSET _BULLET
	#define _TILE_15_OFFSET '<'
	#define _TILE_14_OFFSET '>'	
	#define _TILE_16_OFFSET 0x23	
	
	#define _TILE_23 'X'
	
	// #define _CHASE _BULLET
	#define _TILE_14 _TILE_14_OFFSET
	#define _TILE_15 _TILE_15_OFFSET
	#define _TILE_16 _TILE_16_OFFSET
#endif

