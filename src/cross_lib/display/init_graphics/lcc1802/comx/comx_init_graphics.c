#include <comx/rca_vis_video.h>
#include <comx/rca_system.h>

#if defined(__COMX__) || defined(__PECOM__)
    void redefine_char(const unsigned char * shapelocation, int color)
    {
        unsigned char colored_shape[9];
        unsigned char i;
        
        for(i=0;i<9;++i)
        {
           colored_shape[i]=shapelocation[i+1]+color*64;
        }
        shapechar(colored_shape, *shapelocation, 1);
    }

    #define __PLAYER_DOWN_UDG              12,18,12,51,45,12,18,51
    #define __PLAYER_UP_UDG                12,30,12,51,45,12,18,51
    #define __PLAYER_LEFT_UDG              12,22,12,62,13,12,20,20
    #define __PLAYER_RIGHT_UDG             12,26,12,31,44,12,10,10


    #define __GHOST_UDG                    33,30,33,51,33,45,33,30
    #define __SKULL_UDG                    30,33,51,33,33,18,18,12
    #define __GUN_UDG                       0,32,31,40,56,32, 0, 0
    #define __POWERUP_UDG                   0,30,51,55,51,26,18,12
    #define __BULLET_UDG                    0, 0, 4,28,14, 8, 0, 0
    #define __BOMB_UDG                     30,33,51,45,45,51,33,30
    #define __INVINCIBILITY_UDG            12,18,12,51, 0,12, 0,51

    #define __DEAD_GHOST_UDG               18,30,33,51,33,45,51,30

    #define __ROCKET_UDG                   12,30,30,30,63,45,33,33
    #define __LEFT_HORIZONTAL_MISSILE_UDG   0, 0,56,31,31,56, 0, 0
    #define __RIGHT_HORIZONTAL_MISSILE_UDG  0, 0, 7,62,62, 7, 0, 0
    #define __VERTICAL_BRICK_UDG           12,12,28,12,12,14,12,12
    #define __HORIZONTAL_BRICK_UDG          0, 0, 0,63,63,00,00,00



// 106 - 124
    const unsigned char invincibility_udg[10] =  { 106 , __INVINCIBILITY_UDG, 0};
    const unsigned char player_down[10] = { 107, __PLAYER_DOWN_UDG, 0 };
    const unsigned char player_up[10] = { 108, __PLAYER_UP_UDG, 0 };
    const unsigned char player_right[10] = { 109 , __PLAYER_LEFT_UDG, 0 };
    const unsigned char player_left[10] = { 110 , __PLAYER_RIGHT_UDG, 0};
    const unsigned char bomb[10] =  { 111 , __BOMB_UDG, 0};
    const unsigned char ghost[10] = { 112 , __GHOST_UDG, 0 };
    const unsigned char skull_udg[10] = { 113 , __SKULL_UDG, 0 };
    const unsigned char power_udg[10] = { 114 , __POWERUP_UDG, 0 };
    const unsigned char gun_udg[10] =  { 115 , __GUN_UDG, 0};
    const unsigned char bullet_udg[10] =  { 116 , __BULLET_UDG, 0};
    const unsigned char dead_ghost_udg[10] = {117, __DEAD_GHOST_UDG, 0};
    const unsigned char rocket_udg[10] = { 118 , __ROCKET_UDG, 0 };
    const unsigned char left_missile_udg[10] = { 119 , __LEFT_HORIZONTAL_MISSILE_UDG, 0 };
    const unsigned char right_missile_udg[10] =  { 120 , __RIGHT_HORIZONTAL_MISSILE_UDG, 0};
    const unsigned char vertical_brick_udg[10] =  { 121 , __VERTICAL_BRICK_UDG, 12 };
    const unsigned char horizontal_brick_udg[10] = {122, __HORIZONTAL_BRICK_UDG, 0};
    const unsigned char freeze_udg[10] = { 123 , __POWERUP_UDG, 0 };
    const unsigned char super_udg[10] = { 124 , __POWERUP_UDG, 0 };



#endif

#ifdef __CIDELSA__
static const unsigned char shape_alpha[] =
{
	0x7E,	// First byte is the '3rd' color bit for following lines one bit per line, b7 for first line
	0x00, 0xDF, 0xE8, 0xE8, 0xE8, 0xFF, 0xDF, 0x00,  //A
	0x7E,
	0x00, 0xD6, 0xE9, 0xE9, 0xE9, 0xFF, 0xFF, 0x00,  //B
	0x7E, 
	0x00, 0xD2, 0xE1, 0xE1, 0xE1, 0xFF, 0xDE, 0x00,  //C
	0x7E, 
	0x00, 0xDE, 0xE1, 0xE1, 0xE1, 0xFF, 0xFF, 0x00,  //D
	0x7E, 
	0x00, 0xE1, 0xE1, 0xE9, 0xE9, 0xFF, 0xFF, 0x00,  //E
	0x7E,
	0x00, 0xE0, 0xE0, 0xE8, 0xE8, 0xFF, 0xFF, 0x00,  //F
	0x7E,
	0x00, 0xD6, 0xE5, 0xE1, 0xE1, 0xFF, 0xDE, 0x00,  //G
	0x7E, 
	0x00, 0xFF, 0xC8, 0xC8, 0xC8, 0xFF, 0xFF, 0x00,  //H
	0x3C, 
	0x00, 0x00, 0xE1, 0xFF, 0xFF, 0xE1, 0x00, 0x00,  //I
	0x7E, 
	0x00, 0xFE, 0xFF, 0xC1, 0xC1, 0xC1, 0xC2, 0x00,  //J
	0x3E, 
	0x00, 0x00, 0xE1, 0xD2, 0xCC, 0xFF, 0xFF, 0x00,  //K
	0x3E, 
	0x00, 0x00, 0xC1, 0xC1, 0xC1, 0xFF, 0xFF, 0x00,  //L
	0x7E, 
	0x00, 0xFF, 0xD0, 0xC8, 0xD0, 0xFF, 0xFF, 0x00,  //M
	0x7E, 
	0x00, 0xFF, 0xC4, 0xC8, 0xD0, 0xFF, 0xFF, 0x00,  //N
	0x7E, 
	0x00, 0xDE, 0xE1, 0xE1, 0xE1, 0xFF, 0xDE, 0x00,  //O
	0x7E, 
	0x00, 0xD0, 0xE8, 0xE8, 0xE8, 0xFF, 0xFF, 0x00,  //P
	0x7E, 
	0x00, 0xDE, 0xE1, 0xE7, 0xE1, 0xFF, 0xDE, 0x00,  //Q
	0x7E, 
	0x00, 0xD1, 0xEA, 0xEC, 0xE8, 0xFF, 0xFF, 0x00,  //R
	0x7E, 
	0x00, 0xE6, 0xEF, 0xE9, 0xE9, 0xF9, 0xD1, 0x00,  //S
	0x7E, 
	0x00, 0xE0, 0xE0, 0xFF, 0xFF, 0xE0, 0xE0, 0x00,  //T
	0x7E, 
	0x00, 0xFE, 0xC1, 0xC1, 0xC1, 0xFF, 0xFE, 0x00,  //U
	0x7E, 
	0x00, 0xF0, 0xCC, 0xC3, 0xCF, 0xFC, 0xF0, 0x00,  //V
	0xFF, 
	0xF0, 0xCC, 0xC3, 0xCC, 0xC3, 0xCF, 0xFC, 0xF0,  //W
	0x7E, 
	0x00, 0xF3, 0xD4, 0xC8, 0xD4, 0xF3, 0xF3, 0x00,  //X
	0x7E,
	0x00, 0xE0, 0xD0, 0xCF, 0xDF, 0xF0, 0xE0, 0x00,  //Y
	0x7E,
	0x00, 0xF1, 0xF1, 0xE9, 0xE5, 0xE3, 0xE3, 0x00,  //Z
};

static const unsigned char shape_numbers[] =
{
	0x00,	
	0x5E, 0x61, 0x61, 0x61, 0x5E, 0x00, 0x00, 0x00,	 //0
	0x00, 
	0x41, 0x7F, 0x61, 0x51, 0x41, 0x00, 0x00, 0x00,  //1
	0x00, 
	0x51, 0x69, 0x65, 0x65, 0x53, 0x00, 0x00, 0x00,  //2
	0x00, 
	0x56, 0x69, 0x69, 0x61, 0x61, 0x00, 0x00, 0x00,  //3
	0x00, 
	0x44, 0x7F, 0x64, 0x54, 0x4C, 0x00, 0x00, 0x00,  //4
	0x00, 
	0x66, 0x69, 0x69, 0x69, 0x72, 0x00, 0x00, 0x00,  //5
	0x00, 
	0x66, 0x69, 0x69, 0x69, 0x5E, 0x00, 0x00, 0x00,  //6
	0x00, 
	0x70, 0x68, 0x64, 0x63, 0x60, 0x00, 0x00, 0x00,  //7
	0x00, 
	0x56, 0x69, 0x69, 0x69, 0x56, 0x00, 0x00, 0x00,  //8
	0x00, 
	0x5E, 0x69, 0x69, 0x69, 0x51, 0x00, 0x00, 0x00,  //9
	0x00, 
};

static const unsigned char shape_space[] =
{
	0x00,	
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	 //space
};

#endif


#include "memory_mapped_graphics.h"

void INIT_GRAPHICS(void)
{
    setvideobase(BASE_ADDR);
    
    #if defined(__TMC600__)
    	textcolor(5);
    #endif
    
    #if defined(__COMX__) || defined(__PECOM__)
        redefine_char(player_down, 3);
        redefine_char(player_up, 3);
        redefine_char(player_right, 3);
        redefine_char(player_left, 3);
        redefine_char(ghost, 3);
        redefine_char(bomb, 0);
        redefine_char(invincibility_udg, 3);

        redefine_char(skull_udg, 2);
        redefine_char(power_udg, 2);
        redefine_char(gun_udg, 1);
        redefine_char(bullet_udg, 3);
        redefine_char(dead_ghost_udg, 0);

        redefine_char(rocket_udg, 3);
        redefine_char(left_missile_udg, 3);
        redefine_char(right_missile_udg, 3);
        redefine_char(vertical_brick_udg, 2);
        redefine_char(horizontal_brick_udg, 2);
        redefine_char(freeze_udg, 3);
        redefine_char(super_udg, 0);
    #endif
    #if defined(__COMX__) || defined(__PECOM__) || defined(__CIDELSA__)
        disableinterrupt();
    
        (void) bgcolor(0);

    	textcolordefinition(3);
        monochrome(0);
    #endif
    
    #if defined(__PECOM__)
        shapecolor(36, 22, 2); // to reshape the numbers
        shapecolor(65, 26, 3); // to reshape the capitals
    #endif
    #ifdef __CIDELSA__
        shapechar(shape_space, 0x0001, 0x0);
        shapechar(shape_space, 0x2001, 0x0);
        shapechar(shape_numbers, 0x300A, 0x40);
        shapechar(shape_alpha, 0x411A, 0x0);
        shapechar(shape_space, 0x8001, 0x0);
        shapechar(shape_space, 0xA001, 0x0);
        shapechar(shape_numbers, 0xB00A, 0x80);
        shapechar(shape_alpha, 0xC11A, 0xa0);
    #endif
}

