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


static const Image* image[19] = {
		&PLAYER_DOWN_IMAGE, &PLAYER_UP_IMAGE, &PLAYER_RIGHT_IMAGE, &PLAYER_LEFT_IMAGE, 
		&GHOST_IMAGE, &BOMB_IMAGE, &DEAD_GHOST_IMAGE, &SKULL_IMAGE, 
		&CALM_DOWN_IMAGE, &FIRE_POWER_IMAGE, &BULLET_IMAGE, &VERTICAL_BRICK_IMAGE, 
		&HORIZONTAL_BRICK_IMAGE, &LEFT_HORIZONTAL_MISSILE_IMAGE, &RIGHT_HORIZONTAL_MISSILE_IMAGE, &ROCKET_IMAGE, 
		&FREEZE_IMAGE, &SUPER_IMAGE, &INVINCIBILITY_IMAGE
		};

		
int main(void)
{        
    uint8_t i;
    uint8_t j;
    uint8_t k;
    uint8_t max_j;
	
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
        
		while(1)
		{
			i = RAND() % (XSize-1); 
			max_j = RAND() & (YSize-1);
			for(j=0;j<max_j;++j)
			{
				_XLIB_DRAW(i,j,image[j]);
				for(k=0;k<254;++k)
				{
				}
			}
        }
    } // while(1) -> restart from the beginning

    return EXIT_SUCCESS;
}

