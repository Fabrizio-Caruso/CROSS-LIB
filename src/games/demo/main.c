/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS SHOOT by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 


#if !defined EXIT_SUCCESS
    #define EXIT_SUCCESS 0
#endif

// TODO: REMOVE THIS
// #define DEBUG_STRATEGY
#define DEBUG_ITEMS
// #define DEBUG_END


// #include "images.h"
#include "cross_lib.h"

#include "init_images.h"

#if defined(DEBUG_ITEMS)
    extern Image PLAYER_DOWN_IMAGE;
    extern Image PLAYER_UP_IMAGE;
    extern Image PLAYER_RIGHT_IMAGE;
    extern Image PLAYER_LEFT_IMAGE;

    extern Image GHOST_IMAGE;
    extern Image BOMB_IMAGE;
    extern Image DEAD_GHOST_IMAGE;
    extern Image SKULL_IMAGE;
    extern Image CALM_DOWN_IMAGE;
    extern Image FIRE_POWER_IMAGE;
    extern Image EXTRA_POINTS_IMAGE;
    extern Image BULLET_IMAGE;
    extern Image VERTICAL_BRICK_IMAGE;
    extern Image HORIZONTAL_BRICK_IMAGE;
    extern Image LEFT_HORIZONTAL_MISSILE_IMAGE;
    extern Image RIGHT_HORIZONTAL_MISSILE_IMAGE;
    extern Image ROCKET_IMAGE;
    extern Image FREEZE_IMAGE;
    extern Image SUPER_IMAGE;
    extern Image EXTRA_LIFE_IMAGE;
    extern Image INVINCIBILITY_IMAGE;
    extern Image CONFUSE_IMAGE;
    extern Image SUICIDE_IMAGE;
    extern Image BROKEN_BRICK_IMAGE;
#endif


#if defined(Z88DK_JOYSTICK)
	uint8_t stick;
#endif


int main(void)
{        
    INIT_GRAPHICS();

    INIT_INPUT();

    
    while(1)
    {

        INIT_IMAGES();
        
        CLEAR_SCREEN();
        
        #if defined(DEBUG_ITEMS)
            _draw_stat(1, 0,&PLAYER_DOWN_IMAGE);
            _draw_stat(1, 1,&PLAYER_UP_IMAGE);
            _draw_stat(1, 2,&PLAYER_RIGHT_IMAGE);
            _draw_stat(1, 3,&PLAYER_LEFT_IMAGE);

            
            _draw_stat(1, 4,&GHOST_IMAGE);
            _draw_stat(1, 5,&BOMB_IMAGE);
            
            _draw_stat(1, 6,&DEAD_GHOST_IMAGE);
            _draw_stat(1, 7,&SKULL_IMAGE);
            _draw_stat(1, 8,&CALM_DOWN_IMAGE);
            _draw_stat(1, 9,&FIRE_POWER_IMAGE);
            _draw_stat(1,10,&EXTRA_POINTS_IMAGE);
            _draw_stat(1,11,&BULLET_IMAGE);
            _draw_stat(1,12,&VERTICAL_BRICK_IMAGE);
            _draw_stat(1,13,&HORIZONTAL_BRICK_IMAGE);
            _draw_stat(1,14,&LEFT_HORIZONTAL_MISSILE_IMAGE);
            _draw_stat(1,15,&RIGHT_HORIZONTAL_MISSILE_IMAGE);
            _draw_stat(1,16,&ROCKET_IMAGE);
            _draw_stat(1,17,&FREEZE_IMAGE);
            _draw_stat(1,18,&SUPER_IMAGE);
            _draw_stat(1,19,&EXTRA_LIFE_IMAGE);
            _draw_stat(1,20,&INVINCIBILITY_IMAGE);
            _draw_stat(1,21,&CONFUSE_IMAGE);
            _draw_stat(1,22,&SUICIDE_IMAGE);
            _draw_stat(1,23,&BROKEN_BRICK_IMAGE);
            while(1){};
        #endif
        
    } // while(1) -> restart from the beginning

    return EXIT_SUCCESS;
}

