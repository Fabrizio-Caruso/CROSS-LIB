#include <rca_vis_video.h>


void redefine_char(const unsigned char * shapelocation, int color)
{
    unsigned char colored_shape[10];
    unsigned char i;
    
    colored_shape[0] = *shapelocation;
    for(i=1;i<9;++i)
    {
       colored_shape[i]=shapelocation[i]+color*64;
    }
    colored_shape[9]=0;
    shapechar(colored_shape, 1);
}

// COMX and PECOM
void disableinterrupt(){
    asm(
        " sex 3\n"
        " dis\n"
        " db 0x23\n");
}

// COMX and PECOM
void enableinterrupt(){
    asm(
        " sex 3\n"
        " ret\n"
        " db 0x23\n");
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
#define __VERTICAL_BRICK_UDG           12,12,12,12,12,12,12,12
#define __HORIZONTAL_BRICK_UDG          0, 0, 0,63,63,00,00,00

const unsigned char player_down[10] = {  107, __PLAYER_DOWN_UDG, 0 };
const unsigned char player_up[10] = { 108 , __PLAYER_UP_UDG, 0 };
const unsigned char player_right[10] = { 109 , __PLAYER_LEFT_UDG, 0 };
const unsigned char player_left[10] = { 110 , __PLAYER_RIGHT_UDG, 0};

const unsigned char ghost[10] = { 112 , __GHOST_UDG, 0 };
const unsigned char bomb[10] =  { 111 , __BOMB_UDG, 0};
const unsigned char invincibility_udg[10] =  { 106 , __INVINCIBILITY_UDG, 0};

const unsigned char skull_udg[10] = { 113 , __SKULL_UDG, 0 };
const unsigned char power_udg[10] = { 114 , __POWERUP_UDG, 0 };
const unsigned char freeze_udg[10] = { 123 , __POWERUP_UDG, 0 };
const unsigned char super_udg[10] = { 124 , __POWERUP_UDG, 0 };


const unsigned char gun_udg[10] =  { 115 , __GUN_UDG, 0};
const unsigned char bullet_udg[10] =  { 116 , __BULLET_UDG, 0};
const unsigned char dead_ghost_udg[10] = {117, __DEAD_GHOST_UDG, 0};

const unsigned char rocket_udg[10] = { 118 , __ROCKET_UDG, 0 };
const unsigned char left_missile_udg[10] = { 119 , __LEFT_HORIZONTAL_MISSILE_UDG, 0 };
const unsigned char right_missile_udg[10] =  { 120 , __RIGHT_HORIZONTAL_MISSILE_UDG, 0};
const unsigned char vertical_brick_udg[10] =  { 121 , __VERTICAL_BRICK_UDG, 0};
const unsigned char horizontal_brick_udg[10] = {122, __HORIZONTAL_BRICK_UDG, 0};

void INIT_GRAPHICS(void)
{
    setvideobase(0x7800);

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

    #if defined(__COMX__)
        //disableinterrupt();
    #endif
    (void) bgcolor(0);
    #if defined(__COMX__)
    	textcolordefinition(3);
        monochrome(0);
    #elif defined(__PECOM__)
    	textcolordefinition(3);
    #endif

}
