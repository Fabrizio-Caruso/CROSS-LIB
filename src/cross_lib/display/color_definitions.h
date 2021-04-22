
#ifndef _COLOR_DEFINITIONS_H
#define _COLOR_DEFINITIONS_H

#if defined(NO_GRAPHICS) || defined(NO_PRINT) || defined(NO_COLOR)
    #define _XL_BLACK 0
    #define _XL_WHITE 0
    #define _XL_RED 0
    #define _XL_CYAN 0
    #define _XL_GREEN 0
    #define _XL_YELLOW 0
    #define _XL_BLUE 0
#elif defined(__MO5__)
    #define _XL_BLACK 0
    #define _XL_RED 16
    #define _XL_GREEN 32
    #define _XL_YELLOW 48
    #define _XL_BLUE 64
    #define _XL_MAGENTA 80
    #define _XL_CYAN 96
    #define _XL_WHITE 112
#elif defined(__TO7__)
    #define TO7_PASTEL_OFF 192
    #define _XL_BLACK (0+TO7_PASTEL_OFF)
    #define _XL_RED (8+TO7_PASTEL_OFF)
    #define _XL_GREEN (16+TO7_PASTEL_OFF)
    #define _XL_YELLOW (24+TO7_PASTEL_OFF)
    #define _XL_BLUE (64+TO7_PASTEL_OFF)
    #define _XL_MAGENTA (40+TO7_PASTEL_OFF)
    #define _XL_CYAN (48+TO7_PASTEL_OFF)
    #define _XL_WHITE (56+TO7_PASTEL_OFF)
#elif defined(CPCRSLIB)
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
#elif defined(ATARI_MODE_1_COLOR) || defined(__ATARI5200__)

    #define _ATARI_MODE1_RED 0
    #define _ATARI_MODE1_WHITE 64
    #define _ATARI_MODE1_CYAN 128
    #define _ATARI_MODE1_GREEN 128
    #define _ATARI_MODE1_YELLOW 192

    #define _XL_RED _ATARI_MODE1_RED
    #define _XL_WHITE _ATARI_MODE1_WHITE
    #define _XL_BLUE _ATARI_MODE1_CYAN
    #define _XL_YELLOW _ATARI_MODE1_YELLOW
    
    #if defined(ALTERNATE_COLORS)
        #define _XL_GREEN _ATARI_MODE1_GREEN
        #define _XL_CYAN _ATARI_MODE1_WHITE
    #else
        #define _XL_GREEN _ATARI_MODE1_YELLOW
        #define _XL_CYAN _ATARI_MODE1_CYAN
    #endif

#elif defined(NES_CONIO)

    #define _NES_RED 0
    #define _NES_YELLOW 1
    #define _NES_WHITE 2

    #define _XL_BLACK COLOR_BLACK
    #define _XL_RED _NES_RED
    #define _XL_WHITE _NES_WHITE
    #define _XL_BLUE _NES_WHITE
    #define _XL_YELLOW _NES_YELLOW
    #define _XL_GREEN _NES_YELLOW
    #define _XL_CYAN _NES_WHITE

#elif defined(COMX_COLOR)

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
    #endif
#elif defined(ORIC_COLOR)

    #define _ORIC_NORMAL 0
    #define _ORIC_INVERSE 128
    
    #define _XL_BLACK _ORIC_INVERSE
    #define _XL_RED _ORIC_INVERSE
    #define _XL_WHITE _ORIC_NORMAL
    #define _XL_BLUE _ORIC_INVERSE
    #define _XL_YELLOW _ORIC_NORMAL
    #define _XL_GREEN _ORIC_NORMAL
    #define _XL_CYAN _ORIC_NORMAL
    

#elif defined(CREATIVISION_COLOR)

    #define _CREAT_YELLOW 32
    #define _CREAT_GREEN   0
    #define _CREAT_WHITE  64
    #define _CREAT_CYAN   96
    #define _CREAT_RED   128
    #define _CREAT_BLUE _CREAT_CYAN

    #define _XL_RED _CREAT_RED
    #define _XL_WHITE _CREAT_WHITE
    #define _XL_BLUE _CREAT_BLUE
    #define _XL_YELLOW _CREAT_YELLOW
    #define _XL_GREEN _CREAT_GREEN
    #define _XL_CYAN _CREAT_CYAN

#elif defined(__CC65__) || defined(__NCURSES__) || defined(__TMC600__)
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

    #define _XL_YELLOW YELLOW

    #if defined(__LASER500__)
        #define _XL_RED LIGHTRED
        #define _XL_GREEN LIGHTGREEN
        #define _XL_MAGENTA LIGHTMAGENTA
        #define _XL_BLUE LIGHTBLUE
    #else
        #define _XL_RED RED
        #define _XL_GREEN GREEN
        #define _XL_MAGENTA MAGENTA
        #define _XL_BLUE BLUE
    #endif
#endif


#endif // _COLOR_DEFINITIONS_H
