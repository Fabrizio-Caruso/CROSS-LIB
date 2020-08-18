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

#include "key_pressed.h"

extern Image WALL_1_IMAGE;
extern Image WALL_2_IMAGE;

extern Image TWO_WINDOW_WALL_1_IMAGE;
extern Image TWO_WINDOW_WALL_2_IMAGE;

extern Image THREE_WINDOW_WALL_1_IMAGE;
extern Image THREE_WINDOW_WALL_2_IMAGE;

extern Image SMALL_TWO_WINDOW_WALL_1_IMAGE;
extern Image SMALL_TWO_WINDOW_WALL_2_IMAGE;

extern Image PLANE_BACK_IMAGE;
extern Image PLANE_FRONT_IMAGE;

extern Image BOMB_IMAGE;

#define BUILDINGS_NUMBER (XSize-9)
#define FIRST_BULDING_X_POS 5

uint16_t building_height[XSize];

Image *image[] = {&WALL_1_IMAGE, &WALL_2_IMAGE, &TWO_WINDOW_WALL_1_IMAGE, &TWO_WINDOW_WALL_2_IMAGE, &THREE_WINDOW_WALL_1_IMAGE, &THREE_WINDOW_WALL_2_IMAGE, &SMALL_TWO_WINDOW_WALL_1_IMAGE, &SMALL_TWO_WINDOW_WALL_2_IMAGE};

uint8_t i;
uint8_t j;


void deletePlane(void)
{
    _XLIB_DELETE(j,i);
    _XLIB_DELETE(j+1,i);
}


void drawPlane(void)
{
    _XLIB_DRAW(j,i,&PLANE_BACK_IMAGE);
    _XLIB_DRAW(j+1,i,&PLANE_FRONT_IMAGE);
}


int main(void)
{        
    
    Image *buildingTypePtr;

    INIT_GRAPHICS();

    INIT_INPUT();

    
    while(1)
    {

        INIT_IMAGES();
        
        CLEAR_SCREEN();
        
        SET_TEXT_COLOR(COLOR_WHITE);
        #if !defined(ONLY_SMALL_LETTERS)
            PRINT(4,0,"PRESS FIRE");
        #else
            PRINT(4,0,"press fire");
        #endif
        WAIT_PRESS();
        
        for(i=FIRST_BULDING_X_POS;i<FIRST_BULDING_X_POS+BUILDINGS_NUMBER;++i)
        {
            building_height[i] = (uint8_t) 4+(RAND()&15);
            buildingTypePtr=image[RAND()&7];
            
            for(j=0;j<building_height[i];++j)
            {
                _XLIB_DRAW(i,YSize-1-j,buildingTypePtr);
                TOCK_SOUND();
            }
            PING_SOUND();
        }
        SLEEP(1);
        i = 2;
        j = FIRST_BULDING_X_POS;
        
        while((i<YSize-building_height[j]))
        {
            gotoxy(0,0);cprintf("%d %d", j,i);
            drawPlane();
            DO_SLOW_DOWN(5000);
            

            deletePlane();
            if(j<FIRST_BULDING_X_POS+BUILDINGS_NUMBER)
            {
                ++j;
            }
            else
            {
                j=FIRST_BULDING_X_POS;
                ++i;
            }
            
            if(KEY_PRESSED())
            {
                gotoxy(0,1);cprintf("fire!");sleep(1);gotoxy(0,1);cprintf("     ");
            }
            
        }
        // for(j=0;j<XSize/2-1;++j)
        // {
            // _XLIB_DRAW(j,2,&PLANE_BACK_IMAGE);
            // _XLIB_DRAW(j+1,2,&PLANE_FRONT_IMAGE);
            // SLEEP(1);
            // _XLIB_DELETE(j,2);
            // _XLIB_DELETE(j+1,2);
        // }
        // i=3;
        // for(j=XSize/2;j<XSize-2;++j)
        // {
            // _XLIB_DRAW(j,2,&PLANE_BACK_IMAGE);
            // _XLIB_DRAW(j+1,2,&PLANE_FRONT_IMAGE);
            // _XLIB_DRAW(XSize/2,i,&BOMB_IMAGE);
            // SLEEP(1);
            // _XLIB_DELETE(j,2);
            // _XLIB_DELETE(j+1,2);
            // _XLIB_DELETE(XSize/2,i);
            // ++i;
        // }
        // WAIT_PRESS();
    } // while(1) -> restart from the beginning

    return EXIT_SUCCESS;
}

