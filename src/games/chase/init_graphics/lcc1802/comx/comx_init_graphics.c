#include <devkit/video/vis_video.h>
#include <devkit/system/system.h>

#include "6x8_chars.h"
#include "standard_libs.h"

#if defined(__CIDELSA__)
    #include <devkit/video/cidelsa_char.h>
#endif


#if defined(__COMX__) || defined(__PECOM__) || defined(REDEFINED_CHARS)
    void redefine_char(uint8_t ch, const uint8_t * shapelocation, int color)
    {
        uint8_t colored_shape[9];
        uint8_t i;
        
        for(i=0;i<9;++i)
        {
           colored_shape[i]=shapelocation[i]+color*64;
        }
        shapechar(colored_shape, ch, 1);
    }


    const uint8_t invincibility_udg[9] =  _INVINCIBILITY_UDG;
    const uint8_t player_down[9] = _PLAYER_DOWN_UDG;
    const uint8_t player_up[9] = _PLAYER_UP_UDG;
    const uint8_t player_right[9] = _PLAYER_LEFT_UDG;
    const uint8_t player_left[9] = _PLAYER_RIGHT_UDG;
    const uint8_t bomb[9] =  _BOMB_UDG;
    const uint8_t ghost[9] = _GHOST_UDG;
    const uint8_t skull_udg[9] = _SKULL_UDG;
    const uint8_t power_udg[9] = _POWERUP_UDG;
    const uint8_t gun_udg[9] =  _GUN_UDG;
    const uint8_t bullet_udg[9] =  _BULLET_UDG;
    const uint8_t dead_ghost_udg[9] = _DEAD_GHOST_UDG;
    const uint8_t rocket_udg[9] = _ROCKET_UDG;
    const uint8_t left_missile_udg[9] = _LEFT_HORIZONTAL_MISSILE_UDG;
    const uint8_t right_missile_udg[9] =  _RIGHT_HORIZONTAL_MISSILE_UDG;
    const uint8_t vertical_brick_udg[9] =  _VERTICAL_BRICK_UDG;
    const uint8_t horizontal_brick_udg[9] = _HORIZONTAL_BRICK_UDG;
    const uint8_t freeze_udg[9] = _POWERUP_UDG;
    const uint8_t super_udg[9] = _POWERUP_UDG;


#endif

#include "memory_mapped_graphics.h"

void INIT_GRAPHICS(void)
{
    initvideo();
    setvideobase(BASE_ADDR);
    
    #if defined(__TMC600__)
    	textcolor(5);
    #endif
    
    #if defined(__COMX__) || defined(__PECOM__) || defined(REDEFINED_CHARS) 
        redefine_char(106, invincibility_udg, 3);
        redefine_char(107, player_down, 3);
        redefine_char(108, player_up, 3);
        redefine_char(109, player_right, 3);
        redefine_char(110, player_left, 3);
        redefine_char(111, bomb, 0);
        redefine_char(112, ghost, 3);

        redefine_char(113, skull_udg, 2);
        redefine_char(114, power_udg, 2);
        redefine_char(115, gun_udg, 1);
        redefine_char(116, bullet_udg, 3);
        redefine_char(117, dead_ghost_udg, 0);

        redefine_char(118, rocket_udg, 3);
        redefine_char(119, left_missile_udg, 3);
        redefine_char(120, right_missile_udg, 3);
        redefine_char(121, vertical_brick_udg, 2);
        redefine_char(122, horizontal_brick_udg, 2);
        redefine_char(123, freeze_udg, 3);
        redefine_char(124, super_udg, 0);
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
        character_set(4);
    #endif
}

#if defined(__CIDELSA__)
#include <devkit/video/cidelsa_char.c>
#endif

