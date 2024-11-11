
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

// #include "bbc.h"
char OSBYTE0(__reg("a") char)="\tjsr\t$fff4\n\ttxa";
char OSBYTE1(__reg("a") char,__reg("r0") char)="\tldx\tr0\n\tjsr\t$fff4\n\ttxa";
char OSBYTE1RI(__reg("a") char,__reg("r0") char)="\tldx\tr0\n\tjsr\t$fff4\n\ttxa\n\tpha\n\ttya\n\ttax\n\tpla";
char OSBYTE2(__reg("a") char,__reg("r0") char,__reg("r1") char)="\tldx\tr0\n\tldy\tr1\n\tjsr\t$fff4\n\ttxa";
char OSBYTE2I(__reg("a") char,__reg("r0/r1") unsigned int)="\tldx\tr0\n\tldy\tr1\n\tjsr\t$fff4\n\ttxa";
void OSBYTE0NR(__reg("a") char)="\tjsr\t$fff4";
void OSBYTE1NR(__reg("a") char,__reg("r0") char)="\tldx\tr0\n\tjsr\t$fff4";
void OSBYTE2NR(__reg("a") char,__reg("r0") char,__reg("r1") char)="\tldx\tr0\n\tldy\tr1\n\tjsr\t$fff4";

#define OSVER() OSBYTE0(0)
#define OSUSRFLG(x,y) OSBYTE2(1,(x),(y))
#define OSSELINPUT(x) OSBYTE1NR(2,(x))
#define OSSELOUTPUT(x) OSBYTE2(3,(x),(y))
#define OSCURSORED(x) OSBYTE2(4,(x),(y))
#define OSSELADC(x) OSBYTE1(16,(x))
#define OSFRCADC(x) OSBYTE1NR(17,(x))
#define OSVSYNC() OSBYTE0(19)
#define OSRDVDU() OSBYTE0(117)
#define OSSCANKEY(x) OSBYTE1(121,(x))
#define OSSCANKEY16() OSBYTE0(122)
#define OSADVAL(x) OSBYTE1RI(128,(x))
#define OSINKEY(x) OSBYTE2I(129,(x)) 


void OSWORD(__reg("a") char,__reg("r0/r1") void *)="\tldx\tr0\n\tldy\tr1\n\tjsr\t$fff1";

#define OSRDSYSCLK(x) OSWORD(1,x)
#define OSWRSYSCLK(x) OSWORD(2,x)
#define OSRDTIMER(x) OSWORD(3,x)
#define OSWRTIMER(x) OSWORD(4,x)
#define OSRDIO(x) OSWORD(5,x)
#define OSWRIO(x) OSWORD(6,x)
#define OSSOUND(x) OSWORD(7,x)
#define OSENVELOPE(x) OSWORD(8,x)
#define OSRDPIXEL(x) OSWORD(9,x)
#define OSRDCHARDEF(x) OSWORD(10,x)
#define OSRDPALETTE(x) OSWORD(11,x)
#define OSWRPALETTE(x) OSWORD(12,x)
#define OSRDGRCURSOR(x) OSWORD(13,x)


#define _TILE_0 200
#define _TILE_1 201
#define _TILE_2 202
#define _TILE_3 203
#define _TILE_4 204
#define _TILE_5 205
#define _TILE_6 206
#define _TILE_7 207
#define _TILE_8 208
#define _TILE_9 209
#define _TILE_10 210
#define _TILE_11 211
#define _TILE_12 212
#define _TILE_13 213
#define _TILE_14 214
#define _TILE_15 215
#define _TILE_16 216
#define _TILE_17 217
#define _TILE_18 218
#define _TILE_19 219
#define _TILE_20 220
#define _TILE_21 221
#define _TILE_22 222
#define _TILE_23 223
#define _TILE_24 224
#define _TILE_25 225
#define _TILE_26 226


#include <stdint.h>

void osputc(__reg("a") char)="\tjsr\t0xffee";



#define _TILE_0_UDG     { 198,186,254,170,238,186,170,16}
#define _TILE_1_UDG     { 0,0,198,186,254,170,238,186}
#define _TILE_3_UDG     { 0,0,0,0,198,186,254,170}
#define _TILE_2_UDG     { 170,16,0,0,0,0,0,0}

#define _TILE_4_UDG     {238,186,170,16,0,0,0,0}

#define _TILE_7_UDG     { 198,186,0,170,0,186,214,16}

#define _TILE_9_UDG     {  4,14,14,4,63,68,228,64}

#define _TILE_8_UDG     {  60,126,231,195,195,231,126,60}

#define _TILE_11_UDG    {  0,0,0,0,3,10,4,10}

#define _TILE_5_UDG     { 0,0,0,0,0,0,198,186}

#define _TILE_16_UDG    { 0,0,0,0,248,4,14,4}

#define _TILE_15_UDG    {  0,0,0,16,130,16,84,16}

#define _TILE_14_UDG    { 198,186,68,84,186,214,16,16}

#define _TILE_6_UDG     {254,170,238,186,170,16,0,0}

#define _TILE_12_UDG    { 64,224,224,64,248,68,78,4 }

#define _TILE_13_UDG    {  0,0,0,0,63,64,224,64 }

#define _TILE_10_UDG    { 0,0,0,0,128,160,64,160 }


#define _TILE_17_UDG    { 4,14,14,4,4,4,0,0 }

#define _TILE_18_UDG    { 64,224,224,64,64,64,0,0 }

#define _TILE_19_UDG    { 198,186,254,170,238,186,214,16 }

#define _TILE_20_UDG    { 254,170,238,186,214,16,0,0 }

#define _TILE_21_UDG    { 238,186,214,16,0,0,0,0 }

#define _TILE_22_UDG    { 214,16,0,0,0,0,0,0 }

#define _TILE_23_UDG    { 0,24,60,122,253,122,60,24 }

#define _TILE_24_UDG    { 120,252,255,133,255,161,255,0 }

#define _TILE_25_UDG    { 0,0,0,24,52,122,52,24 }

#define _TILE_26_UDG    { 0,0,0,0,0,16,16,16 }


const struct redefine_struct
{
   const uint8_t ascii;
   const uint8_t bitmap[8];
} ;



const struct redefine_struct redefine_map[] =
{
	{_TILE_0, _TILE_0_UDG},
	{_TILE_1, _TILE_1_UDG},
	{_TILE_2, _TILE_2_UDG},	
	{_TILE_3, _TILE_3_UDG},
	{_TILE_4, _TILE_4_UDG},
	{_TILE_5, _TILE_5_UDG},
	{_TILE_6, _TILE_6_UDG},
    {_TILE_7, _TILE_7_UDG},
    {_TILE_8, _TILE_8_UDG},
    {_TILE_9, _TILE_9_UDG},
    {_TILE_10, _TILE_10_UDG},
    {_TILE_11, _TILE_11_UDG},
    {_TILE_12, _TILE_12_UDG},
    {_TILE_13, _TILE_13_UDG},
    {_TILE_14, _TILE_14_UDG},
    {_TILE_15, _TILE_15_UDG},
    {_TILE_16, _TILE_16_UDG},
    {_TILE_17, _TILE_17_UDG},
    {_TILE_18, _TILE_18_UDG},
    {_TILE_19, _TILE_19_UDG},
    {_TILE_20, _TILE_20_UDG},
    {_TILE_21, _TILE_21_UDG},
    {_TILE_22, _TILE_22_UDG},
    {_TILE_23, _TILE_23_UDG},
    {_TILE_24, _TILE_24_UDG},
    {_TILE_25, _TILE_25_UDG},
    {_TILE_26, _TILE_26_UDG},
};


#define REDEFINE_AT(addr) \
{ \
	uint8_t i; \
	\
	for (i = 0; i < (sizeof(redefine_map)/sizeof(*redefine_map)); ++i) \
	{ \
		memcpy((uint8_t *)(addr) + screenCode((redefine_map[i].ascii))*8, redefine_map[i].bitmap, 8); \
	} \
}




void redefine(const uint8_t ch, const uint8_t *data)
{
    uint8_t i;
    
    osputc(23);
    osputc(ch);
    for(i=0;i<8;++i)
    {
        osputc(data[i]);
    }
}


void SET_UDG_IMAGES(void) 
{ 
    uint8_t i;

    for (i = 0; i < sizeof(redefine_map) / sizeof(*redefine_map); ++i) 
    {
            redefine(redefine_map[i].ascii, redefine_map[i].bitmap);
    } 
}




void _XL_INIT_GRAPHICS(void)
{

    osputc(22);

    osputc(5);

    
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

    SET_UDG_IMAGES();

    osputc('A');
    osputc('B');
    osputc('C');
    
    osputc(_TILE_0);
    osputc(_TILE_1);
    osputc(_TILE_2);

    
    while(1){};

}


int main(void)
{
    _XL_INIT_GRAPHICS();
    return 0;
}