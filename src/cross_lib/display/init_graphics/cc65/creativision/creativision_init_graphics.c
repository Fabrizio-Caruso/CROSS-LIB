#define VDP_DATA  0x3000
#define VDP_CONTROL 0x3001
#include <peekpoke.h>
    
#define CHAR_BASE ((unsigned short) 0x0000)
#define COLOR_DEF 0x1800    

#include "8x8_chars.h"
  // you need to set up the VDP vectors at $BFF0.
  // .byte $00  ; VDP 0: External video off, M2=0
  // .byte $A2  ; VDP 1: 16K mode, Screen not active, Generate interrupts
             // ;        M1=0, M3=0, Sprites = 16x16 unmagnified
  // .byte $04  ; VDP 2: Pattern Name Table = 04 * $400 = $1000
  // .byte $4E  ; VDP 3: Color Table Address =    0001 0011 1000 0000 = $1380
  // .byte $00  ; VDP 4: Pattern Generator Table = $0000
  // .byte $26  ; VDP 5: Sprite Attribute Table = 0001 0011 0000 0000 = $1300
  // .byte $01  ; VDP 6: Sprite Generator Table = 0000 1000 0000 0000 = $0800
  // .byte $05 ; VDP 7: TC=0 (transparent fg), BD=5 (light blue bg)

  // ; VDP memory addresses (not CPU addresses)
  // ; $0000 Pattern generator table = character definitions (2048 bytes)
  // ; $0800 Sprite generator table = sprite definitions (2048 bytes)
  // ; $1000 Pattern name table = screen memory (32x24 = 768 bytes)
  // ; $1300 Sprite attribute table (128 bytes)
  // ; $1380 Color table address (32 bytes)    
    

// ;** VDP
// VDP_DATA_R              = $2000
// VDP_STATUS_R            = $2001
// VDP_DATA_W              = $3000
// VDP_CONTROL_W           = $3001    

// CC65
        // .byte   $00             ; Register 0
        // .byte   $C0             ; Register 1 16K RAM, Active Display, Mode 1, VBI disabled
        // .byte   $04             ; Register 2 Name Table at $1000 - $12FF
        // .byte   $60             ; Register 3 Colour Table at $1800 - $181F
        // .byte   $00             ; Register 4 Pattern Table at $0000 - $07FF
        // .byte   $10             ; Register 5 Sprite Attribute at $0800 - $087F
        // .byte   $01             ; Register 6 Sprite Pattern
        // .byte   $F1             ; Register 7 Text colour Foreground / background

    
// void VDP_REGISTER(unsigned char reg, unsigned char value)
// {
    // POKE(VDP_DATA,value);
    // POKE(VDP_CONTROL,(reg|0x80));
// }

void VDP_POKE(unsigned short address, unsigned char value)
{
    __asm__("sei");
    POKE(VDP_CONTROL,(unsigned char) (address&0x00FF));
    POKE(VDP_CONTROL,(unsigned char) (address>>8)|0x4000);
    POKE(VDP_DATA,value);
    __asm__("cli");
}

void set_group_color(unsigned char group, unsigned char color)
{
	VDP_POKE((unsigned short) COLOR_DEF+ (unsigned short) group, ((unsigned short) color)<<4);
}

void debug(unsigned short base, unsigned short range)
{
    unsigned short i;
    
    for(i=base;i<base+range;++i)
    {
        VDP_POKE(i,i&0xF0);
    }
}

unsigned char player_down_image[8] = _PLAYER_DOWN_UDG;
unsigned char player_up_image[8] = _PLAYER_UP_UDG;
unsigned char player_left_image[8] = _PLAYER_LEFT_UDG;
unsigned char player_right_image[8] = _PLAYER_RIGHT_UDG;


void redefine(unsigned short ch, unsigned char* image)
{
    unsigned short i;
    
    for(i=0;i<8;++i)
    {
        VDP_POKE(CHAR_BASE-1+(unsigned short)ch*(unsigned short)8u+i,image[i]);
    }   
}

#include <conio.h>
void INIT_GRAPHICS(void)
{
    unsigned short k;
    
    // debug(0x000,0x07ff);
    
    //debug(0x0000,0x0400);
    //debug(0x1000,0x0400);
    // VDP_POKE(0x0000,65);
    // VDP_POKE(0x0001,66);
  
    

	// set_group_color(0,9);
	// set_group_color(1,7);
	// set_group_color(2,9);
	// set_group_color(3,11);
	// set_group_color(4,10);
	// set_group_color(5,2);
	// set_group_color(6,4);
	// set_group_color(7,4); 

	// set_group_color(2,255);
	// set_group_color(3,255);
	// set_group_color(4,255);
	// set_group_color(5,255);    
    // POKE(COLOR_DEF,0x1F);
    // POKE(COLOR_DEF+1,64);
    // POKE(COLOR_DEF+2,32);
    // POKE(COLOR_DEF+3,16);
    // POKE(COLOR_DEF+4,128+64);
    // POKE(COLOR_DEF+5,64+32);
    // POKE(COLOR_DEF+6,32+16);
    // POKE(COLOR_DEF+7,16+128);   
    // POKE(COLOR_DEF+8,128);
    // POKE(COLOR_DEF+9,64);
    // POKE(COLOR_DEF+10,32);
    // POKE(COLOR_DEF+11,16);
    // POKE(COLOR_DEF+12,128+64);
    // POKE(COLOR_DEF+13,64+32);
    // POKE(COLOR_DEF+14,32+16);
    // POKE(COLOR_DEF+15,16+128);       

    gotoxy(2,6);
    cprintf("abcdefghijklmnopqrstuvwxyz");    

 
    redefine(160+39,player_right_image);
    redefine(160+40,player_left_image);
    redefine(160+41,player_up_image);
    redefine(160+42,player_down_image);
    
    for(k=0;k<5000;++k){};
}