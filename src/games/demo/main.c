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
#define DEBUG_ITEMS


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


int main(void)
{        
    uint8_t i;
    
    INIT_GRAPHICS();

    INIT_INPUT();

    
    while(1)
    {

        INIT_IMAGES();
        
        CLEAR_SCREEN();
        
        SET_TEXT_COLOR(COLOR_WHITE);
        // This should be handled throw generic CAPITAL_CASE_ONLY, SMALL_CASE_ONLY, ALL_CASES macros
        #if defined(__LCC1802__)
            PRINT(4,0,"PRESS FIRE");
        #else
            PRINT(4,0,"press fire");
        #endif
        
        for(i=1;i<XSize-1;++i)
        {
            _XLIB_DRAW(i, 0,&PLAYER_DOWN_IMAGE);
            _XLIB_DRAW(i, 1,&PLAYER_UP_IMAGE);
            _XLIB_DRAW(i, 2,&PLAYER_RIGHT_IMAGE);
            _XLIB_DRAW(i, 3,&PLAYER_LEFT_IMAGE);
            _XLIB_DRAW(i, 4,&GHOST_IMAGE);
            _XLIB_DRAW(i, 5,&BOMB_IMAGE);
            _XLIB_DRAW(i, 6,&DEAD_GHOST_IMAGE);
            _XLIB_DRAW(i, 7,&SKULL_IMAGE);
            _XLIB_DRAW(i, 8,&CALM_DOWN_IMAGE);
            _XLIB_DRAW(i, 9,&FIRE_POWER_IMAGE);
            _XLIB_DRAW(i,10,&EXTRA_POINTS_IMAGE);
            _XLIB_DRAW(i,11,&BULLET_IMAGE);
            _XLIB_DRAW(i,12,&VERTICAL_BRICK_IMAGE);
            _XLIB_DRAW(i,13,&HORIZONTAL_BRICK_IMAGE);
            _XLIB_DRAW(i,14,&LEFT_HORIZONTAL_MISSILE_IMAGE);
            _XLIB_DRAW(i,15,&RIGHT_HORIZONTAL_MISSILE_IMAGE);
            #if YSize>16
                _XLIB_DRAW(i,16,&ROCKET_IMAGE);
                _XLIB_DRAW(i,17,&FREEZE_IMAGE);
                _XLIB_DRAW(i,18,&SUPER_IMAGE);
                _XLIB_DRAW(i,19,&EXTRA_LIFE_IMAGE);
                _XLIB_DRAW(i,20,&INVINCIBILITY_IMAGE);
                #if YSize>21
                    _XLIB_DRAW(i,21,&CONFUSE_IMAGE);
                    _XLIB_DRAW(i,22,&SUICIDE_IMAGE);
                    #if YSize>23
                        _XLIB_DRAW(i,23,&BROKEN_BRICK_IMAGE);
                    #endif
                #endif
            #endif
            WAIT_PRESS();
            #if defined(SOUNDS)
                EXPLOSION_SOUND();
            #endif
        }
        
    } // while(1) -> restart from the beginning

    return EXIT_SUCCESS;
}

