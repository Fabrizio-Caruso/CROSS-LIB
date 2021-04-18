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

#define INITIAL_LEVEL 0
#define LAST_LEVEL 11
#define INITIAL_LIVES 3
#define MAX_LIVES 9

#define NEXT_EXTRA_LIFE 5000U

#define BOW_Y ((YSize)-3)
#define MAX_BOW_X ((XSize)*2-3)
#define POWER_UPS_Y ((BOW_Y)+2)
#define WALL_Y ((YSize)-8)

#define INITIAL_ZOMBIE_Y 3
#define BOTTOM_WALL_Y ((BOW_Y)+1)

#define POWER_THRESHOLD 4

#define MAX_ARROWS_ON_SCREEN 12
#define INITIAL_BOW_RELOAD_LOOPS 14

#define AUTO_RECHARGE_COOL_DOWN 80
#define AUTO_ARROW_RECAHRGE 9

#define MAX_ZOMBIE_SPEED 20000U
#define INITIAL_ZOMBIE_SPEED 12000U
#define FEW_ZOMBIE_SPEED ((INITIAL_ZOMBIE_SPEED)/3)
#define FEW_ZOMBIES ((XSize)/3)
#define INITIAL_ZOMBIE_SPAWN_LOOPS 2
#define MAX_ZOMBIE_LOOPS 3
#define ZOMBIE_SPEED_INCREASE 50U

#define MINION_POINTS 5
#define BOSS_1_POINTS 20
#define BOSS_2_POINTS 30
#define BOSS_3_POINTS 40

#define EXTRA_POINTS 10
#define RECHARGE_POINTS 15
#define POWERUP_POINTS 25
#define FREEZE_POINTS 35
#define WALL_POINTS 45

#define INITIAL_ARROW_RANGE ((INITIAL_ZOMBIE_Y)+7)
#define ARROW_RECHARGE 25
#define ITEM_SPAWN_CHANCE 11000U

#define MINION_ENERGY 3
#define BOSS_BASE_ENERGY 5
#define WALL_ENERGY 20

#define MAX_ARROWS 99

#define FREEZE_COUNTER_MAX 180;

#define MAX_OCCUPIED_COLUMNS (4*(XSize)/5)

#define ZOMBIE_MOVE_LOOPS 2

#define NUMBER_OF_MISSILES 5

#define MINIONS_ON_FIRST_LEVEL ((XSize)+1)

#if XSize<=80
    #define BOSSES_ON_FIRST_LEVEL ((XSize)/2)
#else
    #define BOSSES_ON_FIRST_LEVEL 40
#endif

static uint8_t main_loop_counter;

static uint8_t next_extra_life_counter;

static const uint8_t zombie_points[] = 
{ 
    MINION_POINTS, // Skeletons
    BOSS_1_POINTS, // Ogre
    BOSS_2_POINTS, // Ghosts
    BOSS_3_POINTS, // Demons
};

static uint16_t minions_to_kill;
static uint16_t bosses_to_kill;

static uint8_t lives;
static uint8_t level;
static uint8_t killed_minions;
static uint8_t killed_bosses;

static uint8_t bosses_to_spawn;
static uint8_t minions_to_spawn;

static uint8_t auto_recharge_counter;

#if defined(COLOR)
static uint8_t power_up_color[3] = {_XL_RED, _XL_YELLOW, _XL_GREEN};
static uint8_t arrow_color[3] = {_XL_CYAN, _XL_YELLOW, _XL_WHITE};
#endif

static uint8_t freeze;
static uint8_t powerUp;

static uint8_t number_of_arrows_per_shot;

static uint8_t zombie_y[XSize];
static uint8_t zombie_shape[XSize];
static uint8_t zombie_x;
static uint16_t zombie_speed;
static uint8_t zombie_active[XSize];

static uint8_t energy[XSize];
static uint8_t zombie_level[XSize];

static uint8_t fire_power;

static uint8_t wall_appeared;
static uint8_t freeze_appeared;

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
static uint8_t arrow_display_color;

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
static Item extraPointsItem;
static Item wallItem;

static uint8_t item_tile[5][2] = 
{
    { POWER_UP_TILE, _XL_WHITE },
    { ARROW_TILE_1, _XL_YELLOW },
    { EXTRA_POINTS_TILE, _XL_YELLOW },
    { FREEZE_TILE, _XL_CYAN },
    { WALL_TILE, _XL_YELLOW },
};

static char item_name[5][9] = 
{
    _XL_P _XL_O _XL_W _XL_E _XL_R _XL_SPACE _XL_U _XL_P,
    _XL_A _XL_R _XL_R _XL_O _XL_W _XL_S,
    _XL_P _XL_O _XL_I _XL_N _XL_T _XL_S,
    _XL_F _XL_R _XL_E _XL_E _XL_Z _XL_E,
    _XL_W _XL_A _XL_L _XL_L,
};

static Missile beamMissile[NUMBER_OF_MISSILES];

static uint8_t unlock_freeze;
static uint8_t unlock_wall;

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


void recharge_arrows(void)
{
    remaining_arrows+=ARROW_RECHARGE;
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
    recharge_arrows();
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

void power_up_effect(void)
{
    ++powerUp;
    increase_score(POWERUP_POINTS);
    
    switch(powerUp)
    {
        case 1:
            arrow_range=INITIAL_ZOMBIE_Y+4;
        break;
            
        case 2:
            arrow_range=INITIAL_ZOMBIE_Y+2;
        break;
        
        case 3:
            bow_reload_loops=INITIAL_BOW_RELOAD_LOOPS/2;
        break;
           
        case 4:
            bow_reload_loops=2;
        break;
        
        case 5:
            number_of_arrows_per_shot = 2;
        break;
        
        case 6:
            number_of_arrows_per_shot = 3;
        break;
        
        case 7:
            fire_power = 2;
        break;
        
        case 8:
            fire_power = 3;
        break;
        
        case 9:
            unlock_freeze = 1;
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINT_CENTERED_ON_ROW(1,_XL_F _XL_R _XL_E _XL_E _XL_Z _XL_E);
        break;
        
        case 10:
            unlock_wall = 1;
            _XL_SET_TEXT_COLOR(_XL_YELLOW);
            _XL_PRINT_CENTERED_ON_ROW(1,_XL_SPACE _XL_W _XL_A _XL_L _XL_L _XL_SPACE);
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


void display_minion(void)
{
    uint8_t status = zombie_shape[zombie_x];
    uint8_t pos = zombie_y[zombie_x];
    uint8_t color;
    
    if(freeze)
    {
        color = _XL_CYAN;
    }
    else
    {
        color = _XL_WHITE;
    }
    
    if(!status)
    {
        _XL_DELETE(zombie_x, pos-1);
        _XL_DRAW(zombie_x, pos, ZOMBIE_TILE_0, color);
    }
    else
    {
        _XL_DRAW(zombie_x, pos, zombie_tile[status<<1], color);
        _XL_DRAW(zombie_x,1 + pos, zombie_tile[1+(status<<1)], color);
    }
}


void display_boss(void)
{
    uint8_t status = zombie_shape[zombie_x];
    uint8_t pos = zombie_y[zombie_x];
    uint8_t color;
    uint8_t tile;

    tile = BOSS_TILE_0;
    if(freeze)
    {
        color = _XL_CYAN;
    }
    else
    {
        if(zombie_level[zombie_x]==1)
        {
            color = _XL_GREEN;
        }
        else if(zombie_level[zombie_x]==2)
        {
            tile = ZOMBIE_DEATH_TILE;
            color = _XL_YELLOW;
        }
        else
        {
            color = _XL_RED;
        }
    }

    if(!status)
    {
        _XL_DELETE(zombie_x, pos-1);
        _XL_DRAW(zombie_x, pos, tile, color);
    }
    else
    {
        _XL_DRAW(zombie_x, pos, boss_tile[status<<1], color);
        _XL_DRAW(zombie_x,1 + pos, boss_tile[1+(status<<1)], color);
    }
}


void display_zombie(void)
{
    if(zombie_level[zombie_x])
    {
        display_boss();
    }
    else
    {
        display_minion();
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
    ++freeze_appeared;
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

    extraPointsItem._active = 0;
    extraPointsItem._tile = EXTRA_POINTS_TILE;
    extraPointsItem._color = _XL_YELLOW;
    extraPointsItem._effect = extra_points_effect;

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
    }
}


void display_level(void)
{
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINTD(XSize-2,0,2,level+1);
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


void drop_item(Item *item, uint8_t max_counter)
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
    handle_item(&extraPointsItem);
    handle_item(&wallItem);
    
    for(i=0;i<NUMBER_OF_MISSILES;++i)
    {
        handle_item(&beamMissile[i]);
    }
}

#if XSize!=13 && XSize!=26 && XSize!=39 && XSize!=52 && XSize!=65 && XSize!=78
    #define STEP 13
#else
    #define STEP 1
#endif 

uint8_t find_zombie(uint8_t value)
{
    uint8_t i;
    uint8_t index;
    
    index = (_XL_RAND())%XSize;
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
    zombie_x = find_zombie(0);
    zombie_active[zombie_x]=1;    
    zombie_shape[zombie_x]=0;
    zombie_y[zombie_x]=INITIAL_ZOMBIE_Y;
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
    
    if(!level)
    {
        rank = 1;
    }
    else if(level==1)
    {
        rank = 1 + ((_XL_RAND())&1);
    }
    else if(level<8)
    {
        rank = 1 + (rand()%3);   
    }
    else
    {
        rank = 2 + ((_XL_RAND())&1); 
    }
    
    activate_zombie();
    zombie_level[zombie_x]=rank;
    energy[zombie_x]=BOSS_BASE_ENERGY;
    --bosses_to_spawn;
}

#if XSize>=32
    #define NORMAL_ZOMBIE_SPEED 1
#else
    #define NORMAL_ZOMBIE_SPEED 3
#endif 

void update_zombie_speed(void)
{
    if(bosses_to_kill<=FEW_ZOMBIES)
    {
        zombie_speed=7;
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


void display_red_zombie(void)
{
    if(!zombie_level[zombie_x])
    {
        _XL_DRAW(zombie_x,zombie_y[zombie_x],ZOMBIE_TILE_0,_XL_RED);
    }
    else
    {
        _XL_DRAW(zombie_x,zombie_y[zombie_x],BOSS_TILE_0,_XL_RED);
    }
}


void zombie_die(void)
{
    uint8_t y_pos = zombie_y[zombie_x];
    
    uint16_t rnd;
    
    // _XL_DELETE(zombie_x,y_pos-1);    
    // _XL_DELETE(zombie_x,y_pos);
    _XL_DELETE(zombie_x,y_pos+1);
    
    _XL_DRAW(zombie_x,y_pos, ZOMBIE_DEATH_TILE, _XL_RED);

    _XL_TICK_SOUND();
    for(rnd=0;rnd<29;++rnd)
    {
        display_red_zombie();
        _XL_DRAW(zombie_x,y_pos, ZOMBIE_DEATH_TILE, _XL_RED);
    } 
    _XL_SHOOT_SOUND();
    _XL_DELETE(zombie_x,y_pos);
    display_wall(BOTTOM_WALL_Y);
    
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
    
    if(((_XL_RAND())<ITEM_SPAWN_CHANCE)||zombie_level[zombie_x])
    {
        rnd = (_XL_RAND())&15;
        if(rnd<2)
        {
            if(!extraPointsItem._active)
            {
                drop_item(&extraPointsItem,30);
            }
        }
        else if(rnd<7)
        {
            if(!rechargeItem._active)
            {
                drop_item(&rechargeItem,25);
            }
        }
        else if((rnd<8)&&(freeze_appeared<3)&&(unlock_freeze)&&(!freeze))
        {
            if(!freezeItem._active)
            {
                drop_item(&freezeItem,20);
            }
        }
        else if((rnd<9)&&!wall_appeared&&(unlock_wall))
        {
            if(!wallItem._active)
            {
                drop_item(&wallItem,20);
            }
        }
        else 
        {
            if(!powerUpItem._active)
            {
                drop_item(&powerUpItem,25);
            }  
        }  
    }
    
    zombie_active[zombie_x]=0;
  
    if(minions_to_spawn)
    {
        spawn_minion();
    }
    else if (bosses_to_spawn)
    {
        spawn_boss();
    }

    update_zombie_speed();
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
          && zombie_y[zombie_x]>=arrow_y[i] && zombie_y[zombie_x]<arrow_y[i]+2)
           {
               if(freeze || (zombie_level[zombie_x]!=2) || zombie_shape[zombie_x])
               {
                   active_arrow[i]=0;
                    --arrows_on_screen;

                   _XL_DELETE(arrow_x[i],arrow_y[i]);
                   return 1;
               }
               else
               {
                   display_boss();
                   // return 0;
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
            }
        }
    }
}


uint8_t find_inactive_missile(void)
{
    uint8_t i;
    
    for(i=0;i<NUMBER_OF_MISSILES;++i)
    {
        if(!beamMissile[i]._active)
        {
            return i;
        }
    }
    return i;
}


void handle_missile_drop(void)
{
    uint8_t missile_index;
    
    if((missile_index = find_inactive_missile())!= NUMBER_OF_MISSILES)
    {
        drop_item(&beamMissile[missile_index],1);
    }
}


void move_zombies(void)
{
    uint8_t j;
    
    zombie_x=find_zombie(1);

    if((zombie_level[zombie_x]>2) && zombie_y[zombie_x]<YSize-10)
    {
        handle_missile_drop();
    }
    if(zombie_y[zombie_x]<BOW_Y)
    {
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
            ++zombie_shape[zombie_x];

            (zombie_shape[zombie_x])&=3;

            if(!zombie_shape[zombie_x])
                {
                    ++zombie_y[zombie_x];
                }
        }
        display_zombie();
    }
    else
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
                --remaining_arrows;
                display_remaining_arrows();
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


#define level_initialization() \
do \
{   \
    fire_power = 1; \
    freeze = 0; \
    powerUp = 0; \
    next_arrow = 0; \
    arrows_on_screen = 0; \
    bow_load_counter = 0; \
    unlock_freeze = 0; \
    unlock_wall = 0; \
    wall_appeared = 0; \
    freeze_appeared = 0; \
    loaded_bow = 1; \
    alive = 1; \
    bow_reload_loops = INITIAL_BOW_RELOAD_LOOPS; \
    auto_recharge_counter = AUTO_RECHARGE_COOL_DOWN; \
    remaining_arrows = MAX_ARROWS; \
    arrow_range = INITIAL_ARROW_RANGE; \
    bow_x = XSize; \
    bow_shape_tile = 2*((bow_x)&1); \
    bow_color = _XL_CYAN; \
    number_of_arrows_per_shot = 1; \
    initialize_items(); \
    _XL_CLEAR_SCREEN(); \
} while(0)


void initialize_zombie(void)
{
    zombie_y[zombie_x]=INITIAL_ARROW_RANGE-2;
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
    #define _NEXT_ROW (i<<1)
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
        _XL_PRINT(XSize/2-5+3,YSize/3+5+_NEXT_ROW, item_name[i]); \
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


#define display_stats() \
do \
{ \
    _XL_SET_TEXT_COLOR(_XL_WHITE); \
    display_score(); \
    _XL_SET_TEXT_COLOR(_XL_GREEN); \
    _XL_PRINT(XSize-10,0,_XL_H _XL_I); \
    _XL_SET_TEXT_COLOR(_XL_WHITE); \
    _XL_PRINTD(XSize-8,0,5, hiscore); \
    _XL_DRAW(6,0,ARROW_TILE_1,_XL_CYAN); \
    display_remaining_arrows(); \
    display_level(); \
    display_lives(); \
    display_power_ups(); \
} while(0)


#define display_top_border() \
do \
{ \
    uint8_t i; \
    \
    for(i=0;i<XSize;++i) \
    { \
        _XL_DRAW(i,1,TOP_BORDER_TILE,_XL_CYAN); \
    } \
} while(0)


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
            remaining_arrows+=AUTO_ARROW_RECAHRGE; \
            display_remaining_arrows(); \
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
        _XL_SLEEP(1); \
        display_lives(); \
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
        if((main_loop_counter&zombie_speed)==1) \
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
    _XL_PRINT_CENTERED(_XL_L _XL_E _XL_V _XL_E _XL_L _XL_SPACE _XL_C _XL_L _XL_E _XL_A _XL_R _XL_E _XL_D); \
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


#define victory() \
do \
{ \
    _XL_SET_TEXT_COLOR(_XL_RED); \
    _XL_CLEAR_SCREEN(); \
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


int main(void)
{           
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();  
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
            
            while(alive && (minions_to_kill || bosses_to_kill) )
            {
                handle_bow_move();
                handle_bow_load();
                handle_arrows(); 
                redraw_wall();  
                handle_auto_recharge();
                handle_extra_life();
                handle_zombie_movement();
                handle_zombie_collisions();
                handle_items();
                _XL_SLOW_DOWN(SLOW_DOWN);     
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

