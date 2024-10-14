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

#include "cross_lib.h"

#include "tile_aliases.h"


// #define DEBUG


#if !defined(_XL_SLOW_DOWN_FACTOR)
    #define LEVEL_SPEED_UP 0
#elif _XL_SLOW_DOWN_FACTOR<20
    #define LEVEL_SPEED_UP 0
#elif _XL_SLOW_DOWN_FACTOR<100
    #define LEVEL_SPEED_UP 2
#elif _XL_SLOW_DOWN_FACTOR<200
    #define LEVEL_SPEED_UP 4
#elif _XL_SLOW_DOWN_FACTOR<400
    #define LEVEL_SPEED_UP 8
#elif _XL_SLOW_DOWN_FACTOR<800
    #define LEVEL_SPEED_UP 16
#elif _XL_SLOW_DOWN_FACTOR<1600
    #define LEVEL_SPEED_UP 32
#elif _XL_SLOW_DOWN_FACTOR<3200
    #define LEVEL_SPEED_UP 64
#else
    #define LEVEL_SPEED_UP 128
#endif 

#define MAX_Y ((YSize)+(Y_OFFSET))

#if YSize>=12
	#define MIN_BUILDING_HEIGHT 2
#else
	#define MIN_BUILDING_HEIGHT 2
#endif

#if YSize>=12
	#define INITIAL_PLANE_Y 2
#else
	#define INITIAL_PLANE_Y 1
#endif

#if YSize>=20
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
    #elif XSize>16
        #define BUILDINGS_NUMBER (XSize-9)
        #define FIRST_BULDING_X_POS 5
    #else
        #define BUILDINGS_NUMBER (XSize-7)
        #define FIRST_BULDING_X_POS 4
    #endif
#elif YSize>=12
    #define BUILDINGS_NUMBER (XSize/2)
    #define FIRST_BULDING_X_POS (XSize/4)  
#else 
    #define BUILDINGS_NUMBER 5
    #define FIRST_BULDING_X_POS ((XSize/2)-2)	
#endif  

// String definitions
#define _BOMBER__STRING _XL_B _XL_O _XL_M _XL_B _XL_E _XL_R
#if XSize>=18
    #define _BY_FABRIZIO_CARUSO__STRING _XL_B _XL_Y _XL_SPACE _XL_F _XL_A _XL_B _XL_R _XL_I _XL_Z _XL_I _XL_O _XL_SPACE _XL_C _XL_A _XL_R _XL_U _XL_S _XL_O
#else
    #define _BY_FABRIZIO_CARUSO__STRING _XL_F _XL_A _XL_B _XL_R _XL_I _XL_Z _XL_I _XL_O _XL_SPACE _XL_C _XL_A _XL_R _XL_U _XL_S _XL_O
#endif
#define _DESTROY_ALL__STRING _XL_D _XL_E _XL_S _XL_T _XL_R _XL_O _XL_Y _XL_SPACE _XL_A _XL_L _XL_L
#define _BUILDINGS__STRING _XL_B _XL_U _XL_I _XL_L _XL_D _XL_I _XL_N _XL_G _XL_S

#if !defined(_XL_NO_JOYSTICK)
    #define _PRESS_FIRE__STRING _XL_P _XL_R _XL_E _XL_S _XL_S _XL_SPACE _XL_F _XL_I _XL_R _XL_E
#else
    #define _PRESS_FIRE__STRING _XL_P _XL_R _XL_E _XL_S _XL_S _XL_SPACE _XL_A _XL_SPACE _XL_K _XL_E _XL_Y
    
#endif
#define _LEVEL__STRING _XL_L _XL_E _XL_V _XL_E _XL_L
#define _LEVEL_COMPLETED__STRING _XL_L _XL_E _XL_V _XL_E _XL_L _XL_SPACE _XL_C _XL_O _XL_M _XL_P _XL_L _XL_E _XL_T _XL_E _XL_D
#define _BONUS__STRING _XL_B _XL_O _XL_N _XL_U _XL_S
#define _GAME_COMPLETED__STRING "GAME COMPLETED"
#define _DEL_STR _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE
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
        
    #define deleteAnimatedBomb()
        // _XL_DELETE(bomb_x,bomb_y-1);
    
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
    _XL_SET_TEXT_COLOR(_XL_WHITE); \
    _XL_PRINTD(2,0,4,score); \
} while(0)


#if XSize<20
    #define REMAINING_X 8
#else
    #define REMAINING_X 9
#endif

#if !defined(TINY_GAME)
    #define displayRemainingBuilings() \
        do { \
            _XL_SET_TEXT_COLOR(_XL_WHITE); \
            _XL_PRINTD(REMAINING_X,0,2,remaining_buildings); \
        } while(0)
#else
    #define displayRemainingBuilings() 
#endif

#define displayHiScore(xpos) \
do { \
    _XL_SET_TEXT_COLOR(_XL_WHITE); \
    _XL_PRINTD(xpos,0,4,hiscore); \
} while(0)


#define displayLevel() \
do { \
    _XL_SET_TEXT_COLOR(_XL_WHITE); \
    _XL_PRINTD(XSize-7,0,1,level); \
} while(0)

#define displayLevelMessage() \
do { \
    _XL_SET_TEXT_COLOR(_XL_WHITE); \
    _XL_PRINTD(XSize/2+2,4,1,level); \
} while(0)

#if !defined(TINY_GAME)
    #define displayNewHiScoreMessage() \
    do { \
        _XL_SET_TEXT_COLOR(_XL_YELLOW); \
        _XL_PRINT(1,4,_NEW_HISCORE__STRING); \
    } while(0)
#else
    #define displayNewHiScoreMessage()
#endif

void PRINT_CENTERED_ON_ROW(uint8_t row, char *Text)
{
	_XL_PRINT(((uint8_t) (XSize - strlen(Text))>>1), row, Text);	
}

#define PRINT_CENTERED(Text) \
    PRINT_CENTERED_ON_ROW((YSize>>1), Text)


void displayGameOverMessage(void)
{
    _XL_SET_TEXT_COLOR(_XL_RED);
    PRINT_CENTERED(_GAME_OVER__STRING);
}

#if !defined(TINY_GAME)

    #define handle_hiscore() \
    do { \
        if(score>hiscore) \
        { \
            hiscore = score; \
            displayNewHiScoreMessage(); \
            _XL_SLEEP(1); \
        } \
    } while(0)

#else
    #define handle_hiscore() \
    do { \
        if(score>hiscore) \
        { \
            hiscore = score; \
        } \
    } while(0)   
#endif

#if MAX_Y<24
    #if YSize<=16
        #define LEVEL_FACTOR_SPEED_UP 4
    #else
        #define LEVEL_FACTOR_SPEED_UP 3
    #endif
#else
    #define LEVEL_FACTOR_SPEED_UP 2
#endif 

#if YSize<12
	#define AND_MASK 1
#elif YSize<=17
    #define AND_MASK 3
#else
    #define AND_MASK 7
#endif

#define INITIAL_LEVEL 1
#define FINAL_LEVEL 9

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

uint8_t buildingType;
uint8_t tile_index;

#if !defined(_XL_NO_COLOR)
uint8_t buildingColor;
uint8_t color_index;
#endif

static const uint8_t building_tiles[] = {
    WALL_1_TILE, WALL_2_TILE, 
    TWO_WINDOW_WALL_1_TILE, TWO_WINDOW_WALL_2_TILE, 
    THREE_WINDOW_WALL_1_TILE, THREE_WINDOW_WALL_2_TILE, 
    SMALL_TWO_WINDOW_WALL_1_TILE, SMALL_TWO_WINDOW_WALL_2_TILE
    };

#if !defined(_XL_NO_COLOR)
static const uint8_t building_colors[] = {
    _WALL_1_COLOR, _WALL_2_COLOR, 
    _TWO_WINDOW_WALL_1_COLOR, _TWO_WINDOW_WALL_2_COLOR, 
    _THREE_WINDOW_WALL_1_COLOR, _THREE_WINDOW_WALL_2_COLOR, 
    _SMALL_TWO_WINDOW_WALL_1_COLOR, _SMALL_TWO_WINDOW_WALL_2_COLOR
    };
#endif


#if defined(DEBUG)
    static const uint8_t tiles[] = {
            _TILE_0, _TILE_1, _TILE_2, _TILE_3, 
            _TILE_4, _TILE_5, _TILE_6, _TILE_7, 
            _TILE_8, _TILE_9, _TILE_10, _TILE_11, 
            _TILE_12, _TILE_13, _TILE_14, _TILE_15, 
            _TILE_16, _TILE_17, _TILE_18, _TILE_19,
            _TILE_20, _TILE_21, _TILE_22, _TILE_23, 
            _TILE_24, _TILE_25
            };

    static const static uint8_t tile_color[] = {_XL_WHITE, _XL_RED, _XL_CYAN, _XL_GREEN, _XL_YELLOW, _XL_BLUE};
#endif


void intro(void)
{
    _XL_CLEAR_SCREEN();
    
    #if !defined(TINY_GAME)
    _XL_DRAW(XSize/2-3,0,HI_TEXT_TILE,_XL_RED);
    #endif
    
    displayHiScore(XSize/2-2);
        
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    PRINT_CENTERED_ON_ROW(2, _BOMBER__STRING);
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    PRINT_CENTERED_ON_ROW(4, _BY_FABRIZIO_CARUSO__STRING);
    
    #if YSize>=16 && !defined(TINY_GAME)
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    PRINT_CENTERED_ON_ROW(YSize/2, _DESTROY_ALL__STRING);
    PRINT_CENTERED_ON_ROW(YSize/2+1, _BUILDINGS__STRING);
    
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    PRINT_CENTERED_ON_ROW(YSize-2, _PRESS_FIRE__STRING);
    #endif

    #if !defined(TINY_GAME)

    x=XSize/2-1;
    y=6;
    
    drawAnimatedPlane();
    
    _XL_DRAW(XSize/2-2,YSize/2+3,WALL_1_TILE,_XL_RED);
    _XL_DRAW(XSize/2-1,YSize/2+3,TWO_WINDOW_WALL_1_TILE,_XL_YELLOW);
    _XL_DRAW(XSize/2-0,YSize/2+3,THREE_WINDOW_WALL_1_TILE,_XL_WHITE);        
    #endif
    
    _XL_WAIT_FOR_INPUT();
}

#define INITIALIZE_CROSS_LIB() \
    _XL_INIT_GRAPHICS(); \
    _XL_INIT_INPUT(); \
    _XL_INIT_SOUND();


void initialize_game(void)
{
    alive = 1;
    score = 0;
    level = INITIAL_LEVEL;
    explosion = 0;
}


void initialize_buildings(void)
{
    for(x=0;x<XSize-2;++x)
    {
        building_height[x] = 0;
    }
    for(x=FIRST_BULDING_X_POS;x<FIRST_BULDING_X_POS+BUILDINGS_NUMBER;++x)
    {
        #if YSize>=12
        building_height[x] = (uint8_t) MIN_BUILDING_HEIGHT+level/LEVEL_FACTOR_SPEED_UP+(_XL_RAND()&AND_MASK);
        #else
        building_height[x] = (uint8_t) MIN_BUILDING_HEIGHT+(_XL_RAND()&AND_MASK);
        #endif
        tile_index = ((uint8_t) _XL_RAND())&7;
        buildingType=building_tiles[tile_index];
        #if !defined(_XL_NO_COLOR)
        color_index = ((uint8_t) _XL_RAND())&7;
        buildingColor=building_colors[color_index];
        #endif
        for(y=1;y<building_height[x];++y)
        {
            drawBuilding();   
        }
        _XL_PING_SOUND();
    }
}

#if !defined(TINY_GAME)

    #define show_level() \
            _XL_CLEAR_SCREEN(); \
            _XL_PRINT(XSize/2-4, 4, _LEVEL__STRING); \
            displayLevelMessage(); \
            _XL_SLEEP(1); \
            _XL_WAIT_FOR_INPUT(); \

#else
    #define show_level()
#endif


void initialize_level(void)
{
            bombActive = 0;
            bomb_x = 0;
            bomb_y = MAX_Y-2;
            bonus = 0;
            remaining_buildings = BUILDINGS_NUMBER;

            _XL_CLEAR_SCREEN();
            
            for(x=0;x<XSize;++x)
            {
                drawRoad();
            }
            initialize_buildings();

            _XL_SLEEP(1);
            y = INITIAL_PLANE_Y;
            x = 1;
            
            displayScore();
            
            #if !defined(TINY_GAME)
            _XL_DRAW(0,0,SCORE_TEXT_LEFT_TILE, _XL_CYAN);
            _XL_DRAW(1,0,SCORE_TEXT_RIGHT_TILE, _XL_CYAN);
            
            _XL_DRAW(XSize-5,0,HI_TEXT_TILE, _XL_RED);
            #endif
            
            #if XSize>=20 && !defined(TINY_GAME)
                _XL_DRAW(XSize-8,0,LV_TEXT_TILE, _XL_GREEN);
                displayLevel();
            #endif
            #if XSize>=16 && !defined(TINY_GAME)
                _XL_DRAW(REMAINING_X-1,0,TWO_WINDOW_WALL_2_TILE, _XL_YELLOW);
                displayRemainingBuilings();
            #endif
            
            
            displayHiScore(XSize-4);
}


void handle_non_active_bomb(void)
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
                bonus = (uint16_t)20u*(uint16_t)(MAX_Y-y)+(uint16_t)level*20u;
            }
        }
    }
}


void handle_active_bomb(void)
{
    // Draw animated bomb
    
    drawAnimatedBomb();
    
    ++bomb_y;

    if(bomb_y>MAX_Y-3) // Bomb reaches the ground
    {
        drawBomb();
        bombActive = 0;
        explosion = 1;
                 
        #if XSize>=16
            displayRemainingBuilings();
        #endif
        displayScore();

        // Delete animated bomb
        
        deleteAnimatedBombUp();

        drawExplosion();

        _XL_SLOW_DOWN(5+_XL_SLOW_DOWN_FACTOR/4);
    }
}


#define handle_bomb_movement() \
    do \
    { \
        if(bombActive) \
        { \
            drawBomb(); \
            deleteAnimatedBombUp(); \
        } \
    } \
    while(0)

#define handle_bomb() \
    do \
    { \
        if(!bombActive ) \
        { \
            handle_non_active_bomb(); \
        } \
        else \
        { \
            handle_active_bomb(); \
        } \
    } \
    while(0)




void handle_plane_movement(void)
{
    // Land safely
    if(!remaining_buildings && (y<MAX_Y-2) && (x<XSize-3) )
    {
        deleteAnimatedPlaneBack();
        deleteAnimatedPlaneCenter();
        ++y;
    }

    drawAnimatedPlane();
    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR/2-level*LEVEL_SPEED_UP);
    
    handle_bomb();
    
    _XL_WAIT_VSYNC();

    deleteAnimatedPlaneBack();
    drawPlane();
    #if defined(DEBUG_GHOST_DISPLAY)
        UNSET_DEBUG_BORDER();
    #endif
    
    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR/2-level*LEVEL_SPEED_UP); // e.g., lv:9, slow-down fact:1800 -> 1800/2-9*32=900-288=612

    handle_bomb_movement();

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
    #if !defined(NO_ANIMATION)
    deleteAnimatedPlaneBack();
    #endif
    
    _XL_REFRESH();
}


void level_completed(void)
{
    #if defined(NO_ANIMATION)                
        _XL_DELETE(x-1,y);
    #endif
    drawPlane();
    
    #if !defined(TINY_GAME)
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(1,2,_LEVEL_COMPLETED__STRING);
    _XL_SLEEP(1);
    #endif
    ++level;
    score+=bonus;
    
    #if !defined(TINY_GAME)
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(1,4,_BONUS__STRING);
    for(bonus_ind=10;bonus_ind<=bonus;bonus_ind+=10)
    {
        _XL_PRINTD(7,4,4,bonus_ind);
        _XL_SHOOT_SOUND();
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
    _XL_PRINTD(7,4,4,bonus);
    #endif
    displayScore();
    _XL_SLEEP(1);
}


void plane_crash(void)
{
    #if defined(NO_ANIMATION)
        deleteAnimatedPlaneBack();
    #endif
    drawPlaneBack();
    _XL_EXPLOSION_SOUND();
    displayScore();
    displayGameOverMessage();
    _XL_SLEEP(2);
    alive = 0;
    handle_hiscore();
}

#define handle_end_of_level() \
    do \
    { \
        if(!remaining_buildings) \
        { \
            level_completed(); \
        } \
        else \
        { \
            plane_crash(); \
        } \
        _XL_WAIT_FOR_INPUT(); \
    } \
    while(0)

#define flying() ((y<MAX_Y-building_height[x+1]) && (y<MAX_Y-2 || x<XSize-3))

#define game_completed() \
    _XL_SET_TEXT_COLOR(_XL_YELLOW); \
    PRINT_CENTERED(_GAME_COMPLETED__STRING); \
    _XL_SLEEP(1); \
    _XL_WAIT_FOR_INPUT(); \
    _XL_PRINT(1,2,_DEL_STR); \
    for(y=2;y<MAX_Y-1;++y) \
    { \
        for(x=1;x<XSize-2;x+=2) \
        { \
            drawPlane(); \
        } \
    } \
    _XL_SLEEP(1); \
    displayGameOverMessage(); \
    _XL_SLEEP(2); \
    handle_hiscore(); \
    _XL_SLEEP(1); \
    _XL_WAIT_FOR_INPUT(); \


int main(void)
{        
    INITIALIZE_CROSS_LIB();
    
    hiscore = 0;

    while(1)
    {
        initialize_game();

        intro();
        while(alive && (level < FINAL_LEVEL+1))
        {
            show_level();
            initialize_level();
            while(flying())
            {
                handle_plane_movement();
            } // while flying
            handle_end_of_level();             // No longer flying
        } // while(alive || (level>FINAL})) -> restart level
        if(alive) // Game completed
        {
            game_completed();
        }
    } // while(1) -> restart from level 1

    return EXIT_SUCCESS;
}

