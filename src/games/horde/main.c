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

#include "images.h"

// #define DEBUG 1
// #define TRAINER 1

#define INITIAL_LEVEL 0

#define LAST_LEVEL 8
#define INITIAL_LIVES 3
#define MAX_LIVES 9

#define NEXT_EXTRA_LIFE 5000U

#define BOW_Y ((YSize)-3)
#define MAX_BOW_X ((XSize)*2-3)
#define POWER_UPS_Y ((BOW_Y)+2)
#define WALL_Y ((YSize)-8)

#define INITIAL_ZOMBIE_Y (((YSize)/2)-2)
#define INITIAL_RESPAWN_ZOMBIE_Y (INITIAL_ZOMBIE_Y)-4
#define BOTTOM_WALL_Y ((BOW_Y)+1)

#define POWER_THRESHOLD 4

#define MAX_ARROWS_ON_SCREEN 12


#define AUTO_RECHARGE_COOL_DOWN 45
#define AUTO_ARROW_RECAHRGE 9

#define MAX_FREEZE 1

#define MINION_POINTS 5
#define BOSS_1_POINTS 10
#define BOSS_2_POINTS 15
#define BOSS_3_POINTS 20

#define EXTRA_POINTS 10
#define RECHARGE_POINTS 15
#define POWERUP_POINTS 20
#define FREEZE_POINTS 30
#define WALL_POINTS 50

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

#define INITIAL_ARROW_RANGE ((INITIAL_ZOMBIE_Y)+1)
#define ITEM_SPAWN_CHANCE 11000U

#define MINION_ENERGY 6
#define BOSS_BASE_ENERGY 14
#define WALL_ENERGY 20

#define MAX_ARROWS 99
#define HYPER_RECHARGE 50
#define ARROW_RECHARGE 30

#define FREEZE_COUNTER_MAX 180;

#define LEVEL_2_ZOMBIE_THRESHOLD (MAX_OCCUPIED_COLUMNS)

#if XSize<=40
    #define MAX_OCCUPIED_COLUMNS (3*(XSize)/4)
#else
    #define MAX_OCCUPIED_COLUMNS (2*(XSize)/3)
#endif

#define FEW_ZOMBIES (2*(MAX_OCCUPIED_COLUMNS)/3)

#define ZOMBIE_MOVE_LOOPS 2

#define NUMBER_OF_MISSILES 5
#define NUMBER_OF_EXTRA_POINTS NUMBER_OF_MISSILES

#define MINIONS_ON_FIRST_LEVEL ((XSize)+1)

#if XSize<=80
    #define BOSSES_ON_FIRST_LEVEL ((XSize)/2)
#else
    #define BOSSES_ON_FIRST_LEVEL 40
#endif

#define MAX_HYPER_COUNTER 100

#if YSize>=20
    #define HEIGHT_SHOOT_THRESHOLD YSize-10
#else
    #define HEIGHT_SHOOT_THRESHOLD YSize-11
#endif

static uint8_t main_loop_counter;

static uint8_t forced_zombie;

static uint8_t forced_zombie_x;

static uint8_t hyper_counter;

static uint8_t item_counter;

static uint8_t next_extra_life_counter;

static const uint8_t zombie_points[] = 
{ 
    MINION_POINTS, // Skeletons
    BOSS_1_POINTS, // Ogre
    BOSS_2_POINTS, // Ghosts
    BOSS_3_POINTS, // Demons
};

static uint8_t minions_to_kill;
static uint8_t bosses_to_kill;

static uint8_t lives;
static uint8_t level;
static uint8_t killed_minions;
static uint8_t killed_bosses;

static uint8_t bosses_to_spawn;
static uint8_t minions_to_spawn;

static uint8_t auto_recharge_counter;

#if defined(COLOR)
static uint8_t arrow_display_color;

static const uint8_t power_up_color[3] = {_XL_RED, _XL_YELLOW, _XL_GREEN};
static const uint8_t arrow_color[3] = {_XL_CYAN, _XL_YELLOW, _XL_WHITE};
#endif

static uint8_t freeze;
static uint8_t powerUp;

static uint8_t number_of_arrows_per_shot;

static uint8_t zombie_y[XSize];
static uint8_t zombie_shape[XSize];
static uint8_t zombie_x;
static uint8_t zombie_speed;
static uint8_t zombie_active[XSize];

static uint8_t energy[XSize];
static uint8_t zombie_level[XSize];

static uint8_t fire_power;

static uint8_t wall_appeared;
static uint8_t freeze_taken;

static const uint8_t zombie_tile[7+1] = 
{
    ZOMBIE_TILE_0, // 0
    ZOMBIE_TILE_0, // 1
    ZOMBIE_TILE_1, // 2
    ZOMBIE_TILE_2, // 3
    ZOMBIE_TILE_3, 
    ZOMBIE_TILE_4, 
    ZOMBIE_TILE_5, 
    ZOMBIE_TILE_6
};

static const uint8_t boss_tile[7+1] =
{
    BOSS_TILE_0,
    BOSS_TILE_0,
    BOSS_TILE_1,
    BOSS_TILE_2,
    BOSS_TILE_3,
    BOSS_TILE_4,
    BOSS_TILE_5,
    BOSS_TILE_6,
};

static const uint8_t bow_tile[8] =
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

static const uint8_t arrow_tile[2] =
{
    ARROW_TILE_0,
    ARROW_TILE_1,
};

static uint8_t bow_x; // range: 0..2*XSize-2^M
static uint8_t bow_shape_tile;
static uint8_t bow_color;

static uint8_t input;

static uint8_t loaded_bow;
static uint8_t active_arrow[MAX_ARROWS_ON_SCREEN];
static uint8_t arrow_shape[MAX_ARROWS_ON_SCREEN];
static uint8_t arrow_x[MAX_ARROWS_ON_SCREEN];
static uint8_t arrow_y[MAX_ARROWS_ON_SCREEN];
static uint8_t remaining_arrows;
static uint8_t arrow_range;

static uint8_t wall[XSize];

static uint8_t bow_reload_loops;

static uint8_t next_arrow;
static uint8_t arrows_on_screen;
static uint8_t bow_load_counter;
static uint8_t alive;

static uint16_t score;
static uint16_t hiscore;

struct ItemStruct
{
    uint8_t _x;
    uint8_t _y;
    uint8_t _tile;
    uint8_t _color;
    uint8_t _active;
    uint8_t _counter;
    void(*_effect)(void);
};
typedef struct ItemStruct Item;
typedef struct ItemStruct Missile;

static Item rechargeItem;
static Item freezeItem;
static Item powerUpItem;
static Item wallItem;

static const uint8_t item_tile[5][2] = 
{
    { POWER_UP_TILE, _XL_WHITE },
    { ARROW_TILE_1, _XL_YELLOW },
    { EXTRA_POINTS_TILE, _XL_YELLOW },
    { FREEZE_TILE, _XL_CYAN },
    { WALL_TILE, _XL_YELLOW },
};

static const char item_name[5][9] = 
{
    _XL_P _XL_O _XL_W _XL_E _XL_R _XL_SPACE _XL_U _XL_P,
    _XL_A _XL_R _XL_R _XL_O _XL_W _XL_S,
    _XL_P _XL_O _XL_I _XL_N _XL_T _XL_S,
    _XL_F _XL_R _XL_E _XL_E _XL_Z _XL_E,
    _XL_W _XL_A _XL_L _XL_L,
};

static Missile beamMissile[NUMBER_OF_MISSILES];
static Item extraPointsItem[NUMBER_OF_MISSILES];


#if XSize>=22
    #define POWER_UP_X 10
#else
    #define POWER_UP_X 9
#endif


void display_power_up_counter(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(POWER_UP_X+1,0,2,powerUp);
}


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


void increase_score(uint8_t value)
{
    score+=value;
    display_score();
}


void recharge_effect(void)
{
    recharge_arrows(ARROW_RECHARGE);
    increase_score(RECHARGE_POINTS);
}


#define RANGE_X 0

#if XSize <= 16
    #define RANGE_STRING _XL_R 
    #define SPEED_STRING _XL_S 
    #define POWER_STRING _XL_P
    #define STR_LEN 1
    #define SPEED_X 3
    #define POWER_X ((XSize)-5)

#elif XSize <= 19
    #define RANGE_STRING _XL_R _XL_N
    #define SPEED_STRING _XL_S _XL_P
    #define POWER_STRING _XL_P _XL_O
    #define STR_LEN 2
    #define SPEED_X 4
    #define POWER_X ((XSize)-6)
#elif XSize <= 25
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
    
    for(i=0;i<NUMBER_OF_MISSILES;++i)
    {
        if(!itemArray[i]._active)
        {
            return i;
        }
    }
    return i;
}


#if defined(COLOR)
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
#else
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
        _XL_DRAW(ARROWS_X+i,POWER_UPS_Y,ARROW_TILE_0,1);
    }
}
#endif


void activate_hyper(void)
{
    _XL_ZAP_SOUND();
    bow_reload_loops=HYPER_SPEED_VALUE;
    recharge_arrows(HYPER_RECHARGE);
    hyper_counter = MAX_HYPER_COUNTER;
    bow_color = _XL_RED;
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT_CENTERED_ON_ROW(1,_XL_SPACE _XL_H _XL_Y _XL_P _XL_E _XL_R _XL_SPACE );
}


void power_up_effect(void)
{
    ++powerUp;
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
            number_of_arrows_per_shot = 2;
        break;
        
        case 6:
            number_of_arrows_per_shot = 3;
        break;
        
        case 7:
            fire_power = YELLOW_FIRE_POWER_VALUE;
        break;
        
        case 8:
            fire_power = GREEN_FIRE_POWER_VALUE;
        break;
        #endif
        
        case 10:
        case 20:
        case 30:
        case 40:
            activate_hyper();
        break;
        
        case 15:
        case 25:
        case 35:
            freeze_taken=0;
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


void wall_effect(void)
{
    uint8_t i;
    
    increase_score(WALL_POINTS);
    
    for(i=3*(XSize)/8;i<1+3*(XSize)/8+(XSize)/4;++i)
    {
        if(zombie_y[i]<WALL_Y-1 || !zombie_active[i])
        {
            wall[i]=WALL_ENERGY;
            _XL_DRAW(i,WALL_Y,WALL_TILE,_XL_YELLOW);            
        }
        else
        {
            wall[i]=0;
        }
    }
    wall_appeared = 1;
}


void delete_above_zombie(void)
{
    _XL_DELETE(zombie_x, zombie_y[zombie_x]-1);
}


void display_zombie(void)
{
    uint8_t status = zombie_shape[zombie_x];
    uint8_t pos = zombie_y[zombie_x];
    uint8_t color;
    uint8_t tile0;

    tile0 = BOSS_TILE_0;

    if(zombie_level[zombie_x]==1)
    {
        color = _XL_GREEN;
    }
    else if(zombie_level[zombie_x]==2)
    {
        if(!freeze)
        {
            tile0 = ZOMBIE_DEATH_TILE;
            color = _XL_YELLOW;
        }
    }
    else if(!zombie_level[zombie_x])
    {
        tile0 = ZOMBIE_TILE_0;
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
        delete_above_zombie();
        _XL_DRAW(zombie_x, pos, tile0, color);
    }
    else
    {
        uint8_t tile1;

        if(!zombie_level[zombie_x])
        {
            tile0 = zombie_tile[status<<1];
            tile1 = zombie_tile[1+(status<<1)];
        }
        else
        {
            tile0 = boss_tile[status<<1];
            tile1 = boss_tile[1+(status<<1)]; 
        }
        _XL_DRAW(zombie_x, pos, tile0, color);
        _XL_DRAW(zombie_x,1 + pos, tile1, color);
    }
}


void freeze_effect(void)
{
    freeze=FREEZE_COUNTER_MAX;
    for(zombie_x=0;zombie_x<XSize;++zombie_x)
    {
        if(zombie_active[zombie_x])
        {
            display_zombie();
        }
    }
    increase_score(FREEZE_POINTS);
    ++freeze_taken;
}


void beam_effect(void)
{
    alive=0;
}


void initialize_items(void)
{
    uint8_t i;
    
    rechargeItem._active = 0;
    rechargeItem._tile = ARROW_TILE_0;
    rechargeItem._color = _XL_YELLOW;
    rechargeItem._effect = recharge_effect;
    
    freezeItem._active = 0;
    freezeItem._tile = FREEZE_TILE;
    freezeItem._color = _XL_CYAN;
    freezeItem._effect = freeze_effect;

    powerUpItem._active = 0;
    powerUpItem._tile = POWER_UP_TILE;
    powerUpItem._color = _XL_WHITE;
    powerUpItem._effect = power_up_effect;    

    wallItem._active = 0;
    wallItem._tile = WALL_TILE;
    wallItem._color = _XL_YELLOW;
    wallItem._effect = wall_effect; 
    
    for(i=0;i<NUMBER_OF_MISSILES;++i)
    {
        beamMissile[i]._active = 0;
        beamMissile[i]._tile = BEAM_TILE;
        beamMissile[i]._color = _XL_CYAN;
        beamMissile[i]._effect = beam_effect;
        
        extraPointsItem[i]._active = 0;
        extraPointsItem[i]._tile = EXTRA_POINTS_TILE;
        extraPointsItem[i]._color = _XL_YELLOW;
        extraPointsItem[i]._effect = extra_points_effect;
    }
}


void display_level(void)
{
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINTD(XSize-1,0,1,level+1);
}


void display_lives(void)
{
    _XL_DRAW(XSize-3,POWER_UPS_Y,bow_tile[4+0+bow_shape_tile],_XL_CYAN);
    _XL_DRAW(XSize-2,POWER_UPS_Y,bow_tile[1+4+bow_shape_tile],_XL_CYAN);
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(XSize-1,POWER_UPS_Y,1,lives);
}


void display_bow(void)
{
    _XL_DRAW(bow_x/2,BOW_Y,bow_tile[4*loaded_bow+0+bow_shape_tile],bow_color);
    _XL_DRAW(bow_x/2+1,BOW_Y,bow_tile[1+4*loaded_bow+bow_shape_tile],bow_color);  
}


void move_left(void)
{
    bow_shape_tile = 2*((--bow_x)&1);
    if(bow_shape_tile)
    {
        _XL_DELETE(bow_x/2+2,BOW_Y);
    }
 
    display_bow();
}


void move_right(void)
{
    bow_shape_tile = 2*((++bow_x)&1);
    if(!bow_shape_tile)
    {
        _XL_DELETE(bow_x/2-1,BOW_Y);
    }

    display_bow();
}


void drop_item(register Item *item, uint8_t max_counter)
{
    _XL_TICK_SOUND();
    item->_active = 1;
    item->_x = zombie_x;
    item->_y = zombie_y[zombie_x]+2;
    item->_counter=max_counter;
}


void handle_item(register Item* item)
{
    if(item->_active)
    {
        if(item->_y<BOW_Y)
        {
            _XL_DELETE(item->_x,item->_y);
            if(main_loop_counter&1)
            {
                ++(item->_y);
            }
            _XL_DRAW(item->_x,item->_y,item->_tile,item->_color);
        }
        
        if((item->_y==BOW_Y) && (item->_counter))
        {
            if(item->_counter&1)
            {
                _XL_DELETE(item->_x,item->_y);
            }
            else
            {
                _XL_DRAW(item->_x,item->_y,item->_tile,item->_color);
            }
 
            if(item->_x==(bow_x/2)+(bow_x&1))
            {
                item->_effect();
                _XL_PING_SOUND();
                item->_active=0;
            }
            display_bow();
            --item->_counter;
            if(!(item->_counter))
            {
                item->_active=0;
            }
        }
    }   
}


void handle_items(void)
{
    uint8_t i;
    
    handle_item(&rechargeItem);
    handle_item(&freezeItem);
    handle_item(&powerUpItem);
    handle_item(&wallItem);
    
    for(i=0;i<NUMBER_OF_MISSILES;++i)
    {
        handle_item(&extraPointsItem[i]);
        handle_item(&beamMissile[i]);
    }
}


#if XSize!=13 && XSize!=26 && XSize!=39 && XSize!=52 && XSize!=65 && XSize!=78
    #define STEP 13
#else
    #define STEP 17
#endif 

// #define STEP 1

uint8_t find_zombie(uint8_t value)
{
    uint8_t i;
    uint8_t index;
    
    index = (uint8_t) (_XL_RAND())%XSize;

    for(i=0;i<XSize;++i)
    {
        index = (index+STEP)%XSize;
        if(zombie_active[index]==value)
        {
            return index;
        }
    }
    return XSize;
}


void activate_zombie(void)
{
    uint8_t old_x;
    
    old_x = zombie_x;
    
    while((old_x==zombie_x) && (old_x+1!=zombie_x) && (old_x-1!=zombie_x))
    {
        zombie_x = find_zombie(0);
    };    
  
    zombie_active[zombie_x]=1;    
    zombie_shape[zombie_x]=0;
    zombie_y[zombie_x]=INITIAL_RESPAWN_ZOMBIE_Y+(level>>1);
}


void spawn_minion(void)
{
    activate_zombie();
    zombie_level[zombie_x]=0;
    energy[zombie_x]=MINION_ENERGY;  
    --minions_to_spawn;
}


void spawn_boss(void)
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
        else if(level<5) // 2, 3, 4
        {
            rank = (uint8_t) (1 + ((_XL_RAND())%3));   
        }
        else // 5, 6, 7, 8
        {
            rank = (uint8_t) (2 + ((_XL_RAND())&1)); 
        }
    } while((rank==2)&&(bosses_to_kill<LEVEL_2_ZOMBIE_THRESHOLD));

    activate_zombie();
    zombie_level[zombie_x]=rank;
    energy[zombie_x]=BOSS_BASE_ENERGY-1;
    --bosses_to_spawn;
}


#if XSize>=32
    #define NORMAL_ZOMBIE_SPEED 1
    #define SLOW_ZOMBIE_SPEED 7
#else
    #define NORMAL_ZOMBIE_SPEED 3
    #define SLOW_ZOMBIE_SPEED 7
#endif 


void update_zombie_speed(void)
{
    if(minions_to_kill + bosses_to_kill<=FEW_ZOMBIES)
    {
        zombie_speed=SLOW_ZOMBIE_SPEED;
    }
    else
    {
        zombie_speed=NORMAL_ZOMBIE_SPEED;
    }
}


void display_wall(uint8_t y)
{
    uint8_t i;
    
    for(i=0; i<XSize; ++i)
    {   
        _XL_DRAW(i,y,WALL_TILE,_XL_YELLOW); 
    }
}


#define _display_red_zombie(tile) \
{ \
    _XL_DRAW(zombie_x,zombie_y[zombie_x],tile,_XL_RED); \
}


void display_red_zombie(void)
{
    uint8_t tile;
    
    if(!zombie_level[zombie_x])
    {
        tile=ZOMBIE_TILE_0;
    }
    else
    {
        tile=BOSS_TILE_0;
    }
    _display_red_zombie(tile);
}


void handle_item_drop(void)
{
    if(zombie_level[zombie_x] || ((uint8_t) (_XL_RAND()) <64))
    {        
        ++item_counter;
        item_counter&=3;
        
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
        else if((freeze_taken<MAX_FREEZE)&&(powerUp>=5)&&(!freeze))
        {
            if(!freezeItem._active)
            {
                drop_item(&freezeItem,45);
            }
        }
        else if(!wall_appeared&&(powerUp>=6)) 
        {
            if(!wallItem._active)
            {
                drop_item(&wallItem,35);
            }
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


void respawn(void)
{
    if(minions_to_spawn || bosses_to_spawn)
    {
        if (minions_to_spawn)
        {
            spawn_minion();
        }
        else 
        {
            spawn_boss();
        }
        display_zombie();
    }

    update_zombie_speed();
}


void zombie_die(void)
{
    uint8_t y_pos = zombie_y[zombie_x];
    uint8_t i;
    

    _XL_DELETE(zombie_x,y_pos+1);
    
    _XL_DRAW(zombie_x,y_pos, ZOMBIE_DEATH_TILE, _XL_RED);


    for(i=0;i<29;++i)
    {
        _XL_DRAW(zombie_x,y_pos, ZOMBIE_DEATH_TILE, _XL_RED);
        display_red_zombie();
    } 
    _XL_SHOOT_SOUND();
    // delete_above_zombie();
    _XL_DELETE(zombie_x,y_pos);
    display_wall(BOTTOM_WALL_Y);
    display_bow();
        
    if(zombie_level[zombie_x])
    {
        ++killed_bosses;
        --bosses_to_kill;
    }
    else
    {
        ++killed_minions;
        --minions_to_kill;
    }
   
    if(zombie_x==forced_zombie_x)
    {
        forced_zombie = 0;
    }
    
    handle_item_drop();
    
    zombie_active[zombie_x]=0;
}


uint8_t compute_next_available_arrow_index(void)
{
    uint8_t i;
    
    for(i=0;i<MAX_ARROWS_ON_SCREEN;++i)
    {
        if(!active_arrow[i])
        {
            return i;
        }
    }
    return i;
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
                    _XL_DRAW(arrow_x[i],arrow_y[i],arrow_shape[i],arrow_display_color);
                }
            }
        }
    }
}


uint8_t zombie_hit(void)
{
    uint8_t i;
    
    for(i=0;i<MAX_ARROWS_ON_SCREEN;++i)
    {
        if(active_arrow[i] && arrow_x[i]==zombie_x
          && zombie_y[zombie_x]>=arrow_y[i] && zombie_y[zombie_x]<=arrow_y[i]+1)
           {
               if(freeze || (zombie_level[zombie_x]!=2) || zombie_shape[zombie_x])
               {
                   active_arrow[i]=0;
                    --arrows_on_screen;

                   _XL_DELETE(arrow_x[i],arrow_y[i]);
                   return 1;
               }
               else // Arrows goes through ghost !free (non-frozen) && zombie_level==2 (i.e., ghost zombie) && !zombie_shape (i.e. invincible shape)
               {
                   display_zombie(); // display invincible ghost zombie
                   return 0; // two arrows cannot be at the same place
               }
           }
    }
    return 0;
}


void decrease_energy(void)
{
    if(energy[zombie_x]<=fire_power)
    {
        energy[zombie_x]=0;
    }
    else
    {
        energy[zombie_x]-=fire_power;
    }
}


void redraw_wall(void)
{
    uint8_t i;
    
    for(i=3*(XSize)/8;i<1+3*(XSize)/8+(XSize)/4;++i)
    {
        if(wall[i])
        {
            _XL_DRAW(i,WALL_Y,WALL_TILE,_XL_YELLOW);
        }
    }        
}


void push_zombie(void)
{
    if(!zombie_shape[zombie_x])
    {
        --zombie_y[zombie_x];
        zombie_shape[zombie_x]=3;
    }
    else 
    {
        --zombie_shape[zombie_x];
        _XL_DELETE(zombie_x,zombie_y[zombie_x]+1);
        display_zombie();
    }
}


void handle_zombie_collisions(void)
{
    for(zombie_x=0;zombie_x<XSize;++zombie_x)
    {
        if(zombie_active[zombie_x] && zombie_hit())
        {
            decrease_energy();

            if(energy[zombie_x])
            {
                display_red_zombie();
                _XL_SLOW_DOWN(SLOW_DOWN);
                
                _XL_TOCK_SOUND();
                push_zombie();
                display_zombie();
            }
            else
            {
                increase_score(zombie_points[zombie_level[zombie_x]]);
                zombie_die();
                respawn();
            }
        }
    }
}


void handle_missile_drop(void)
{
    uint8_t missile_index;
    
    if(!freeze && ((missile_index = find_inactive(beamMissile)) < NUMBER_OF_MISSILES))
    {
        drop_item(&beamMissile[missile_index],1);
    }
}


#define _move_zombie() \
{ \
    ++zombie_shape[zombie_x]; \
    (zombie_shape[zombie_x])&=3; \
    if(!zombie_shape[zombie_x]) \
        { \
            ++zombie_y[zombie_x]; \
        } \
}

void handle_missile_drops(void)
{
    zombie_x = (uint8_t) (_XL_RAND())%XSize;
    if(zombie_active[zombie_x] && (zombie_level[zombie_x]>2) && zombie_y[zombie_x]<HEIGHT_SHOOT_THRESHOLD)
    {
        handle_missile_drop();
    }
}


void move_zombies(void)
{
    uint8_t j;
    
    if(forced_zombie)
    {
        zombie_x = forced_zombie_x;
    }
    else
    {
        zombie_x=find_zombie(1);
    }

    if((zombie_shape[zombie_x]==3)||
        (
            ((zombie_level[zombie_x]==2)&&(zombie_shape[zombie_x]&1))
            &&
            (zombie_y[zombie_x]!=BOW_Y-1)
        )
    )
    {
        forced_zombie = 0;
    }
    else
    {
        forced_zombie = 1;
        forced_zombie_x = zombie_x; 
    }
    if(wall[zombie_x] && zombie_y[zombie_x]==WALL_Y-1)
    {
        --wall[zombie_x];
        
        for(j=0;j<3;++j)
        {
            _XL_DRAW(zombie_x, WALL_Y, WALL_TILE, _XL_RED);
            _XL_TICK_SOUND();
            _XL_DRAW(zombie_x, WALL_Y, WALL_TILE, _XL_YELLOW);
        }
        if(!wall[zombie_x])
        {
            _XL_DRAW(zombie_x, WALL_Y, WALL_TILE, _XL_RED);
            _XL_EXPLOSION_SOUND();
            _XL_DELETE(zombie_x, WALL_Y);
        }
    }
    else
    {
        _move_zombie();
    }
    display_zombie();

    
    if((zombie_y[zombie_x]==BOW_Y))
    {
        alive = 0;
        display_red_zombie();
    }
}


void handle_bow_load(void)
{
    if(!loaded_bow && arrows_on_screen<MAX_ARROWS_ON_SCREEN && !bow_load_counter && remaining_arrows)
    {
        loaded_bow = 1;
        display_bow();
    }
    if(bow_load_counter)
    {
        --bow_load_counter;
    }
}


void fire(void)
{
    uint8_t i;
    uint8_t new_arrow_x;  
    uint8_t offset;
    _XL_TICK_SOUND();

    new_arrow_x = (bow_x/2)+(bow_x&1);
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
            if(new_arrow_x<XSize)
            {
                next_arrow = compute_next_available_arrow_index();

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


void handle_bow_move(void)
{
    input = _XL_INPUT();
    
    if(_XL_LEFT(input) && bow_x)
    {
        move_left();
    }
    else if (_XL_RIGHT(input) && bow_x<MAX_BOW_X)
    {
        move_right();
    }
    else if (_XL_FIRE(input) && loaded_bow)
    {
        fire();
    }
}


#define game_over() \
do \
{ \
    _XL_EXPLOSION_SOUND(); \
    _XL_SET_TEXT_COLOR(_XL_RED); \
    _XL_PRINT_CENTERED(_XL_G _XL_A _XL_M _XL_E _XL_SPACE _XL_O _XL_V _XL_E _XL_R); \
    _XL_SLEEP(1); \
    _XL_WAIT_FOR_INPUT(); \
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
    next_extra_life_counter = 1; \
    level = INITIAL_LEVEL; \
    killed_bosses = 0; \
    killed_minions = 0; \
    lives = INITIAL_LIVES; \
} while(0)


#if defined(TRAINER)
    #define level_initialization() \
        do \
        {   \
            fire_power = 2; \
            freeze = 0; \
            powerUp = 8; \
            next_arrow = 0; \
            arrows_on_screen = 0; \
            bow_load_counter = 0; \
            wall_appeared = 0; \
            freeze_taken = 0; \
            forced_zombie = 0; \
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
            wall_appeared = 0; \
            freeze_taken = 0; \
            hyper_counter = 0; \
            forced_zombie = 0; \
            loaded_bow = 1; \
            alive = 1; \
            bow_reload_loops = RED_SPEED_VALUE; \
            auto_recharge_counter = AUTO_RECHARGE_COOL_DOWN; \
            remaining_arrows = MAX_ARROWS; \
            arrow_range = RED_RANGE_VALUE; \
            bow_x = XSize; \
            bow_shape_tile = 2*((bow_x)&1); \
            bow_color = _XL_CYAN; \
            number_of_arrows_per_shot = 1; \
            initialize_items(); \
            _XL_CLEAR_SCREEN(); \
        } while(0)

#endif


void initialize_zombie(void)
{
    zombie_y[zombie_x]=INITIAL_ZOMBIE_Y;
    ++main_loop_counter;
    display_zombie();
    _XL_TOCK_SOUND();
}


void zombie_initialization(void)
{
    uint8_t to_spawn_initially;

    for(zombie_x=0;zombie_x<XSize;++zombie_x)
    {
        zombie_shape[zombie_x]=0;
        zombie_level[zombie_x]=0;
        wall[zombie_x]=0;
        zombie_active[zombie_x]=0;
    }
    
    #if !defined(DEBUG)
        minions_to_kill = MINIONS_ON_FIRST_LEVEL+level-killed_minions; 
    #else
        minions_to_kill = 2;
    #endif

    if(minions_to_kill<MAX_OCCUPIED_COLUMNS)
    {
        to_spawn_initially=minions_to_kill;
    }
    else
    {
        to_spawn_initially=MAX_OCCUPIED_COLUMNS;
    }
    
    main_loop_counter = 0;

    while(main_loop_counter<to_spawn_initially)
    {
        spawn_minion();
        initialize_zombie();
    }
    
    minions_to_spawn = minions_to_kill-to_spawn_initially;
    
    #if !defined(DEBUG)
        bosses_to_kill = BOSSES_ON_FIRST_LEVEL+(level<<4)-killed_bosses;
    #else
        bosses_to_kill=7;
    #endif
    
    if(bosses_to_kill<MAX_OCCUPIED_COLUMNS - to_spawn_initially)
    {
        to_spawn_initially = bosses_to_kill;  
    }
    else
    {
        to_spawn_initially = MAX_OCCUPIED_COLUMNS - to_spawn_initially;
    }
   
    main_loop_counter = 0;

    while(main_loop_counter<to_spawn_initially)
    {
        spawn_boss();
        initialize_zombie();
    }
   
    bosses_to_spawn = bosses_to_kill-to_spawn_initially;
    
    update_zombie_speed();
    
    for(zombie_x=0;zombie_x<MAX_ARROWS_ON_SCREEN;++zombie_x)
    {
        active_arrow[zombie_x] = 0;
    }
}


#if YSize<=22
    #define _NEXT_ROW i
#else
    #define _NEXT_ROW ((i)<<1)
#endif

#define display_items() \
do \
{ \
    uint8_t i; \
    \
    for(i=0;i<5;++i) \
    { \
        _XL_DRAW(XSize/2-5,YSize/3+5+_NEXT_ROW, item_tile[i][0], item_tile[i][1]); \
        _XL_SET_TEXT_COLOR(_XL_GREEN); \
        _XL_PRINT(XSize/2-5+3,YSize/3+5+_NEXT_ROW, (char *)item_name[i]); \
    } \
} while(0)

#if YSize<=22
    #define _HISCORE_Y 1
#else
    #define _HISCORE_Y 2
#endif

#if XSize>=20
    #define _CROSS_HORDE_STRING \
        _XL_C _XL_SPACE _XL_R _XL_SPACE _XL_O _XL_SPACE _XL_S _XL_SPACE _XL_S \
        _XL_SPACE _XL_SPACE \
        _XL_H _XL_SPACE _XL_O _XL_SPACE _XL_R _XL_SPACE _XL_D _XL_SPACE _XL_E

#else
    #define _CROSS_HORDE_STRING \
        _XL_C _XL_R _XL_O _XL_S _XL_S \
        _XL_SPACE \
        _XL_H _XL_O _XL_R _XL_D _XL_E  
#endif


void display_initial_screen(void)
{
    _XL_CLEAR_SCREEN();
    
    display_wall(0);
    display_wall(BOTTOM_WALL_Y+1);

    _XL_SET_TEXT_COLOR(_XL_CYAN);               
    _XL_PRINT_CENTERED_ON_ROW(_HISCORE_Y, _XL_H _XL_I _XL_S _XL_C _XL_O _XL_R _XL_E); 

    _XL_SET_TEXT_COLOR(_XL_WHITE);                
    _XL_PRINTD(XSize/2-3,_HISCORE_Y+1,5,hiscore);
    
    
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT_CENTERED_ON_ROW(YSize/3-3,_CROSS_HORDE_STRING);

    _XL_SET_TEXT_COLOR(_XL_WHITE);               
    _XL_PRINT_CENTERED_ON_ROW(YSize/3-1, _XL_F _XL_A _XL_B _XL_R _XL_I _XL_Z _XL_I _XL_O _XL_SPACE _XL_C _XL_A _XL_R _XL_U _XL_S _XL_O);

    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT_CENTERED_ON_ROW(YSize/3+2, _XL_R _XL_E _XL_S _XL_I _XL_S _XL_T _XL_SPACE _XL_T _XL_H _XL_E _XL_SPACE _XL_H _XL_O _XL_R _XL_D _XL_E );

    _XL_SLEEP(1);
 
    display_items();
    _XL_WAIT_FOR_INPUT();
    _XL_CLEAR_SCREEN();
}


#if XSize>=23
    #define HI_X ((XSize-9))
    #define HISCORE_STRING _XL_H _XL_I
#else
    #define HI_X ((XSize-8))
    #define HISCORE_STRING _XL_H
#endif


#define display_stats() \
do \
{ \
    _XL_SET_TEXT_COLOR(_XL_WHITE); \
    display_score(); \
    _XL_SET_TEXT_COLOR(_XL_GREEN); \
    _XL_PRINT(HI_X,0,_XL_H _XL_I); \
    _XL_SET_TEXT_COLOR(_XL_WHITE); \
    _XL_PRINTD(XSize-7,0,5, hiscore); \
    _XL_DRAW(6,0,ARROW_TILE_1,_XL_CYAN); \
    _XL_DRAW(POWER_UP_X,0,POWER_UP_TILE, _XL_WHITE); \
    display_remaining_arrows(); \
    display_power_up_counter(); \
    display_level(); \
    display_lives(); \
    display_power_ups(); \
} while(0)


void display_top_border(void)
{ 
    uint8_t i; 
    
    for(i=0;i<XSize;++i) 
    { 
        _XL_DRAW(i,1,TOP_BORDER_TILE,_XL_CYAN);
    } 
}


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


#define handle_extra_life() \
    if(score>=NEXT_EXTRA_LIFE*next_extra_life_counter) \
    { \
        if(lives<MAX_LIVES) \
        { \
            ++lives; \
        } \
        _XL_PING_SOUND(); \
        ++next_extra_life_counter; \
        display_lives(); \
        _XL_SLOW_DOWN(SLOW_DOWN); \
        _XL_PING_SOUND(); \
    } \


#define display_level_at_start_up()  \
do \
{ \
    _XL_SET_TEXT_COLOR(_XL_CYAN); \
    _XL_PRINT(XSize/2-4, YSize/2,_XL_L _XL_E _XL_V _XL_E _XL_L); \
    _XL_PRINTD(XSize/2+2,YSize/2,2,level+1); \
    _XL_SLEEP(1); \
    _XL_WAIT_FOR_INPUT(); \
    _XL_PRINT(XSize/2-4, YSize/2,_XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE _XL_SPACE); \
} while(0)


#define handle_zombie_movement() \
    if(!freeze) \
    { \
        if(!(main_loop_counter&zombie_speed)) \
        { \
            move_zombies(); \
        } \
    } \
    else \
    { \
        --freeze; \
    }


#define handle_next_level() \
do \
{ \
    ++level; \
    _XL_SET_TEXT_COLOR(_XL_CYAN); \
    _XL_PRINT_CENTERED(_XL_C _XL_L _XL_E _XL_A _XL_R _XL_E _XL_D); \
    _XL_SLEEP(1); \
    _XL_WAIT_FOR_INPUT(); \
    killed_bosses = 0; \
    killed_minions = 0; \
} while(0)


#define player_dies() \
do \
{ \
    --lives; \
    bow_color=_XL_RED; \
    display_bow(); \
    bow_color=_XL_CYAN; \
    _XL_EXPLOSION_SOUND(); \
    _XL_SLEEP(1); \
    _XL_WAIT_FOR_INPUT(); \
} while(0)


void zombie_animation(void)
{
        zombie_y[zombie_x]=YSize/2-7+(uint8_t) ((_XL_RAND())&15);
        zombie_level[zombie_x]=0;
        display_zombie();
        _XL_SLOW_DOWN(SLOW_DOWN);
        zombie_die();
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINT_CENTERED(_XL_C _XL_L _XL_E _XL_A _XL_R _XL_E _XL_D);
}


#define victory() \
do \
{ \
    uint8_t i; \
    for(i=0;i<XSize;++i) \
    { \
        zombie_x = i; \
        zombie_animation(); \
        zombie_x = XSize-i; \
        zombie_animation(); \
    } \
    _XL_SET_TEXT_COLOR(_XL_WHITE); \
    _XL_PRINT_CENTERED(_XL_V _XL_I _XL_C _XL_T _XL_O _XL_R _XL_Y); \
    _XL_SLEEP(1); \
    _XL_WAIT_FOR_INPUT(); \
    _XL_CLEAR_SCREEN(); \
} while(0)


#define display_level_screen() \
do \
{ \
    display_top_border(); \
    display_wall(BOTTOM_WALL_Y); \
    display_bow(); \
    display_stats(); \
    display_level_at_start_up(); \
} while(0)


void handle_hyper(void)
{
    if(hyper_counter)
    {   
        --hyper_counter;
        
        if(hyper_counter==1)
        {
            bow_color = _XL_CYAN;
            bow_reload_loops=GREEN_SPEED_VALUE;
            display_top_border();
        }
    }
}


int main(void)
{           
    _XL_INIT_GRAPHICS();
    
    _XL_INIT_INPUT()
    
    _XL_INIT_SOUND();

    hiscore = 0;

    while(1) // Game (re-)start
    {
        global_initialization();
        display_initial_screen();
        
        while(lives && level<=LAST_LEVEL) // Level (re)-start 
        {            
            level_initialization();
            zombie_initialization();
            display_level_screen();
            
            while(alive && (minions_to_kill || bosses_to_kill) ) // Inner game loop
            {
                handle_hyper();
                handle_bow_move();
                handle_bow_load();
                handle_arrows(); 
                redraw_wall();  
                handle_auto_recharge();
                handle_extra_life();
                handle_zombie_movement();
                handle_zombie_collisions();
                handle_missile_drops();
                handle_items();
                _XL_SLOW_DOWN(SLOW_DOWN);     
                ++main_loop_counter;
                
                // _XL_PRINTD(4,4,2,zombie_speed);
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

