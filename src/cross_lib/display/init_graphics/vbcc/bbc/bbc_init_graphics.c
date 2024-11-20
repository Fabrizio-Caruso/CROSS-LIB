
// DOC on VDU
// https://www.bbcbasic.co.uk/bbcbasic/manual/vdu.html#vdu22

/*
0 	Null - it does nothing.
1 	Send the next character to the printer ONLY.
2 	Enable the printer.
3 	Disable the printer.
4 	Write text at the text cursor position.
5 	Write text at the graphics cursor position.
6 	Enable output to the screen.
7 	Bell - make a short 'beep'.
8 	Move the text cursor backwards one character.
9 	Move the text cursor forwards one character.
10 	Move the text cursor down one line.
11 	Move the text cursor up one line.
12 	Clear the text area - identical to CLS.
13 	Move the text cursor to the start of the current line.
14 	Enable the auto-paging mode.
15 	Disable the auto-paging mode.
16 	Clear the graphics area - identical to CLG.
17 	Define a text colour - identical to COLOUR.
18 	Define a graphics colour - identical to GCOL.
19 	Select a colour palette.
20 	Restore the default logical colours.
21 	Disable output to the screen.
22 	Select the screen mode - identical to MODE.
23 	Program characters and control the cursor.
24 	Define a graphics window.
25 	Identical to PLOT.
26 	Restore the default text and graphics windows.
27 	Send the next character to the screen.
28 	Define a text window.
29 	Set the graphics origin.
30 	Home the text cursor to the top left of the screen.
31 	Move the text cursor - identical to TAB(x,y).
127 	Backspace and delete.
*/

#include "bbc.h"
// char OSBYTE0(__reg("a") char)="\tjsr\t$fff4\n\ttxa";
// char OSBYTE1(__reg("a") char,__reg("r0") char)="\tldx\tr0\n\tjsr\t$fff4\n\ttxa";
// char OSBYTE1RI(__reg("a") char,__reg("r0") char)="\tldx\tr0\n\tjsr\t$fff4\n\ttxa\n\tpha\n\ttya\n\ttax\n\tpla";
// char OSBYTE2(__reg("a") char,__reg("r0") char,__reg("r1") char)="\tldx\tr0\n\tldy\tr1\n\tjsr\t$fff4\n\ttxa";
// char OSBYTE2I(__reg("a") char,__reg("r0/r1") unsigned int)="\tldx\tr0\n\tldy\tr1\n\tjsr\t$fff4\n\ttxa";
// void OSBYTE0NR(__reg("a") char)="\tjsr\t$fff4";
// void OSBYTE1NR(__reg("a") char,__reg("r0") char)="\tldx\tr0\n\tjsr\t$fff4";
// void OSBYTE2NR(__reg("a") char,__reg("r0") char,__reg("r1") char)="\tldx\tr0\n\tldy\tr1\n\tjsr\t$fff4";

// #define OSVER() OSBYTE0(0)
// #define OSUSRFLG(x,y) OSBYTE2(1,(x),(y))
// #define OSSELINPUT(x) OSBYTE1NR(2,(x))
// #define OSSELOUTPUT(x) OSBYTE2(3,(x),(y))
// #define OSCURSORED(x) OSBYTE2(4,(x),(y))
// #define OSSELADC(x) OSBYTE1(16,(x))
// #define OSFRCADC(x) OSBYTE1NR(17,(x))
// #define OSVSYNC() OSBYTE0(19)
// #define OSRDVDU() OSBYTE0(117)
// #define OSSCANKEY(x) OSBYTE1(121,(x))
// #define OSSCANKEY16() OSBYTE0(122)
// #define OSADVAL(x) OSBYTE1RI(128,(x))
// #define OSINKEY(x) OSBYTE2I(129,(x)) 


// void OSWORD(__reg("a") char,__reg("r0/r1") void *)="\tldx\tr0\n\tldy\tr1\n\tjsr\t$fff1";

// #define OSRDSYSCLK(x) OSWORD(1,x)
// #define OSWRSYSCLK(x) OSWORD(2,x)
// #define OSRDTIMER(x) OSWORD(3,x)
// #define OSWRTIMER(x) OSWORD(4,x)
// #define OSRDIO(x) OSWORD(5,x)
// #define OSWRIO(x) OSWORD(6,x)
// #define OSSOUND(x) OSWORD(7,x)
// #define OSENVELOPE(x) OSWORD(8,x)
// #define OSRDPIXEL(x) OSWORD(9,x)
// #define OSRDCHARDEF(x) OSWORD(10,x)
// #define OSRDPALETTE(x) OSWORD(11,x)
// #define OSWRPALETTE(x) OSWORD(12,x)
// #define OSRDGRCURSOR(x) OSWORD(13,x)

#include "8x8_chars.h"
#include "udg_map.h"

#include <stdint.h>
// #include <stdio.h>

void osputc(__reg("a") char)="\tjsr\t0xffee";

// static uint8_t stripes[] = {255,0,255,0,255,0,255,0};

// static uint8_t player[] = {24, 36, 24,102,153, 24, 36, 102};




// void redefine(const uint8_t ch, const uint8_t* image) 
// { 
    // uint8_t i; 
    
    // for(i=0;i<8;++i) 
    // { 
        // DISPLAY_POKE(CHAR_BASE +(uint16_t)(ch<<3)+i,image[i]); 
    // } 
// } 








void redefine(const uint8_t ch, const uint8_t *data)
{
    uint8_t i;
    
    osputc(23);
    osputc(ch);
    for(i=0;i<8;++i)
    {
        osputc(data[i]);
    }
    // osputc('\n');
}


void SET_UDG_IMAGES(void) 
{ 
    uint8_t i;
    // uint8_t j;

    for (i = 0; i < sizeof(redefine_map) / sizeof(*redefine_map); ++i) 
    {
            redefine(redefine_map[i].ascii, redefine_map[i].bitmap);
    } 
}


// void _gotoxy(uint8_t x, uint8_t y)
// {
    // osputc(31);
    // osputc(x);
    // osputc(y);
// }
// COLORS
// 0 -> black
// 1 -> red
// 2 -> green
// 3 -> yellow
// 4 -> blue
// 5 -> purple
// 6 -> cyan
// 7 -> white



// OSSCANKEY
// 'b' <-> SPACE
// '%' <-> 'I'
// 'F' <-> 'K'
// 'E' <-> 'J'
// 'V' <-> 'L'

#if !defined(__BBC_MODE)
    #define __BBC_MODE 7
#endif

void _XL_INIT_GRAPHICS(void)
{
    // uint8_t res;

    #if __BBC_MODE!=7
        osputc(22);
        #if defined(__SHADOW_RAM)
            osputc((uint8_t)__BBC_MODE+128U);
        #else
            osputc(__BBC_MODE);
        #endif
    #endif


    #if __BBC_MODE==5
        #if !defined(__USE_GREEN)
            osputc(19);
            osputc(2);
            osputc(6);
            osputc(1);
            osputc(1);
            osputc(1);
        #else
            osputc(19);
            osputc(2);
            osputc(2);
            osputc(0);
            osputc(0);
            osputc(0);
        #endif
    
    #endif
    
    // Disable the cursor
    osputc(23);
    osputc(1);
    osputc(0);
    osputc(0);
    osputc(0);
    osputc(0);
    osputc(0);
    osputc(0);
    osputc(0);
    osputc(0);

    // Delete graphics screen data
    // osputc(16);
    // osputc(12);

        
    // Redefine characters
    // TODO: This is wrong
#if !(__BBC_MODE==7) 
    SET_UDG_IMAGES();
#endif



}
