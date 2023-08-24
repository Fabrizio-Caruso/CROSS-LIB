/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS STINGER by Fabrizio Caruso
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

#include "cross_lib.h"

#include "images.h"

// #define DEBUG 1
//#define TRAINER 1

#define INITIAL_LEVEL 2

#define LAST_LEVEL 4
#define INITIAL_LIVES 3
#define MAX_LIVES 9

#define NEXT_EXTRA_LIFE 5000U

#define BOW_Y ((YSize)-3)
#define MAX_BOW_X ((XSize)*2-3)
#define POWER_UPS_Y ((BOW_Y)+2)

#if YSize<=16
    #define WALL_Y ((YSize)-8)
#else
    #define WALL_Y ((YSize)-6)
#endif

#if YSize>=16
    #define INITIAL_TANK_Y (((YSize)/2)-2)
#else
    #define INITIAL_TANK_Y (((YSize)/2)-1)
#endif

#if YSize>=18
    #define INITIAL_RESPAWN_TANK_Y (INITIAL_TANK_Y)-4
#elif YSize>=16
    #define INITIAL_RESPAWN_TANK_Y (INITIAL_TANK_Y)-3
#else 
    #define INITIAL_RESPAWN_TANK_Y (INITIAL_TANK_Y)-2
#endif

#define BOTTOM_WALL_Y ((BOW_Y)+1)

#define POWER_THRESHOLD 4

#if !defined(MAX_ARROWS_ON_SCREEN)
    #define MAX_ARROWS_ON_SCREEN 12
#endif

#define AUTO_RECHARGE_COOL_DOWN 50
#define AUTO_ARROW_RECAHRGE 9

#define MAX_FREEZE 1

#define LIGHT_TANK_POINTS 10
#define HEAVY_TANK_1_POINTS 15
#define HEAVY_TANK_2_POINTS 25
#define HEAVY_TANK_3_POINTS 30

#define EXTRA_POINTS 100
#define RECHARGE_POINTS 25
#define POWERUP_POINTS 30
#define FREEZE_POINTS 60
#define WALL_POINTS 80
#define SECRET_ITEM_POINTS 500 
#define POWER_UP_BONUS 25
#define LEVEL_BONUS 200


#define RED_FIRE_POWER_VALUE 2
#define YELLOW_FIRE_POWER_VALUE 3
#define GREEN_FIRE_POWER_VALUE 4

#define INITIAL_BOW_RELOAD_LOOPS 9
#define RED_SPEED_VALUE INITIAL_BOW_RELOAD_LOOPS
#define YELLOW_SPEED_VALUE 7
#define GREEN_SPEED_VALUE 4
#define HYPER_SPEED_VALUE 3

#define RED_RANGE_VALUE INITIAL_ARROW_RANGE
#define YELLOW_RANGE_VALUE ((INITIAL_ARROW_RANGE)-2)
#define GREEN_RANGE_VALUE ((INITIAL_ARROW_RANGE)-4)

#define INITIAL_ARROW_RANGE ((INITIAL_TANK_Y)+1)
#define ITEM_SPAWN_CHANCE 11000U

// (2 basic hits)
#define LIGHT_TANK_ENERGY 4

// Boss energy: 7 (4 basic hits), 9 (5 basic hits), 11 (6 basic hits)
#define HEAVY_TANK_BASE_ENERGY 5

#define WALL_ENERGY 20

#define MAX_ARROWS 99
#define HYPER_RECHARGE 30
#define ARROW_RECHARGE 15

#define FREEZE_COUNTER_MAX 100;

#define WALL_COLOR _XL_GREEN
#define FREEZE_COLOR _XL_CYAN
#define SECRET_COLOR _XL_GREEN


uint8_t max_occupied_columns;

#if XSize<=40
    #define MAX_OCCUPIED_COLUMNS (3*(XSize)/4)
#else
    #define MAX_OCCUPIED_COLUMNS (2*(XSize)/3)
#endif

#if XSize<=40
    #define MAX_DENSILY_OCCUPIED_COLUMNS (XSize-2-3)
#else
    #define MAX_DENSILY_OCCUPIED_COLUMNS (XSize-2-6)
#endif

#define FEW_TANKS (2*(MAX_OCCUPIED_COLUMNS)/3)

#define MAX_NUMBER_OF_MISSILES 6

#define NUMBER_OF_EXTRA_POINTS MAX_NUMBER_OF_MISSILES

#define HELP_ITEM_LEVEL_THRESHOLD 2

#if XSize<=30
    #define LIGHT_TANKS_ON_FIRST_LEVEL 30
#elif XSize<=40
	#defime LIGHT_TANKS_ON_FIRST_LEVEL 40
#else
    #define LIGHT_TANKS_ON_FIRST_LEVEL 80
#endif

#if XSize<64
    #define HEAVY_TANKS_ON_FIRST_LEVEL (XSize)
#else
    #define HEAVY_TANKS_ON_FIRST_LEVEL 50
#endif

const uint8_t heavy_tanks_on_level[LAST_LEVEL+1] = {30,50,70,100,120};

#define LEVEL_2_TANK_THRESHOLD 8

#define MAX_HYPER_COUNTER 160

#if YSize>=20
    #define HEIGHT_SHOOT_THRESHOLD YSize-10
#else
    #define HEIGHT_SHOOT_THRESHOLD YSize-11
#endif

#define FAST_TANK_SHOOT_MASK 3

#define SLOW_TANK_SHOOT_MASK 15

uint8_t tank_speed_mask;

uint16_t next_threshold;

uint8_t main_loop_counter;

uint8_t forced_tank;

uint8_t forced_tank_x;

uint8_t hyper_counter;

uint8_t item_counter;

const uint8_t tank_points[] = 
{ 
    LIGHT_TANK_POINTS, // Skeletons
    HEAVY_TANK_1_POINTS, // Ogre
    HEAVY_TANK_2_POINTS, // Ghosts
    HEAVY_TANK_3_POINTS, // Demons
};

 uint8_t light_tanks_to_kill;
 uint8_t heavy_tanks_to_kill;

 uint8_t lives;
 uint8_t level;
 uint8_t killed_light_tanks;
 uint8_t killed_heavy_tanks;

 uint8_t heavy_tanks_to_spawn;
 uint8_t light_tanks_to_spawn;

 uint8_t auto_recharge_counter;

#if !defined(_XL_NO_COLOR)
     uint8_t arrow_display_color;
#else
    #define arrow_display_color _DUMMY_
#endif

#if !defined(_XL_NO_COLOR) && !defined(_XL_NO_TEXT_COLOR)
 const uint8_t power_up_color[3] = {_XL_RED, _XL_YELLOW, _XL_GREEN};
#endif

#if !defined(_XL_NO_COLOR)
 const uint8_t arrow_color[3] = {_XL_CYAN, _XL_YELLOW, _XL_WHITE};
#endif

 uint8_t freeze;
 uint8_t powerUp;

 uint8_t number_of_arrows_per_shot;

 uint8_t tank_y[XSize];
 uint8_t tank_shape[XSize];
 uint8_t tank_x;
 uint8_t tank_speed;
 uint8_t tank_active[XSize];

 uint8_t energy[XSize];
 uint8_t tank_level[XSize];
 const uint8_t rank_energy[4] = {4,7,11,17}; // 2, 4, 6, 9

 uint8_t fire_power;

 uint8_t freeze_locked;
 uint8_t secret_locked;

#if !defined(_XL_NO_UDG)
 const uint8_t tank_tile[7+1] = 
{
    LIGHT_TANK_TILE_0, // 0
    LIGHT_TANK_TILE_0, // 1
    LIGHT_TANK_TILE_1, // 2
    LIGHT_TANK_TILE_2, // 3
    LIGHT_TANK_TILE_3, 
    LIGHT_TANK_TILE_4, 
    LIGHT_TANK_TILE_5, 
    LIGHT_TANK_TILE_6
};

 const uint8_t heavy_tank_tile[7+1] =
{
    HEAVY_TANK_TILE_0,
    HEAVY_TANK_TILE_0,
    HEAVY_TANK_TILE_1,
    HEAVY_TANK_TILE_2,
    HEAVY_TANK_TILE_3,
    HEAVY_TANK_TILE_4,
    HEAVY_TANK_TILE_5,
    HEAVY_TANK_TILE_6,
};
#endif

 const uint8_t bow_tile[8] =
{
    EMPTY_BOW_LEFT_TILE_0,
    EMPTY_BOW_RIGHT_TILE_0,
    EMPTY_BOW_LEFT_TILE_1,
    EMPTY_BOW_RIGHT_TILE_1,
    LOADED_BOW_LEFT_TILE_0,
    LOADED_BOW_RIGHT_TILE_0,
    LOADED_BOW_LEFT_TILE_1,
    LOADED_BOW_RIGHT_TILE_1,
};

 const uint8_t arrow_tile[2] =
{
    ARROW_TILE_0,
    ARROW_TILE_1,
};

 uint8_t bow_x; // range: 0..2*XSize-2^M
 uint8_t bow_shape_tile;
 uint8_t bow_color;

 uint8_t input;

 uint8_t loaded_bow;
 uint8_t active_arrow[MAX_ARROWS_ON_SCREEN];
 uint8_t arrow_shape[MAX_ARROWS_ON_SCREEN];
 uint8_t arrow_x[MAX_ARROWS_ON_SCREEN];
 uint8_t arrow_y[MAX_ARROWS_ON_SCREEN];
 uint8_t remaining_arrows;
 uint8_t arrow_range;

 uint8_t bow_reload_loops;

 uint8_t next_arrow;
 uint8_t arrows_on_screen;
 uint8_t bow_load_counter;
 uint8_t alive;

 uint16_t score;
 uint16_t hiscore;

struct  ItemStruct
{
    uint8_t _x;
    uint8_t _y;
    uint8_t _tile;
    #if !defined(_XL_NO_COLOR)
    uint8_t _color;
    #endif
    uint8_t _active;
    uint8_t _counter;
    void(*_effect)(void);
} ;
typedef struct ItemStruct Item;
typedef struct ItemStruct Missile;

 Item rechargeItem;
 Item freezeItem;
 Item powerUpItem;
 Item secretItem;

#if !defined(NO_EXTRA_TITLE)

#define NUMBER_OF_ITEMS 4

 const uint8_t item_tile[NUMBER_OF_ITEMS][2] = 
{
    { POWER_UP_TILE, _XL_WHITE },
    { ARROW_TILE_0, _XL_YELLOW },
    { EXTRA_POINTS_TILE, _XL_YELLOW },
    { FREEZE_TILE, _XL_CYAN },
};

 const char item_name[NUMBER_OF_ITEMS][9] = 
{
    _XL_P _XL_O _XL_W _XL_E _XL_R _XL_SPACE _XL_U _XL_P,
    _XL_R _XL_O _XL_C _XL_K _XL_E _XL_T _XL_S,
    _XL_P _XL_O _XL_I _XL_N _XL_T _XL_S,
    _XL_F _XL_R _XL_E _XL_E _XL_Z _XL_E,
};
#endif

#if !defined(NO_EXTRA_TITLE)
 const uint8_t enemy_tile[5][2] = 
{
    { LIGHT_TANK_TILE_0, _XL_WHITE },
    { HEAVY_TANK_TILE_0, _XL_GREEN },
    { TANK_DEATH_TILE, _XL_YELLOW },
    { HEAVY_TANK_TILE_0, _XL_RED },
};

 const char enemy_name[4][8] = 
{
    _XL_L _XL_I _XL_G _XL_H _XL_T,
    _XL_M _XL_E _XL_D _XL_I _XL_U _XL_M,
    _XL_S _XL_T _XL_E _XL_A _XL_L _XL_T _XL_H,
    _XL_H _XL_E _XL_A _XL_V _XL_Y,
};
#endif



 Missile enemyMissile[MAX_NUMBER_OF_MISSILES];
 Item extraPointsItem[MAX_NUMBER_OF_MISSILES];


void PRINT_CENTERED_ON_ROW(uint8_t row, char *Text)
{
	_XL_PRINT(((uint8_t) (XSize - strlen(Text))>>1), row, Text);	
}


#define PRINT_CENTERED(Text) \
	PRINT_CENTERED_ON_ROW((YSize>>1), Text)





void sleep_and_wait_for_input(void)
{
    _XL_SLEEP(1);
    _XL_WAIT_FOR_INPUT();
}

#if XSize>=22
    #define POWER_UP_X 10
#elif XSize>=20
    #define POWER_UP_X 9
#else
    #define POWER_UP_X 10
#endif


void display_power_up_counter(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(POWER_UP_X+1,0,2,powerUp);
}

#if XSize>=32
    #define TANK_COUNTER_X (POWER_UP_X+4)
    
    void display_enemy_counter(void)
    {
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(TANK_COUNTER_X+1,0,3,light_tanks_to_kill+heavy_tanks_to_kill);
    }
#else
    #define display_enemy_counter()
#endif


void display_remaining_arrows(void)
{
    uint8_t color;
    
    if(remaining_arrows<20)
    {
        color = _XL_RED;
    }
    else
    {
        color = _XL_WHITE;
    }
    _XL_SET_TEXT_COLOR(color);
    _XL_PRINTD(7,0,2,remaining_arrows);
}


void recharge_arrows(uint8_t value)
{
    remaining_arrows+=value;
    if(remaining_arrows>MAX_ARROWS)
    {
        remaining_arrows=MAX_ARROWS;
    }
    display_remaining_arrows();
}


void display_score(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(0,0,5,score);
}

#define LIVES_X (XSize-3)

#if !defined(_XL_NO_TEXT_COLOR)
    void display_lives(uint8_t color)
    {
        _XL_DRAW(LIVES_X,POWER_UPS_Y,bow_tile[4+0+bow_shape_tile],_XL_CYAN);
        _XL_DRAW(LIVES_X+1,POWER_UPS_Y,bow_tile[1+4+bow_shape_tile],_XL_CYAN);
        _XL_SET_TEXT_COLOR(color);
        _XL_PRINTD(LIVES_X+2,POWER_UPS_Y,1,lives);
    }
#else
    #define display_lives(color) \
    { \
        _XL_DRAW(LIVES_X,POWER_UPS_Y,bow_tile[4+0+bow_shape_tile],_XL_CYAN); \
        _XL_DRAW(LIVES_X+1,POWER_UPS_Y,bow_tile[1+4+bow_shape_tile],_XL_CYAN); \
        _XL_PRINTD(LIVES_X+2,POWER_UPS_Y,1,lives); \
    } 
#endif


#if !defined(_XL_NO_COLOR)
    #define _extra_life_color_effect(color) display_lives(color) 
#else
    #define _extra_life_color_effect(color)
#endif


void display_bow(void)
{
    _XL_DRAW((bow_x>>1),BOW_Y,bow_tile[4*loaded_bow+0+bow_shape_tile],bow_color);
    _XL_DRAW((bow_x>>1)+1,BOW_Y,bow_tile[1+4*loaded_bow+bow_shape_tile],bow_color);  
}



void display_tank(void)
{
    uint8_t status = tank_shape[tank_x];
    uint8_t pos = tank_y[tank_x];
    uint8_t color;
    uint8_t tile0;

    tile0 = HEAVY_TANK_TILE_0;

    if(tank_level[tank_x]==1)
    {
        color = _XL_GREEN;
    }
    else if(tank_level[tank_x]==2)
    {
        if(!freeze)
        {
            tile0 = TANK_DEATH_TILE;
            color = _XL_YELLOW;
        }
    }
    else if(!tank_level[tank_x])
    {
        tile0 = LIGHT_TANK_TILE_0;
        color = _XL_WHITE;
    }
    else 
    {
        color = _XL_RED;
    }
    if(freeze)
    {
        color = _XL_CYAN;  
    }

    if(!status)
    {
        _XL_DELETE(tank_x, tank_y[tank_x]-1);
        _XL_DRAW(tank_x, pos, tile0, color);
    }
    else
    {
        #if !defined(_XL_NO_UDG)
        uint8_t tile1;

        if(!tank_level[tank_x])
        {
            tile0 = tank_tile[status<<1];
            tile1 = tank_tile[1+(status<<1)];
        }
        else
        {
            tile0 = heavy_tank_tile[status<<1];
            tile1 = heavy_tank_tile[1+(status<<1)]; 
        }
        _XL_DRAW(tank_x, pos, tile0, color);
        _XL_DRAW(tank_x,1 + pos, tile1, color);
        #else
        // Avoid using the upper border / beam tile in ASCII mode

        if(!tank_level[tank_x])
        {
            
            tile0 = LIGHT_TANK_TILE_0;
        }
        else
        {
            if((tank_y[tank_x])&1)
            {
                tile0 = HEAVY_TANK_TILE_0;
            }
            else
            {
                tile0 = HEAVY_TANK_TILE_1;
            }
        }
        _XL_DRAW(tank_x, pos, tile0, color);
        #endif
    }
}


#define handle_extra_life() \
    do \
    { \
        uint8_t i; \
        if(score>=next_threshold) \
        { \
            if(lives<MAX_LIVES) \
            { \
                ++lives; \
            } \
            next_threshold+=NEXT_EXTRA_LIFE; \
            _XL_PING_SOUND(); \
            \
            for(i=0;i<16;++i) \
            { \
                bow_color = _XL_YELLOW; \
                display_bow(); \
                _extra_life_color_effect(_XL_RED); \
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR); \
                bow_color = _XL_CYAN; \
                display_bow(); \
                _extra_life_color_effect(_XL_YELLOW); \
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR); \
            } \
            _XL_PING_SOUND(); \
            display_lives(_XL_WHITE); \
        } \
    } while(0)


void increase_score(uint8_t value)
{
    score+=value;
    display_score();
    handle_extra_life();
}


void recharge_effect(void)
{
    recharge_arrows(ARROW_RECHARGE);
    increase_score(RECHARGE_POINTS);
}


#define RANGE_X 0

#if XSize <= 15
    #define RANGE_STRING _XL_R 
    #define SPEED_STRING _XL_S 
    #define POWER_STRING _XL_P
    #define STR_LEN 1
    #define SPEED_X 3
    #define POWER_X ((XSize)-5)

#elif XSize <= 19
    #define RANGE_STRING _XL_R _XL_N
    #define SPEED_STRING _XL_S _XL_P
    #define POWER_STRING _XL_P _XL_W
    #define STR_LEN 2
    #define SPEED_X 4
    #define POWER_X ((XSize)-6)
#elif XSize <= 26
    #define RANGE_STRING _XL_R _XL_N _XL_G
    #define SPEED_STRING _XL_S _XL_P _XL_D
    #define POWER_STRING _XL_P _XL_O _XL_W
    #define STR_LEN 3
    #define SPEED_X 5
    #define POWER_X ((XSize)-7)
#else
    #define RANGE_STRING _XL_R _XL_A _XL_N _XL_G _XL_E
    #define SPEED_STRING _XL_S _XL_P _XL_E _XL_E _XL_D
    #define POWER_STRING _XL_P _XL_O _XL_W _XL_E _XL_R
    #define STR_LEN 5    
    #if XSize>=32
        #define SPEED_X 9
    #else
        #define SPEED_X 8
    #endif
    #if XSize>=32
        #define POWER_X ((XSize)-10)
    #else
        #define POWER_X ((XSize)-9)
    #endif
#endif 

#if XSize>=32
    #define ARROWS_X POWER_X-6
#else
    #define ARROWS_X POWER_X-4
#endif


uint8_t find_inactive(Item* itemArray)
{
    uint8_t i;
    
    for(i=0;i<MAX_NUMBER_OF_MISSILES;++i)
    {
        if(!itemArray[i]._active)
        {
            return i;
        }
    }
    return MAX_NUMBER_OF_MISSILES;
}


#if !defined(_XL_NO_COLOR) && !defined(_XL_NO_TEXT_COLOR)
void display_power_ups(void)
{
    uint8_t range_color;
    uint8_t speed_color;
    uint8_t color;

    uint8_t power_color;
    
    uint8_t i;
    
    speed_color = _XL_RED;
    arrow_display_color = _XL_CYAN;
    power_color = _XL_RED;
    
    if(powerUp<3) // range
    {
        range_color = power_up_color[powerUp];
    }
    else
    {
        range_color = _XL_GREEN;

        if(powerUp<5) // speed
        {
            speed_color = power_up_color[powerUp-2];
        }
        else
        {
            speed_color = _XL_GREEN;
    
            if(powerUp>6)
            {
                if(powerUp<9)
                {
                    power_color = power_up_color[powerUp-6];
                    arrow_display_color = arrow_color[powerUp-6];
                }
                else
                {
                    power_color = _XL_GREEN;
                    arrow_display_color = _XL_WHITE;
                }
            }
        }
    }

    _XL_SET_TEXT_COLOR(range_color);
    _XL_PRINT(RANGE_X,POWER_UPS_Y,RANGE_STRING);
    
    _XL_SET_TEXT_COLOR(speed_color);
    _XL_PRINT(SPEED_X,POWER_UPS_Y,SPEED_STRING);
    
    _XL_SET_TEXT_COLOR(power_color);
    _XL_PRINT(POWER_X,POWER_UPS_Y,POWER_STRING);

    for(i=0;i<3;++i)
    {
        if(i<=number_of_arrows_per_shot-1)
        {
           color = arrow_display_color;
        }
        else
        {
           color = _XL_RED;
        }
        _XL_DRAW(ARROWS_X+i,POWER_UPS_Y,ARROW_TILE_0,color);
    }
}
#elif !defined(_XL_NO_COLOR) // COLOR but NO TEXT COLOR
void display_power_ups(void)
{
    uint8_t range_value;
    uint8_t speed_value;
    uint8_t power_value;
    uint8_t color;
    
    uint8_t i;
    
    arrow_display_color = _XL_CYAN;    
    speed_value = 1;
    power_value = 1;
    
    if(powerUp<3) // range
    {
        range_value = powerUp+1;
    }
    else
    {
        range_value = 3;

        if(powerUp<5) // speed
        {
            speed_value = powerUp+1-2;
        }
        else
        {
            speed_value = 3;
    
            if(powerUp>6)
            {
                if(powerUp<9)
                {
                    power_value = powerUp+1-6;
                    arrow_display_color = arrow_color[powerUp-6];
                }
                else
                {
                    power_value = 3;
                    arrow_display_color = _XL_WHITE;
                }
            }
        }
    }

    _XL_PRINT(RANGE_X,POWER_UPS_Y,RANGE_STRING);
    _XL_PRINTD(RANGE_X+STR_LEN,POWER_UPS_Y,1,range_value);
    
    _XL_PRINT(SPEED_X,POWER_UPS_Y,SPEED_STRING);
    _XL_PRINTD(SPEED_X+STR_LEN,POWER_UPS_Y,1,speed_value);
    
    _XL_PRINT(POWER_X,POWER_UPS_Y,POWER_STRING);
    _XL_PRINTD(POWER_X+STR_LEN,POWER_UPS_Y,1,power_value);

    for(i=0;i<3;++i)
    {
        if(i<=number_of_arrows_per_shot-1)
        {
           color = arrow_display_color;
        }
        else
        {
           color = _XL_RED;
        }
        _XL_DRAW(ARROWS_X+i,POWER_UPS_Y,ARROW_TILE_0,color);
    }
}
#else // NO COLOR and NO TEXT COLOR
void display_power_ups(void)
{
    uint8_t range_value;
    uint8_t speed_value;
    uint8_t power_value;
    
    uint8_t i;
    
    speed_value = 1;
    power_value = 1;
    
    if(powerUp<3) // range
    {
        range_value = powerUp+1;
    }
    else
    {
        range_value = 3;

        if(powerUp<5) // speed
        {
            speed_value = powerUp+1-2;
        }
        else
        {
            speed_value = 3;
    
            if(powerUp>6)
            {
                if(powerUp<9)
                {
                    power_value = powerUp+1-6;
                }
                else
                {
                    power_value = 3;
                }
            }
        }
    }

    _XL_PRINT(RANGE_X,POWER_UPS_Y,RANGE_STRING);
    _XL_PRINTD(RANGE_X+STR_LEN,POWER_UPS_Y,1,range_value);
    
    _XL_PRINT(SPEED_X,POWER_UPS_Y,SPEED_STRING);
    _XL_PRINTD(SPEED_X+STR_LEN,POWER_UPS_Y,1,speed_value);
    
    _XL_PRINT(POWER_X,POWER_UPS_Y,POWER_STRING);
    _XL_PRINTD(POWER_X+STR_LEN,POWER_UPS_Y,1,power_value);

    for(i=0;i<number_of_arrows_per_shot;++i)
    {
        _XL_DRAW(ARROWS_X+i,POWER_UPS_Y,ARROW_TILE_0,_XL_CYAN);
    }
}
#endif


#define activate_hyper() \
{ \
    _XL_ZAP_SOUND(); \
    bow_reload_loops=HYPER_SPEED_VALUE; \
    recharge_arrows(HYPER_RECHARGE); \
    if(powerUp>10) \
    { \
        number_of_arrows_per_shot=3; \
    } \
    else \
    { \
        number_of_arrows_per_shot=2; \
    } \
    hyper_counter = MAX_HYPER_COUNTER; \
    bow_color = _XL_RED; \
    _XL_SET_TEXT_COLOR(_XL_RED); \
    PRINT_CENTERED_ON_ROW(1," HYPER "); \
}


void power_up_effect(void)
{
    uint8_t pmod10;
    
    ++powerUp;
    
    pmod10 = powerUp%10;    

    #if defined(BUGGY_MOD10)
        if(pmod10==0)
        {
            activate_hyper();
            #if !defined(_XL_NO_COLOR)
            powerUpItem._color = _XL_WHITE;
            #endif
        } 
        else if(pmod10==4)
        {
            #if !defined(_XL_NO_COLOR)
            powerUpItem._color = FREEZE_COLOR; 
            #endif
        } 
        else if(pmod10==5)
        {
            freeze_locked=0;
            #if !defined(_XL_NO_COLOR)
            powerUpItem._color = _XL_WHITE;
            #endif
        }  
        else if(pmod10==9)
        {
            #if !defined(_XL_NO_COLOR)
            powerUpItem._color = _XL_RED;
            #endif
        }
    #else
        switch(pmod10)
        {
            case 0:
                activate_hyper();
                #if !defined(_XL_NO_COLOR)
                powerUpItem._color = _XL_WHITE;
                #endif
            break;
            
            case 4:
                #if !defined(_XL_NO_COLOR)
                powerUpItem._color = FREEZE_COLOR; 
                #endif
            break;
            
            case 5:
                freeze_locked=0;
                #if !defined(_XL_NO_COLOR)
                powerUpItem._color = _XL_WHITE;
                #endif
            break;
            
            case 9:
                #if !defined(_XL_NO_COLOR)
                powerUpItem._color = _XL_RED;
                #endif
            break;
        } 
    #endif
    
    display_power_up_counter();
    increase_score(POWERUP_POINTS);
    
    switch(powerUp)
    {
        #if !defined(TRAINER)
        case 1:
            arrow_range=YELLOW_RANGE_VALUE;
        break;
            
        case 2:
            arrow_range=GREEN_RANGE_VALUE;
        break;
        
        case 3:
            bow_reload_loops=YELLOW_SPEED_VALUE;
        break;
           
        case 4:
            bow_reload_loops=GREEN_SPEED_VALUE;
        break;

        case 5:
            fire_power = YELLOW_FIRE_POWER_VALUE;
        break;
        
        case 6:
            fire_power = GREEN_FIRE_POWER_VALUE;
        break;
        
        // case 7:
            // number_of_arrows_per_shot = 2;
        // break;
       
        #endif
        
        case 17:
            #if !defined(_XL_NO_COLOR)
            powerUpItem._color = SECRET_COLOR;
            #endif
        break;
        
        case 18:
            secret_locked = 0;
			powerUpItem._color = _XL_WHITE;
        break;
		
        default:
        break;
    }
    display_power_ups();
}

void extra_points_effect(void)
{
    increase_score(EXTRA_POINTS);
}

/*
void wall_effect(void)
{
    uint8_t i;
    
    increase_score(WALL_POINTS);

#if defined(SMALL_WALL)
    for(i=7*(XSize)/16;i<1+7*(XSize)/16+(XSize)/8;++i)
#else    
    for(i=3*(XSize)/8;i<1+3*(XSize)/8+(XSize)/4;++i)
#endif
    {
        if(tank_y[i]<WALL_Y-1 || !tank_active[i])
        {
            wall[i]=WALL_ENERGY;
            _XL_DRAW(i,WALL_Y,WALL_TILE,WALL_COLOR);            
        }
        else
        {
            wall[i]=0;
        }
    }
    ++wall_appeared;
    active_wall = 1;
}
*/


void display_tanks(void)
{
    for(tank_x=0;tank_x<XSize;++tank_x)
    {
        if(tank_active[tank_x])
        {
            display_tank();
        }
    }
}


void freeze_effect(void)
{
    freeze=FREEZE_COUNTER_MAX;

    display_tanks();
    increase_score(FREEZE_POINTS);
    ++freeze_locked;
}


void tank_effect(void)
{
    for(tank_x=0;tank_x<XSize;++tank_x)
    {
        tank_level[tank_x]=1;
    }
    display_tanks();

    secret_locked = 1;
    increase_score(SECRET_ITEM_POINTS);
}

void beam_effect(void)
{
    alive=0;
}

#if !defined(_XL_NO_COLOR)
    #define initialize_items() \
    { \
        uint8_t i; \
        \
        rechargeItem._active = 0; \
        rechargeItem._tile = ARROW_TILE_0; \
        rechargeItem._color = _XL_YELLOW; \
        rechargeItem._effect = recharge_effect; \
        \
        freezeItem._active = 0; \
        freezeItem._tile = FREEZE_TILE; \
        freezeItem._color = FREEZE_COLOR; \
        freezeItem._effect = freeze_effect; \
        \
        powerUpItem._active = 0; \
        powerUpItem._tile = POWER_UP_TILE; \
        powerUpItem._color = _XL_WHITE; \
        powerUpItem._effect = power_up_effect; \
        \
        secretItem._active = 0; \
        secretItem._tile = SECRET_TILE; \
        secretItem._color = SECRET_COLOR; \
        secretItem._effect = tank_effect; \
        \
        for(i=0;i<MAX_NUMBER_OF_MISSILES;++i) \
        { \
            enemyMissile[i]._active = 0; \
            enemyMissile[i]._tile = BEAM_TILE; \
            enemyMissile[i]._color = _XL_CYAN; \
            enemyMissile[i]._effect = beam_effect; \
            \
            extraPointsItem[i]._active = 0; \
            extraPointsItem[i]._tile = EXTRA_POINTS_TILE; \
            extraPointsItem[i]._color = _XL_YELLOW; \
            extraPointsItem[i]._effect = extra_points_effect; \
        } \
    }
#else
    #define initialize_items() \
    { \
        uint8_t i; \
        \
        rechargeItem._active = 0; \
        rechargeItem._tile = ARROW_TILE_0; \
        rechargeItem._effect = recharge_effect; \
        \
        freezeItem._active = 0; \
        freezeItem._tile = FREEZE_TILE; \
        freezeItem._effect = freeze_effect; \
        \
        powerUpItem._active = 0; \
        powerUpItem._tile = POWER_UP_TILE; \
        powerUpItem._effect = power_up_effect; \
        \
        secretItem._active = 0; \
        secretItem._tile = HEAVY_TANK_TILE_0; \
        secretItem._effect = tank_effect; \
        \
        for(i=0;i<MAX_NUMBER_OF_MISSILES;++i) \
        { \
            enemyMissile[i]._active = 0; \
            enemyMissile[i]._tile = BEAM_TILE; \
            enemyMissile[i]._effect = beam_effect; \
            \
            extraPointsItem[i]._active = 0; \
            extraPointsItem[i]._tile = EXTRA_POINTS_TILE; \
            extraPointsItem[i]._effect = extra_points_effect; \
        } \
    }
#endif

#define display_level() \
{ \
    _XL_SET_TEXT_COLOR(_XL_YELLOW); \
    _XL_PRINTD(XSize-1,0,1,level+1); \
}



#define move_left() \
{ \
    bow_shape_tile = 2*((--bow_x)&1); \
    if(bow_shape_tile) \
    { \
        _XL_DELETE((bow_x>>1)+2,BOW_Y); \
    } \
    display_bow(); \
}


#define move_right() \
{ \
    bow_shape_tile = 2*((++bow_x)&1); \
    if(!bow_shape_tile) \
    { \
        _XL_DELETE((bow_x>>1)-1,BOW_Y); \
    } \
    display_bow(); \
}


void drop_item(register Item *item, uint8_t max_counter)
{
    uint8_t offset;

    offset = tank_y[tank_x]+1;
    
    _XL_TICK_SOUND();
    item->_active = 1;
    item->_x = tank_x;

    if(tank_shape[tank_x])
    {
        ++offset;
    }
    item->_y = offset;
    
    item->_counter=max_counter;
}


void handle_item(register Item* item)
{
    if(item->_active)
    {
        // TODO: Necessary for GCC for TI99
        uint8_t item_tile = item->_tile;
        
        if(item->_y<BOW_Y)
        {
            _XL_DELETE(item->_x,item->_y);
            if(main_loop_counter&1)
            {
                ++(item->_y);
            }
                  
            #if !defined(_XL_NO_COLOR)
                    // TODO: GCC for TI99 does not display the correct tile with item->_tile
                    _XL_DRAW(item->_x,item->_y,item_tile,item->_color);
            #else
                _XL_DRAW(item->_x,item->_y,item_tile,0);
            #endif
        }
        else
        {
            if(item->_counter&1)
            {
                _XL_DELETE(item->_x,item->_y);
            }
            else
            {
                #if !defined(_XL_NO_COLOR)
                _XL_DRAW(item->_x,item->_y,item_tile,item->_color);
                #else
                _XL_DRAW(item->_x,item->_y,item_tile,0);
                #endif
            }
 
            if(item->_x==(bow_x>>1)+(bow_x&1))
            {
                item->_effect();
                _XL_PING_SOUND();
                item->_active=0;
            }
            --(item->_counter);
            if(!(item->_counter))
            {
                _XL_DRAW(item->_x,item->_y,EXPLOSION_TILE,item->_color);
				_XL_TICK_SOUND();
				_XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);

                item->_active=0;
                _XL_DELETE(item->_x,item->_y);
				
            }
			display_bow();
        }
    }   
}


#define handle_items() \
{ \
    uint8_t i; \
    \
    handle_item(&rechargeItem); \
    handle_item(&freezeItem); \
    handle_item(&powerUpItem); \
    handle_item(&secretItem); \
    \
    for(i=0;i<MAX_NUMBER_OF_MISSILES;++i) \
    { \
        handle_item(&extraPointsItem[i]); \
    } \
    for(i=0;i<MAX_NUMBER_OF_MISSILES;++i) \
    { \
        handle_item(&enemyMissile[i]); \
    } \
}


#if (XSize-2)!=13 && (XSize-2)!=26 && (XSize-2)!=39 && (XSize-2)!=52 && (XSize-2)!=65 && (XSize-2)!=78
    #define STEP 13
#else
    #define STEP 17
#endif 

#if XSize<=40
    #define RANDOM_TANK_RANGE_START (31)
#else
    #define RANDOM_TANK_RANGE_START (63)
#endif

uint8_t find_random_tank(uint8_t value)
{
    uint8_t i;
    uint8_t index;
    
    // index = (uint8_t) (_XL_RAND())&RANDOM_TANK_RANGE_START;

    // for(i=1;i<XSize-1;++i)
    // {
        // index = 1+(index+STEP)%(XSize-2);
        // if(tank_active[index]==value)
        // {
            // return index;
        // }
    // }
	
    index = (uint8_t) (_XL_RAND())%(XSize-2);

    for(i=1;i<XSize-1;++i)
    {
        index%=(XSize-2);
		++index;
        if(tank_active[index]==value)
        {
            return index;
        }
    }	
	
	// while(1){}; // TODO: Only for debugging
    return 1;
}


void activate_tank(void)
{
    uint8_t old_x;
    
    old_x = tank_x;
    
    while((old_x==tank_x) || (old_x+1==tank_x) || (old_x-1==tank_x))
    {
        tank_x = find_random_tank(0);
		
		// TODO: Is this possible?
		// if(tank_x==XSize)
		// {
			// return;
		// }
    };    
  
    #if YSize<=16
        tank_y[tank_x]=INITIAL_RESPAWN_TANK_Y;
		// tank_y[tank_x]=INITIAL_TANK_Y;

    #else
        tank_y[tank_x]=INITIAL_RESPAWN_TANK_Y+(level>>1);
		// tank_y[tank_x]=INITIAL_TANK_Y;

    #endif
	
	_XL_DRAW(tank_x, tank_y[tank_x], TANK_DEATH_TILE, _XL_WHITE);
	_XL_TOCK_SOUND();
	_XL_SLOW_DOWN(3*_XL_SLOW_DOWN_FACTOR);
	tank_active[tank_x]=1;    
    tank_shape[tank_x]=0;
}


void spawn_light_tank(void)
{
    activate_tank();
    tank_level[tank_x]=0;
    energy[tank_x]=LIGHT_TANK_ENERGY;  
    --light_tanks_to_spawn;
}


void spawn_heavy_tank(void)
{
    uint8_t rank;
    
    do
    {
        if(!level) // 0
        {
            rank = 1;
        }
        else if(level==1) // 1
        {
            rank = (uint8_t) (1 + ((_XL_RAND())&1));
        }
        else if(level<4) // 2, 3
        {
            rank = (uint8_t) (1 + ((_XL_RAND())%3));   
        }
        else // 4, 5
        {
            rank = (uint8_t) (2 + ((_XL_RAND())&1)); 
        }
    } while((rank==2)&&(heavy_tanks_to_spawn<LEVEL_2_TANK_THRESHOLD));

    activate_tank();
    tank_level[tank_x]=rank;
    energy[tank_x]=rank_energy[rank];//HEAVY_TANK_BASE_ENERGY+rank*2;
    --heavy_tanks_to_spawn;
}

#if !defined(NORMAL_TANK_SPEED) && !defined(SLOW_TANK_SPEED)
    #if XSize>=32
        #define NORMAL_TANK_SPEED 1
        #define SLOW_TANK_SPEED 7
    #else
        #define NORMAL_TANK_SPEED 3
        #define SLOW_TANK_SPEED 7
    #endif 
#endif

void update_tank_speed(void)
{
    if(light_tanks_to_kill + heavy_tanks_to_kill<=FEW_TANKS)
    {
        tank_speed=SLOW_TANK_SPEED;
    }
    else
    {
        tank_speed=NORMAL_TANK_SPEED;
    }
}


void display_wall(uint8_t y)
{
    uint8_t i;
    
    for(i=0; i<XSize; ++i)
    {   
        _XL_DRAW(i,y,WALL_TILE,WALL_COLOR); 
    }
}


#define _display_red_tank(tile) \
{ \
    _XL_DRAW(tank_x,tank_y[tank_x],tile,_XL_RED); \
}


void display_red_tank(void)
{
    uint8_t tile;
    
    if(!tank_level[tank_x])
    {
        tile=LIGHT_TANK_TILE_0;
    }
    else
    {
        tile=HEAVY_TANK_TILE_0;
    }
    _display_red_tank(tile);
}


void handle_item_drop(void)
{
    if(tank_level[tank_x] || ((uint8_t) (_XL_RAND()) <75))
    {        
        ++item_counter;
        item_counter&=3;
        
        if((level>=HELP_ITEM_LEVEL_THRESHOLD)&&(powerUp<=3))
        {
            item_counter&=1;
        }
        
        if(!item_counter)
        {
            if(!rechargeItem._active)
            {
                drop_item(&rechargeItem,45);
            }
        }
        else if(item_counter==1)
        {
            if(!powerUpItem._active)
            {
                drop_item(&powerUpItem,35);
            } 
        }
        else if((!freeze_locked)&&(!freeze))
        {
            if(!freezeItem._active)
            {
                drop_item(&freezeItem,45);
            }
        }
        // else if(!wall_appeared&&(powerUp>=9)) 
        // {
            // if(!wallItem._active)
            // {
                // drop_item(&wallItem,35);
            // }
        // }
        else if(!secret_locked && !secretItem._active)
            {
                drop_item(&secretItem,50);
            }
        else
        {
            uint8_t index; 
            
            index = find_inactive(extraPointsItem);
            if(index!=NUMBER_OF_EXTRA_POINTS)
            {
                drop_item(&extraPointsItem[index],90);
            }            
        }  
    }
}



void handle_tank_speed_mask(void)
{
	if(heavy_tanks_to_kill<heavy_tanks_on_level[level]/4)
	{
		tank_speed_mask = FAST_TANK_SHOOT_MASK;
	}
}


void respawn(void)
{
    if(light_tanks_to_spawn || heavy_tanks_to_spawn)
    {
        if (light_tanks_to_spawn)
        {
            spawn_light_tank();
        }
        else 
        {
            spawn_heavy_tank();
        }
        display_tank();
    }

    update_tank_speed();
	handle_tank_speed_mask();
}


void tank_dies(void)
{
    uint8_t y_pos = tank_y[tank_x];
    uint8_t i;

    _XL_DELETE(tank_x,y_pos+1);
    
    _XL_DRAW(tank_x,y_pos, TANK_DEATH_TILE, _XL_RED);


    for(i=0;i<12;++i)
    {
        _XL_DRAW(tank_x,y_pos, TANK_DEATH_TILE, _XL_RED);
		_XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR/8);
        display_red_tank();
    } 
    _XL_SHOOT_SOUND();
    _XL_DELETE(tank_x,y_pos);
    
    #if defined(NO_BOTTOM_WALL_REDRAW)
    #else
        display_wall(BOTTOM_WALL_Y);
    #endif
    
    display_bow();
        
    if(tank_level[tank_x])
    {
        ++killed_heavy_tanks;
        --heavy_tanks_to_kill;
    }
    else
    {
        ++killed_light_tanks;
        --light_tanks_to_kill;
    }
   
    if(tank_x==forced_tank_x)
    {
        forced_tank = 0;
    }
    
    if(y_pos<BOW_Y-2)
    {
        handle_item_drop();
    }
    
    tank_active[tank_x]=0;
    
    display_enemy_counter();
}


#define compute_next_available_arrow_index() \
    for(next_arrow=0;next_arrow<MAX_ARROWS_ON_SCREEN;++next_arrow) \
    { \
        if(!active_arrow[next_arrow]) \
        { \
            break; \
        } \
    }


void handle_arrows(void)
{
    uint8_t i;
    
    for(i=0;i<MAX_ARROWS_ON_SCREEN;++i)
    {
        if(active_arrow[i]) // ACTIVE
        {    
            if(arrow_y[i]<(arrow_range))
            {
                active_arrow[i]=0;
                --arrows_on_screen;
            }
            else
            {
                _XL_DELETE(arrow_x[i],arrow_y[i]);
                --arrow_y[i];
                if(arrow_y[i]>=(arrow_range))
                {
                    #if !defined(_XL_NO_COLOR)
                    _XL_DRAW(arrow_x[i],arrow_y[i],arrow_shape[i],arrow_display_color);
                    #else
                    _XL_DRAW(arrow_x[i],arrow_y[i],arrow_shape[i],0);
                    #endif
                }
            }
        }
    }
}


uint8_t tank_hit(void)
{
    uint8_t i;
    
    for(i=0;i<MAX_ARROWS_ON_SCREEN;++i)
    {
        if(active_arrow[i] && arrow_x[i]==tank_x
          && tank_y[tank_x]>=arrow_y[i]-1 && tank_y[tank_x]<=arrow_y[i]+1)
           {
               if(freeze || (tank_level[tank_x]!=2) || tank_shape[tank_x])
               {
                   active_arrow[i]=0;
                    --arrows_on_screen;

                   _XL_DELETE(arrow_x[i],arrow_y[i]);
                   return 1;
               }
               else // Arrows goes through ghost !free (non-frozen) && tank_level==2 (i.e., ghost tank) && !tank_shape (i.e. invincible shape)
               {
                   display_tank(); // display invincible ghost tank
                   return 0; // two arrows cannot be at the same place
               }
           }
    }
    return 0;
}


#define decrease_energy() \
do \
{ \
    if(energy[tank_x]<=fire_power) \
    { \
        energy[tank_x]=0; \
    } \
    else \
    { \
        energy[tank_x]-=fire_power; \
    } \
} while(0)


void push_tank(void)
{
    if(!tank_shape[tank_x])
    {
        --tank_y[tank_x];
        tank_shape[tank_x]=3;
    }
    else 
    {
        --tank_shape[tank_x];
        _XL_DELETE(tank_x,tank_y[tank_x]+1);
    }
}


void handle_tank_collisions(void)
{
    for(tank_x=0;tank_x<XSize;++tank_x)
    {
        if(tank_active[tank_x] && tank_hit())
        {
            decrease_energy();

            if(energy[tank_x])
            {
                display_red_tank();
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                
                _XL_TOCK_SOUND();
                push_tank();
                display_tank();
                #if defined(_XL_NO_UDG)
                _XL_DELETE(tank_x,tank_y[tank_x]+1);
                #endif
            }
            else
            {
                tank_dies();
                increase_score(tank_points[tank_level[tank_x]]);
                respawn();
            }
        }
    }
}





#define _move_tank() \
{ \
    ++tank_shape[tank_x]; \
    (tank_shape[tank_x])&=3; \
    if(!tank_shape[tank_x]) \
        { \
            ++tank_y[tank_x]; \
        } \
}


#if !defined(MISSILE_DROP_LOOP_MASK)
    #define MISSILE_DROP_LOOP_MASK (1)
#endif

#define handle_missile_drops() \
{ \
	if(!freeze && !(main_loop_counter&1)) \
	{ \
		uint8_t missile_index; \
		if((missile_index = find_inactive(enemyMissile)) < MAX_NUMBER_OF_MISSILES) \
		{ \
			\
			tank_x = (bow_x>>1)+(bow_x&1)-1+(_XL_RAND()%3); \
			\
			if(tank_active[tank_x] && tank_y[tank_x]<HEIGHT_SHOOT_THRESHOLD && ((tank_level[tank_x]==3) || (!(main_loop_counter&tank_speed_mask)))) \
			{ \
				drop_item(&enemyMissile[missile_index],1); \
			} \
		} \
	} \
} 


void move_tanks(void)
{    
    if(forced_tank) // There is a tank that should keep on moving
    {
        tank_x = forced_tank_x;
    }
    else
    {
        tank_x=find_random_tank(1);
    }

    // The forced tank is no longer forced when it has completed the move (half-move for stealth tanks)
    if((tank_shape[tank_x]==3)||(((tank_level[tank_x]==2)&&(tank_shape[tank_x]&1))&&(tank_y[tank_x]!=BOW_Y-1)))
    {
        forced_tank = 0;
    }
    else
    {
        if(_XL_RAND()&3)
        {
            forced_tank = 1;
        }
        forced_tank_x = tank_x; 
    }

    _move_tank();
    
    display_tank();

    if((tank_y[tank_x]==BOW_Y))
    {
        alive = 0;
        display_red_tank();
    }
}


// void move_some_tanks(void)
// {
	// uint8_t i;
	
	// for(i=0;i<XSize/2;++i)
	// {
		// move_tanks();
	// }
// }

#define handle_bow_load() \
do \
{ \
    if(!loaded_bow && arrows_on_screen<MAX_ARROWS_ON_SCREEN && !bow_load_counter && remaining_arrows) \
    { \
        loaded_bow = 1; \
        display_bow(); \
    } \
    if(bow_load_counter) \
    { \
        --bow_load_counter; \
    } \
} while(0)


void fire(void)
{
    uint8_t i;
    uint8_t new_arrow_x;  
    uint8_t offset;
    
    _XL_TICK_SOUND();
    new_arrow_x = (bow_x>>1)+(bow_x&1);
    for(i=0;i<number_of_arrows_per_shot;++i)
    {
        if(remaining_arrows && arrows_on_screen<MAX_ARROWS_ON_SCREEN)
        {
            if((number_of_arrows_per_shot==2)&&i)
            {
                offset = i-2*(bow_x&1);
            }
            else
            {
                offset = i;
            }
            new_arrow_x+=offset;
            if(i==2)
            {
                if(new_arrow_x>=4)
                {
                    new_arrow_x-=4; 
                }
                else
                {
                    continue;
                }
            }
            if(new_arrow_x<XSize-1 && new_arrow_x)
            {
                compute_next_available_arrow_index();

                active_arrow[next_arrow] = 1;
                ++arrows_on_screen;
                if(number_of_arrows_per_shot==2)
                {
                    arrow_shape[next_arrow] = arrow_tile[!(bow_x&1)];
                }
                else
                {
                    arrow_shape[next_arrow] = arrow_tile[bow_x&1];
                }
                arrow_y[next_arrow] = BOW_Y-1;
                arrow_x[next_arrow] = new_arrow_x;
                #if !defined(TRAINER)
                    --remaining_arrows;
                #endif
            }
        }
    }
    display_remaining_arrows();

    bow_load_counter = bow_reload_loops;
    loaded_bow = 0;
    display_bow();
}

#if defined(_XL_NO_UDG)
    #define handle_bow_move() \
    do \
    { \
        input = _XL_INPUT(); \
        \
        if(_XL_LEFT(input) && bow_x>1) \
        { \
            move_left(); \
            if(bow_x) \
            { \
                move_left(); \
            } \
        } \
        else if (_XL_RIGHT(input) && bow_x<MAX_BOW_X-1) \
        { \
            move_right(); \
            if(bow_x<MAX_BOW_X) \
            { \
                move_right(); \
            } \
        } \
        else if (_XL_FIRE(input) && loaded_bow) \
        { \
            fire(); \
        } \
    } while(0)
#else
    #define handle_bow_move() \
    do \
    { \
        input = _XL_INPUT(); \
        \
        if(_XL_LEFT(input) && bow_x>1) \
        { \
            move_left(); \
        } \
        else if (_XL_RIGHT(input) && bow_x<MAX_BOW_X-1) \
        { \
            move_right(); \
        } \
        else if (_XL_FIRE(input) && loaded_bow) \
        { \
            fire(); \
        } \
    } while(0)
#endif


#define game_over() \
do \
{ \
    _XL_EXPLOSION_SOUND(); \
    _XL_SET_TEXT_COLOR(_XL_RED); \
    PRINT_CENTERED(_XL_G _XL_A _XL_M _XL_E _XL_SPACE _XL_O _XL_V _XL_E _XL_R); \
    sleep_and_wait_for_input(); \
    _XL_CLEAR_SCREEN(); \
} while(0)


#define global_initialization() \
do \
{ \
    if(score>hiscore) \
    { \
        hiscore=score; \
    } \
    score = 0; \
    level = INITIAL_LEVEL; \
    killed_heavy_tanks = 0; \
    killed_light_tanks = 0; \
    lives = INITIAL_LIVES; \
    next_threshold = NEXT_EXTRA_LIFE; \
} while(0)


#if defined(TRAINER)
    #define level_initialization() \
        do \
        {   \
            fire_power = GREEN_FIRE_POWER_VALUE; \
            freeze = 0; \
            powerUp = 8; \
            next_arrow = 0; \
            arrows_on_screen = 0; \
            bow_load_counter = 0; \
            freeze_locked = 1; \
            secret_locked = 1; \
            forced_tank = 0; \
            loaded_bow = 1; \
            alive = 1; \
            bow_reload_loops = GREEN_SPEED_VALUE; \
            auto_recharge_counter = AUTO_RECHARGE_COOL_DOWN; \
            remaining_arrows = MAX_ARROWS; \
            arrow_range = GREEN_RANGE_VALUE; \
            bow_x = XSize; \
            bow_shape_tile = 2*((bow_x)&1); \
            bow_color = _XL_CYAN; \
            number_of_arrows_per_shot = 3; \
            initialize_items(); \
            hyper_counter = 0; \
            _XL_CLEAR_SCREEN(); \
        } while(0)

#else
    #define level_initialization() \
        do \
        {   \
            fire_power = RED_FIRE_POWER_VALUE; \
            freeze = 0; \
            powerUp = 0; \
            next_arrow = 0; \
            arrows_on_screen = 0; \
            bow_load_counter = 0; \
            hyper_counter = 0; \
            forced_tank = 0; \
            freeze_locked = 1; \
            secret_locked = 1; \
            loaded_bow = 1; \
            alive = 1; \
			if(level>=2) \
			{ \
				max_occupied_columns = MAX_DENSILY_OCCUPIED_COLUMNS; \
			} \
			else \
			{ \
				max_occupied_columns = MAX_OCCUPIED_COLUMNS; \
			} \
            bow_reload_loops = RED_SPEED_VALUE; \
            auto_recharge_counter = AUTO_RECHARGE_COOL_DOWN; \
            remaining_arrows = MAX_ARROWS; \
            arrow_range = RED_RANGE_VALUE; \
            bow_x = XSize; \
            bow_shape_tile = (uint8_t) 2*((bow_x)&1); \
            bow_color = _XL_CYAN; \
            number_of_arrows_per_shot = 1; \
			if(level>=3) \
			{ \
				tank_speed_mask = FAST_TANK_SHOOT_MASK; \
			} \
			else \
			{ \
				tank_speed_mask = SLOW_TANK_SHOOT_MASK; \
			} \
            initialize_items(); \
            _XL_CLEAR_SCREEN(); \
			_XL_DRAW(0,HEIGHT_SHOOT_THRESHOLD,WALL_TILE,WALL_COLOR); \
			_XL_DRAW(XSize-1,HEIGHT_SHOOT_THRESHOLD,WALL_TILE,WALL_COLOR); \
        } while(0)

#endif

#define tank_counter main_loop_counter

void initialize_tank_at_level_restart(void)
{
    // tank_y[tank_x]=INITIAL_TANK_Y;
    ++tank_counter;
    display_tank();
    // _XL_TOCK_SOUND();
}

#define reset_tanks() \
do \
{ \
    for(tank_x=0;tank_x<XSize;++tank_x) \
    { \
        tank_active[tank_x]=0; \
    } \
} while(0)    

#define spawn_initial_light_tanks() \
    light_tanks_to_kill = LIGHT_TANKS_ON_FIRST_LEVEL-killed_light_tanks;  \
    \
    if(light_tanks_to_kill<max_occupied_columns) \
    { \
        to_spawn_initially=light_tanks_to_kill; \
    } \
    else \
    { \
        to_spawn_initially=max_occupied_columns; \
    } \
    \
    tank_counter = 0; \
    \
    while(tank_counter<to_spawn_initially) \
    { \
        spawn_light_tank(); \
        initialize_tank_at_level_restart(); \
    } \
    \
    light_tanks_to_spawn = light_tanks_to_kill-to_spawn_initially;

#define spawn_initial_heavy_tanks() \
    heavy_tanks_to_kill = heavy_tanks_on_level[level]-killed_heavy_tanks; \
    \
    if(heavy_tanks_to_kill<max_occupied_columns - to_spawn_initially) \
    { \
        to_spawn_initially = heavy_tanks_to_kill;   \
    } \
    else \
    { \
        to_spawn_initially = max_occupied_columns - to_spawn_initially; \
    } \
    \
    tank_counter = 0; \
    \
    while(tank_counter<to_spawn_initially) \
    { \
        spawn_heavy_tank(); \
        initialize_tank_at_level_restart(); \
    } \
    \
    heavy_tanks_to_spawn = heavy_tanks_to_kill-to_spawn_initially;


#define tank_initialization() \
{ \
    uint8_t to_spawn_initially; \
    \
    reset_tanks(); \
    \
    spawn_initial_light_tanks(); \
    \
    spawn_initial_heavy_tanks(); \
    \
    update_tank_speed(); \
	\
    \
    for(tank_x=0;tank_x<MAX_ARROWS_ON_SCREEN;++tank_x) \
    { \
        active_arrow[tank_x] = 0; \
    } \
}


#if YSize<=20
    #define _NEXT_ROW i
#else
    #define _NEXT_ROW ((i)<<1)
#endif

#if defined(_XL_NO_JOYSTICK)
    #define CONTROLS_STRING "USE J L SPACE"
	#define CONTROLS_LEN 13
#else
    #if defined(_XL_CURSORS)
        #if XSize>=21
            #define CONTROLS_STRING "USE CURSORS AND SPACE"
			#define CONTROLS_LEN 21
        #elif XSize>=19
            #define CONTROLS_STRING "USE CURSORS AND SPC"
			#define CONTROLS_LEN 19
        #else
            #define CONTROLS_STRING "CURSORS AND SPC"
			#define CONTROLS_LEN 15
        #endif
    #else
        #define CONTROLS_STRING "USE JOYSTICK"
		#define CONTROLS_LEN 12
    #endif
#endif

#if YSize<=23
    #define CONTROLS_Y YSize-2
#else
    #define CONTROLS_Y YSize-3
#endif

#if !defined(NO_EXTRA_TITLE)
    #if !defined(NO_CONTROL_INSTRUCTIONS) && YSize>=15
        #define control_instructions() PRINT_CENTERED_ON_ROW(CONTROLS_Y,\
                                       CONTROLS_STRING)
    #else
        #define control_instructions()
    #endif

    #define display_items() \
    do \
    { \
        uint8_t i; \
        \
        for(i=0;i<NUMBER_OF_ITEMS;++i) \
        { \
            _XL_DRAW(XSize/2-5,YSize/3+4+_NEXT_ROW, item_tile[i][0], item_tile[i][1]); \
            _XL_SET_TEXT_COLOR(_XL_GREEN); \
            _XL_PRINT(XSize/2-5+3,YSize/3+4+_NEXT_ROW, (char *)item_name[i]); \
        } \
		_XL_SET_TEXT_COLOR(_XL_YELLOW); \
        control_instructions(); \
    } while(0)
		
	void display_enemies(void)
	{
		uint8_t i;
		
		for(i=0;i<4;++i)
		{
			_XL_DRAW(XSize/2-5,YSize/3+3+_NEXT_ROW, enemy_tile[i][0], enemy_tile[i][1]);
			_XL_SET_TEXT_COLOR(_XL_GREEN);
			_XL_PRINT(XSize/2-5+3,YSize/3+3+_NEXT_ROW, (char *)enemy_name[i]);
		}
	}
#else
    #define display_items()
	#define display_enemies()
#endif





#if YSize<=22
    #define _HISCORE_Y 1
#else
    #define _HISCORE_Y 2
#endif

#if XSize>=20
    #define _CROSS_HORDE_STRING \
        "S T I N G E R"
#else
    #define _CROSS_HORDE_STRING \
        "STINGER"
#endif


#define display_initial_screen() \
{ \
    _XL_CLEAR_SCREEN(); \
    \
    display_wall(0); \
    display_wall(BOTTOM_WALL_Y+1); \
    \
    _XL_SET_TEXT_COLOR(_XL_CYAN); \
    PRINT_CENTERED_ON_ROW(_HISCORE_Y, "HISCORE"); \
    \
    _XL_SET_TEXT_COLOR(_XL_WHITE); \
    _XL_PRINTD(XSize/2-3,_HISCORE_Y+1,5,hiscore); \
    \
    _XL_SET_TEXT_COLOR(_XL_RED); \
    PRINT_CENTERED_ON_ROW(YSize/3-2,_CROSS_HORDE_STRING); \
    \
    _XL_SET_TEXT_COLOR(_XL_WHITE); \
    PRINT_CENTERED_ON_ROW(YSize/3, "FABRIZIO CARUSO"); \
    \
    display_items(); \
    sleep_and_wait_for_input(); \
}


void display_second_screen() 
{
    _XL_CLEAR_SCREEN();
    
    display_wall(0);
    display_wall(BOTTOM_WALL_Y+1);
	_XL_SET_TEXT_COLOR(_XL_CYAN);
	PRINT_CENTERED_ON_ROW(YSize/3, "ENEMY ARMOR");
	display_enemies();
    sleep_and_wait_for_input(); \
    _XL_CLEAR_SCREEN(); \
}


#if XSize>=23
    #define HI_X ((XSize-9))
    #define HISCORE_STRING _XL_H _XL_I
#else
    #define HI_X ((XSize-8))
    #define HISCORE_STRING _XL_H
#endif

#if XSize>=32
    #define draw_tank_counter_tile() _XL_DRAW(TANK_COUNTER_X,0,LIGHT_TANK_TILE_0, _XL_WHITE)
#else
    #define draw_tank_counter_tile()
#endif


#if XSize>=20
    #define display_stats() \
    do \
    { \
        _XL_SET_TEXT_COLOR(_XL_WHITE); \
        display_score(); \
        _XL_SET_TEXT_COLOR(_XL_GREEN); \
        _XL_CHAR(HI_X,0,'H'); \
        _XL_SET_TEXT_COLOR(_XL_WHITE); \
        _XL_PRINTD(HI_X+1,0,5, hiscore); \
        _XL_DRAW(6,0,ARROW_TILE_1,_XL_CYAN); \
        _XL_DRAW(POWER_UP_X,0,POWER_UP_TILE, _XL_WHITE); \
        draw_tank_counter_tile(); \
        display_remaining_arrows(); \
        display_power_up_counter(); \
        display_level(); \
        display_lives(_XL_WHITE); \
        display_power_ups(); \
        display_enemy_counter(); \
    } while(0)
#else
    #define display_stats() \
    do \
    { \
        _XL_SET_TEXT_COLOR(_XL_WHITE); \
        display_score(); \
        _XL_DRAW(6,0,ARROW_TILE_1,_XL_CYAN); \
        _XL_DRAW(POWER_UP_X,0,POWER_UP_TILE, _XL_WHITE); \
        draw_tank_counter_tile(); \
        display_remaining_arrows(); \
        display_power_up_counter(); \
        display_level(); \
        display_lives(_XL_WHITE); \
        display_power_ups(); \
        display_enemy_counter(); \
    } while(0)
#endif



#define handle_auto_recharge() \
do \
{ \
    if(!remaining_arrows) \
    { \
        if(auto_recharge_counter) \
        { \
            --auto_recharge_counter; \
        } \
        else \
        { \
            recharge_arrows(AUTO_ARROW_RECAHRGE); \
            auto_recharge_counter=AUTO_RECHARGE_COOL_DOWN; \
            _XL_PING_SOUND(); \
        } \
    } \
} while(0)


#define display_level_at_start_up()  \
do \
{ \
    _XL_SET_TEXT_COLOR(_XL_YELLOW); \
    if(level==LAST_LEVEL) \
    { \
        _XL_PRINT(XSize/2-4, YSize/2,  "F I N A L"); \
    } \
    _XL_SLEEP(1); \
    _XL_SET_TEXT_COLOR(_XL_CYAN); \
    _XL_PRINT(XSize/2-4, YSize/2,      "LEVEL    " ); \
    _XL_PRINTD(XSize/2+2,YSize/2,1,level+1); \
    sleep_and_wait_for_input(); \
    _XL_PRINT(XSize/2-4, YSize/2,_XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE); \
} while(0)


#define handle_tank_movement() \
    if(!freeze) \
    { \
        if(!(main_loop_counter&tank_speed)) \
        { \
            move_tanks(); \
        } \
    } \
    else \
    { \
        --freeze; \
    }

void display_cleared(void)
{
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    PRINT_CENTERED(_XL_C _XL_SPACE _XL_L _XL_SPACE _XL_E _XL_SPACE _XL_A _XL_SPACE _XL_R _XL_SPACE _XL_E _XL_SPACE _XL_D);
}


#define handle_next_level() \
do \
{ \
    ++level; \
    display_cleared(); \
    sleep_and_wait_for_input(); \
    _XL_TOCK_SOUND(); \
    increase_score(LEVEL_BONUS); \
    if(powerUp) \
    { \
        do \
        { \
            --powerUp; \
            increase_score(POWER_UP_BONUS); \
            display_power_up_counter(); \
            _XL_TICK_SOUND(); \
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR*4); \
        } while(powerUp); \
    } \
    \
    sleep_and_wait_for_input(); \
    killed_heavy_tanks = 0; \
    killed_light_tanks = 0; \
} while(0)


#define player_dies() \
do \
{ \
    --lives; \
    bow_color=_XL_RED; \
    display_bow(); \
    bow_color=_XL_CYAN; \
    _XL_EXPLOSION_SOUND(); \
    sleep_and_wait_for_input(); \
} while(0)


void tank_animation(void)
{
    tank_y[tank_x]=YSize/2-7+(uint8_t) ((_XL_RAND())&15);
    tank_level[tank_x]=0;
    display_tank();
    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    tank_dies();
    display_cleared();
}


#define victory() \
do \
{ \
    uint8_t i; \
    \
    light_tanks_to_kill = XSize*2; \
    display_enemy_counter(); \
    for(i=0;i<XSize;++i) \
    { \
        tank_x = i; \
        tank_animation(); \
        tank_x = XSize-i; \
        tank_animation(); \
    } \
    sleep_and_wait_for_input(); \
    _XL_CLEAR_SCREEN(); \
} while(0)


#define display_level_screen() \
do \
{ \
    display_wall(BOTTOM_WALL_Y); \
    display_bow(); \
    display_stats(); \
    display_level_at_start_up(); \
} while(0)


#define handle_hyper() \
{ \
    if(hyper_counter) \
    { \
        --hyper_counter; \
        \
        if(!hyper_counter) \
        { \
            bow_color = _XL_CYAN; \
            bow_reload_loops=GREEN_SPEED_VALUE; \
            number_of_arrows_per_shot=1; \
			PRINT_CENTERED_ON_ROW(1,"     "); \
            display_power_ups(); \
        } \
    } \
}


#define INITIALIZE_CROSS_LIB() \
    _XL_INIT_GRAPHICS(); \
    _XL_INIT_INPUT() \
    _XL_INIT_SOUND() \


int main(void)
{           
	INITIALIZE_CROSS_LIB();

    hiscore = 0;

    while(1) // Game (re-)start
    {
        global_initialization();
        display_initial_screen();
		display_second_screen();
        
        while(lives && level<=LAST_LEVEL) // Level (re)-start 
        {            
            level_initialization();
            tank_initialization();
            display_level_screen();
            
            while(alive && (light_tanks_to_kill || heavy_tanks_to_kill) ) // Inner game loop
            {
                handle_hyper();
                handle_bow_move();
                handle_bow_load();
                handle_arrows(); 
                handle_auto_recharge();
                handle_tank_movement();
                handle_tank_collisions();
                handle_missile_drops();
                handle_items();
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);     
                ++main_loop_counter;
            }
            if(alive)
            {
                handle_next_level();
            }
            else
            {
                player_dies();
            }
        }
        if(lives)
        {
            victory();
            
        }
        game_over();
    }

    return EXIT_SUCCESS;
}

