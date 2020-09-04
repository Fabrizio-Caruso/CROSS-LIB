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

# if SLOW_DOWN<100
    #define LEVEL_SPEED_UP 0
#elif SLOW_DOWN<200
    #define LEVEL_SPEED_UP 8
#elif SLOW_DOWN<400
    #define LEVEL_SPEED_UP 16
#elif SLOW_DOWN<800
    #define LEVEL_SPEED_UP 32
#elif SLOW_DOWN<1600
    #define LEVEL_SPEED_UP 64
#elif SLOW_DOWN<3200
    #define LEVEL_SPEED_UP 128
#else
    #define LEVEL_SPEED_UP 256
#endif 

#define MAX_Y ((YSize)+(Y_OFFSET))

#define MIN_BUILDING_HEIGHT 2

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

extern Image ANIMATED_PLANE_BACK_IMAGE;
extern Image ANIMATED_PLANE_CENTER_IMAGE;
extern Image ANIMATED_PLANE_FRONT_IMAGE;

extern Image BOMB_IMAGE;
extern Image ANIMATED_BOMB_UP_IMAGE;
extern Image ANIMATED_BOMB_DOWN_IMAGE;

extern Image ROAD_IMAGE;


extern Image SCORE_TEXT_LEFT_IMAGE;
extern Image SCORE_TEXT_RIGHT_IMAGE;

extern Image HI_TEXT_IMAGE;
extern Image LV_TEXT_IMAGE;

extern Image EXPLOSION_IMAGE;

#if XSize>63
    #define BUILDINGS_NUMBER (XSize-24)
    #define FIRST_BULDING_X_POS 12
#elif XSize>48
    #define BUILDINGS_NUMBER (XSize-14)
    #define FIRST_BULDING_X_POS 8
#elif XSize>32
    #define BUILDINGS_NUMBER (XSize-10)
    #define FIRST_BULDING_X_POS 5
#elif XSize>27
    #define BUILDINGS_NUMBER (XSize-8)
    #define FIRST_BULDING_X_POS 4
#else
    #define BUILDINGS_NUMBER (XSize-7)
    #define FIRST_BULDING_X_POS 4
#endif


#define drawPlane() \
{ \
    _XLIB_DRAW(x,y,&PLANE_BACK_IMAGE); \
    _XLIB_DRAW(x+1,y,&PLANE_FRONT_IMAGE); \
}



#define drawPlaneBack() \
    _XLIB_DRAW(x,y,&PLANE_BACK_IMAGE);

#define deletePlaneBack() \
    _XLIB_DELETE(x,y);

#define deletePlaneFront() \
    _XLIB_DELETE(x+1,y);

#define deletePlane() \
{ \
    deletePlaneBack() ; \
    deletePlaneFront() ; \
}    


#define drawAnimatedPlane() \
{ \
    _XLIB_DRAW(x-1,y,&ANIMATED_PLANE_BACK_IMAGE); \
    _XLIB_DRAW(x,y,&ANIMATED_PLANE_CENTER_IMAGE); \
    _XLIB_DRAW(x+1,y,&ANIMATED_PLANE_FRONT_IMAGE); \
}

#define deleteAnimatedPlaneBack() \
    _XLIB_DELETE(x-1,y); 

#define drawRoad() \
    _XLIB_DRAW(x,MAX_Y-1,&ROAD_IMAGE);

#define drawBuilding() \
    _XLIB_DRAW(x,MAX_Y-1-y,buildingTypePtr);

#define drawBomb() \
    _XLIB_DRAW(bomb_x,bomb_y,&BOMB_IMAGE);

#define drawAnimatedBomb() \
{ \
    _XLIB_DRAW(bomb_x,bomb_y,&ANIMATED_BOMB_UP_IMAGE); \
    _XLIB_DRAW(bomb_x,bomb_y+1,&ANIMATED_BOMB_DOWN_IMAGE); \
}

#define deleteAnimatedBomb() \
{ \
    _XLIB_DELETE(bomb_x,bomb_y-1); \
    _XLIB_DELETE(bomb_x,bomb_y); \
}

#define deleteAnimatedBombUp() \
{ \
    _XLIB_DELETE(bomb_x,bomb_y-1); \
}

#define drawExplosion() \
    _XLIB_DRAW(bomb_x,bomb_y,&EXPLOSION_IMAGE);

#define deleteExplosion() \
    _XLIB_DELETE(bomb_x,bomb_y);

#define displayScore() \
{ \
    SET_TEXT_COLOR(COLOR_WHITE); \
    PRINTD(2,0,5,score); \
}

#define displayRemainingBuilings() \
{ \
    SET_TEXT_COLOR(COLOR_WHITE); \
    PRINTD(9,0,2,remaining_buildings); \
}

#define displayHiScore() \
{ \
    SET_TEXT_COLOR(COLOR_WHITE); \
    PRINTD(XSize-5,0,5,hiscore); \
}


#define displayLevel() \
{ \
    SET_TEXT_COLOR(COLOR_WHITE); \
    PRINTD(XSize-9,0,2,level); \
}

#define displayLevelMessage() \
{ \
    SET_TEXT_COLOR(COLOR_WHITE); \
    PRINTD(XSize/2+2,4,2,level); \
}


uint16_t building_height[XSize];

Image *image[] = {&WALL_1_IMAGE, &WALL_2_IMAGE, &TWO_WINDOW_WALL_1_IMAGE, &TWO_WINDOW_WALL_2_IMAGE, &THREE_WINDOW_WALL_1_IMAGE, &THREE_WINDOW_WALL_2_IMAGE, &SMALL_TWO_WINDOW_WALL_1_IMAGE, &SMALL_TWO_WINDOW_WALL_2_IMAGE};


uint8_t x;
uint8_t y;

uint8_t bombActive;
uint8_t bomb_x;
uint8_t bomb_y;

uint8_t level;

uint16_t score;
uint16_t bonus;
uint16_t bonus_ind;
uint16_t hiscore;

uint8_t remaining_buildings;

uint8_t alive;
uint8_t explosion;



int main(void)
{        
    
    Image *buildingTypePtr;

    INIT_GRAPHICS();

    INIT_INPUT();    
    hiscore = 0;


    while(1)
    {
        alive = 1;
        score = 0;
        level = 1;
        explosion = 0;

        INIT_IMAGES();
        CLEAR_SCREEN();
            
        SET_TEXT_COLOR(COLOR_RED);
        printCenteredMessageOnRow(2, _XL_C _XL_R _XL_O _XL_S _XL_S _XL_SPACE _XL_B _XL_O _XL_M _XL_B _XL_E _XL_R);
        SET_TEXT_COLOR(COLOR_CYAN);
        printCenteredMessageOnRow(4, _XL_B _XL_Y _XL_SPACE _XL_F _XL_A _XL_B _XL_R _XL_I _XL_Z _XL_I _XL_O _XL_SPACE _XL_C _XL_A _XL_R _XL_U _XL_S _XL_O);
        SET_TEXT_COLOR(COLOR_WHITE);
        printCenteredMessageOnRow(8, _XL_D _XL_E _XL_S _XL_T _XL_R _XL_O _XL_Y _XL_SPACE _XL_B _XL_U _XL_I _XL_L _XL_D _XL_I _XL_N _XL_G _XL_S);
        SET_TEXT_COLOR(COLOR_WHITE);
        printCenteredMessageOnRow(MAX_Y-5, _XL_P _XL_R _XL_E _XL_S _XL_S _XL_SPACE _XL_F _XL_I _XL_R _XL_E);
        WAIT_PRESS();
        while(alive)
        {
            bombActive = 0;
            bomb_x = 0;
            bomb_y = MAX_Y-2;
            bonus = 0;
            remaining_buildings = BUILDINGS_NUMBER;
            
            CLEAR_SCREEN();
            PRINT(XSize/2-4, 4, _XL_L _XL_E _XL_V _XL_E _XL_L);
            displayLevelMessage();
            SLEEP(1);
            WAIT_PRESS();
            CLEAR_SCREEN();
            
            for(x=0;x<XSize;++x)
            {
                drawRoad();
            }
            for(x=0;x<XSize-2;++x)
            {
                building_height[x] = 0;
            }
            for(x=FIRST_BULDING_X_POS;x<FIRST_BULDING_X_POS+BUILDINGS_NUMBER;++x)
            {
                building_height[x] = (uint8_t) MIN_BUILDING_HEIGHT+level/2+(RAND()&7);
                buildingTypePtr=image[RAND()&7];
                
                for(y=1;y<building_height[x];++y)
                {
                    drawBuilding();
                    TOCK_SOUND();
                }
                PING_SOUND();
            }

            SLEEP(1);
            y = 1;
            x = 1;
            
            displayScore();
            
            _XLIB_DRAW(0,0,&SCORE_TEXT_LEFT_IMAGE);
            _XLIB_DRAW(1,0,&SCORE_TEXT_RIGHT_IMAGE);

            _XLIB_DRAW(XSize-6,0,&HI_TEXT_IMAGE);
            #if XSize>=20
                _XLIB_DRAW(XSize-10,0,&LV_TEXT_IMAGE);
            #endif
            #if XSize>=22
                _XLIB_DRAW(8,0,&TWO_WINDOW_WALL_2_IMAGE);
                displayRemainingBuilings();
            #endif
            
            displayLevel();
            
            displayHiScore();
            while((y<MAX_Y-building_height[x+1]) && (y<MAX_Y-2 || x<XSize-3))
            {

                // Land safely
                if(!remaining_buildings && (y<MAX_Y-2) && (x<XSize-3) )
                {
                    deleteAnimatedPlaneBack();
                    ++y;
                }
                drawAnimatedPlane();
                DO_SLOW_DOWN(SLOW_DOWN/2-level*LEVEL_SPEED_UP);
                

                if(!bombActive)
                {   
                    if(explosion)
                    {
                        deleteExplosion();
                        explosion = 0;
                    }
                    
                    if(KEY_PRESSED())
                    {
                        SHOOT_SOUND();
                        ++bombActive;
                        bomb_x = x;
                        bomb_y = y;
                        if(building_height[x])
                        {
                            building_height[x] = 0;
                            score+=10;
                            --remaining_buildings;
                            if(!remaining_buildings)
                            {
                                bonus = 20*(MAX_Y-y)+level*30;
                            }
                            displayScore();
                        }
                    }
                }


                if(bombActive)
                {
                    // Draw animated bomb
                    drawAnimatedBomb();
                    
                    ++bomb_y;
                
                    if(bomb_y>MAX_Y-3) // Bomb reaches the ground
                    {
                        drawBomb();
                        bombActive = 0;
                        explosion = 1;
                        
                        
                        #if XSize>=22
                            displayRemainingBuilings();
                        #endif
                        // Delete animated bomb
                        deleteAnimatedBombUp();
                        drawExplosion();
                        DO_SLOW_DOWN(10+SLOW_DOWN/8);
                    }

                }
                deleteAnimatedPlaneBack();
                drawPlane();
                
                DO_SLOW_DOWN(SLOW_DOWN/2-level*LEVEL_SPEED_UP);

                if(bombActive)
                {
                    drawBomb();
                    deleteAnimatedBombUp();
                }

                
                if(x<XSize-3)
                {
                    ++x;
                }
                else if(y<MAX_Y-2)
                {
                    deletePlane();
                    x=1;
                    ++y;
                }
                
                deletePlaneBack();
                deleteAnimatedPlaneBack();
            } // while flying
            if(!remaining_buildings)
            {
                drawPlane();
                SET_TEXT_COLOR(COLOR_YELLOW);
                PRINT(1,2,_XL_L _XL_E _XL_V _XL_E _XL_L _XL_SPACE _XL_C _XL_O _XL_M _XL_P _XL_L _XL_E _XL_T _XL_E _XL_D);
                SLEEP(1);
                ++level;
                score+=bonus;
                SET_TEXT_COLOR(COLOR_WHITE);
                PRINT(1,4,_XL_B _XL_O _XL_N _XL_U _XL_S);
                for(bonus_ind=10;bonus_ind<=bonus;bonus_ind+=10)
                {
                    PRINTD(7,4,5,bonus_ind);
                    SHOOT_SOUND();
                    DO_SLOW_DOWN(SLOW_DOWN);
                    DO_SLOW_DOWN(SLOW_DOWN);
                }
                displayScore();
                SLEEP(1);
            }
            else
            {
                drawPlaneBack();
                EXPLOSION_SOUND();
                SET_TEXT_COLOR(COLOR_RED);
                PRINT(1,2,_XL_G _XL_A _XL_M _XL_E _XL_SPACE _XL_O _XL_V _XL_E _XL_R);
                SLEEP(2);
                alive = 0;
                if(score>hiscore)
                {
                    hiscore = score;
                    SET_TEXT_COLOR(COLOR_CYAN);
                    PRINT(1,4,_XL_N _XL_E _XL_W _XL_SPACE _XL_H _XL_I _XL_S _XL_C _XL_O _XL_R _XL_E);
                    SLEEP(1);
                }
                score = 0;
            }
            WAIT_PRESS();
        } // while(alive) -> restart level
    } // while(1) -> restart from level 1

    return EXIT_SUCCESS;
}

