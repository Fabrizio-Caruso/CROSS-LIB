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


extern Image SPACE_SHIP_1_W_IMAGE;
extern Image SPACE_SHIP_1_E_IMAGE;

extern Image SPACE_SHIP_2_W_IMAGE;
extern Image SPACE_SHIP_2_E_IMAGE;

extern Image SPACE_SHIP_3_W_IMAGE;
extern Image SPACE_SHIP_3_C_IMAGE;
extern Image SPACE_SHIP_3_E_IMAGE;

extern Image SPACE_SHIP_4_W_IMAGE;
extern Image SPACE_SHIP_4_E_IMAGE;

extern Image MID_INVADER_CLOSED_W_IMAGE;
extern Image MID_INVADER_CLOSED_E_IMAGE;

extern Image MID_INVADER_OPEN_W_IMAGE;
extern Image MID_INVADER_OPEN_E_IMAGE;

extern Image LOW_INVADER_CLOSED_W_IMAGE;
extern Image LOW_INVADER_CLOSED_E_IMAGE;

extern Image LOW_INVADER_OPEN_W_IMAGE;
extern Image LOW_INVADER_OPEN_E_IMAGE;

extern Image TOP_INVADER_OPEN_W_IMAGE;
extern Image TOP_INVADER_OPEN_E_IMAGE;

extern Image TOP_INVADER_CLOSED_IMAGE;

uint8_t x;

uint8_t mid_invader_x;

uint8_t ship_x;

uint8_t ship_fire;


#define SPACE_SHIP_Y (MAX_Y-4)
#define TOP_INVADER_Y (MAX_Y-14)
#define MID_INVADER_Y (MAX_Y-12)
#define LOW_INVADER_Y (MAX_Y-10)

void draw_ship_1(void) 
{
    _XLIB_DRAW(x,SPACE_SHIP_Y,&SPACE_SHIP_1_W_IMAGE);
    _XLIB_DRAW(x+1,SPACE_SHIP_Y,&SPACE_SHIP_1_E_IMAGE);
}


void draw_ship_2(void)
{
    _XLIB_DRAW(x,SPACE_SHIP_Y,&SPACE_SHIP_2_W_IMAGE);
    _XLIB_DRAW(x+1,SPACE_SHIP_Y,&SPACE_SHIP_2_E_IMAGE);
}

void draw_ship_3(void) 
{
    _XLIB_DRAW(x,SPACE_SHIP_Y,&SPACE_SHIP_3_W_IMAGE);
    _XLIB_DRAW(x+1,SPACE_SHIP_Y,&SPACE_SHIP_3_C_IMAGE);
    _XLIB_DRAW(x+2,SPACE_SHIP_Y,&SPACE_SHIP_3_E_IMAGE);
}


void draw_ship_4(void)
{
    _XLIB_DRAW(x,SPACE_SHIP_Y,&SPACE_SHIP_4_W_IMAGE);
    _XLIB_DRAW(x+1,SPACE_SHIP_Y,&SPACE_SHIP_4_E_IMAGE);
}


void delete_ship(void)
{
    _XLIB_DELETE(x,SPACE_SHIP_Y);
    _XLIB_DELETE(x+1,SPACE_SHIP_Y);
    _XLIB_DELETE(x+2,SPACE_SHIP_Y);
    
}


void draw_mid_invader_closed(void)
{
    _XLIB_DRAW(mid_invader_x,MID_INVADER_Y,&MID_INVADER_CLOSED_W_IMAGE);
    _XLIB_DRAW(mid_invader_x+1,MID_INVADER_Y,&MID_INVADER_CLOSED_E_IMAGE);
}

void draw_mid_invader_open(void)
{
    _XLIB_DRAW(mid_invader_x,MID_INVADER_Y,&MID_INVADER_OPEN_W_IMAGE);
    _XLIB_DRAW(mid_invader_x+1,MID_INVADER_Y,&MID_INVADER_OPEN_E_IMAGE);
}


void delete_mid_invader(void)
{
    _XLIB_DELETE(mid_invader_x,MID_INVADER_Y);
    _XLIB_DELETE(mid_invader_x+1,MID_INVADER_Y);
}


void draw_low_invader_open(void)
{
    _XLIB_DRAW(mid_invader_x,LOW_INVADER_Y,&LOW_INVADER_OPEN_W_IMAGE);
    _XLIB_DRAW(mid_invader_x+1,LOW_INVADER_Y,&LOW_INVADER_OPEN_E_IMAGE);
}

void draw_low_invader_closed(void)
{
    _XLIB_DRAW(mid_invader_x,LOW_INVADER_Y,&LOW_INVADER_CLOSED_W_IMAGE);
    _XLIB_DRAW(mid_invader_x+1,LOW_INVADER_Y,&LOW_INVADER_CLOSED_E_IMAGE);
}


void delete_low_invader(void)
{
    _XLIB_DELETE(mid_invader_x,LOW_INVADER_Y);
    _XLIB_DELETE(mid_invader_x+1,LOW_INVADER_Y);
}


void draw_top_invader_closed(void)
{
    _XLIB_DRAW(mid_invader_x,TOP_INVADER_Y,&TOP_INVADER_CLOSED_IMAGE);
}

void draw_top_invader_open(void)
{
    _XLIB_DRAW(mid_invader_x,TOP_INVADER_Y,&TOP_INVADER_OPEN_W_IMAGE);
    _XLIB_DRAW(mid_invader_x+1,TOP_INVADER_Y,&TOP_INVADER_OPEN_E_IMAGE);
}


void delete_top_invader(void)
{
    _XLIB_DELETE(mid_invader_x,TOP_INVADER_Y);
    _XLIB_DELETE(mid_invader_x+1,TOP_INVADER_Y);
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
        mid_invader_x = 0;
        
        while(x<XSize-4)
        {
            WAIT_V_SYNC();
            // WAIT_PRESS();

            delete_ship();
            draw_ship_1();
            
            delete_mid_invader();
            delete_low_invader();
            delete_top_invader();
            
            ++mid_invader_x;
            draw_top_invader_closed();
            draw_mid_invader_closed();
            draw_low_invader_open();
            
            DO_SLOW_DOWN(2000);

            WAIT_V_SYNC();
            // WAIT_PRESS();

            delete_ship();
            draw_ship_2();
            DO_SLOW_DOWN(2000);
            
            WAIT_V_SYNC();
            // WAIT_PRESS();

            delete_ship();
            draw_ship_3();
            
            delete_top_invader();
            delete_mid_invader();
            delete_low_invader();
            
            draw_top_invader_open();
            draw_mid_invader_open();
            draw_low_invader_closed();
            
            DO_SLOW_DOWN(2000);
            
            WAIT_V_SYNC();
            // WAIT_PRESS();

            delete_ship();
            ++x;
            draw_ship_4();
            DO_SLOW_DOWN(2000);

        }
        // WAIT_PRESS();
        delete_ship();
        delete_top_invader();
        delete_mid_invader();
        delete_low_invader();
    }


    return EXIT_SUCCESS;
}

