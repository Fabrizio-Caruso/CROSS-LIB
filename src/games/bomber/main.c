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

#include "images.h"


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

#define MIN_BUILDING_HEIGHT 2


#if XSize>78
    #define BUILDINGS_NUMBER (XSize-24)
    #define FIRST_BULDING_X_POS 12
#elif XSize>63
    #define BUILDINGS_NUMBER (XSize-22)
    #define FIRST_BULDING_X_POS 10
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

// #define BUILDINGS_NUMBER 1
// #define FIRST_BULDING_X_POS 10


// String definitions
#define _CROSS_BOMBER__STRING _XL_C _XL_R _XL_O _XL_S _XL_S _XL_SPACE _XL_B _XL_O _XL_M _XL_B _XL_E _XL_R
#define _BY_FABRIZIO_CARUSO__STRING _XL_B _XL_Y _XL_SPACE _XL_F _XL_A _XL_B _XL_R _XL_I _XL_Z _XL_I _XL_O _XL_SPACE _XL_C _XL_A _XL_R _XL_U _XL_S _XL_O
#define _DESTROY_ALL__STRING _XL_D _XL_E _XL_S _XL_T _XL_R _XL_O _XL_Y _XL_SPACE _XL_A _XL_L _XL_L
#define _BUILDINGS__STRING _XL_B _XL_U _XL_I _XL_L _XL_D _XL_I _XL_N _XL_G _XL_S
#define _PRESS_FIRE__STRING _XL_P _XL_R _XL_E _XL_S _XL_S _XL_SPACE _XL_F _XL_I _XL_R _XL_E
#define _LEVEL__STRING _XL_L _XL_E _XL_V _XL_E _XL_L
#define _LEVEL_COMPLETED__STRING _XL_L _XL_E _XL_V _XL_E _XL_L _XL_SPACE _XL_C _XL_O _XL_M _XL_P _XL_L _XL_E _XL_T _XL_E _XL_D
#define _BONUS__STRING _XL_B _XL_O _XL_N _XL_U _XL_S
#define _GAME_COMPLETED__STRING _XL_G _XL_A _XL_M _XL_E _XL_SPACE _XL_C _XL_O _XL_M _XL_P _XL_E _XL_T _XL_E _XL_D
#define _NEW_HISCORE__STRING _XL_N _XL_E _XL_W _XL_SPACE _XL_H _XL_I _XL_S _XL_C _XL_O _XL_R _XL_E
#define _GAME_OVER__STRING _XL_G _XL_A _XL_M _XL_E _XL_SPACE _XL_O _XL_V _XL_E _XL_R


#define drawPlane() \
do { \
    _XL_DRAW(x,y,PLANE_BACK_TILE, _PLANE_COLOR); \
    _XL_DRAW(x+1,y,PLANE_FRONT_TILE, _PLANE_COLOR); \
} while(0)



#define drawPlaneBack() \
    _XL_DRAW(x,y,PLANE_BACK_TILE,_PLANE_COLOR);

#define deletePlaneBack() \
    _XL_DELETE(x,y);

#define deletePlaneFront() \
    _XL_DELETE(x+1,y);



#define deleteAnimatedPlaneBack() \
    _XL_DELETE(x-1,y); 

#define deleteAnimatedPlaneCenter() \
    deletePlaneBack(); 


#define deleteAnimatedBombUp() \
    _XL_DELETE(bomb_x,bomb_y-1);

#if !defined(NO_ANIMATION)
    #define drawAnimatedPlane() \
        do { \
            _XL_DRAW(x-1,y,ANIMATED_PLANE_BACK_TILE,_PLANE_COLOR); \
            _XL_DRAW(x,y,ANIMATED_PLANE_CENTER_TILE,_PLANE_COLOR); \
            _XL_DRAW(x+1,y,ANIMATED_PLANE_FRONT_TILE, _PLANE_COLOR); \
        } while(0)


        
    #define drawAnimatedBomb() \
    do { \
        _XL_DRAW(bomb_x,bomb_y,ANIMATED_BOMB_UP_TILE, _BOMB_COLOR); \
        _XL_DRAW(bomb_x,bomb_y+1,ANIMATED_BOMB_DOWN_TILE,_BOMB_COLOR); \
    } while(0)

    #define deletePlane() \
    do  {\
        deletePlaneBack() ; \
        deletePlaneFront() ; \
    } while(0)  

    #define deleteAnimatedBomb() \
    do { \
        _XL_DELETE(bomb_x,bomb_y-1); \
        _XL_DELETE(bomb_x,bomb_y); \
    } while(0)
        
#else 
    #define drawAnimatedPlane() 

        
    #define drawAnimatedBomb() 

    #define deletePlane() \
        deletePlaneFront(); 
        
    #define deleteAnimatedBomb() \
        _XL_DELETE(bomb_x,bomb_y-1);
    
#endif

#define drawRoad() \
    _XL_DRAW(x,MAX_Y-1,ROAD_TILE,_ROAD_COLOR);

#define drawBuilding() \
    _XL_DRAW(x,MAX_Y-1-y,buildingType,buildingColor);

#define drawBomb() \
    _XL_DRAW(bomb_x,bomb_y,BOMB_TILE,_BOMB_COLOR);


#define drawExplosion() \
    _XL_DRAW(bomb_x,bomb_y,EXPLOSION_TILE,_XL_RED);

#define deleteExplosion() \
    _XL_DELETE(bomb_x,bomb_y);

#define displayScore() \
do { \
    SET_TEXT_COLOR(_XL_WHITE); \
    _XL_PRINTD(2,0,5,score); \
} while(0)

#define displayRemainingBuilings() \
do { \
    SET_TEXT_COLOR(_XL_WHITE); \
    _XL_PRINTD(9,0,2,remaining_buildings); \
} while(0)

#define displayHiScore() \
do { \
    SET_TEXT_COLOR(_XL_WHITE); \
    _XL_PRINTD(XSize-5,0,5,hiscore); \
} while(0)


#define displayLevel() \
do { \
    SET_TEXT_COLOR(_XL_WHITE); \
    _XL_PRINTD(XSize-8,0,1,level); \
} while(0)

#define displayLevelMessage() \
do { \
    SET_TEXT_COLOR(_XL_WHITE); \
    _XL_PRINTD(XSize/2+2,4,2,level); \
} while(0)

#define displayNewHiScoreMessage() \
do { \
    SET_TEXT_COLOR(_XL_CYAN); \
    _XL_PRINT(1,4,_NEW_HISCORE__STRING); \
} while(0)

#define displayGameOverMessage() \
do { \
    SET_TEXT_COLOR(_XL_RED); \
    printCenteredMessage(_GAME_OVER__STRING); \
} while(0)

#define handle_hiscore() \
do { \
    if(score>hiscore) \
    { \
        hiscore = score; \
        displayNewHiScoreMessage(); \
        _XL_SLEEP(1); \
    } \
} while(0)

#if MAX_Y<24
    #define LEVEL_FACTOR_SPEED_UP 3
#else
    #define LEVEL_FACTOR_SPEED_UP 2
#endif 


#define INITIAL_LEVEL 1
#define FINAL_LEVEL 8

uint16_t building_height[XSize];


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


const uint8_t building_tiles[] = {
    WALL_1_TILE, WALL_2_TILE, 
    TWO_WINDOW_WALL_1_TILE, TWO_WINDOW_WALL_2_TILE, 
    THREE_WINDOW_WALL_1_TILE, THREE_WINDOW_WALL_2_TILE, 
    SMALL_TWO_WINDOW_WALL_1_TILE, SMALL_TWO_WINDOW_WALL_2_TILE
    };

const uint8_t building_colors[] = {
    _WALL_1_COLOR, _WALL_2_COLOR, 
    _TWO_WINDOW_WALL_1_COLOR, _TWO_WINDOW_WALL_2_COLOR, 
    _THREE_WINDOW_WALL_1_COLOR, _THREE_WINDOW_WALL_2_COLOR, 
    _SMALL_TWO_WINDOW_WALL_1_COLOR, _SMALL_TWO_WINDOW_WALL_2_COLOR
    };

int main(void)
{        

    uint8_t buildingType;
    uint8_t buildingColor;
    uint8_t rnd;

    _XL_INIT_GRAPHICS();

    _XL_INIT_INPUT();
    
    _XL_INIT_SOUND();
    
    hiscore = 0;


    while(1)
    {
        alive = 1;
        score = 0;
        level = INITIAL_LEVEL;
        explosion = 0;

        _XL_CLEAR_SCREEN();
            
        SET_TEXT_COLOR(_XL_RED);
        printCenteredMessageOnRow(2, _CROSS_BOMBER__STRING);
        SET_TEXT_COLOR(_XL_CYAN);
        printCenteredMessageOnRow(4, _BY_FABRIZIO_CARUSO__STRING);
        SET_TEXT_COLOR(_XL_YELLOW);
        printCenteredMessageOnRow(8, _DESTROY_ALL__STRING);
        printCenteredMessageOnRow(9, _BUILDINGS__STRING);
        SET_TEXT_COLOR(_XL_WHITE);
        printCenteredMessageOnRow(14, _PRESS_FIRE__STRING);
        _XL_WAIT_FOR_INPUT();
        while(alive && (level < FINAL_LEVEL+1))
        {
            bombActive = 0;
            bomb_x = 0;
            bomb_y = MAX_Y-2;
            bonus = 0;
            remaining_buildings = BUILDINGS_NUMBER;
            
            _XL_CLEAR_SCREEN();
            _XL_PRINT(XSize/2-4, 4, _LEVEL__STRING);
            displayLevelMessage();
            _XL_SLEEP(1);
            _XL_WAIT_FOR_INPUT();
            _XL_CLEAR_SCREEN();
            
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
                building_height[x] = (uint8_t) MIN_BUILDING_HEIGHT+level/LEVEL_FACTOR_SPEED_UP+(_XL_RAND()&7);
                rnd = ((uint8_t) _XL_RAND())&7;
                buildingType=building_tiles[rnd];
                buildingColor=building_colors[rnd];
                
                for(y=1;y<building_height[x];++y)
                {
                    drawBuilding();     
                }
                _XL_PING_SOUND();
            }

            _XL_SLEEP(1);
            y = 2;
            x = 1;
            
            displayScore();
            
            
            _XL_DRAW(0,0,SCORE_TEXT_LEFT_TILE, _XL_CYAN);
            _XL_DRAW(1,0,SCORE_TEXT_RIGHT_TILE, _XL_CYAN);
            
            _XL_DRAW(XSize-6,0,HI_TEXT_TILE, _XL_RED);
            #if XSize>=20
                _XL_DRAW(XSize-9,0,LV_TEXT_TILE, _XL_GREEN);
                displayLevel();
            #endif
            #if XSize>=20
                _XL_DRAW(8,0,TWO_WINDOW_WALL_2_TILE, _XL_YELLOW);
                displayRemainingBuilings();
            #endif
            
            
            displayHiScore();
            while((y<MAX_Y-building_height[x+1]) && (y<MAX_Y-2 || x<XSize-3))
            {
                // Land safely
                if(!remaining_buildings && (y<MAX_Y-2) && (x<XSize-3) )
                {
                    deleteAnimatedPlaneBack();
                    deleteAnimatedPlaneCenter();
                    ++y;
                }
                #if defined(DEBUG_GHOST_DISPLAY)
                    UNSET_DEBUG_BORDER();
                #endif
                drawAnimatedPlane();
                _XL_SLOW_DOWN(SLOW_DOWN/2-level*LEVEL_SPEED_UP);
                

                if(!bombActive )
                {   
                    if(explosion)
                    {
                        deleteExplosion();
                        explosion = 0;
                    }
                    
                    if((y<MAX_Y-2) && _XL_KEY_PRESSED())
                    {
                        _XL_SHOOT_SOUND();
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
                                bonus = 10*(MAX_Y-y)+level*20;
                            }
                            displayScore();
                        }
                    }
                }
                else
                {
                    // Draw animated bomb
                    drawAnimatedBomb();
                    
                    ++bomb_y;
                
                    if(bomb_y>MAX_Y-3) // Bomb reaches the ground
                    {
                        drawBomb();
                        bombActive = 0;
                        explosion = 1;
                        
                        
                        #if XSize>=20
                            displayRemainingBuilings();
                        #endif
                        // Delete animated bomb
                        deleteAnimatedBombUp();
                        drawExplosion();
                        _XL_SLOW_DOWN(5+SLOW_DOWN/4);
                    }

                }
                _XL_WAIT_VSYNC();
                #if defined(DEBUG_GHOST_DISPLAY)
                    SET_DEBUG_BORDER();
                #endif
                deleteAnimatedPlaneBack();
                drawPlane();
                #if defined(DEBUG_GHOST_DISPLAY)
                    UNSET_DEBUG_BORDER();
                #endif
                
                #if !defined(NO_ANIMATION)
                    _XL_SLOW_DOWN(SLOW_DOWN/2-level*LEVEL_SPEED_UP);
                #endif
                
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
                    #if defined(NO_ANIMATION) // Required because deletePlane() in this case only deletes the front
                        deletePlaneBack();
                    #endif
                    deletePlane();
                    x=1;
                    ++y;
                }
                
                _XL_WAIT_VSYNC();
                #if defined(DEBUG_GHOST_DISPLAY)
                    SET_DEBUG_BORDER();
                #endif
                deleteAnimatedPlaneBack();
            } // while flying
            #if defined(DEBUG_GHOST_DISPLAY)
                UNSET_DEBUG_BORDER();
            #endif
            if(!remaining_buildings)
            {
                drawPlane();
                SET_TEXT_COLOR(_XL_YELLOW);
                _XL_PRINT(1,2,_LEVEL_COMPLETED__STRING);
                _XL_SLEEP(1);
                ++level;
                score+=bonus;
                SET_TEXT_COLOR(_XL_WHITE);
                _XL_PRINT(1,4,_BONUS__STRING);
                for(bonus_ind=10;bonus_ind<=bonus;bonus_ind+=10)
                {
                    _XL_PRINTD(7,4,5,bonus_ind);
                    _XL_SHOOT_SOUND();
                    _XL_SLOW_DOWN(SLOW_DOWN);
                    _XL_SLOW_DOWN(SLOW_DOWN);
                }
                displayScore();
                _XL_SLEEP(1);
            }
            else
            {
                drawPlaneBack();
                _XL_EXPLOSION_SOUND();
                displayGameOverMessage();
                _XL_SLEEP(2);
                alive = 0;
                handle_hiscore();
            }
            _XL_WAIT_FOR_INPUT();
        } // while(alive || (level>FINAL})) -> restart level
        if(alive) // Game completed
        {
            SET_TEXT_COLOR(_XL_YELLOW);
            printCenteredMessage(_GAME_COMPLETED__STRING);
            _XL_SLEEP(1);
            _XL_WAIT_FOR_INPUT();
            for(y=2;y<MAX_Y-1;++y)
            {
                for(x=1;x<XSize-2;x+=2)
                {
                    drawPlane();
                }
            }
            _XL_SLEEP(1);
            displayGameOverMessage();
            _XL_SLEEP(2);
            handle_hiscore();
            _XL_SLEEP(1);
            _XL_WAIT_FOR_INPUT();
        }
    } // while(1) -> restart from level 1

    return EXIT_SUCCESS;
}

