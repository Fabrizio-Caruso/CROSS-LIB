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
// subxect to the following restrictions:

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

#if !defined(SLOW_DOWN)
    #define SLOW_DOWN 0
#endif

#  if SLOW_DOWN<20
    #define LEVEL_SPEED_UP 0
#elif SLOW_DOWN<100
    #define LEVEL_SPEED_UP 2
#elif SLOW_DOWN<200
    #define LEVEL_SPEED_UP 4
#elif SLOW_DOWN<400
    #define LEVEL_SPEED_UP 8
#elif SLOW_DOWN<800
    #define LEVEL_SPEED_UP 16
#elif SLOW_DOWN<1600
    #define LEVEL_SPEED_UP 32
#elif SLOW_DOWN<3200
    #define LEVEL_SPEED_UP 64
#else
    #define LEVEL_SPEED_UP 128
#endif 

#define MAX_Y ((YSize)+(Y_OFFSET))


extern Image SPACE_SHIP_1_NW_IMAGE;
extern Image SPACE_SHIP_1_NE_IMAGE;
extern Image SPACE_SHIP_1_SW_IMAGE;
extern Image SPACE_SHIP_1_SE_IMAGE;

extern Image SPACE_SHIP_2_N_IMAGE;
extern Image SPACE_SHIP_2_SW_IMAGE;
extern Image SPACE_SHIP_2_S_IMAGE;
extern Image SPACE_SHIP_2_SE_IMAGE;

uint8_t x;

uint8_t ship_x;

uint8_t ship_fire;


#define SPACE_SHIP_Y (MAX_Y-4)

// SQUARE
void draw_ship_1(void) 
{
    _XLIB_DRAW(x,SPACE_SHIP_Y,&SPACE_SHIP_1_NW_IMAGE);
    _XLIB_DRAW(x+1,SPACE_SHIP_Y,&SPACE_SHIP_1_NE_IMAGE);
    _XLIB_DRAW(x,SPACE_SHIP_Y+1,&SPACE_SHIP_1_SW_IMAGE);
    _XLIB_DRAW(x+1,SPACE_SHIP_Y+1,&SPACE_SHIP_1_SE_IMAGE);
}


void delete_ship_1(void)
{
    _XLIB_DELETE(x,SPACE_SHIP_Y);
    _XLIB_DELETE(x+1,SPACE_SHIP_Y);
}

// PYRAMID
void draw_ship_2(void)
{
    _XLIB_DRAW(x+1,SPACE_SHIP_Y,&SPACE_SHIP_2_N_IMAGE);
    _XLIB_DRAW(x,SPACE_SHIP_Y+1,&SPACE_SHIP_2_SW_IMAGE);
    _XLIB_DRAW(x+1,SPACE_SHIP_Y+1,&SPACE_SHIP_2_S_IMAGE);
    _XLIB_DRAW(x+2,SPACE_SHIP_Y+1,&SPACE_SHIP_2_SE_IMAGE);
}

void delete_ship_2(void)
{
    _XLIB_DELETE(x,SPACE_SHIP_Y+1);
    _XLIB_DELETE(x+2,SPACE_SHIP_Y+1);
}

void delete_ship(void)
{
    if(ship_x&1)
    {
        delete_ship_1();
    }
    else
    {
        delete_ship_2();
    }
}

int main(void)
{        

    INIT_GRAPHICS();

    INIT_INPUT();    
    
    INIT_IMAGES();
    
    ship_x = 1;


    CLEAR_SCREEN();

    PRINT(0,SPACE_SHIP_Y-3,"01234567890123456789");

    while(1)
    {
        x = 0;
        while(x<XSize-4)
        {
            WAIT_V_SYNC();

            delete_ship_2();
            ++x;
            draw_ship_1();
            DO_SLOW_DOWN(1500);

            // WAIT_PRESS();
            WAIT_V_SYNC();

            delete_ship_1();
            draw_ship_2();
            // WAIT_PRESS();
            DO_SLOW_DOWN(1500);
        }
        _XLIB_DELETE(x+1,SPACE_SHIP_Y);
        _XLIB_DELETE(x,SPACE_SHIP_Y+1);
        _XLIB_DELETE(x+1,SPACE_SHIP_Y+1);
        _XLIB_DELETE(x+2,SPACE_SHIP_Y+1);
    }


    return EXIT_SUCCESS;
}

