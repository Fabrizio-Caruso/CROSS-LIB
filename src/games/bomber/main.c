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


uint8_t x;
uint8_t y;

uint8_t bombActive;
uint8_t bomb_x;
uint8_t bomb_y;


void deletePlane(void)
{
    _XLIB_DELETE(x,y);
    _XLIB_DELETE(x+1,y);
}


void drawPlane(void)
{
    _XLIB_DRAW(x,y,&PLANE_BACK_IMAGE);
    _XLIB_DRAW(x+1,y,&PLANE_FRONT_IMAGE);
}


int main(void)
{        
    
    Image *buildingTypePtr;

    INIT_GRAPHICS();

    INIT_INPUT();


    
    while(1)
    {
        bombActive = 0;
        bomb_x = 0;
        bomb_y = 0;
        
        INIT_IMAGES();
        
        CLEAR_SCREEN();
        
        SET_TEXT_COLOR(COLOR_WHITE);
        #if !defined(ONLY_SMALL_LETTERS)
            PRINT(4,0,"PRESS FIRE");
        #else
            PRINT(4,0,"press fire");
        #endif
        WAIT_PRESS();
        CLEAR_SCREEN();
        
        for(y=FIRST_BULDING_X_POS;y<FIRST_BULDING_X_POS+BUILDINGS_NUMBER;++y)
        {
            building_height[y] = (uint8_t) 3+(RAND()&7);
            buildingTypePtr=image[RAND()&7];
            
            for(x=0;x<building_height[y];++x)
            {
                _XLIB_DRAW(y,YSize-1-x,buildingTypePtr);
                TOCK_SOUND();
            }
            PING_SOUND();
        }
        SLEEP(1);
        y = 2;
        x = FIRST_BULDING_X_POS-4;
        
        while((y<YSize-building_height[x+1]) && y<YSize-1)
        {
            // gotoxy(0,0);cprintf("%d %d", x,y);
            drawPlane();
            DO_SLOW_DOWN(7000);
            

            deletePlane();
            if(x<FIRST_BULDING_X_POS+BUILDINGS_NUMBER+2)
            {
                ++x;
            }
            else
            {
                x=FIRST_BULDING_X_POS-3;
                ++y;
            }
            
            if(!bombActive && KEY_PRESSED())
            {   
                ++bombActive;
                bomb_x = x;
                bomb_y = y;
                building_height[x]=0;
                // gotoxy(0,1);cprintf("fire!");sleep(1);gotoxy(0,1);cprintf("     ");
            }
            
            if(bombActive)
            {
                _XLIB_DELETE(bomb_x,bomb_y);
                ++bomb_y;
                _XLIB_DRAW(bomb_x,bomb_y,&BOMB_IMAGE);
            
                if(bomb_y>YSize-2)
                {
                    bombActive = 0;
                }
            }
            
        }
        drawPlane();
        SLEEP(1);
        if(y==YSize-1)
        {
            CLEAR_SCREEN();
            #if !defined(ONLY_SMALL_LETTERS)
                PRINT(4,0,"YOU WON");
            #else
                PRINT(4,0,"you won");
            #endif
            SLEEP(3);
        }
        WAIT_PRESS();
    } // while(1) -> restart from the beginning

    return EXIT_SUCCESS;
}

