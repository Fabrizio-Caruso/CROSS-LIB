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

#include "cross_lib.h"

#include "init_images.h"

// TODO: REMOVE THIS
#define MAX_INIT_Y_POS ((YSize)+(Y_OFFSET)-19)

#if ((YSize)+(Y_OFFSET)-1)>19
    #define MAX_TILES 19
#else
    #define MAX_TILES ((YSize)+(Y_OFFSET)-1)
#endif


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

#define NUM_OF_TILES 19

static const Image* image[NUM_OF_TILES] = {
		&PLAYER_DOWN_IMAGE, &PLAYER_UP_IMAGE, &PLAYER_RIGHT_IMAGE, &PLAYER_LEFT_IMAGE, 
		&GHOST_IMAGE, &BOMB_IMAGE, &DEAD_GHOST_IMAGE, &SKULL_IMAGE, 
		&CALM_DOWN_IMAGE, &FIRE_POWER_IMAGE, &BULLET_IMAGE, &VERTICAL_BRICK_IMAGE, 
		&HORIZONTAL_BRICK_IMAGE, &LEFT_HORIZONTAL_MISSILE_IMAGE, &RIGHT_HORIZONTAL_MISSILE_IMAGE, &ROCKET_IMAGE, 
		&FREEZE_IMAGE, &SUPER_IMAGE, &INVINCIBILITY_IMAGE, 
        //&EXREA1, &EXTRA2, &EXTRA3, &EXTRA4, &EXTRA5, &EXTRA6
		};

#define COL_OFFSET ((XSize-16)/2-1)
#define ROW_OFFSET 3

int main(void)
{        
    uint8_t i;
	
    INIT_GRAPHICS();

    INIT_INPUT();

    
    INIT_IMAGES();
    
    CLEAR_SCREEN();
    
    SET_TEXT_COLOR(COLOR_WHITE);
    PRINT(COL_OFFSET,0, _XL_P _XL_R _XL_E _XL_S _XL_S _XL_SPACE _XL_F _XL_I _XL_R _XL_E);
    WAIT_PRESS();
    CLEAR_SCREEN();

    for(i=0;i<NUM_OF_TILES;++i)
    {
        _XLIB_DRAW((i&7)*2+COL_OFFSET,(i/8)*2+ROW_OFFSET,image[i]);
        DO_SLOW_DOWN(900);
    }

    
    
    SET_TEXT_COLOR(COLOR_WHITE);
    PRINT(COL_OFFSET,YSize-5, _XL_P _XL_R _XL_E _XL_S _XL_S _XL_SPACE _XL_F _XL_I _XL_R _XL_E);
    WAIT_PRESS();

    return EXIT_SUCCESS;
}

