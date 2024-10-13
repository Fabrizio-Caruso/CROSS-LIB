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

// #define TRAINER 1
//#define BENCHMARK

#define INITIAL_LEVEL 0

#define LAST_LEVEL 5
#define INITIAL_LIVES 3
#define MAX_LIVES 9

#if !defined(MAX_NUMBER_OF_MISSILES)
	#if _XL_SLOW_DOWN_FACTOR<=1
		#define MAX_NUMBER_OF_MISSILES 3
	#elif _XL_SLOW_DOWN_FACTOR<=30
		#define MAX_NUMBER_OF_MISSILES 4
	#elif _XL_SLOW_DOWN_FACTOR<=50
		#define MAX_NUMBER_OF_MISSILES 5
	#else
		#define MAX_NUMBER_OF_MISSILES 6
	#endif
#endif
#define MAX_NUMBER_OF_EXTRA_POINTS MAX_NUMBER_OF_MISSILES


#if !defined(MAX_ROCKETS_ON_SCREEN)
	#if _XL_SLOW_DOWN_FACTOR<=1
		#define MAX_ROCKETS_ON_SCREEN 5
	#elif _XL_SLOW_DOWN_FACTOR<=30
		#define MAX_ROCKETS_ON_SCREEN 6		
	#elif _XL_SLOW_DOWN_FACTOR<=50
		#define MAX_ROCKETS_ON_SCREEN 7
	#else
		#define MAX_ROCKETS_ON_SCREEN 8
	#endif	
#endif


#if !defined(DEATH_LOOP)
	#if _XL_SLOW_DOWN_FACTOR<=1
		#define DEATH_LOOP 1
	#elif _XL_SLOW_DOWN_FACTOR<=10
		#define DEATH_LOOP 2
	#elif _XL_SLOW_DOWN_FACTOR<=30
		#define DEATH_LOOP 3
	#elif _XL_SLOW_DOWN_FACTOR<=50
		#define DEATH_LOOP 4
	#else
		#define DEATH_LOOP 5
	#endif
#endif


#define LIGHT_TANKS_ON_FIRST_LEVEL 40

#define HEAVY_TANKS_0 0
#define HEAVY_TANKS_1 28
#define HEAVY_TANKS_2 46
#define HEAVY_TANKS_3 64
#define HEAVY_TANKS_4 91
#define HEAVY_TANKS_5 99

// #define HEAVY_TANKS_5 1


#define NEXT_EXTRA_LIFE 5000U

#define STINGER_Y ((YSize)-3)
#define MAX_STINGER_X ((XSize)*2-3)
#define POWER_UPS_Y ((STINGER_Y)+2)

#if YSize<=16
    #define WALL_Y ((YSize)-8)
#else
    #define WALL_Y ((YSize)-6)
#endif


#if YSize>=18
    #define INITIAL_RESPAWN_TANK_Y (((YSize)/2)-5)
#elif YSize>=16
    #define INITIAL_RESPAWN_TANK_Y (((YSize)/2)-4)
#else 
    #define INITIAL_RESPAWN_TANK_Y (((YSize)/2)-2)
#endif

#define BOTTOM_WALL_Y ((STINGER_Y)+1)

#define POWER_THRESHOLD 4


#define AUTO_RECHARGE_COOL_DOWN 50
#define AUTO_ROCKET_RECAHRGE 9

#define MAX_FREEZE 1

#define LIGHT_TANK_POINTS 10
#define HEAVY_TANK_1_POINTS 15
#define HEAVY_TANK_2_POINTS 25
#define HEAVY_TANK_3_POINTS 30
#define ARTILLERY_POINTS 50

#define EXTRA_POINTS 80
#define RECHARGE_POINTS 25
#define POWERUP_POINTS 30
#define FREEZE_POINTS 100
#define SECRET_ITEM_POINTS 250U

#define POWER_UP_BONUS 100
// #define LEVEL_BONUS 200


#define RED_FIRE_POWER_VALUE 2
#define YELLOW_FIRE_POWER_VALUE 3
#define GREEN_FIRE_POWER_VALUE 4

#define INITIAL_STINGER_RELOAD_LOOPS 8
#define RED_SPEED_VALUE INITIAL_STINGER_RELOAD_LOOPS
#define YELLOW_SPEED_VALUE 5
#define GREEN_SPEED_VALUE 3
// #define HYPER_SPEED_VALUE 2

// #define RED_RANGE_VALUE INITIAL_ROCKET_RANGE
// #define YELLOW_RANGE_VALUE ((INITIAL_ROCKET_RANGE)-2)
#define MAX_RANGE_VALUE ((INITIAL_RESPAWN_TANK_Y)-1)

// #define INITIAL_ROCKET_RANGE ((INITIAL_TANK_Y)+1)
#define ITEM_SPAWN_CHANCE 11000U

// (2 basic hits)
#define LIGHT_TANK_ENERGY 3

// Boss energy: 7 (4 basic hits), 9 (5 basic hits), 11 (6 basic hits)
#define HEAVY_TANK_BASE_ENERGY 5

#define MAX_ROCKETS 99
#define HYPER_RECHARGE 40
#define ROCKET_RECHARGE 20

#define FREEZE_COUNTER_MAX 150

#define WALL_COLOR _XL_GREEN
#define FREEZE_COLOR _XL_CYAN
#define SECRET_COLOR _XL_GREEN

#define LEFT_DIRECTION 0
#define RIGHT_DIRECTION 1
#define ACCELERATION_THRESHOLD 10

uint8_t fire_pressed;
uint8_t time_counter;

uint8_t level_count_down;

uint8_t max_occupied_columns;

uint8_t heavy_tank_counter;

uint8_t rank;

uint8_t direction;

// uint8_t switch_counter
// uint8_t acceleration;
// uint8_t acceleration_counter;

// Re-use variables used in intro
#define switch_counter direction
#define acceleration fire_pressed
#define acceleration_counter time_counter
#define speed_value fire_pressed
#define power_value time_counter

#if !defined(_XL_NO_COLOR)
uint8_t wall_color;
#endif

#if !defined(_XL_NO_COLOR)
	#define set_wall_color(color) wall_color=color
#else
	#define set_wall_color(color)
#endif

uint8_t extra_points_counter;

const uint8_t level_color[2] = {_XL_GREEN, _XL_YELLOW};

#if XSize<=40
    #define MAX_SPARSELY_OCCUPIED_COLUMNS (3*(XSize)/5)
#else
    #define MAX_SPARSELY_OCCUPIED_COLUMNS (2*(XSize)/3)
#endif

#if XSize<=40
    #define MAX_DENSILY_OCCUPIED_COLUMNS ((XSize-2-3)-LAST_LEVEL)
#else
    #define MAX_DENSILY_OCCUPIED_COLUMNS ((XSize-2-6)-LAST_LEVEL)
#endif


#define HELP_ITEM_LEVEL_THRESHOLD 3
#define HELP_ITEM_POWER_THRESHOLD 1
#define HELP_ITEM_POWER_UPPER_THRESHOLD 15

#if XSize<64
    #define HEAVY_TANKS_ON_FIRST_LEVEL (XSize)
#else
    #define HEAVY_TANKS_ON_FIRST_LEVEL 50
#endif


// level 1:  40 = 40 +   0 -> light 
// level 2:  60 = 32 +  28 -> light,  medium 100% 
// level 3:  70 = 24 +  46 -> light,  medium  50%, stealth 50%
// level 4:  80 = 16 +  64 -> light,  medium  25%, stealth 25%, heavy 50%
// level 5:  99 =  8 +  92 -> light,  medium  25%, stealth 25%, heavy 25%, artillery 25%
// level 6:  99 =  0 +  99 -> light,  medium   0%, stealth 25%, heavy 50%, artillery 25% (medium if secret item is taken)
const uint8_t heavy_tanks_on_level[LAST_LEVEL+1] = {HEAVY_TANKS_0,HEAVY_TANKS_1,HEAVY_TANKS_2,HEAVY_TANKS_3,HEAVY_TANKS_4,HEAVY_TANKS_5};//99};//91,99}; //99};

#define LEVEL_2_TANK_THRESHOLD 8

#define MAX_HYPER_COUNTER 200

// #if YSize>=20
    // #define HEIGHT_SHOOT_THRESHOLD YSize-10
// #else
    // #define HEIGHT_SHOOT_THRESHOLD YSize-11
// #endif


#if YSize>=20
    #define HEIGHT_SHOOT_THRESHOLD YSize-10
#elif YSize>=16
    #define HEIGHT_SHOOT_THRESHOLD YSize-9
#else
    #define HEIGHT_SHOOT_THRESHOLD YSize-8
#endif

#define VERY_FAST_TANK_SHOOT_MASK 1

#define FAST_TANK_SHOOT_MASK 3

#define SLOW_TANK_SHOOT_MASK 15

#define MAX_TANK_LEVEL 3

#define INITIAL_ARTILLERY_LEVEL 3

uint8_t tank_shoot_speed_mask;

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
    ARTILLERY_POINTS,
};

 uint8_t light_tanks_to_kill;
 uint8_t heavy_tanks_to_kill;
 // uint8_t artillery_to_kill;

 uint8_t lives;
 uint8_t level;
 uint8_t killed_light_tanks;
 uint8_t killed_heavy_tanks;
 uint8_t killed_artillery;

 uint8_t heavy_tanks_to_spawn;
 uint8_t light_tanks_to_spawn;

 uint8_t auto_recharge_counter;

#if !defined(_XL_NO_COLOR)
     uint8_t rocket_display_color;
#else
    #define rocket_display_color _DUMMY_
#endif

#if !defined(_XL_NO_COLOR) && !defined(_XL_NO_TEXT_COLOR)
 const uint8_t power_up_color[3] = {_XL_RED, _XL_YELLOW, _XL_GREEN};
#endif

#if !defined(_XL_NO_COLOR)
 const uint8_t rocket_color[3] = {_XL_CYAN, _XL_WHITE, _XL_YELLOW };
#endif

 uint8_t freeze;
 uint8_t powerUp;

 uint8_t number_of_rockets_per_shot;

 uint8_t tank_y_array[XSize];
 uint8_t tank_shape[XSize];
 uint8_t tank_x; // To be used as an index to the current tank
 // uint8_t tank_active[XSize];

#define tank_active tank_y_array

 uint8_t energy[XSize];
 uint8_t tank_level[XSize];
 
 // Red hits    (2): 2, 4, 6, 9, 6
 // Yellow hits (3): 1, 3, 4, 6, 4
 // Green hits  (4): 1, 2, 3, 5, 3
 const uint8_t rank_energy[5] = {LIGHT_TANK_ENERGY,7,11,17,12};

 uint8_t fire_power;

 uint8_t freeze_locked;
 uint8_t secret_locked;


uint8_t tank_move_speed_mask;

// #if !defined(_XL_NO_UDG)
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
// #endif

 const uint8_t stinger_tile[8] =
{
    EMPTY_STINGER_LEFT_TILE_0,
    EMPTY_STINGER_RIGHT_TILE_0,
    EMPTY_STINGER_LEFT_TILE_1,
    EMPTY_STINGER_RIGHT_TILE_1,
    LOADED_STINGER_LEFT_TILE_0,
    LOADED_STINGER_RIGHT_TILE_0,
    LOADED_STINGER_LEFT_TILE_1,
    LOADED_STINGER_RIGHT_TILE_1,
};

 const uint8_t rocket_tile[2] =
{
    ROCKET_TILE_0,
    ROCKET_TILE_1,
};

 uint8_t stinger_x; // range: 0..2*XSize-2^M
 uint8_t stinger_shape_tile;
 uint8_t stinger_color;

 uint8_t input;

 uint8_t loaded_stinger;
 // uint8_t active_rocket[MAX_ROCKETS_ON_SCREEN];
 uint8_t rocket_shape[MAX_ROCKETS_ON_SCREEN];
 uint8_t rocket_x[MAX_ROCKETS_ON_SCREEN];
 uint8_t rocket_y[MAX_ROCKETS_ON_SCREEN];
 uint8_t remaining_rockets;
 
 #define active_rocket rocket_y
 // uint8_t rocket_range;

 uint8_t stinger_reload_loops;

 uint8_t next_rocket;
 uint8_t rockets_on_screen;
 uint8_t stinger_load_counter;
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

// #if !defined(NO_EXTRA_TITLE)

#define NUMBER_OF_ITEMS 4

 const uint8_t item_tile[NUMBER_OF_ITEMS][2] = 
{
    { POWER_UP_TILE, _XL_WHITE },
    { ROCKET_TILE_0, _XL_YELLOW },
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
// #endif

// #if !defined(NO_EXTRA_TITLE)
 const uint8_t enemy_tile[5][2] = 
{
    { LIGHT_TANK_TILE_0, _XL_WHITE },
    { HEAVY_TANK_TILE_0, _XL_GREEN },
    { TANK_DEATH_TILE, _XL_YELLOW },
    { HEAVY_TANK_TILE_0, _XL_RED },
    { MORTAR_TILE, _XL_GREEN },
};

 const char enemy_name[5][9] = 
{
    "LIGHT",
    "MEDIUM",
    "STEALTH",
    "HEAVY",
    "HOWITZER",
};
// #endif


Missile enemyMissile[MAX_NUMBER_OF_MISSILES];
Item extraPointsItem[MAX_NUMBER_OF_EXTRA_POINTS];

// uint8_t artillery_shell_active;
uint8_t artillery_shell_x;
uint8_t artillery_shell_y;

#define artillery_shell_active artillery_shell_y

void short_sleep(void)
{
    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
}

void less_short_sleep(void)
{
    _XL_SLOW_DOWN(3*_XL_SLOW_DOWN_FACTOR);
}


void one_second(void)
{
    _XL_SLEEP(1);
}


void PRINT_CENTERED_ON_ROW(uint8_t row, char *Text)
{
    _XL_PRINT(((uint8_t) (XSize - strlen(Text))>>1), row, Text);    
}


#define PRINT_CENTERED(Text) \
    PRINT_CENTERED_ON_ROW((YSize>>1), Text)




void sleep_and_wait_for_input(void)
{
    less_short_sleep();
    _XL_WAIT_FOR_INPUT();
}

#if XSize>=26
    #define POWER_X 8
#else
    #define POWER_X 6
#endif

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

#if XSize>=28
    #define TANK_COUNTER_X (POWER_UP_X+4)
    #define TANK_COUNTER_Y 0
#elif XSize>=26
    #define TANK_COUNTER_X (POWER_X+7)
    #define TANK_COUNTER_Y (YSize-1)
#elif XSize>=22
    #define TANK_COUNTER_X (POWER_X+6)
    #define TANK_COUNTER_Y (YSize-1)
#else
    #define TANK_COUNTER_X (POWER_X+5)
    #define TANK_COUNTER_Y (YSize-1)
#endif

#if XSize>=20   
    void display_enemy_counter(void)
    {
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(TANK_COUNTER_X+1,TANK_COUNTER_Y,2,light_tanks_to_kill+heavy_tanks_to_kill);
    }
#else
     #define display_enemy_counter()
#endif

void display_remaining_rockets(void)
{
	#if !defined(_XL_NO_COLOR)
    uint8_t color;
    
    if(remaining_rockets<20)
    {
        color = _XL_RED;
    }
    else
    {
        color = _XL_WHITE;
    }
    _XL_SET_TEXT_COLOR(color);
	#endif
    _XL_PRINTD(7,0,2,remaining_rockets);
}


void recharge_rockets(uint8_t value)
{
    remaining_rockets+=value;
    if(remaining_rockets>MAX_ROCKETS)
    {
        remaining_rockets=MAX_ROCKETS;
    }
    display_remaining_rockets();
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
        _XL_DRAW(LIVES_X,POWER_UPS_Y,stinger_tile[4+0+stinger_shape_tile],_XL_CYAN);
        _XL_DRAW(LIVES_X+1,POWER_UPS_Y,stinger_tile[1+4+stinger_shape_tile],_XL_CYAN);
        _XL_SET_TEXT_COLOR(color);
        _XL_PRINTD(LIVES_X+2,POWER_UPS_Y,1,lives);
    }
#else
    #define display_lives(color) \
    { \
        _XL_DRAW(LIVES_X,POWER_UPS_Y,stinger_tile[4+0+stinger_shape_tile],_XL_CYAN); \
        _XL_DRAW(LIVES_X+1,POWER_UPS_Y,stinger_tile[1+4+stinger_shape_tile],_XL_CYAN); \
        _XL_PRINTD(LIVES_X+2,POWER_UPS_Y,1,lives); \
    } 
#endif


#if !defined(_XL_NO_COLOR)
    #define _extra_life_color_effect(color) display_lives(color) 
#else
    #define _extra_life_color_effect(color)
#endif


void display_stinger(void)
{
    _XL_DRAW((stinger_x>>1),STINGER_Y,stinger_tile[4*loaded_stinger+0+stinger_shape_tile],stinger_color);
    _XL_DRAW((stinger_x>>1)+1,STINGER_Y,stinger_tile[1+4*loaded_stinger+stinger_shape_tile],stinger_color);  
}


#if !defined(_XL_NO_COLOR)
	#define set_tank_color(tank_color) color=tank_color
#else
	#define set_tank_color(tank_color)
#endif

void display_tank(void)
{
    uint8_t status = tank_shape[tank_x];
    uint8_t pos = tank_y_array[tank_x];
	#if !defined(_XL_NO_COLOR)
    uint8_t color;
	#endif
    uint8_t tile0;

    tile0 = HEAVY_TANK_TILE_0;

    if(tank_level[tank_x]==1)
    {
        set_tank_color(_XL_GREEN);
    }
    else if(tank_level[tank_x]==2)
    {
        if(!freeze)
        {
            tile0 = TANK_DEATH_TILE;
            set_tank_color(_XL_YELLOW);
        }
    }
    else if(!tank_level[tank_x])
    {
        tile0 = LIGHT_TANK_TILE_0;
        set_tank_color(_XL_WHITE);
    }
    else if(tank_level[tank_x]==3)
    {
        set_tank_color(_XL_RED);
    }
    else 
    {
        // tile0 = MORTAR_TILE;
        if(tank_level[tank_x]==4)
        {
            set_tank_color(_XL_GREEN);
        }
        else
        {
            set_tank_color(_XL_RED);
        }
    }
    if(freeze)
    {
        set_tank_color(_XL_CYAN);  
    }

    if(tank_level[tank_x]<=MAX_TANK_LEVEL)
    {
        if(!status)
        {
            _XL_DELETE(tank_x, tank_y_array[tank_x]-1);
            _XL_DRAW(tank_x, pos, tile0, color);
        }
        else
        {
            // #if !defined(_XL_NO_UDG)
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
            // #else

            // if(!tank_level[tank_x])
            // {
                
                // tile0 = LIGHT_TANK_TILE_0;
            // }
            // else
            // {
                // if((tank_y_array[tank_x])&1)
                // {
                    // tile0 = HEAVY_TANK_TILE_0;
                // }
                // else
                // {
                    // tile0 = HEAVY_TANK_TILE_1;
                // }
            // }
            // _XL_DRAW(tank_x, pos, tile0, color);
            // #endif
        }
    }
    else
    {
        _XL_DRAW(tank_x, pos, MORTAR_TILE, color);
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
                stinger_color = _XL_YELLOW; \
                display_stinger(); \
                _extra_life_color_effect(_XL_RED); \
                short_sleep(); \
                stinger_color = _XL_CYAN; \
                display_stinger(); \
                _extra_life_color_effect(_XL_YELLOW); \
                short_sleep(); \
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
    recharge_rockets(ROCKET_RECHARGE);
    increase_score(RECHARGE_POINTS);
    PRINT_CENTERED_ON_ROW(2,"        "); \

}


#define RANGE_X 0

#if defined(_XL_NO_TEXT_COLOR)
	#if XSize<=20
		#define SPEED_STRING "SPD"
		#define POWER_STRING "PWR"
	#else
		#define SPEED_STRING "SPEED"
		#define POWER_STRING "POWER"
	#endif
#else
	#if XSize<=17
		#define SPEED_STRING "SPD"
		#define POWER_STRING "PWR"
	#else
		#define SPEED_STRING "SPEED"
		#define POWER_STRING "POWER"
	#endif
#endif


#if XSize>=31
    #define ROCKETS_X (XSize-7)
#else
    #define ROCKETS_X (XSize-6)
#endif

#define SPEED_X 0


uint8_t find_inactive(Item* itemArray)
{
    uint8_t i;
    
    for(i=0;i<MAX_NUMBER_OF_MISSILES;++i)
    {
        // _XL_PRINTD(i*3,YSize-5,1,itemArray[i]._active);
        
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
    uint8_t speed_color;
    uint8_t color;

    uint8_t power_color;
    
    uint8_t i;
    
    speed_color = _XL_RED;
    rocket_display_color = _XL_CYAN;
    power_color = _XL_RED;
    
    if(powerUp<2) // speed
    {
        speed_color = power_up_color[powerUp];
    }
    else
    {
        speed_color = _XL_GREEN;

        if(powerUp<4)
        {
            power_color = power_up_color[powerUp-2];
            rocket_display_color = rocket_color[powerUp-2];
        }
        else
        {
            power_color = _XL_GREEN;
            rocket_display_color = _XL_YELLOW;
        }

    }

    _XL_SET_TEXT_COLOR(speed_color);
    _XL_PRINT(SPEED_X,POWER_UPS_Y,SPEED_STRING);
    
    _XL_SET_TEXT_COLOR(power_color);
    _XL_PRINT(POWER_X,POWER_UPS_Y,POWER_STRING);

    for(i=0;i<3;++i)
    {
        if(i<=number_of_rockets_per_shot-1)
        {
           color = rocket_display_color;
        }
        else
        {
           color = _XL_RED;
        }
        _XL_DRAW(ROCKETS_X+i,POWER_UPS_Y,ROCKET_TILE_0,color);
    }
}
#elif !defined(_XL_NO_COLOR) // COLOR but NO TEXT COLOR

#if XSize<=20
	#define STR_LEN 3
#else
	#define STR_LEN 5
#endif
void display_power_ups(void)
{
    uint8_t color;
    
    uint8_t i;
    
    rocket_display_color = _XL_CYAN;    
    speed_value = 1;
    power_value = 1;
    
    {

        if(powerUp<2) // speed
        {
            speed_value = powerUp+1;
        }
        else
        {
            speed_value = 3;
    
            if(powerUp<4)
            {
				power_value = powerUp-1;
            }
			else
			{
				power_value = 3;
			}
        }
    }

    _XL_PRINT(SPEED_X,POWER_UPS_Y,SPEED_STRING);
    _XL_PRINTD(SPEED_X+STR_LEN,POWER_UPS_Y,1,speed_value);
    
    _XL_PRINT(POWER_X,POWER_UPS_Y,POWER_STRING);
    _XL_PRINTD(POWER_X+STR_LEN,POWER_UPS_Y,1,power_value);

    for(i=0;i<3;++i)
    {
        if(i<=number_of_rockets_per_shot-1)
        {
           color = rocket_display_color;
        }
        else
        {
           color = _XL_RED;
        }
        _XL_DRAW(ROCKETS_X+i,POWER_UPS_Y,ROCKET_TILE_0,color);
    }
}
#else // NO COLOR and NO TEXT COLOR
#if XSize<=20
	#define STR_LEN 3
#else
	#define STR_LEN 5
#endif
void display_power_ups(void)
{    
    uint8_t i;
    
    speed_value = 1;
    power_value = 1;
    
    {

        if(powerUp<2) // speed
        {
            speed_value = powerUp+1;
        }
        else
        {
            speed_value = 3;
    
            if(powerUp<4)
            {
				power_value = powerUp-1;
            }
			else
			{
				power_value = 3;
			}
        }
    }

    _XL_PRINT(SPEED_X,POWER_UPS_Y,SPEED_STRING);
    _XL_PRINTD(SPEED_X+STR_LEN,POWER_UPS_Y,1,speed_value);
    
    _XL_PRINT(POWER_X,POWER_UPS_Y,POWER_STRING);
    _XL_PRINTD(POWER_X+STR_LEN,POWER_UPS_Y,1,power_value);

    for(i=0;i<3;++i)
    {
		if(i<number_of_rockets_per_shot)
		{
			_XL_DRAW(ROCKETS_X+i,POWER_UPS_Y,ROCKET_TILE_0,0);
		}
		else
		{
			_XL_DELETE(ROCKETS_X+i,POWER_UPS_Y);
		}
    }
}
#endif


#define activate_hyper() \
{ \
    _XL_ZAP_SOUND(); \
    recharge_rockets(HYPER_RECHARGE); \
    hyper_counter = MAX_HYPER_COUNTER; \
    stinger_color = level_color[(level+1)&1]; \
    _XL_SET_TEXT_COLOR(_XL_CYAN); \
    if(powerUp>5) \
    { \
        number_of_rockets_per_shot=3; \
        PRINT_CENTERED_ON_ROW(1,"TRIPLE"); \
    } \
    else \
    { \
        number_of_rockets_per_shot=2; \
        PRINT_CENTERED_ON_ROW(1,"DOUBLE"); \
    } \
}


void power_up_effect(void)
{    
    ++powerUp;
    
    if(!(powerUp%5))
    {
        activate_hyper();
    }
    
    display_power_up_counter();
    increase_score(POWERUP_POINTS);
    
    switch(powerUp)
    {    
        case 1:
            stinger_reload_loops=YELLOW_SPEED_VALUE;
        break;
           
        case 2:
            stinger_reload_loops=GREEN_SPEED_VALUE;
        break;
        
        case 3:
            fire_power = YELLOW_FIRE_POWER_VALUE;
        break;
        
        case 4:
            fire_power = GREEN_FIRE_POWER_VALUE;
        break;
        
        // case 5:
        // break;
        
        case 17:
            #if !defined(_XL_NO_COLOR)
            powerUpItem._color = SECRET_COLOR;
            #endif
        break;
        
        case 18:
            secret_locked = 0;
			#if !defined(_XL_NO_COLOR)
            powerUpItem._color = _XL_WHITE;
			#endif

        break;
        
        default:
        break;
    }
    display_power_ups();
}


void extra_points_effect(void)
{
    increase_score(EXTRA_POINTS);
    ++extra_points_counter;
    if(!(extra_points_counter&15))
    {
        freeze_locked=0;
    }
}


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
        if(energy[tank_x]>HEAVY_TANK_1_POINTS)
        {
            energy[tank_x]=HEAVY_TANK_1_POINTS;
        }
    }
    display_tanks();

    secret_locked = 1;
    increase_score(SECRET_ITEM_POINTS);
    increase_score(SECRET_ITEM_POINTS);
}



void player_hit(void)
{
	uint8_t i;
	uint8_t player_x = (stinger_x>>1)+(stinger_x&1);
	
	_XL_DRAW(player_x,STINGER_Y, EXPLOSION_TILE,_XL_RED);

	_XL_EXPLOSION_SOUND();
	for(i=0;i<3;++i)
	{
		display_stinger();
		short_sleep();	
		_XL_DRAW(player_x,STINGER_Y, EXPLOSION_TILE,_XL_RED);
		short_sleep();
	}

}

void bullet_effect(void)
{
    alive=0;
	player_hit();
}

#if !defined(_XL_NO_COLOR)
    #define initialize_items() \
    { \
        uint8_t i; \
        \
        rechargeItem._active = 0; \
        rechargeItem._tile = ROCKET_TILE_0; \
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
            enemyMissile[i]._tile = BULLET_TILE; \
            enemyMissile[i]._color = _XL_CYAN; \
            enemyMissile[i]._effect = bullet_effect; \
            \
            extraPointsItem[i]._active = 0; \
            extraPointsItem[i]._tile = EXTRA_POINTS_TILE; \
            extraPointsItem[i]._color = level_color[(level+1)&1]; \
            extraPointsItem[i]._effect = extra_points_effect; \
        } \
    }
#else
    #define initialize_items() \
    { \
        uint8_t i; \
        \
        rechargeItem._active = 0; \
        rechargeItem._tile = ROCKET_TILE_0; \
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
            enemyMissile[i]._tile = BULLET_TILE; \
            enemyMissile[i]._effect = bullet_effect; \
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



void move_left(void)
{
    stinger_shape_tile = 2*((--stinger_x)&1);
    if(stinger_shape_tile)
    {
        _XL_DELETE((stinger_x>>1)+2,STINGER_Y);
    }
    display_stinger();
}


void move_right(void)
{
    stinger_shape_tile = 2*((++stinger_x)&1);
    if(!stinger_shape_tile)
    {
        _XL_DELETE((stinger_x>>1)-1,STINGER_Y);
    }
    display_stinger();
}


void drop_item(register Item *item, uint8_t max_counter)
{
    uint8_t offset;

    offset = tank_y_array[tank_x]+1;
    
    // _XL_TICK_SOUND();
    item->_active = 1;
    item->_x = tank_x;

    if(tank_shape[tank_x])
    {
        ++offset;
    }
    item->_y = offset;
    
    item->_counter=max_counter;
}


void artillery_fire(void)
{
    _XL_DRAW(tank_x,tank_y_array[tank_x],MORTAR_TILE,_XL_WHITE);
    artillery_shell_y = tank_y_array[tank_x]+1;
    _XL_TOCK_SOUND();
    // artillery_shell_active = 1;
    artillery_shell_x = tank_x;
    _XL_DRAW(tank_x,tank_y_array[tank_x],MORTAR_TILE,_XL_GREEN);

}


#define EXPLOSION_THRESHOLD 4U

void handle_item(register Item* item)
{
    if(item->_active)
    {
        // TODO: Necessary for GCC for TI99

        
        if(item->_y<STINGER_Y)
        {
            _XL_DELETE(item->_x,item->_y);
            if(main_loop_counter&1)
            {
                ++(item->_y);
            }
                  
            #if !defined(_XL_NO_COLOR)
                    // TODO: GCC for TI99 does not display the correct tile with item->_tile
                    _XL_DRAW(item->_x,item->_y,item->_tile,item->_color);
            #else
                _XL_DRAW(item->_x,item->_y,item->_tile,0);
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
				uint8_t item_tile;

				if(item->_counter<EXPLOSION_THRESHOLD)
				{
					item_tile = EXPLOSION_TILE;
				}
				else
				{
					item_tile = item->_tile;
				}
                #if !defined(_XL_NO_COLOR)
                _XL_DRAW(item->_x,item->_y,item_tile,item->_color);
                #else
                _XL_DRAW(item->_x,item->_y,item_tile,0);
                #endif
            }

// TODO: Use uint8_t player_x = (stinger_x>>1)+(stinger_x&1);
// TODO: compare item_x with player_x to decide whether the player has to be redisplayed
            if((item->_counter>=EXPLOSION_THRESHOLD)&&(item->_x==(stinger_x>>1)+(stinger_x&1)))
            {
                item->_effect();
                _XL_PING_SOUND();
                item->_active=0;
            }
            --(item->_counter);
            if(!(item->_counter))
            {
                item->_active=0;
                _XL_DELETE(item->_x,item->_y);
                
            }
            display_stinger();
        }
    }   
}


void handle_artillery_shell(void)
{
    if(artillery_shell_active) 
    {
        if(artillery_shell_y<=STINGER_Y-1)
        {
            _XL_DELETE(artillery_shell_x,artillery_shell_y);

            ++artillery_shell_y;

                  
            #if !defined(_XL_NO_COLOR)
                    // TODO: GCC for TI99 does not display the correct tile with item->_tile
                _XL_DRAW(artillery_shell_x,artillery_shell_y,BULLET_TILE,_XL_WHITE);
            #else
                _XL_DRAW(artillery_shell_x,artillery_shell_y,BULLET_TILE,0);
            #endif
        }
        else
        {
            uint8_t player_x = (stinger_x>>1)+(stinger_x&1);
            // _XL_DRAW(artillery_shell_x-1,artillery_shell_y,EXPLOSION_TILE,_XL_RED);
            _XL_DRAW(artillery_shell_x,artillery_shell_y,EXPLOSION_TILE,_XL_RED);
            // _XL_DRAW(artillery_shell_x+1,artillery_shell_y,EXPLOSION_TILE,_XL_RED);
            less_short_sleep();
            if(artillery_shell_x==player_x) // || artillery_shell_x==player_x-1 || artillery_shell_x==player_x+1)
            {
               alive=0;
			   player_hit();
            }    
            // _XL_DELETE(artillery_shell_x-1,artillery_shell_y);
            _XL_DELETE(artillery_shell_x,artillery_shell_y);
			artillery_shell_active = 0;
            // _XL_DELETE(artillery_shell_x+1,artillery_shell_y);
            display_stinger();
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

uint8_t find_random_active_tank(void)
{
    uint8_t i;
    uint8_t index;

    index = (uint8_t) (_XL_RAND())%(XSize-2);

    for(i=1;i<XSize-1;++i)
    {
        index%=(XSize-2);
        ++index;
        if(tank_active[index])
        {
            return index;
        }
    }    
    
    return 1;
}


uint8_t find_random_inactive_tank(void)
{
    uint8_t i;
    uint8_t index;

    index = (uint8_t) (_XL_RAND())%(XSize-2);

    for(i=1;i<XSize-1;++i)
    {
        index%=(XSize-2);
        ++index;
        if(!tank_active[index])
        {
            return index;
        }
    }    
    
    return 1;
}


void activate_tank(void)
{
    uint8_t old_x;
    	
    old_x = tank_x;
    
    do
    {
        tank_x = find_random_inactive_tank();
        
    } while((old_x==tank_x) || (old_x+1==tank_x) || (old_x-1==tank_x));
    // tank_active[tank_x]=1;    


    if(rank==4)
    {
        tank_y_array[tank_x]=INITIAL_RESPAWN_TANK_Y-1;
    }
    else
    {
        tank_y_array[tank_x]=INITIAL_RESPAWN_TANK_Y+(level>>1);
    }
    _XL_DRAW(tank_x, tank_y_array[tank_x], TANK_DEATH_TILE, _XL_WHITE);
    _XL_TOCK_SOUND();
    less_short_sleep();
    tank_shape[tank_x]=0;
}


void spawn_light_tank(void)
{
    rank=0;
    activate_tank();
    tank_level[tank_x]=0;
    energy[tank_x]=LIGHT_TANK_ENERGY;  
    --light_tanks_to_spawn;
}


void spawn_heavy_tank(void)
{

    if(!level) // TODO: Not necessary
    {
        return;
    }
    else if(level==1)
    {
        rank = 1;
    }
    else if(level==2)
    {
        rank = 1+(heavy_tank_counter&1);        
    }
    else if(level==3) // 3
    {
        switch(heavy_tank_counter&3)
        {
            case 0 :
                rank = 1;
            break;
            case 1 :
                rank = 2;
            break;
            default:
                rank = 3;
        }
    }
    else if(level==4) // 4
    {
        rank = 1+(heavy_tank_counter&3);
    }
    else // 5
    {
        switch(heavy_tank_counter&3)
        {
            case 0 :
                rank = 2;
            break;
            case 1 :
                rank = 4;
            break;
            default:
                rank = 3;
        }
    }
    ++heavy_tank_counter;

    activate_tank();
    tank_level[tank_x]=rank;
    energy[tank_x]=rank_energy[rank];//HEAVY_TANK_BASE_ENERGY+rank*2;
    --heavy_tanks_to_spawn;
}

// #if !defined(NORMAL_TANK_SPEED) && !defined(SLOW_TANK_SPEED)
    // #if XSize>=32
        // #define NORMAL_TANK_SPEED 3
        // #define SLOW_TANK_SPEED 7
    // #else
#define NORMAL_TANK_SPEED 3
#define SLOW_TANK_SPEED 7
    // #endif 
// #endif

#define FEW_TANKS 5
// #define MANY_TANKS 60
// #define FASTER_TANK_MOVE_LEVEL_THRESHOLD 1

void update_tank_move_speed_mask(void)
{
    if(!powerUp || (light_tanks_to_kill+heavy_tanks_to_kill<=FEW_TANKS))
    {
        tank_move_speed_mask=SLOW_TANK_SPEED;
    }
    else
    {
        tank_move_speed_mask=NORMAL_TANK_SPEED;
    }
	// _XL_PRINTD(0,YSize-1,1,tank_move_speed_mask);
}


void display_wall(uint8_t y)
{
    uint8_t i;
    
    for(i=0; i<XSize; ++i)
    {   
        _XL_DRAW(i,y,WALL_TILE,wall_color); 
    }
}


#define _display_red_tank(tile) \
{ \
    _XL_DRAW(tank_x,tank_y_array[tank_x],tile,_XL_RED); \
}


void display_red_tank(void)
{
    uint8_t tile;
    
    if(!tank_level[tank_x])
    {
        tile=LIGHT_TANK_TILE_0;
    }
    else if(tank_level[tank_x]<=MAX_TANK_LEVEL)
    {
        tile=HEAVY_TANK_TILE_0;
    }
    else
    {
        tile=MORTAR_TILE;
    }
    _display_red_tank(tile);
}

#define POWER_UP_COOL_DOWN 38
#define EXTRA_POINTS_COOL_DOWN 60
#define SECRET_COOL_DOWN 55
#define FREEZE_COOL_DOWN 45
#define RECHARGE_COOL_DOWN 53

void handle_item_drop(void)
{
    if(tank_level[tank_x] || (!(main_loop_counter&3)) )
    {        
        ++item_counter;
        item_counter&=3;
        
        if(((level>=HELP_ITEM_LEVEL_THRESHOLD)&&(powerUp<=HELP_ITEM_POWER_THRESHOLD)))
        {
            item_counter&=1;
        }
        
        if(!item_counter)
        {
            if(!rechargeItem._active)
            {
                drop_item(&rechargeItem,RECHARGE_COOL_DOWN);
            }
        }
        else if(item_counter==1)
        {
            if(!powerUpItem._active)
            {
                drop_item(&powerUpItem,POWER_UP_COOL_DOWN);
            } 
        }
        else if((!freeze_locked)&&(!freeze))
        {
            if(!freezeItem._active)
            {
                drop_item(&freezeItem,FREEZE_COOL_DOWN);
            }
        }
        else if(!secret_locked && !secretItem._active)
            {
                drop_item(&secretItem,SECRET_COOL_DOWN);
            }
        else
        {
            uint8_t index; 
            
            index = find_inactive(extraPointsItem);
            if(index!=MAX_NUMBER_OF_EXTRA_POINTS)
            {
                drop_item(&extraPointsItem[index],EXTRA_POINTS_COOL_DOWN);
            }            
        }  
    }
}


// void handle_tank_shoot_speed_mask(void) // TODO: Necessary??
// {
    // if(heavy_tanks_to_kill<heavy_tanks_on_level[level]/4)
    // {
        // tank_shoot_speed_mask = FAST_TANK_SHOOT_MASK; 
    // }
// }


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
        
        // handle_tank_shoot_speed_mask();
    }
    // else
    // {
        // _XL_PRINT(0,YSize-1,"NOTHING TO SPAWN");
    // }

}


void tank_dies(void)
{
    uint8_t y_pos = tank_y_array[tank_x];
    uint8_t i;

    _XL_DELETE(tank_x,y_pos+1);
    
    _XL_DRAW(tank_x,y_pos, TANK_DEATH_TILE, _XL_RED);

    for(i=0;i<DEATH_LOOP+1;++i)
    {
		display_red_tank();
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR/4);
		_XL_DRAW(tank_x,y_pos, TANK_DEATH_TILE, _XL_RED);
    } 
    for(i=0;i<DEATH_LOOP;++i)
    {
        _XL_DRAW(tank_x,y_pos, EXPLOSION_TILE, _XL_WHITE);
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR/8);
        _XL_DRAW(tank_x,y_pos, TANK_DEATH_TILE, _XL_RED);
    }     
    _XL_SHOOT_SOUND();
    _XL_DELETE(tank_x,y_pos);
    
    #if defined(NO_BOTTOM_WALL_REDRAW)
    #else
        display_wall(BOTTOM_WALL_Y);
    #endif
    
    display_stinger();
        
    if(!tank_level[tank_x])
    {
        ++killed_light_tanks;
        --light_tanks_to_kill;
    }
    else //if(tank_level[tank_x]<=MAX_TANK_LEVEL)
    {
        ++killed_heavy_tanks;
        --heavy_tanks_to_kill;
    }

   
    if(tank_x==forced_tank_x)
    {
        forced_tank = 0;
    }
    
    if(y_pos<STINGER_Y-2)
    {
        handle_item_drop();
    }
    
    tank_active[tank_x]=0;
    
    display_enemy_counter();
    update_tank_move_speed_mask();
}


#define compute_next_available_rocket_index() \
    for(next_rocket=0;next_rocket<MAX_ROCKETS_ON_SCREEN;++next_rocket) \
    { \
        if(!active_rocket[next_rocket]) \
        { \
            break; \
        } \
    }


void handle_rockets(void)
{
    uint8_t i;
    
    for(i=0;i<MAX_ROCKETS_ON_SCREEN;++i)
    {
        if(active_rocket[i]) // ACTIVE
        {    
            if(rocket_y[i]<MAX_RANGE_VALUE)
            {
                active_rocket[i]=0;
                --rockets_on_screen;
            }
            else
            {
                _XL_DELETE(rocket_x[i],rocket_y[i]);
                --rocket_y[i];
                if(rocket_y[i]>=MAX_RANGE_VALUE)
                {
                    #if !defined(_XL_NO_COLOR)
                    _XL_DRAW(rocket_x[i],rocket_y[i],rocket_shape[i],rocket_display_color);
                    #else
                    _XL_DRAW(rocket_x[i],rocket_y[i],rocket_shape[i],0);
                    #endif
                }
            }
        }
    }
}




void decrease_energy(void)
{
    if(energy[tank_x]<=fire_power)
    {
        energy[tank_x]=0;
    }
    else
    {
        energy[tank_x]-=fire_power;
    }
}


void push_tank(void)
{
    if(!tank_shape[tank_x])
    {
        --tank_y_array[tank_x];
        tank_shape[tank_x]=3;
    }
    else 
    {
        --tank_shape[tank_x];
        _XL_DELETE(tank_x,tank_y_array[tank_x]+1);
    }
}


void push_display_tank(void)
{
    push_tank();
    display_tank();
}


#define not_stealth() freeze || (tank_level[tank_x]!=2) || tank_shape[tank_x]

void handle_tank_collisions(void)
{
    uint8_t i;
	
    for(i=0;i<MAX_ROCKETS_ON_SCREEN;++i)
    {
		if(active_rocket[i])
		{
			tank_x = rocket_x[i];
			if(tank_active[tank_x])
			{
				if(tank_y_array[tank_x]>=rocket_y[i]-1 && tank_y_array[tank_x]<= rocket_y[i]+1)
				{
					if(not_stealth())
					{
						--rockets_on_screen;

						_XL_DELETE(rocket_x[i],rocket_y[i]);
						active_rocket[i]=0;
						decrease_energy();


						if(energy[tank_x])
						{
							display_red_tank();
							short_sleep();
							
							_XL_TOCK_SOUND();                
							if(tank_level[tank_x]<=MAX_TANK_LEVEL)
							{
								// push_tank();
								// display_tank();
								push_display_tank();
								// #if defined(_XL_NO_UDG)
								// _XL_DELETE(tank_x,tank_y_array[tank_x]+1);
								// #endif
							}
							else
							{
								display_tank();
							}
						}
						else
						{
							tank_dies();
							increase_score(tank_points[tank_level[tank_x]]);
							respawn();
						}						
					}
					else
					{
						display_tank();
					}
				
				}
			}
		}
	}
}

// uint8_t tank_hit(void)
// {
    // uint8_t i;
    
    // for(i=0;i<MAX_ROCKETS_ON_SCREEN;++i)
    // {
        // if(active_rocket[i] && rocket_x[i]==tank_x
          // && tank_y_array[tank_x]>=rocket_y[i]-1 && tank_y_array[tank_x]<=rocket_y[i]+1)
           // {
               // if(freeze || (tank_level[tank_x]!=2) || tank_shape[tank_x])
               // {
                   // active_rocket[i]=0;
                    // --rockets_on_screen;

                   // _XL_DELETE(rocket_x[i],rocket_y[i]);

               // }
               // else // rockets goes through ghost !free (non-frozen) && tank_level==2 (i.e., ghost tank) && !tank_shape (i.e. invincible shape)
               // {
                   // display_tank(); // display invincible ghost tank
                   // return 0; // two rockets cannot be at the same place
               // }
           // }
    // }
    // return 0;
// }


// void handle_tank_collisions(void)
// {
    // for(tank_x=0;tank_x<XSize;++tank_x)
    // {
        
        // if(tank_active[tank_x] && tank_hit())
        // {
            // decrease_energy();


            // if(energy[tank_x])
            // {
                // display_red_tank();
                // short_sleep();
                
                // _XL_TOCK_SOUND();                
                // if(tank_level[tank_x]<=MAX_TANK_LEVEL)
                // {
                    // push_display_tank();
                    // #if defined(_XL_NO_UDG)
                    // _XL_DELETE(tank_x,tank_y_array[tank_x]+1);
                    // #endif
                // }
                // else
                // {
                    // display_tank();
                // }
            // }
            // else
            // {
                // tank_dies();
                // increase_score(tank_points[tank_level[tank_x]]);
                // respawn();
            // }
        // }
    // }
// }


void move_tank(void)
{ 
    ++tank_shape[tank_x];
    (tank_shape[tank_x])&=3;
    if(!tank_shape[tank_x])
        {
            ++tank_y_array[tank_x];
        }
}


void move_display_tank(void)
{
    move_tank();
    display_tank();
}


#if !defined(MISSILE_DROP_LOOP_MASK)
    #define MISSILE_DROP_LOOP_MASK (1)
#endif

void handle_missile_drops(void)
{
    if(!freeze && !(main_loop_counter&1))
    {
        uint8_t missile_index;
        
        if((missile_index = find_inactive(enemyMissile)) < MAX_NUMBER_OF_MISSILES)
        {
            
            tank_x = (uint8_t) (stinger_x>>1)+(stinger_x&1)-1U+ (uint8_t)(_XL_RAND()%3);
            
            if(tank_active[tank_x])
            {
                if(tank_level[tank_x]<=MAX_TANK_LEVEL)
                {
                    if(tank_y_array[tank_x]<HEIGHT_SHOOT_THRESHOLD && ((tank_level[tank_x]==3) || (!(main_loop_counter&tank_shoot_speed_mask))))
                    {
                        drop_item(&enemyMissile[missile_index],EXPLOSION_THRESHOLD+1);
                    }
                }
                else
                {
                    if(!artillery_shell_active)
                    {
                        artillery_fire();
                    }
                }
            }
        }
    }
} 


#define FORCED_TANK_MASK 15

void move_tanks(void)
{    
    if(forced_tank) // There is a tank that should keep on moving
    {
        tank_x = forced_tank_x;
		// _XL_PRINT(0,YSize-3,"FORCED");
    }
    else
    {
        tank_x=find_random_active_tank();
		// _XL_PRINT(0,YSize-3,"RAMDOM");
		// _XL_SLEEP(1);

    }
	
	// _XL_PRINTD(7,YSize-3,2,tank_x);
	
	if(tank_level[tank_x]>MAX_TANK_LEVEL)
	{
		// _XL_PRINT(0,YSize-2,"HOWITZER");

		return;
	}
	// else
	// {
		// _XL_PRINT(0,YSize-2,"TANK    ");

	// }
		
    if (((tank_level[tank_x]==2)&&(tank_shape[tank_x]&1))&&(tank_y_array[tank_x]!=STINGER_Y-1))
    {
        forced_tank = 0;
    }
    else if((tank_y_array[tank_x]>=HEIGHT_SHOOT_THRESHOLD) || (_XL_RAND()&FORCED_TANK_MASK))
    {
        forced_tank = 1;
        forced_tank_x = tank_x;
    }
    else
    {
        forced_tank = 0;
    }

    move_display_tank();

    if((tank_y_array[tank_x]==STINGER_Y))
    {
        alive = 0;
        display_red_tank();
    }
}


#define handle_stinger_load() \
do \
{ \
    if(!loaded_stinger && rockets_on_screen<MAX_ROCKETS_ON_SCREEN && !stinger_load_counter && remaining_rockets) \
    { \
        loaded_stinger = 1; \
        display_stinger(); \
    } \
    if(stinger_load_counter) \
    { \
        --stinger_load_counter; \
    } \
} while(0)


void fire(void)
{
    uint8_t i;
    uint8_t new_rocket_x;  
    uint8_t offset;
    
    _XL_TICK_SOUND();
    new_rocket_x = (stinger_x>>1)+(stinger_x&1);
    for(i=0;i<number_of_rockets_per_shot;++i)
    {
        if(remaining_rockets && rockets_on_screen<MAX_ROCKETS_ON_SCREEN)
        {
            if((number_of_rockets_per_shot==2)&&i)
            {
                offset = i-2*(stinger_x&1);
            }
            else
            {
                offset = i;
            }
            new_rocket_x+=offset;
            if(i==2)
            {
                if(new_rocket_x>=4)
                {
                    new_rocket_x-=4; 
                }
                else
                {
                    continue;
                }
            }
            if(new_rocket_x<XSize-1 && new_rocket_x)
            {
                compute_next_available_rocket_index();

                // active_rocket[next_rocket] = 1;
                ++rockets_on_screen;
                if(number_of_rockets_per_shot==2)
                {
                    rocket_shape[next_rocket] = rocket_tile[!(stinger_x&1)];
                }
                else
                {
                    rocket_shape[next_rocket] = rocket_tile[stinger_x&1];
                }
                rocket_y[next_rocket] = STINGER_Y-1;
                rocket_x[next_rocket] = new_rocket_x;
                #if !defined(TRAINER)
                    --remaining_rockets;
                #endif
            }
        }
    }
    display_remaining_rockets();

    stinger_load_counter = stinger_reload_loops;
    loaded_stinger = 0;
    display_stinger();
}


#if defined(_XL_NO_UDG)
    void handle_stinger_move(void)
    {
        input = _XL_INPUT();
       
        if(_XL_LEFT(input) && stinger_x>2)
        {
            if(direction==LEFT_DIRECTION)
            {
                ++acceleration_counter;
                if(acceleration_counter>=ACCELERATION_THRESHOLD)
                {
                    acceleration=1;
                }
            }
            else
            {
                acceleration_counter=0;
                acceleration=0;
            }
            direction = LEFT_DIRECTION;			
            move_left();
            if(stinger_x)
            {
                move_left();
            }
            if(acceleration && stinger_x>1)
            {
                move_left();
            }
        }
        else if (_XL_RIGHT(input) && stinger_x<MAX_STINGER_X-1)
        {
            if(direction==RIGHT_DIRECTION)
            {
                ++acceleration_counter;
                if(acceleration_counter>=ACCELERATION_THRESHOLD)
                {
                    acceleration=1;
                }
            }
            else
            {
                acceleration_counter=0;
                acceleration=0;
            }
            direction = RIGHT_DIRECTION;  			
            move_right();
            if(stinger_x<MAX_STINGER_X)
            {
                move_right();
            }
            if(acceleration && (stinger_x<MAX_STINGER_X-1))
            {
                move_right();
            }
        }
        else if (_XL_FIRE(input) && loaded_stinger)
        {
            fire();
            acceleration_counter=0;
            acceleration=0;
        }
        else
        {
            acceleration_counter=0;
            acceleration=0;
        }
    }
#else
    void handle_stinger_move(void)
    {
        input = _XL_INPUT();
        
        if(_XL_LEFT(input) && stinger_x>1)
        {
            if(direction==LEFT_DIRECTION)
            {
                ++acceleration_counter;
                if(acceleration_counter>=ACCELERATION_THRESHOLD)
                {
                    acceleration=1;
                }
            }
            else
            {
                acceleration_counter=0;
                acceleration=0;
            }
            direction = LEFT_DIRECTION;
            move_left();
            if(acceleration && stinger_x>1)
            {
                move_left();
            }
        }
        else if (_XL_RIGHT(input) && (stinger_x<MAX_STINGER_X-1))
        {
            if(direction==RIGHT_DIRECTION)
            {
                ++acceleration_counter;
                if(acceleration_counter>=ACCELERATION_THRESHOLD)
                {
                    acceleration=1;
                }
            }
            else
            {
                acceleration_counter=0;
                acceleration=0;
            }
            direction = RIGHT_DIRECTION;            
            move_right();
            if(acceleration && (stinger_x<MAX_STINGER_X-1))
            {
                move_right();
            }
        }
        else if (_XL_FIRE(input) && loaded_stinger)
        {
            fire();
            acceleration_counter=0;
            acceleration=0;
        }
        else
        {
            acceleration_counter=0;
            acceleration=0;
        }
    }
#endif


#define game_over() \
do \
{ \
    _XL_EXPLOSION_SOUND(); \
    _XL_SET_TEXT_COLOR(_XL_RED); \
    PRINT_CENTERED("GAME OVER"); \
    one_second(); \
    control_instructions(); \
    freeze=1; \
    tank_intro_animation(); \
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
    killed_artillery = 0; \
    freeze = 0; \
    lives = INITIAL_LIVES; \
    next_threshold = NEXT_EXTRA_LIFE; \
} while(0)



#define level_initialization() \
do \
{   \
    level_count_down=LEVEL_COUNT_DOWN+level*32; \
    extra_points_counter = 6; \
    fire_power = RED_FIRE_POWER_VALUE; \
    freeze = 0; \
    acceleration = 0; \
    acceleration_counter = 0; \
    powerUp = 0; \
    next_rocket = 0; \
    rockets_on_screen = 0; \
    stinger_load_counter = 0; \
    hyper_counter = 0; \
    forced_tank = 0; \
    freeze_locked = 1; \
    secret_locked = 1; \
    loaded_stinger = 1; \
    alive = 1; \
    if(level>=2) \
    { \
        max_occupied_columns = MAX_DENSILY_OCCUPIED_COLUMNS+level; \
    } \
    else \
    { \
        max_occupied_columns = MAX_SPARSELY_OCCUPIED_COLUMNS; \
    } \
    stinger_reload_loops = RED_SPEED_VALUE; \
    auto_recharge_counter = AUTO_RECHARGE_COOL_DOWN; \
    remaining_rockets = MAX_ROCKETS; \
    stinger_x = XSize; \
    stinger_shape_tile = (uint8_t) 2*((stinger_x)&1); \
    stinger_color = _XL_CYAN; \
    number_of_rockets_per_shot = 1; \
    if(level>=4) \
    { \
        tank_shoot_speed_mask = VERY_FAST_TANK_SHOOT_MASK; \
    } \
    else if(level>=2) \
    { \
        tank_shoot_speed_mask = FAST_TANK_SHOOT_MASK; \
    } \
    else \
    { \
        tank_shoot_speed_mask = SLOW_TANK_SHOOT_MASK; \
    } \
    initialize_items(); \
    artillery_shell_active = 0; \
    heavy_tank_counter = 0; \
    _XL_CLEAR_SCREEN(); \
    set_wall_color(level_color[level&1]); \
    display_wall(BOTTOM_WALL_Y); \
    _XL_DRAW(0,HEIGHT_SHOOT_THRESHOLD,WALL_TILE,_XL_CYAN); \
    _XL_DRAW(XSize-1,HEIGHT_SHOOT_THRESHOLD,WALL_TILE,_XL_CYAN); \
} while(0)


#define tank_counter main_loop_counter

void initialize_tank_at_level_restart(void)
{
    // tank_y_array[tank_x]=INITIAL_RESPAWN_TANK_Y;
    ++tank_counter;
    display_tank();
    // _XL_TOCK_SOUND();
}



void reset_tanks(void)
{ 
    for(tank_x=0;tank_x<XSize;++tank_x)
    {
        tank_active[tank_x]=0;
    }
} 


#define spawn_initial_light_tanks() \
    light_tanks_to_kill = LIGHT_TANKS_ON_FIRST_LEVEL-killed_light_tanks-(level<<3);  \
    \
    if(light_tanks_to_kill<max_occupied_columns) \
    { \
        spawned_light=light_tanks_to_kill; \
    } \
    else \
    { \
        spawned_light=max_occupied_columns; \
    } \
    \
    tank_counter = 0; \
    \
    while(tank_counter<spawned_light) \
    { \
        spawn_light_tank(); \
        initialize_tank_at_level_restart(); \
    } \
    \
    light_tanks_to_spawn = light_tanks_to_kill-spawned_light;


#define spawn_initial_heavy_tanks() \
    heavy_tanks_to_kill = heavy_tanks_on_level[level]-killed_heavy_tanks; \
    \
    if(heavy_tanks_to_kill<max_occupied_columns - spawned_light) \
    { \
        spawned_heavy = heavy_tanks_to_kill;   \
    } \
    else \
    { \
        spawned_heavy = max_occupied_columns - spawned_light; \
    } \
    \
    tank_counter = 0; \
    \
    while(tank_counter<spawned_heavy) \
    { \
        spawn_heavy_tank(); \
        initialize_tank_at_level_restart(); \
    } \
    \
    heavy_tanks_to_spawn = heavy_tanks_to_kill-spawned_heavy;

/*
    _XL_PRINTD(0,1,3,spawned_light); \
    _XL_PRINTD(0,0,3,spawned_heavy); \
    _XL_WAIT_FOR_INPUT(); \
*/



#define tank_initialization() \
{ \
    uint8_t spawned_light; \
    uint8_t spawned_heavy; \
    \
    reset_tanks(); \
    \
    spawn_initial_light_tanks(); \
    \
    spawn_initial_heavy_tanks(); \
    \
    update_tank_move_speed_mask(); \
    \
    \
    for(tank_x=0;tank_x<MAX_ROCKETS_ON_SCREEN;++tank_x) \
    { \
        active_rocket[tank_x] = 0; \
    } \
}


#if YSize<=20
    #define _NEXT_ROW i
#else
    #define _NEXT_ROW ((i)<<1)
#endif

#if defined(_XL_NO_JOYSTICK)
    #define CONTROLS_STRING "PRESS A KEY"
    #define CONTROLS_LEN 13
    #define DELETE_CONTROLS "           "
#else
    #if defined(_XL_CURSORS)
        #if XSize>=21
            #define CONTROLS_STRING "PRESS A KEY"
            #define CONTROLS_LEN 21
        #elif XSize>=19
            #define CONTROLS_STRING "PRESS A KEY"
            #define CONTROLS_LEN 19
        #else
            #define CONTROLS_STRING "PRESS A KEY"
            #define CONTROLS_LEN 15
        #endif
    #else
        #define CONTROLS_STRING     "PRESS FIRE"
        #define CONTROLS_LEN 12
        #define DELETE_CONTROLS     "          "

    #endif
#endif

// #if YSize<=23
#define CONTROLS_Y YSize-2
// #else
    // #define CONTROLS_Y YSize-3
// #endif



#if YSize<=22
    #define _HISCORE_Y 1
#else
    #define _HISCORE_Y 2
#endif

#if XSize>=20
    #define _STINGER_STRING \
        "S T I N G E R"
#else
    #define _STINGER_STRING \
        "STINGER"
#endif

void display_cleared(void)
{
	_XL_SET_TEXT_COLOR(_XL_CYAN);
	PRINT_CENTERED("C L E A R E D");

}
	
#if !defined(_XL_NO_TEXT_COLOR) && !defined(_XL_NO_COLOR)
	void display_stinger_string(uint8_t color)
	{
		_XL_SET_TEXT_COLOR(color);
		PRINT_CENTERED_ON_ROW(YSize/3-2,_STINGER_STRING);
	}


	#if !defined(NO_EXTRA_TITLE)
	void display_enemies_string(uint8_t color)
	{
		_XL_SET_TEXT_COLOR(color);
		PRINT_CENTERED_ON_ROW(YSize/3-2, "ENEMIES");
	}
	#endif

	void display_victory_string(uint8_t color)
	{
		_XL_SET_TEXT_COLOR(color);
		PRINT_CENTERED_ON_ROW(YSize/2,"V I C T O R Y");
	}	
#else
	void _display_stinger_string(void)
	{
		PRINT_CENTERED_ON_ROW(YSize/3-2,_STINGER_STRING);
	}

	#if !defined(NO_EXTRA_TITLE)

	void _display_enemies_string(void)
	{
		PRINT_CENTERED_ON_ROW(YSize/3-2, "ENEMIES");
	}
	#endif

	void _display_victory_string(void)
	{
		PRINT_CENTERED_ON_ROW(YSize/2,"V I C T O R Y");
	}	

	#define display_stinger_string(color) \
		_display_stinger_string();

	#define display_enemies_string(color) \
	    _display_enemies_string();

	#define display_victory_string(color) \
		_display_victory_string();

#endif


uint8_t fire_pressed_after_time(void)
{
	return (!time_counter) && (fire_pressed = _XL_FIRE(_XL_INPUT()));
}


#if !defined(NO_EXTRA_TITLE) || YSize<15
    // #if YSize>=15
        void control_instructions(void) 
        {
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            PRINT_CENTERED_ON_ROW(CONTROLS_Y,\
                                       CONTROLS_STRING);
        }
        void delete_instructions(void)
        {
            PRINT_CENTERED_ON_ROW(CONTROLS_Y,\
                                       DELETE_CONTROLS);
        }
#else
        #define control_instructions()
        #define delete_instructions()
    // #endif
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
    } while(0)
        
    void display_enemies(void)
    {
        uint8_t i;
        
        for(i=0;i<5;++i)
        {
            _XL_DRAW(XSize/2-5,YSize/3+1+_NEXT_ROW, enemy_tile[i][0], enemy_tile[i][1]);
            _XL_SET_TEXT_COLOR(_XL_GREEN);
            _XL_PRINT(XSize/2-5+3,YSize/3+1+_NEXT_ROW, (char *)enemy_name[i]);
        }
    }
// #else
    // #define display_items()
    // #define display_enemies()
    // #define control_instructions()
    // #define delete_instructions()
// #endif


#if !defined(NO_EXTRA_TITLE)
void mortar_intro_animation(void)
{
    uint8_t i;
    // uint8_t fire = 0;
    // uint8_t time_counter = 25;
    switch_counter = 0;

	fire_pressed = 0;
	time_counter = 5;

    tank_active[1]=1;    
    tank_shape[1]=0;
    tank_y_array[1]=2;
    tank_level[1]=2;

    
    tank_active[XSize-2]=1;    
    tank_shape[XSize-2]=0;    
    tank_y_array[XSize-2]=2;
    tank_level[XSize-2]=2;

    
    _XL_DRAW(0,4,MORTAR_TILE,_XL_GREEN);
    _XL_DRAW(XSize-1,4,MORTAR_TILE,_XL_GREEN);
    
    do
    {
        // _XL_SET_TEXT_COLOR(_XL_WHITE);

        for(i=5;i<YSize-2;++i)
        {
			
            if(fire_pressed_after_time())
            {
                break;
            }
			
            if(!(i&3))
            {
                tank_x=1;
                move_display_tank();

                tank_x=XSize-2;
                move_display_tank();     

                if(tank_y_array[1]>=YSize-2)
                {
                    tank_y_array[1]=2;
                    tank_y_array[XSize-2]=2;
                    _XL_DELETE(1,YSize-2);
                    _XL_DELETE(XSize-2,YSize-2);    
                }                
                delete_instructions(); 
            }
            else
            {
                control_instructions();
            }

            // if(fire_pressed_after_time())
            // {
                // break;
            // }

            if(time_counter)
            {
                --time_counter;
            }
            
            _XL_DRAW(0,i,BULLET_TILE,_XL_WHITE);
            _XL_DRAW(XSize-1,i,BULLET_TILE,_XL_WHITE);
            display_enemies_string(_XL_CYAN);

            // if(fire_pressed_after_time())
            // {
                // break;
            // }
            // short_sleep();

            short_sleep();
			#if!defined (_XL_NO_TEXT_COLOR)
            display_enemies_string(_XL_RED);
			#endif

            _XL_DELETE(0,i);
            _XL_DELETE(XSize-1,i);

        }

        _XL_DRAW(0,YSize-2,EXPLOSION_TILE,_XL_RED);
        _XL_DRAW(XSize-1,YSize-2,EXPLOSION_TILE,_XL_RED);
        less_short_sleep();
        _XL_DELETE(0,YSize-2);
        _XL_DELETE(XSize-1,YSize-2);

        ++switch_counter;
    }
    while(!fire_pressed);
    _XL_ZAP_SOUND();
    one_second();
}
#else
#define mortar_intro_animation()
#endif

#if defined(_XL_NO_COLOR) || defined(_XL_NO_TEXT_COLOR)
	#define display_initial_screen() \
	do \
	{ \
		_XL_CLEAR_SCREEN(); \
		\
		set_wall_color(_XL_GREEN); \
		display_wall(0); \
		display_wall(BOTTOM_WALL_Y+1); \
		\
		_XL_SET_TEXT_COLOR(_XL_CYAN); \
		PRINT_CENTERED_ON_ROW(_HISCORE_Y, "HISCORE"); \
		\
		_XL_SET_TEXT_COLOR(_XL_WHITE); \
		_XL_PRINTD(XSize/2-3,_HISCORE_Y+1,5,hiscore); \
		\
		_XL_SET_TEXT_COLOR(_XL_WHITE); \
		PRINT_CENTERED_ON_ROW(YSize/3, "FABRIZIO CARUSO"); \
		\
		display_items(); \
		tank_intro_animation(); \
		_XL_ZAP_SOUND(); \
		one_second();    \
	} while(0)
#elif XSize>=18
	#define display_initial_screen() \
	do \
	{ \
		_XL_CLEAR_SCREEN(); \
		\
		set_wall_color(_XL_GREEN); \
		display_wall(0); \
		display_wall(BOTTOM_WALL_Y+1); \
		\
		_XL_SET_TEXT_COLOR(_XL_CYAN); \
		PRINT_CENTERED_ON_ROW(_HISCORE_Y, "HISCORE"); \
		\
		_XL_SET_TEXT_COLOR(_XL_WHITE); \
		_XL_PRINTD(XSize/2-3,_HISCORE_Y+1,5,hiscore); \
		\
		_XL_SET_TEXT_COLOR(_XL_WHITE); \
		PRINT_CENTERED_ON_ROW(YSize/3, "FABRIZIO CARUSO"); \
		\
		display_items(); \
		tank_intro_animation(); \
		_XL_ZAP_SOUND(); \
		display_stinger_string(_XL_CYAN); \
		one_second();    \
	} while(0)
#else
	#define display_initial_screen() \
	do \
	{ \
		_XL_CLEAR_SCREEN(); \
		\
		set_wall_color(_XL_GREEN); \
		display_wall(0); \
		display_wall(BOTTOM_WALL_Y+1); \
		\
		_XL_SET_TEXT_COLOR(_XL_CYAN); \
		PRINT_CENTERED_ON_ROW(_HISCORE_Y, "HISCORE"); \
		\
		_XL_SET_TEXT_COLOR(_XL_WHITE); \
		_XL_PRINTD(XSize/2-3,_HISCORE_Y+1,5,hiscore); \
		\
		_XL_SET_TEXT_COLOR(_XL_WHITE); \
		PRINT_CENTERED_ON_ROW(YSize/3, "FABRIZIO"); \
		PRINT_CENTERED_ON_ROW(YSize/3+1, "CARUSO"); \
		\
		display_items(); \
		tank_intro_animation(); \
		_XL_ZAP_SOUND(); \
		display_stinger_string(_XL_CYAN); \
		one_second();    \
	} while(0)	
#endif

void tank_intro_animation(void)
{
    uint8_t i;
    switch_counter = 0;

	fire_pressed = 0;
	time_counter = 5;

    reset_tanks();
    
    tank_active[0]=1;    
    tank_shape[0]=0;
    tank_y_array[0]=2;
    tank_level[0]=3;
    
    tank_active[XSize-1]=1;    
    tank_shape[XSize-1]=0;    
    tank_y_array[XSize-1]=2;
    tank_level[XSize-1]=3;

    tank_active[1]=1;    
    tank_shape[1]=0;
    tank_y_array[1]=YSize-3;
    tank_level[1]=1;
    
    tank_active[XSize-2]=1;    
    tank_shape[XSize-2]=0;    
    tank_y_array[XSize-2]=YSize-3;
    tank_level[XSize-2]=1;

	#if defined(_XL_NO_TEXT_COLOR) || defined(_XL_NO_COLOR)
	display_stinger_string(_XL_RED);
	#endif
    do
    {
        for(i=3;i<(YSize-5)*4;++i)
        {
			#if !defined(_XL_NO_TEXT_COLOR) && !defined(_XL_NO_COLOR)
            display_stinger_string(_XL_RED);
			#endif
			// if(fire_pressed_after_time())
            // {
                // break;
            // }
            if(!(i&3))
            {
                delete_instructions(); 
            }
            else
            {
                control_instructions();
            }            
            if(time_counter)
            {
                --time_counter;
            }
            if(fire_pressed_after_time())
            {
				// _XL_PRINT(0,YSize-3,"FIRST");
				// _XL_SLEEP(1);
                break;
            }
            if(!(switch_counter&1))
            {
                tank_x=0;
                move_display_tank();
                
                tank_x=XSize-1;
                move_display_tank();
                
				// if(fire_pressed_after_time())
				// {
					// break;
				// }
				
                tank_x=1;
                push_display_tank();
                
                tank_x=XSize-2;
                push_display_tank();
            }
            else
            {
                tank_x=1;
                move_display_tank();
                
                tank_x=XSize-2;
                move_display_tank();
                
				// if(fire_pressed_after_time())
				// {
					// break;
				// }
				
                tank_x=0;
                push_display_tank();
                
                tank_x=XSize-1;
                push_display_tank();
            }
            
			#if!defined (_XL_NO_TEXT_COLOR)
            display_stinger_string(_XL_YELLOW);
            #endif
			short_sleep();

            if(fire_pressed_after_time())
            {
				// _XL_PRINT(0,YSize-3,"SECOND");
				// _XL_SLEEP(1);
                break;
            }
        }
        ++switch_counter;
        if(!fire_pressed)
        {
            for(i=0;i<3;++i)
            {
                _XL_DELETE(0,YSize-3-i);
                _XL_DELETE(XSize-1,YSize-3-i);
                _XL_DELETE(1,1+i);
                _XL_DELETE(XSize-2,1+i);
            }
        }

    }
    while(!fire_pressed);    
}


#if !defined(NO_EXTRA_TITLE)
void display_second_screen(void) 
{
    _XL_CLEAR_SCREEN();
    
    set_wall_color(_XL_YELLOW);
    display_wall(0);
    display_wall(BOTTOM_WALL_Y+1);
    display_enemies();
    mortar_intro_animation();
    _XL_CLEAR_SCREEN(); \
}
#else
#define display_second_screen() 

#endif

#if XSize>=31
    #define HI_X ((XSize-10))
#elif XSize>=23
    #define HI_X ((XSize-9))
#else
    #define HI_X ((XSize-8))
#endif

#if XSize>=20
    #define draw_tank_counter_tile() _XL_DRAW(TANK_COUNTER_X,TANK_COUNTER_Y,HEAVY_TANK_TILE_0, level_color[(level+1)&1])

#else
    #define draw_tank_counter_tile()
#endif


#if XSize>=31
    #define display_stats() \
    do \
    { \
        _XL_SET_TEXT_COLOR(_XL_WHITE); \
        display_score(); \
        _XL_SET_TEXT_COLOR(_XL_GREEN); \
        _XL_PRINT(HI_X,0,"HI"); \
        _XL_SET_TEXT_COLOR(_XL_WHITE); \
        _XL_PRINTD(HI_X+2,0,5, hiscore); \
        _XL_DRAW(6,0,ROCKET_TILE_1,_XL_CYAN); \
        _XL_DRAW(POWER_UP_X,0,POWER_UP_TILE, _XL_WHITE); \
        draw_tank_counter_tile(); \
        display_remaining_rockets(); \
        display_power_up_counter(); \
        display_level(); \
        display_lives(_XL_WHITE); \
        display_power_ups(); \
        display_enemy_counter(); \
    } while(0)
#elif XSize>=20
    #define display_stats() \
    do \
    { \
        _XL_SET_TEXT_COLOR(_XL_WHITE); \
        display_score(); \
        _XL_SET_TEXT_COLOR(_XL_GREEN); \
        _XL_CHAR(HI_X,0,'H'); \
        _XL_SET_TEXT_COLOR(_XL_WHITE); \
        _XL_PRINTD(HI_X+1,0,5, hiscore); \
        _XL_DRAW(6,0,ROCKET_TILE_1,_XL_CYAN); \
        _XL_DRAW(POWER_UP_X,0,POWER_UP_TILE, _XL_WHITE); \
        draw_tank_counter_tile(); \
        display_remaining_rockets(); \
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
        _XL_DRAW(6,0,ROCKET_TILE_1,_XL_CYAN); \
        _XL_DRAW(POWER_UP_X,0,POWER_UP_TILE, _XL_WHITE); \
        draw_tank_counter_tile(); \
        display_remaining_rockets(); \
        display_power_up_counter(); \
        display_level(); \
        display_lives(_XL_WHITE); \
        display_power_ups(); \
        display_enemy_counter(); \
    } while(0)
#endif



void handle_auto_recharge(void)
{
    if(!remaining_rockets)
    {
        _XL_SET_TEXT_COLOR(_XL_RED);
        PRINT_CENTERED_ON_ROW(2,"RECHARGE"); \
        if(auto_recharge_counter)
        {
            --auto_recharge_counter;
        }
        else if(!rechargeItem._active)
        {
            if(stinger_x<XSize)
            {
                rechargeItem._x = XSize-2;
            }
            else
            {
                rechargeItem._x = 1;
            }
                
            rechargeItem._y = STINGER_Y;
            rechargeItem._active = 1;
            rechargeItem._counter = 2*XSize+XSize/2;
            
            auto_recharge_counter=AUTO_RECHARGE_COOL_DOWN;
            _XL_PING_SOUND();
        }
    }
}


void display_level_at_start_up(void)
{
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    
    if(level==LAST_LEVEL)
    {
        _XL_PRINT(XSize/2-6, YSize/2,    "FINAL LEVEL" );
    }
    else
    {
        _XL_PRINT(XSize/2-4, YSize/2,      "LEVEL " );
        _XL_PRINTD(XSize/2+2,YSize/2,1,level+1);
    }
	// #if !defined(NO_EXTRA_TITLE)
    if(level<LAST_LEVEL)
    {
        uint8_t i;
        
        _XL_PRINT(XSize/2-2-level, YSize/2+2, "VS");
        for(i=0;i<=level;++i)
        {
            _XL_DRAW(XSize/2+1-level+i*2, YSize/2+2, enemy_tile[i][0], enemy_tile[i][1]);
        }
    }
    // #endif
    one_second();
    control_instructions();
    sleep_and_wait_for_input();
    _XL_PRINT(XSize/2-6, YSize/2,   "           ");
    {
        uint8_t i;
        for(i=0;i<=2+2*level+1;++i)
        {
            _XL_DELETE(XSize/2-2-level+i, YSize/2+2);
        }
    }
    display_wall(CONTROLS_Y);
    // _XL_PRINT(XSize/2-2-level, YSize/2+2,_XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE);

}


void handle_tank_movement(void)
{
    if(!freeze)
    {
        if(!(main_loop_counter&tank_move_speed_mask))
        {
            move_tanks();
        }
    }
    else
    {
        --freeze;
        if(!freeze)
        {
            display_tanks();
        }
    }
}


#define handle_next_level() \
do \
{ \
    ++level; \
    PRINT_CENTERED_ON_ROW(1,"      "); \
    display_cleared(); \
    _XL_TOCK_SOUND(); \
    if(powerUp) \
    { \
        do \
        { \
            --powerUp; \
            increase_score(POWER_UP_BONUS); \
            display_power_up_counter(); \
            _XL_TICK_SOUND(); \
            less_short_sleep(); \
        } while(powerUp); \
    } \
    \
    control_instructions();    \
    sleep_and_wait_for_input(); \
    display_wall(CONTROLS_Y); \
    killed_heavy_tanks = 0; \
    killed_light_tanks = 0; \
} while(0)


#define player_dies() \
do \
{ \
    --lives; \
    stinger_color=_XL_RED; \
    display_stinger(); \
    stinger_color=_XL_CYAN; \
    _XL_EXPLOSION_SOUND(); \
    one_second(); \
    control_instructions(); \
    sleep_and_wait_for_input(); \
} while(0)


void tank_animation(void)
{
    uint8_t i;
    
    tank_y_array[tank_x]=YSize/2-7+(uint8_t) ((_XL_RAND())&15);
    tank_level[tank_x]=1;
    tank_shape[tank_x]=0;

    display_victory_string(_XL_YELLOW);

    for(i=0;i<3;++i)
    {
        display_tank();
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR/4);            
        _XL_DRAW(tank_x,tank_y_array[tank_x],TANK_DEATH_TILE,_XL_RED);
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR/4);            
    }
    _XL_EXPLOSION_SOUND();
    _XL_DELETE(tank_x,tank_y_array[tank_x]);
    display_victory_string(_XL_GREEN);
    short_sleep();
}


void victory_animation(void)
{
    uint8_t i;
    uint8_t j;
    uint8_t k;

    for(i=1;i<XSize-1;++i)
    {
        tank_x = i;
        tank_animation();
        tank_x = XSize-1-i;
        tank_animation();
    }
    
    for(i=0;i<XSize;++i)
    {
        _XL_DRAW(i,YSize/2-1,WALL_TILE,_XL_CYAN);
        _XL_DRAW(XSize-1-i,YSize/2+1,WALL_TILE,_XL_CYAN);
        _XL_TICK_SOUND();
        short_sleep();
    }    
    less_short_sleep();        

	// #if !defined(NO_EXTRA_TITLE)
    for(i=0;i<=4;++i)
    {
        _XL_DRAW(XSize/2+1-level+i*2, YSize/2+2, enemy_tile[i][0], enemy_tile[i][1]);
        _XL_TOCK_SOUND();
        less_short_sleep();
    }    
	// #endif

    for(j=0;j<5;++j)
    {
        for(i=0;i<XSize-2;++i)
        {
            for(k=0;k<3;++k)
            {
                _XL_DRAW(i+k,YSize/2-1,WALL_TILE,_XL_YELLOW);
                _XL_DRAW(XSize-1-i-k,YSize/2+1,WALL_TILE,_XL_YELLOW);
            }
            short_sleep();
            display_victory_string(_XL_RED);

            for(k=0;k<3;++k)
            {
                _XL_DRAW(i+k,YSize/2-1,WALL_TILE,_XL_CYAN);
                _XL_DRAW(XSize-1-i-k,YSize/2+1,WALL_TILE,_XL_CYAN);
            }        
            short_sleep();

            display_victory_string(_XL_GREEN);

        }
    }        
    
    one_second();
    for(i=0;i<=4;++i)
    {
		k=XSize/2+1-level+i*2;

		// #if !defined(NO_EXTRA_TITLE)
        for(j=0;j<9;++j)
        {
            _XL_DRAW(k, YSize/2+2, enemy_tile[i][0], enemy_tile[i][1]);
            short_sleep();
            _XL_DRAW(k, YSize/2+2, EXPLOSION_TILE, _XL_RED);
            short_sleep();
        }
		// #endif
        _XL_EXPLOSION_SOUND();
        _XL_SLOW_DOWN(12U*_XL_SLOW_DOWN_FACTOR);        
        _XL_DELETE(k, YSize/2+2);
    }
    short_sleep();
    
    control_instructions();
    sleep_and_wait_for_input();
    _XL_CLEAR_SCREEN();
}


#define display_level_screen() \
do \
{ \
    display_stinger(); \
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
            stinger_color = _XL_CYAN; \
            number_of_rockets_per_shot=1; \
            PRINT_CENTERED_ON_ROW(1,"      "); \
            display_power_ups(); \
        } \
    } \
}


#define INITIALIZE_CROSS_LIB() \
    _XL_INIT_GRAPHICS(); \
    _XL_INIT_INPUT() \
    _XL_INIT_SOUND() \


#define LEVEL_COUNT_DOWN 95
#define BONUS_DROP_THRESHOLD 55
#define SECRET_ITEM_DROP_THRESHOLD 128

void handle_level_end(void)
{
	if(level_count_down==(uint8_t) LEVEL_COUNT_DOWN+(uint8_t) level*32U)
	{
		uint8_t i;
	   
		for(i=0;i<25;++i)
		{
			_XL_SET_TEXT_COLOR(_XL_RED);
			PRINT_CENTERED_ON_ROW(YSize/2,"BONUS ");
			short_sleep();
			_XL_TICK_SOUND();
			#if !defined(_XL_NO_TEXT_COLOR)
			_XL_SET_TEXT_COLOR(_XL_YELLOW);
			PRINT_CENTERED_ON_ROW(YSize/2,"BONUS ");
			#else
			PRINT_CENTERED_ON_ROW(YSize/2,"      ");
			#endif
			short_sleep();
		}
		_XL_PING_SOUND();
		less_short_sleep();
		PRINT_CENTERED_ON_ROW(YSize/2,"     ");
		// _XL_SET_TEXT_COLOR(_XL_WHITE);
		// PRINT_CENTERED_ON_ROW(YSize-2,"BONUS");
	}
   
	if(level_count_down>BONUS_DROP_THRESHOLD)
	{
		uint8_t index;
	   
	    // if(!(level_count_down&7))
		// {
			// _XL_SET_TEXT_COLOR(_XL_RED);
			// PRINT_CENTERED_ON_ROW(1,"BONUS ");
		// }
		// else if(((level_count_down&7)==1))
		// {
			// _XL_SET_TEXT_COLOR(_XL_YELLOW);
			// PRINT_CENTERED_ON_ROW(1,"BONUS ");
		// }
	   
		for(index=0;(index<lives) && (index<MAX_NUMBER_OF_EXTRA_POINTS);++index)
		{
			if(!extraPointsItem[index]._active)
			{
				tank_x = (uint8_t) 1U+(uint8_t) (_XL_RAND()%(XSize-2));
				drop_item(&extraPointsItem[index],EXTRA_POINTS_COOL_DOWN);
			}
		}
	   
		if(level>=2)
		{
			if((lives>2) && (!freezeItem._active))
			{
				tank_x = 1U+(uint8_t) (_XL_RAND()%(XSize-2));
				drop_item(&freezeItem,FREEZE_COOL_DOWN);
			}
		   

			if((lives>5) && (!powerUpItem._active))
			{
				tank_x = 1+(uint8_t) (_XL_RAND()%(XSize-2));
				drop_item(&powerUpItem,POWER_UP_COOL_DOWN);
			}
		   
			if(level==LAST_LEVEL)
			{
				if((lives>8) && (!secretItem._active) && (level_count_down<SECRET_ITEM_DROP_THRESHOLD) )
				{
					tank_x = 1+(uint8_t) (_XL_RAND()%(XSize-2));
					drop_item(&secretItem,SECRET_COOL_DOWN);
				}
			}

		}
	}
   
	--level_count_down;
}

#if defined(BENCHMARK)
	#define BENCHMARK_CODE() \
		TicksDelta = clock() - Ticks; \
		Milli = ((TicksDelta % CLOCKS_PER_SEC) * 1000) / CLOCKS_PER_SEC; \
		Cumulative+=Milli; \
		if((main_loop_counter&31)==31) \
		{ \
			if(Average!=0) \
			{ \
				Average=Cumulative/32; \
			} \
			else \
			{ \
				Average=Cumulative; \
			} \
			Cumulative = 0; \
			_XL_PRINTD(0,YSize-6,4,Average); \
		} \
		_XL_PRINTD(0,YSize-7,4,Milli);
#else
	#define BENCHMARK_CODE()
#endif


#if defined(BENCHMARK)
	#define INIT_BENCHMARK_CODE() \
		Ticks = clock();
#else
	#define INIT_BENCHMARK_CODE()
#endif

int main(void)
{
    #if defined(BENCHMARK)
    clock_t Ticks, TicksDelta;
    // unsigned int Sec;
    unsigned int Milli, Cumulative, Average;    
    Cumulative = 0;
    Average = 0;
    #endif
    
    INITIALIZE_CROSS_LIB();

    hiscore = 0;

	#if defined(_XL_NO_COLOR) || defined(_XL_NO_TEXT_COLOR)
		display_stinger_string(_XL_CYAN);
	#endif
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
            
            while(alive && level_count_down) // Inner game loop
            {            
				INIT_BENCHMARK_CODE();
            
                // _XL_PRINTD(0,YSize-4,2,MAX_ROCKETS_ON_SCREEN);
                // _XL_PRINTD(4,YSize-4,2,MAX_NUMBER_OF_MISSILES);
                // _XL_PRINTD(9,YSize-6,2,DEATH_LOOP);
                
                handle_hyper();
                handle_stinger_move();
                handle_stinger_load();
                handle_rockets(); 
                handle_auto_recharge();
                if(!light_tanks_to_kill && !heavy_tanks_to_kill)
                {
                    handle_level_end();
                }
                else
                {
                    handle_tank_movement();
                }
                handle_tank_collisions();
				short_sleep();     

                handle_missile_drops();
                handle_items();
				// if(!alive)
				// {
					// break;
				// }
                handle_artillery_shell();
                ++main_loop_counter;
                
				BENCHMARK_CODE();
            
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
            victory_animation();
        }
        else
        {
            display_wall(CONTROLS_Y);
        }
        game_over();
    }

    return EXIT_SUCCESS;
}

