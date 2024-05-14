
#ifndef _COLOR_DEFINITIONS_H
#define _COLOR_DEFINITIONS_H


#if defined(__NO_GRAPHICS) || defined(_XL_NO_COLOR)
    #define _XL_BLACK 0
    #define _XL_WHITE 0
    #define _XL_RED 0
    #define _XL_CYAN 0
    #define _XL_GREEN 0
    #define _XL_YELLOW 0
    #define _XL_BLUE 0
    
#elif defined(__AQUARIUS__) && !defined(__CONIO_GRAPHICS)
    #define _XL_CYAN (16*6)
    #define _XL_BLUE (16*4)
    #define _XL_YELLOW (16*CYAN)
    #define _XL_RED 16
    #define _XL_WHITE (16*7)
    #define _XL_GREEN (16*GREEN)
    #define _XL_BLACK (16*BLACK)

#elif defined(__MO5__)
    #if defined(__BACKGROUND_COLOR) && __BACKGROUND_COLOR==1
        #define _XL_BLACK (0+7)
        #define _XL_RED (16+7)
        #define _XL_GREEN (32+7)
        #define _XL_YELLOW (48+7)
        #define _XL_BLUE (64+7)
        #define _XL_MAGENTA (80+7)
        #define _XL_CYAN (96+7)
        #define _XL_WHITE (112+7)
    #else
        #define _XL_BLACK 0
        #define _XL_RED 16
        #define _XL_GREEN 32
        #define _XL_YELLOW 48
        #define _XL_BLUE 64
        #define _XL_MAGENTA 80
        #define _XL_CYAN 96
        #define _XL_WHITE 112
    #endif
#elif defined(__TO7__)
    #define TO7_PASTEL_OFF 192U
    #define _XL_BLACK (0+TO7_PASTEL_OFF)
    #define _XL_RED (8+TO7_PASTEL_OFF)
    #define _XL_GREEN (16+TO7_PASTEL_OFF)
    #define _XL_YELLOW (24+TO7_PASTEL_OFF)
    #define _XL_BLUE (48+TO7_PASTEL_OFF)
    #define _XL_MAGENTA (40+TO7_PASTEL_OFF)
    #define _XL_CYAN (48+TO7_PASTEL_OFF)
    #define _XL_WHITE (56+TO7_PASTEL_OFF)
#elif defined(__CPCRSLIB_GRAPHICS)
    #define CPC_WHITE 1
    #define CPC_YELLOW 2 
    #define CPC_RED 3
    #define CPC_BLACK 4

    #define CPC_TEXT_WHITE 3
    #define CPC_TEXT_RED 4
    #define CPC_TEXT_BLACK 7
    #define CPC_TEXT_YELLOW 0

    #define _XL_BLACK CPC_BLACK
    #define _XL_BLUE CPC_WHITE
    #define _XL_CYAN CPC_WHITE
    #define _XL_RED CPC_RED
    #define _XL_WHITE CPC_WHITE
    #define _XL_YELLOW CPC_YELLOW
    #define _XL_PURPLE CPC_YELLOW
    #define _XL_GREEN CPC_YELLOW
#elif defined(__ANTIC_MODE6_GRAPHICS) || defined(__ATARI5200__)

    #define _ATARI_MODE1_RED 0
    #define _ATARI_MODE1_WHITE 64
    #define _ATARI_MODE1_BLACK 64
    #define _ATARI_MODE1_CYAN 128
    #define _ATARI_MODE1_GREEN 128
    #define _ATARI_MODE1_YELLOW 192

    #define _XL_RED _ATARI_MODE1_RED
    #define _XL_WHITE _ATARI_MODE1_WHITE
    #define _XL_BLACK _ATARI_MODE1_BLACK
    #define _XL_BLUE _ATARI_MODE1_CYAN
    #define _XL_YELLOW _ATARI_MODE1_YELLOW
    
    #if defined(__ALTERNATE_COLORS)
        #define _XL_GREEN _ATARI_MODE1_GREEN
        #define _XL_CYAN _ATARI_MODE1_WHITE
    #elif defined(__ALTERNATE_COLORS2)
        #define _XL_GREEN _ATARI_MODE1_GREEN
        #define _XL_CYAN _ATARI_MODE1_CYAN
    #else
        #define _XL_GREEN _ATARI_MODE1_YELLOW
        #define _XL_CYAN _ATARI_MODE1_CYAN
    #endif

#elif defined(__NES_CONIO_GRAPHICS)

    #define _NES_RED 0
    #define _NES_YELLOW 1
    #define _NES_WHITE 2

    #define _XL_BLACK COLOR_BLACK
    #define _XL_RED _NES_RED
    #define _XL_WHITE _NES_WHITE
    #define _XL_BLUE _NES_WHITE
    #define _XL_YELLOW _NES_YELLOW
    #define _XL_GREEN _NES_YELLOW
	#if !defined(__ALTERNATE_COLORS)
		#define _XL_CYAN _NES_WHITE
	#else
		#define _XL_CYAN _NES_RED
	#endif

#elif defined(__COMX_COLOR_GRAPHICS)

    /*
    
    _textcolor:             ; set text to 'color' in R12.0
                            ; 0: black
                            ; 1: green
                            ; 2: blue
                            ; 3: cyan
                            ; 4: red
                            ; 5: yellow
                            ; 6: magenta
                            ; 7: white
--
    _textcolor:             ; set text to 'color' in R12.0
                            ; 0: 'low' character color (character 0-127)
                            ; 1: 'high' chatacter color (character 128-255)
    */
    #define _COMX_TEXT_CYAN 128
    #define _COMX_TEXT_GREEN 128
    #define _COMX_TEXT_YELLOW 0
    #define _COMX_TEXT_RED 0
    #define _COMX_TEXT_WHITE 0

    #if defined(__MICRO__) 
        #define _COMX_BLUE 0
        #define _COMX_GREEN 0
        #define _COMX_CYAN 0
        #define _COMX_PURPLE 128
        #define _COMX_YELLOW 128
        #define _COMX_WHITE  128

        #define _XL_RED _COMX_PURPLE
        #define _XL_WHITE _COMX_WHITE
        #define _XL_BLUE _COMX_BLUE
        #define _XL_YELLOW _COMX_YELLOW
        #define _XL_GREEN _COMX_GREEN
        #define _XL_CYAN _COMX_CYAN
        
        #define _XL_BLACK 0
    
    #elif defined(__CIDELSA__)
        #define _COMX_BLUE 0
        #define _COMX_GREEN 0
        #define _COMX_CYAN 0
        #define _COMX_PURPLE 0
        #define _COMX_YELLOW 0
        #define _COMX_WHITE  0

        #define _XL_RED _COMX_PURPLE
        #define _XL_WHITE _COMX_WHITE
        #define _XL_BLUE _COMX_BLUE
        #define _XL_YELLOW _COMX_YELLOW
        #define _XL_GREEN _COMX_GREEN
        #define _XL_CYAN _COMX_CYAN
        
        #define _XL_BLACK 0
        
    #else
        #define _COMX_BLUE 0
        #define _COMX_GREEN 1
        #define _COMX_CYAN 2
        #define _COMX_PURPLE (0+128)
        #define _COMX_YELLOW (1+128)
        #define _COMX_WHITE  (2+128) 

        #define _XL_RED _COMX_PURPLE
        #define _XL_WHITE _COMX_WHITE
        #define _XL_BLUE _COMX_BLUE
        #define _XL_YELLOW _COMX_YELLOW
        #define _XL_GREEN _COMX_GREEN
        #define _XL_CYAN _COMX_CYAN
        
        #define _XL_BLACK 3
    #endif
#elif defined(__ORIC_COLOR_GRAPHICS)

    #define _ORIC_NORMAL 0
    #define _ORIC_INVERSE 128
    
    #define _XL_BLACK _ORIC_INVERSE
    #define _XL_RED _ORIC_INVERSE
    #define _XL_WHITE _ORIC_NORMAL
    #define _XL_BLUE _ORIC_INVERSE
    #define _XL_YELLOW _ORIC_NORMAL
    #define _XL_GREEN _ORIC_NORMAL
    #define _XL_CYAN _ORIC_NORMAL
    
#elif defined(__ATARI7800__)
    #define _XL_WHITE 3
    #define _XL_YELLOW 3
    #define _XL_CYAN 1
    #define _XL_BLUE _XL_CYAN
    #define _XL_RED 0
    #define _XL_GREEN _XL_CYAN
	#define _XL_BLACK 2
    
#elif defined(__VDP_MODE1_GRAPHICS)

    #if defined(__TI99__)
        #define _COLOR_OFFSET -64
    #else
        #define _COLOR_OFFSET 0
    #endif
        
    #if defined(__TI99__)
        #define _CREAT_YELLOW (192+_COLOR_OFFSET)
        #define _CREAT_GREEN  (160+_COLOR_OFFSET)
    #else
        #define _CREAT_YELLOW (32 +_COLOR_OFFSET)
        #define _CREAT_GREEN  (0 +_COLOR_OFFSET)
    #endif
    #define _CREAT_WHITE  (64 +_COLOR_OFFSET)
    #define _CREAT_CYAN   (96 +_COLOR_OFFSET)
    #define _CREAT_RED    (128+_COLOR_OFFSET)
    #define _CREAT_BLUE _CREAT_CYAN

    #define _XL_RED _CREAT_RED
    #define _XL_WHITE _CREAT_WHITE
    #define _XL_BLUE _CREAT_BLUE
    #define _XL_YELLOW _CREAT_YELLOW
    #define _XL_GREEN _CREAT_GREEN
    #define _XL_CYAN _CREAT_CYAN
    #define _XL_BLACK _CREAT_WHITE

#elif (defined(__APPLE2__) || defined(__APPLE2ENH__)) && defined(__APPLE2_HGR_GRAPHICS)
	// TODO: This has to be fixed probably by avoiding __BACKGROUND_COLOR = a macro color
	// TODO: White option

    #if defined(__BACKGROUND_COLOR) && __BACKGROUND_COLOR==1
        #define _XL_PURPLE 0
        #define _XL_GREEN 1
        #define _XL_MAGENTA 0
        #define _XL_CYAN ((_XL_PURPLE)+4)
        #define _XL_BLUE _XL_CYAN
        #if !defined(__ALTERNATE_COLORS)
            #define _XL_RED ((_XL_GREEN)+4)
            #if !defined(_APPLE_YELLOW_IS_PURPLE)
                #define _XL_YELLOW _XL_WHITE
            #else
                #define _XL_YELLOW _XL_MAGENTA
            #endif
        #else
            #define _XL_RED ((_XL_GREEN)+4)
            #define _XL_YELLOW _XL_MAGENTA   
        #endif
        #define _XL_BLACK 2
        
        #define _XL_WHITE _XL_BLUE
    #else
        #define _XL_WHITE 2

        #define _XL_PURPLE 1
        #define _XL_GREEN 0
        #define _XL_MAGENTA 1
        #define _XL_CYAN ((_XL_PURPLE)+4)
        #define _XL_BLUE _XL_CYAN
        #if !defined(__ALTERNATE_COLORS)
            #define _XL_RED ((_XL_GREEN)+4)
            #if !defined(_APPLE_YELLOW_IS_PURPLE)
                #define _XL_YELLOW _XL_WHITE
            #else
                #define _XL_YELLOW _XL_MAGENTA
            #endif
        #else
            #define _XL_RED ((_XL_GREEN)+4)
            #define _XL_YELLOW _XL_MAGENTA   
        #endif
        
        // TODO: Remove this hack and handle real black on white background
        #define _XL_BLACK _XL_BLUE
	#endif
#elif defined(__ATARI_LYNX__) && defined(__LYNX_TGI_GRAPHICS)
	// 3 -> pink
	// 0 -> white
	// 4 -> grey 
	// 5 -> darker grey ?
	// 6 -> even darker grey
	// 7 -> grey/red
	// 8 -> light red
	// B -> dark green
	// C -> dark blue
	
	// #define _XL_BLACK 0x6
	

	#define _XL_BLACK 0x1
	#define _XL_RED 0x2
	#define _XL_WHITE 0x0F
	#define _XL_BLUE 0xD
	#define _XL_YELLOW 0x9
	#define _XL_GREEN 0xA
	#define _XL_CYAN 0xE
	
#elif defined(__SUPERVISION__)

    #define _AUX_BLACK 0xFF
    #define _AUX_DARK_GREY 0xAA
    #define _AUX_LIGHT_GREY 0x55
    #define _AUX_WHITE 0x00

    #define _XL_BLACK _AUX_BLACK
    #define _XL_WHITE _AUX_BLACK
    #define _XL_YELLOW _AUX_DARK_GREY
    #define _XL_RED _AUX_LIGHT_GREY
    #define _XL_GREEN _AUX_BLACK
    #define _XL_CYAN _AUX_DARK_GREY
    #define _XL_BLUE _AUX_DARK_GREY
    

#elif defined(__GAMATE__)
    #define _AUX_BLACK COLOR_BLACK
    #define _AUX_DARK_GREY 2
    #define _AUX_LIGHT_GREY 1
    #define _AUX_WHITE 0

    #define _XL_BLACK _AUX_BLACK
    #define _XL_WHITE _AUX_BLACK
    #define _XL_YELLOW _AUX_DARK_GREY
    #define _XL_RED _AUX_LIGHT_GREY
    #define _XL_GREEN _AUX_BLACK
    #define _XL_CYAN _AUX_DARK_GREY
    #define _XL_BLUE _AUX_DARK_GREY

#elif (defined(__COCO3__)) && defined(__BIT_MAPPED_16_GRAPHICS)

	// TODO: BOGUS

    #define _XL_BLACK 0
	
    #define _XL_WHITE 0x11
    #define _XL_GREEN 0x33
	#define _XL_CYAN 0x44
	
    #define _XL_YELLOW 0x55	
    #define _XL_RED 0xAA	
    #define _XL_BLUE 0xFF

#elif (defined(__COCO__) || defined(__DRAGON__) || defined(__MC10__)) && defined(__BIT_MAPPED_4_GRAPHICS)

    #define _COCO_RED 0xFF
    #define _COCO_YELLOW 0x55
    #define _COCO_CYAN 0xAA

    #define _XL_BLACK _COCO_CYAN
    #define _XL_WHITE _COCO_RED
    #define _XL_YELLOW _COCO_YELLOW
    #define _XL_RED _COCO_RED
    #define _XL_GREEN _COCO_CYAN
    #define _XL_CYAN _COCO_CYAN
    #define _XL_BLUE _COCO_CYAN

#elif defined(__TI99__)
    #define _XL_BLACK COLOR_BLACK
    #define _XL_RED COLOR_LTRED
    #define _XL_WHITE COLOR_WHITE
    #define _XL_BLUE COLOR_DKBLUE
    #define _XL_YELLOW COLOR_YELLOW
    #define _XL_GREEN COLOR_GREEN
    #define _XL_CYAN COLOR_LTBLUE
#elif defined(__CC65__) || defined(__TMC600__)

    #define _XL_BLACK COLOR_BLACK
    #define _XL_RED COLOR_RED
    #define _XL_WHITE COLOR_WHITE
    #define _XL_BLUE COLOR_BLUE
    #define _XL_YELLOW COLOR_YELLOW
    #define _XL_GREEN COLOR_GREEN

    #if defined(COLOR_CYAN)
        #define _XL_CYAN COLOR_CYAN
    #else
        #define _XL_CYAN COLOR_BLUE
    #endif

#elif defined(__NCURSES__)
    #define _XL_YELLOW 1
    #define _XL_CYAN 2
    #define _XL_RED 3
    #define _XL_GREEN 4
    #define _XL_BLUE 5
    #define _XL_WHITE 6
    #define _XL_MAGENTA 7
    #define _XL_BLACK 8
    
#elif defined(__MC1000__)
    #if defined(__BACKGROUND_COLOR) && __BACKGROUND_COLOR==1
        #define _XL_WHITE MAGENTA
        #define _XL_CYAN CYAN
        #define _XL_YELLOW MAGENTA
        #define _XL_GREEN CYAN
        #define _XL_RED RED
        #define _XL_MAGENTA MAGENTA
        #define _XL_BLUE BLUE
        #define _XL_BLACK BLUE
    #else
        #define _XL_BLACK CYAN
        #define _XL_WHITE RED
        #define _XL_YELLOW YELLOW
        #define _XL_RED RED
        #define _XL_GREEN BLUE
        #define _XL_CYAN BLUE
        #define _XL_BLUE BLUE
    #endif
#else // Z88DK
    #define _XL_BLACK BLACK
    #if defined(__PC6001__) || defined(__SPC1000__)
        #define _XL_WHITE 7
    #else
        #define _XL_WHITE WHITE
    #endif

    #if defined(__CPC__) || defined(__LASER500__)
        #define _XL_CYAN LIGHTCYAN
    #else
        #define _XL_CYAN CYAN
    #endif

    #if defined(__SMS__) || defined(__GAME_GEAR__)
        #define _XL_YELLOW LIGHTGRAY
    #else
        #define _XL_YELLOW YELLOW
    #endif

    #if defined(__LASER500__)
        #define _XL_RED LIGHTRED
        #define _XL_GREEN LIGHTGREEN
        #define _XL_MAGENTA LIGHTMAGENTA
        #define _XL_BLUE LIGHTBLUE
    #else

        #if defined(__SMS__) || defined(__GAME_GEAR__)
            #define _XL_RED LIGHTRED
            #define _XL_GREEN LIGHTGREEN
        #else
            #define _XL_RED RED
            #define _XL_GREEN GREEN
        #endif
        #define _XL_MAGENTA MAGENTA
        #define _XL_BLUE BLUE
    #endif

   

#endif


#endif // _COLOR_DEFINITIONS_H
