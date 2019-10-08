
#include "8x8_chars.h"

#include "creativision_settings.h"

#include "udg_map.h"

#include "memory_mapped_graphics.h"

const unsigned char player_down_image[8] = _PLAYER_DOWN_UDG;
const unsigned char player_up_image[8] = _PLAYER_UP_UDG;
const unsigned char player_left_image[8] = _PLAYER_LEFT_UDG;
const unsigned char player_right_image[8] = _PLAYER_RIGHT_UDG;
const unsigned char skull_image[8] = _SKULL_UDG;
const unsigned char ghost_image[8] = _GHOST_UDG;
const unsigned char bomb_image[8] = _BOMB_UDG;
const unsigned char gun_image[8] = _GUN_UDG;
const unsigned char powerup_image[8] = _POWERUP_UDG;

const unsigned char super_image[8] = _SUPER_UDG;
const unsigned char extra_life_image[8] = _EXTRA_LIFE_UDG;
const unsigned char bullet_image[8] = _BULLET_UDG;
const unsigned char freeze_image[8] = _FREEZE_UDG;
const unsigned char invincibility_image[8] = _INVINCIBILITY_UDG;

const unsigned char dead_ghost_image[8] = _DEAD_GHOST_UDG;
const unsigned char rocket_image[8] = _ROCKET_UDG;
const unsigned char left_horizontal_missile_image[8] = _LEFT_HORIZONTAL_MISSILE_UDG;
const unsigned char right_horizontal_missile_image[8] = _RIGHT_HORIZONTAL_MISSILE_UDG;

const unsigned char extra_points_image[8] = _EXTRA_POINTS_UDG;

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


void set_group_color(unsigned char group, unsigned char color)
{
	DISPLAY_POKE((unsigned short) COLOR_DEF+ (unsigned short) group, ((unsigned short) color)<<4);
}

void redefine(const unsigned short ch, const unsigned char* image)
{
    unsigned short i;
    
    for(i=0;i<8;++i)
    {
        DISPLAY_POKE(CHAR_BASE+(unsigned short)(ch<<3)+i,image[i]);
    }   
}

void set_udg_colors(void)
{
	unsigned char i;
	
	set_group_color(0,9);
	set_group_color(1,7);
	set_group_color(2,9);
	set_group_color(3,11);
	set_group_color(4,10);
	set_group_color(5,2);
	set_group_color(6,4);
	set_group_color(7,4);
	
	for(i=8;i<=11;++i)
	{
		set_group_color(i,8);
	}
}


void set_udg_images(void)
{
	unsigned char i;
	
	for (i = 0; i < sizeof(redefine_map) / sizeof(*redefine_map); ++i)
	{
	   redefine(redefine_map[i].ascii, redefine_map[i].bitmap);
	}
}

#include <conio.h>
void INIT_GRAPHICS(void)
{

    // psg_silence();
 
    // redefine(_PLAYER_RIGHT,player_right_image);
    // redefine(_PLAYER_LEFT,player_left_image);
    // redefine(_PLAYER_UP,player_up_image);
    // redefine(_PLAYER_DOWN,player_down_image);
    // redefine(_SKULL,skull_image);
    // redefine(_GUN,gun_image);
    // redefine(_POWERUP,powerup_image);
    // redefine(_BOMB,bomb_image);
    // redefine(32-0x20+_GHOST,ghost_image);
    // redefine(32-0x20+_ROCKET,rocket_image);
    // redefine(32-0x20+_LEFT_HORIZONTAL_MISSILE,left_horizontal_missile_image);
    // redefine(32-0x20+_RIGHT_HORIZONTAL_MISSILE,right_horizontal_missile_image);
    // redefine(_BULLET,bullet_image); 
    // redefine(_DEAD_GHOST,dead_ghost_image);    
    // redefine(_EXTRA_POINTS,extra_points_image);
 
    // redefine(160+_SUPER,super_image);
    // redefine(160+_EXTRA_LIFE,extra_life_image);
    // redefine(160-0x40+_FREEZE,freeze_image);
    // redefine(160-0x20+_INVINCIBILITY,invincibility_image); 
    // redefine(160-0x60+_DEAD_GHOST,dead_ghost_image);
    
    set_udg_colors();
    
}