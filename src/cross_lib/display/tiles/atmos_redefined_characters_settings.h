

#if !defined(ORIC_COLOR)
    #define _INVERTED 128u
#else
    #define _INVERTED 0
#endif


// OK
#define _TILE_19_OFFSET ('Z'+35)

// OK
#define _TILE_20_OFFSET ('Z'+36)

// OK
#define _TILE_21_OFFSET ('Z'+37)

// OK
#define _TILE_22_OFFSET ('<'+1)

// OK
#define _TILE_23_OFFSET ('+')

#define _TILE_24_OFFSET ('?')

#define _TILE_19 _TILE_19_OFFSET
#define _TILE_20 _TILE_20_OFFSET
#define _TILE_21 _TILE_21_OFFSET
#define _TILE_22 _TILE_22_OFFSET
#define _TILE_23 _TILE_23_OFFSET
#define _TILE_24 _TILE_24_OFFSET

#define _TILE_2_OFFSET ','
#define _TILE_3_OFFSET '*'
#define _TILE_1_OFFSET '.'
#define _TILE_0_OFFSET '/'

#define _TILE_2 _TILE_2_OFFSET
#define _TILE_3 _TILE_3_OFFSET
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

#if !defined(_TILE_5_COLOR)
    #define _TILE_5 (_TILE_5_OFFSET + _INVERTED)
#else
    #define _TILE_5 (_TILE_5_OFFSET + _TILE_5_COLOR)
#endif

#if !defined(_TILE_8_COLOR)
    #define _TILE_8 ( _TILE_8_OFFSET + _INVERTED)
#else
    #define _TILE_8 (_TILE_8_OFFSET + _TILE_8_COLOR)
#endif

#if !defined(_TILE_9_COLOR)
    #define _TILE_9 (_TILE_9_OFFSET + _INVERTED)
#else
    #define _TILE_9 (_TILE_9_OFFSET + _TILE_9_COLOR)
#endif

#define _DOLLAR_TILE '$'
	
#define _TILE_11 _TILE_11_OFFSET


#define _TILE_6_OFFSET ('Z'+2)
#define _TILE_6 _TILE_6_OFFSET

#define _TILE_12_OFFSET '|'
#define _TILE_13_OFFSET '-'

#define _TILE_12 _TILE_12_OFFSET
#define _TILE_13 _TILE_13_OFFSET


#define _TILE_0_B_OFFSET _TILE_0
#define _TILE_10_OFFSET 0x3B  

#define _TILE_17_OFFSET 0x28

#define _TILE_18_OFFSET 0x29

#if !defined(_TILE_17_COLOR)
    #define _TILE_17 (_TILE_17_OFFSET +_INVERTED)
#else
    #define _TILE_17 (_TILE_17_OFFSET + _TILE_17_COLOR)
#endif

#define _TILE_0_B (_TILE_0 + _INVERTED)

#if !defined(_TILE_10_COLOR)
    #define _TILE_10 (_TILE_10_OFFSET + _INVERTED) 
#else
    #define _TILE_10 (_TILE_10_OFFSET + _TILE_10_COLOR)
#endif

#define _TILE_7_B (_TILE_7 + _INVERTED)

#if !defined(_TILE_18_COLOR)
    #define _TILE_18 (_TILE_18_OFFSET + _INVERTED)
#else
    #define _TILE_18 (_TILE_18_OFFSET + _TILE_18_COLOR)
#endif

#define _TILE_4_B (_TILE_4 + _INVERTED)

#define _TILE_15_OFFSET '<'
#define _TILE_14_OFFSET '>'
#define _TILE_16_OFFSET 0x23

#define _TILE_5_B 'X'

#define _TILE_14 _TILE_14_OFFSET
#define _TILE_15 _TILE_15_OFFSET
#define _TILE_16 _TILE_16_OFFSET


