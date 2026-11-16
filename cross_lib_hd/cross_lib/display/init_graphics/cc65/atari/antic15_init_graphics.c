#if !defined(__ATARI5200__)
#include <atari.h>
#else
#include <atari5200.h>
#endif

#include<peekpoke.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>

#define SCREEN_WIDTH 40
#define CHAR_HEIGHT 8
#define CHAR_WIDTH 2

#include "8x8_chars.h"
#include "udg_map.h"

uint8_t _atari_text_color;

char nibble2byte[16] = {
	0x00, 0x03, 0x0C, 0x0F,
	0x30, 0x33, 0x3C, 0x3F,
	0xC0, 0xC3, 0xCC, 0xCF,
	0xF0, 0xF3, 0xFC, 0xFF
};

char background_mask = 0x00;
char foreground_mask = 0xFF;

extern char _FONT_START__[];


void outc_left(const char *src, char *dest)
{
	char i, n, c;
	for (i=0; i<8; i++)
	{
		c = *src++;
		n = nibble2byte[c >> 4];
		c = ((n ^ 0xFF) & background_mask) | (n & foreground_mask);
		*dest = c;
		dest += SCREEN_WIDTH;
	}
}

void outc_right(const char *src, char *dest)
{
	char i, n, c;
	for (i=0; i<8; i++)
	{
		c = *src++;
		n = nibble2byte[c & 15];
		c = ((n ^ 0xFF) & background_mask) | (n & foreground_mask);
		*dest = c;
		dest += SCREEN_WIDTH;
	}
}


void output_code(char c)
{
	char *src, *dest;

    // if (c >= 32 && c <= 95) c = c - 32;
    // if (c >= 96 && c <= 127) c = (c + 64) & 127;
#if !defined(__ATARI5200__)
    src = (char *)(OS.chbas << 8) + (8 * c);
#else
    src = (char *)(_FONT_START__ + (8*c));
#endif
	dest = OS.savmsc;
	outc_left(src, dest);
	outc_right(src, ++dest);
	OS.savmsc = ++dest;
}

void output_char(char c)
{
	char *src, *dest;

    if (c >= 32 && c <= 95) c = c - 32;
    if (c >= 96 && c <= 127) c = (c + 64) & 127;
#if !defined(__ATARI5200__)
    src = (char *)(OS.chbas << 8) + (8 * c);
#else
    src = (char *)(0xF800 + (8 * c));
#endif
	dest = OS.savmsc;
	outc_left(src, dest);
	outc_right(src, ++dest);
	OS.savmsc = ++dest;
}

void output_str(const char *s)
{
	char c;
    set_fore_color(_atari_text_color);
	for (c = *s; c != 0; c = *(++s))
	{
		output_char(c);
	}		
}

void set_position(char x, char y)
{
	char *dlist;
	dlist = (char *)(OS.sdlst);
	OS.savmsc = (char *)(*(unsigned int *)(dlist+4));
	OS.savmsc += (CHAR_HEIGHT * SCREEN_WIDTH * y);
	OS.savmsc += (x * CHAR_WIDTH);
}

void set_back_color(char c)
{
	background_mask = 0x55 * (c & 3);
}

void set_fore_color(char c)
{
	foreground_mask = 0x55 * (c & 3);
}

#if !defined(__ATARI5200__)
// void getkey(void)
// {
	// OS.ch = 0xFF;
	// while (OS.ch == 0xFF);
	// OS.ch = 0xFF;
// }
#else
extern void _graphics(char mode);
#endif


#if !defined(__ATARI5200__)
void set_udg(void)
{
	uint8_t *_CHBAS = (uint8_t *) 0x2F4;

	memcpy(_FONT_START__, (void *)0xE000, 512);
	
	/* modify your font at _FONT_START__, etc, then set the new font: */
	REDEFINE_AT(_FONT_START__);
	
	*_CHBAS = ((int)_FONT_START__ >> 8);  /* enable the new font */
}
#else
void set_udg(void)
{
	extern char _FONT_START__[];
	
	uint8_t *CHBASE = (uint8_t *)0xD409;

	memcpy(_FONT_START__, (void *)0xF800, 512);
	REDEFINE_AT(_FONT_START__);
	
	*CHBASE = ((int)_FONT_START__ >> 8);
}

#endif

// void main(void)
void _XL_INIT_GRAPHICS(void)
{
	// char i;
	// char *msg = "Hi World!";

    // unsigned char j;
    // int j;
    


    _graphics(15+16);

	OS.color0 = 0x0E; // White
	OS.color1 = 0xD6; // Green
	OS.color2 = 0x86; // Blue
	OS.color4 = 0x00; // Black

    // for(j=0;j<128*8;++j)
    // {
        // POKE(0xF800+j,255);
    // }

    set_udg();

    // for(j=0;j<128;++j)
    // {
        // set_position(j%20,j/20);
        // output_code(j);
    // }

    // for(i=0;i<3;++i)
    // {
        // set_position(10, 4+i);
        // set_back_color(0);
        // set_fore_color(i+1);
        // output_str(msg);
    // }
	// for (i=0; i<3; i++)
	// {
		// set_position(10, 4+i);
		// set_back_color(0);
		// set_fore_color(i+1);
		// output_str(msg);
	// }



// #if !defined(__ATARI5200__)
	// getkey();
// #else
	// while (1);
// #endif
}

// int main(void)
// {
    // _XL_INIT_GRAPHICS();
// }