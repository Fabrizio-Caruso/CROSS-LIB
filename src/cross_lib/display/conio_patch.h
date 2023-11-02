
#ifndef _CONIO_PATCH_H
#define _CONIO_PATCH_H


#if defined(ACK)
    #define cputc(c) putchar(c)
#endif

#if defined(NO_PRINT) || defined(__NO_GRAPHICS)
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
    
    #if defined(__C16__) && defined(__MEMORY_MAPPED_GRAPHICS)

        // #if !defined(PEEK) && !defined(POKE)
            // #include <peekpoke.h>
        // #endif
        #define textcolor(c) POKE(0x053B,c)
        #define bgcolor(c)
        #define bordercolor(c)
    #endif

    #if defined(__Z88DK__) || defined(ACK)
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

    #if defined(__Z88DK__) && !defined(__SMS__) && !defined(__ZX80__) && !defined(__ZX81__) && !defined(__LAMBDA__)
        #undef cputc
        #define cputc(c) fputc_cons(c)
    #endif

    #  if defined(__ADM3A)
        #define gotoxy(x,y) printf("\033=%c%c",y+32,x+32);
        #define clrscr() printf("\032")

    #elif defined(__ADM3A_WITH_UNDEF)
        // TODO: Remove undef's below
        #undef gotoxy
        #undef clrscr 
        #define gotoxy(x,y) printf("\033=%c%c",y+32,x+32);
        #define clrscr() printf("\032")	
    #elif defined(__VT52)
        #undef gotoxy
        #define gotoxy(x,y) printf("\033Y%c%c",y+32,x+32)

        #undef clrscr
        #define clrscr() printf("\033E")

    #elif defined(__VT100)
        #include <stdio.h>

        #define gotoxy(x,y) printf("\033[%d;%dH", y+1, x+1)
        #define clrscr() printf("\033[2J\033[H")

    #elif defined(__PDP)
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
        

     
    #elif (defined(__MSX__) && defined(__MEMORY_MAPPED_GRAPHICS)) || (defined(__SVI__) && defined(MSX_MODE0))
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
        // #if defined(__ATARI_ST__)
            // #include <ncurses/curses.h>
        // #else
            #include <ncurses.h>
        // #endif
        
        #define gotoxy(x,y) do { move(y,x); refresh(); } while(0)
        #define cputc(c) do { addch(c); refresh(); } while(0)

    #else
        //
    #endif

#endif // defined(NO_PRINT) || defined(__NO_GRAPHICS)
#endif // _CONIO_PATCH_H

