
#ifndef _CONIO_PATH_H
#define _CONIO_PATH_H


#if defined(ACK)
    #define cputc(c) putchar(c)
#endif

#if defined(NO_PRINT) || defined(NO_GRAPHICS)
    #ifndef gotoxy
        #define gotoxy(x,y)
    #endif
    #ifndef clrscr
        #define clrscr()
    #endif
    #ifndef textcolor
        #define textcolor(c)
    #endif
#else
    
    #if defined(FORCE_NO_CONIO_LIB) && defined(C16_UNEXPANDED)
        #include<peekpoke.h>
        #define textcolor(c) POKE(0x053B,c)
        #define bgcolor(c)
        #define bordercolor(c)
    #endif

    #if defined(Z88DK) || defined(ACK)
        #define cprintf printf
    #endif


    #if defined(__SMS__) && defined(NEW_LIB)
        #include <stdio.h>
        #include <stropts.h>
        #include <arch/sms.h>
        #include <rect.h>
        #include <input.h>
        #include <arch/sms/SMSlib.h>
        #include <arch/sms/PSGlib.h>

        #define gotoxy(x,y) \
            SMS_setNextTileatXY(x,y)
            
        #define cputc(c) \
            SMS_setTile(c)

    #endif

    #if defined(Z88DK) && !defined(__SMS__) && !defined(Z88DK_PUTC4X6) && !defined(__ZX80__) && !defined(__ZX81__) && !defined(__LAMBDA__)
        #undef cputc
        #define cputc(c) fputc_cons(c)
    #endif

    #  if defined(CONIO_ADM3A)
        #define gotoxy(x,y) printf("\033=%c%c",y+32,x+32);
        #define clrscr() printf("\032")

    #elif defined(CONIO_ADM3A_WITH_UNDEF)
        // TODO: Remove undef's below
        #undef gotoxy
        #undef clrscr 
        #define gotoxy(x,y) printf("\033=%c%c",y+32,x+32);
        #define clrscr() printf("\032")	
    #elif defined(CONIO_VT52)
        #undef gotoxy
        #define gotoxy(x,y) printf("\033Y%c%c",y+32,x+32)

        #undef clrscr
        #define clrscr() printf("\033E")

    #elif defined(CONIO_VT100)
        #include <stdio.h>

        #define gotoxy(x,y) printf("\033[%d;%dH", y+1, x+1)
        #define clrscr() printf("\033[2J\033[H")

    #elif defined(CONIO_PDP)
/*
                       <ESC>+     FUNCTION
                       ---------------------------------------------------
     
                       =          Alternate keypad mode
                       >          Exit alternate keypad mode
                       A          Move cursor up one line
                       B          Move cursor down one line
                       C          Move cursor left one space
                       D          Move cursor right one space
                       F          Enter graphics mode
                       G          Exit graphics mode
                       H          Home cursor
                       I          Reverse scroll
                       J          Erase screen from cursor to bottom
                       K          Erase screen from cursor to end of line
                       Z          Ident terminal type
                       [          Enter hold screen mode
                       ]          "Copy" screen to printer
                       \          Exit hold screen mode
                       ----------------------------------------------------
*/
        #include <stdio.h>
        
        // TODO: BOGUS gotoxy
        #define gotoxy(x,y)
        // #define gotoxy(x,y) printf("\033[%d;%dH", y+1, x+1)
        #define clrscr() printf("\033H\033J")

    #elif defined(Z88DK_PUTC4X6)
        #include <stdio.h>
        #include <graphics.h>
        #include <games.h>

        extern int _LIB_putc4x6(char c);
        extern uint8_t x_4x6;
        extern uint8_t y_4x6;

        #undef gotoxy
        #define gotoxy(x,y) do \
            { \
                x_4x6 = (x)*4; \
                y_4x6 = (y)*6; \
            \
            } while(0)
            
        #undef clrscr
        #define clrscr() clg();

        #define cprintf printf
                
        #define cputc(c) putc4x6(c)
        
    #elif defined(__ATARI_LYNX__)
        #include <tgi.h>
        #include <stdio.h>
        #include <lynx.h>
        
        #define gotoxy(a,b) tgi_gotoxy(a*8,b*8)
        #define clrscr() tgi_clear()
        #define cprintf(str) tgi_outtext(str)
        #define cputc(c) 
        #define textcolor tgi_setcolor

    #elif defined(__SUPERVISION__)
        #include <stdio.h>
        
        #define gotoxy(a,b) 
        #define clrscr() 
        #define cprintf 
        #define cputc(c) 	
        #define textcolor
        
    #elif defined(__ZX81__) || defined(__ZX80__) || defined(__LAMBDA__)
        #undef gotoxy
        
        #if defined(__ZX80__) && defined(ZX80_GEN_TV_FIELD)
            #define gotoxy(x,y) do {gen_tv_field(); zx_setcursorpos(y,x); gen_tv_field();} while(0)
            #define cputc(c) do { gen_tv_field(); fputc_cons(c); gen_tv_field(); } while(0)
        #else
            #define gotoxy(x,y) zx_setcursorpos(y,x)
        #endif
        

     
    #elif (defined(__MSX__) && defined(MEMORY_MAPPED)) || (defined(__SVI__) && defined(MSX_MODE0))
        #define gotoxy(a,b)     printf("\033Y%c%c",b+31+1,a+31)
        #define clrscr() printf("\033E") 


    #elif defined(__KC__)
        #include <stdio.h>
        
        #define gotoxy(x,y) do \
            { \
            printf("\1B%c%c",y+0x80, x+0x80); \
            \
            } while(0)
            
        #define clrscr() printf("\0C");

        #define cprintf printf

    #elif defined(__COCO__) || defined(__DRAGON__)

        #define cprintf printf
        #define gotoxy(x,y) locate(y,x)
        #define cputc(c) printf("%c",c)
    #elif defined(__MO5__) || defined(__TO7__)
        #include "coco.h" // uint8_t definition
        void SWITCH_COLOR_BANK_OFF(void);
        void SWITCH_COLOR_BANK_ON(void);
        
        void PUTCH(unsigned char ch);
        
        #define cputc(c) \
            PUTCH(c)

        void gotoxy(uint8_t x, uint8_t y);
                
            
    #elif defined(__NCURSES__)
        #if defined(__ATARI_ST__)
            #include <ncurses/curses.h>
        #else
            #include <ncurses.h>
        #endif
        
        #define gotoxy(x,y) do { move(y,x); refresh(); } while(0)
        #define cputc(c) do { addch(c); refresh(); } while(0)

    #else
        //
    #endif

    #if !defined(NO_COLOR)
        #if defined(__MO5__)
            #define COLOR_BLACK 0
            #define COLOR_RED 16
            #define COLOR_GREEN 32
            #define COLOR_YELLOW 48	
            #define COLOR_BLUE 64
            #define COLOR_MAGENTA 80
            #define COLOR_CYAN 96
            #define COLOR_WHITE 112	
        #elif defined(__TO7__)
            #define TO7_PASTEL_OFF 192
            #define COLOR_BLACK (0+TO7_PASTEL_OFF)
            #define COLOR_RED (8+TO7_PASTEL_OFF)
            #define COLOR_GREEN (16+TO7_PASTEL_OFF)
            #define COLOR_YELLOW (24+TO7_PASTEL_OFF)
            #define COLOR_BLUE (64+TO7_PASTEL_OFF)
            #define COLOR_MAGENTA (40+TO7_PASTEL_OFF)
            #define COLOR_CYAN (48+TO7_PASTEL_OFF)
            #define COLOR_WHITE (56+TO7_PASTEL_OFF)
        #endif

        #ifndef COLOR_BLACK
            #if defined(Z88DK)
                #define COLOR_BLUE BLUE
                #define COLOR_BLACK BLACK
                #if defined(__PC6001__)
                    #define COLOR_WHITE 7
                #else
                    #define COLOR_WHITE WHITE
                #endif
            #endif
        #endif
        
        #ifndef COLOR_RED
            #define COLOR_GREEN GREEN
            #define COLOR_CYAN CYAN
            #define COLOR_MAGENTA MAGENTA
            #define COLOR_RED RED
            #define COLOR_YELLOW YELLOW
        #endif
    #else
        #ifndef COLOR_BLACK
        #define COLOR_BLACK 0
        #endif
        
        #ifndef COLOR_WHITE
        #define COLOR_WHITE 0
        #endif
        
        #ifndef COLOR_RED
        #define COLOR_RED 0
        #endif
        
        #ifndef COLOR_CYAN
        #define COLOR_CYAN 0
        #endif
        
        #ifndef COLOR_GREEN
        #define COLOR_GREEN 0
        #endif
        
        #ifndef COLOR_YELLOW
        #define COLOR_YELLOW 0
        #endif
        
        #ifndef COLOR_BLUE
        #define COLOR_BLUE 0
        #endif
    #endif
#endif // defined(NO_GRAPHICS)

#define CPC_WHITE 1
#define CPC_YELLOW 2 
#define CPC_RED 3
#define CPC_BLACK 4

#define CPC_TEXT_WHITE 3
#define CPC_TEXT_RED 4
#define CPC_TEXT_BLACK 7
#define CPC_TEXT_YELLOW 0

#if defined(NO_GRAPHICS) || defined(NO_PRINT)
    #if !defined(COLOR_RED)
        #define COLOR_RED 0
    #endif
    #if !defined(COLOR_BLUE)
        #define COLOR_BLUE 0
    #endif
    #if !defined(COLOR_WHITE)
        #define COLOR_WHITE 0
    #endif
    #if !defined(COLOR_YELLOW)
        #define COLOR_YELLOW 0
    #endif
    #if !defined(COLOR_GREEN)
        #define COLOR_GREEN 0
    #endif
#endif 


#ifndef COLOR_CYAN
	#define COLOR_CYAN COLOR_BLUE
#endif

#ifndef COLOR_PURPLE
	#define COLOR_PURPLE COLOR_CYAN
#endif


#if defined(CPCRSLIB)
    #undef COLOR_BLUE
    #define COLOR_BLUE CPC_WHITE
    
    #undef COLOR_CYAN
    #define COLOR_CYAN CPC_WHITE
    
    #undef COLOR_RED 
    #define COLOR_RED CPC_RED
    
    #undef COLOR_WHITE 
    #define COLOR_WHITE CPC_WHITE
    
    #undef COLOR_YELLOW
    #define COLOR_YELLOW CPC_YELLOW

    #undef COLOR_PURPLE 
    #define COLOR_PURPLE CPC_YELLOW
    
    #undef COLOR_GREEN
    #define COLOR_GREEN CPC_YELLOW
#endif

#  if defined(__PC6001__) || defined(__SPC1000__)
    #undef COLOR_WHITE
    #define COLOR_WHITE COLOR_CYAN
#endif


#if defined(ATARI_MODE_1_COLOR) || defined(__ATARI5200__)

    #define _ATARI_MODE1_RED 0
    #define _ATARI_MODE1_WHITE 64
    #define _ATARI_MODE1_BLUE 128
    #define _ATARI_MODE1_GREEN 128
    #define _ATARI_MODE1_YELLOW 192

    #undef COLOR_RED
    #define COLOR_RED _ATARI_MODE1_RED
    
    #undef COLOR_WHITE
    #define COLOR_WHITE _ATARI_MODE1_WHITE

    #undef COLOR_BLUE
    #define COLOR_BLUE _ATARI_MODE1_BLUE
    
    #undef COLOR_YELLOW
    #define COLOR_YELLOW _ATARI_MODE1_YELLOW
    
    #undef COLOR_GREEN
    #define COLOR_GREEN _ATARI_MODE1_GREEN
    
    #undef COLOR_CYAN
    #define COLOR_CYAN _ATARI_MODE1_WHITE
#endif

#if defined(__NES__) && defined(NES_CONIO)

    #define _NES_RED 0
    #define _NES_YELLOW 1
    #define _NES_WHITE 2

    #undef COLOR_RED
    #define COLOR_RED _NES_RED
    
    #undef COLOR_WHITE
    #define COLOR_WHITE _NES_WHITE

    #undef COLOR_BLUE
    #define COLOR_BLUE _NES_WHITE
    
    #undef COLOR_YELLOW
    #define COLOR_YELLOW _NES_YELLOW
    
    #undef COLOR_GREEN
    #define COLOR_GREEN _NES_YELLOW
    
    #undef COLOR_CYAN
    #define COLOR_CYAN _NES_WHITE
#endif

#if defined(COMX_COLOR)

    #define _COMX_BLUE 0
    #define _COMX_GREEN 1
    #define _COMX_CYAN 2
    #define _COMX_PURPLE (0+128)
    #define _COMX_YELLOW (1+128)
    #define _COMX_WHITE  (2+128) 

    #undef COLOR_RED
    #define COLOR_RED _COMX_PURPLE

    #undef COLOR_WHITE
    #define COLOR_WHITE _COMX_WHITE

    #undef COLOR_BLUE
    #define COLOR_BLUE _COMX_BLUE

    #undef COLOR_YELLOW
    #define COLOR_YELLOW _COMX_YELLOW

    #undef COLOR_GREEN
    #define COLOR_GREEN _COMX_GREEN

    #undef COLOR_CYAN
    #define COLOR_CYAN _COMX_CYAN

#endif

#endif // _CONIO_PATH_H

