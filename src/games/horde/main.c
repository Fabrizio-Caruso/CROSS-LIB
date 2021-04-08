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


#define PLAYER_Y ((YSize)-3)
#define MAX_PLAYER_X ((XSize)*2-3)
#define POWER_UPS_Y ((YSize)-1)

#define ZOMBIE_INITIAL_Y 6
#define BOTTOM_WALL_Y ((PLAYER_Y)+1)

#define POWER_THRESHOLD 4

#define NUMBER_OF_ARROWS_ON_SCREEN 6
#define INITIAL_BOW_RELOAD_LOOPS 14

#define MAX_ZOMBIE_SPEED 40000U
#define INITIAL_ZOMBIE_SPEED 20000U
#define INITIAL_ZOMBIE_SPAWN_LOOPS 2
#define MAX_ZOMBIE_SPAWN_LOOPS 10
#define ZOMBIE_SPEED_INCREASE 500U

#define MINION_POINTS 5
#define BOSS_POINTS 50
#define RECHARGE_POINTS 20
#define FREEZE_POINTS 25
#define POWERUP_POINTS 40
#define EXTRA_POINTS 15
#define WALL_POINTS 30

#define INITIAL_ARROW_RANGE ((ZOMBIE_INITIAL_Y)+6)
#define ARROW_RECAHRGE 20
#define ITEM_SPAWN_CHANCE 15000U


#define BOSS_ENERGY 6

#define MAX_ARROWS 99

#define FREEZE_COUNTER_MAX 200;

static uint8_t freeze;
static uint8_t powerUp;

static uint8_t number_of_arrows;

static uint8_t zombie_y[XSize];
static uint8_t zombie_shape[XSize];
static uint8_t zombie_x;
static uint16_t zombie_speed;
static uint8_t zombie_spawn_loops;

static uint8_t boss[XSize];

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

static uint8_t input;

static uint8_t loaded_bow;
static uint8_t active_arrow[NUMBER_OF_ARROWS_ON_SCREEN];
static uint8_t arrow_shape[NUMBER_OF_ARROWS_ON_SCREEN];
static uint8_t arrow_x[NUMBER_OF_ARROWS_ON_SCREEN];
static uint8_t arrow_y[NUMBER_OF_ARROWS_ON_SCREEN];
static uint8_t arrows;
static uint8_t arrow_range;
static uint8_t bow_reload_loops;

static uint8_t next_arrow;
static uint8_t arrows_on_screen_counter;
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

static Item rechargeItem;
static Item freezeItem;
static Item powerUpItem;
static Item extraPointsItem;
static Item wallItem;

void display_remaining_arrows(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(7,0,2,arrows);
}

void recharge_arrows(void)
{
    arrows+=ARROW_RECAHRGE;
    if(arrows>MAX_ARROWS)
    {
        arrows=MAX_ARROWS;
    }
    display_remaining_arrows();
}

void recharge_effect(void)
{
    recharge_arrows();
    score+=RECHARGE_POINTS;
}

void freeze_effect(void)
{
    freeze=FREEZE_COUNTER_MAX;
    score+=FREEZE_POINTS;
}

void display_power_ups(void)
{
    uint8_t range_color;
    uint8_t speed_color;
    uint8_t power_color;
    
    uint8_t i;
    
    if(powerUp)
    {
        range_color = _XL_GREEN;
        if(powerUp==1)
        {
            speed_color = _XL_RED;
        }
        else if(powerUp==2)
        {
            speed_color = _XL_YELLOW;
        }
        else // >=3
        {
            speed_color = _XL_GREEN;
        }
        if(powerUp>=POWER_THRESHOLD)
        {
            power_color = _XL_GREEN;
        }
        else
        {
            power_color = _XL_RED;
        }
    }
    else
    {
        range_color = _XL_RED;
        speed_color = _XL_RED;
        power_color = _XL_RED;
    }
    if(powerUp==POWER_THRESHOLD+1)
    {
        number_of_arrows=2;
    }
    else if(powerUp>=POWER_THRESHOLD+2)
    {
        number_of_arrows=3;
    }
    else
    {
        number_of_arrows=1;
    }
    
    #if defined(COLOR)
    _XL_SET_TEXT_COLOR(range_color);
    _XL_PRINT(0,POWER_UPS_Y,_XL_R _XL_A _XL_N _XL_G _XL_E);
    
    _XL_SET_TEXT_COLOR(speed_color);
    _XL_PRINT(6,POWER_UPS_Y,_XL_S _XL_P _XL_E _XL_E _XL_D);
    
    _XL_SET_TEXT_COLOR(power_color);
    _XL_PRINT(XSize-5,POWER_UPS_Y, _XL_P _XL_O _XL_W _XL_E _XL_R);
    #else
        if(range_color!=_XL_RED)
        {
            _XL_PRINT(0,POWER_UPS_Y,_XL_R _XL_A _XL_N _XL_G _XL_E);
        }
        if(speed_color!=_XL_RED)
        {
            _XL_PRINT(6,POWER_UPS_Y,_XL_S _XL_P _XL_E _XL_E _XL_D);
        }
        if(power_color!=_XL_RED)
        {
            _XL_PRINT(XSize-5,POWER_UPS_Y, _XL_P _XL_O _XL_W _XL_E _XL_R);
        }
    #endif
    
    #if XSize>=20

        for(i=0;i<number_of_arrows;++i)
        {
            _XL_DRAW(XSize-5-4+i,POWER_UPS_Y,ARROW_TILE_0,_XL_YELLOW);
        }

    #endif

}

void power_up_effect(void)
{
    
    ++powerUp;
    score+=POWERUP_POINTS;
    
    // _XL_PRINTD(0,YSize-1,2,powerUp);
    
    switch(powerUp)
    {
        case 1:
            arrow_range-=5;
        break;
        
        case 2:
            bow_reload_loops=INITIAL_BOW_RELOAD_LOOPS/2;
        break;
        
        case 3:
            bow_reload_loops=2;
        break;
        
        case 4:
        
        break;
        
        case 5:
            number_of_arrows = 2;
        break;
        
        case 6:
            number_of_arrows = 3;
        break;
        
        default:
        break;
    }
    display_power_ups();
}

void extra_points_effect(void)
{
    score+=EXTRA_POINTS;
}

void wall_effect(void)
{
    // TODO: To be implemented
}

void initialize_items(void)
{
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
}


void display_level(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(XSize-1,0,1,zombie_spawn_loops-INITIAL_ZOMBIE_SPAWN_LOOPS+1);
}


void display_bow(void)
{
    _XL_DRAW(bow_x/2,PLAYER_Y,bow_tile[4*loaded_bow+0+bow_shape_tile],_XL_CYAN);
    _XL_DRAW(bow_x/2+1,PLAYER_Y,bow_tile[1+4*loaded_bow+bow_shape_tile],_XL_CYAN);  
}

void move_left(void)
{
    bow_shape_tile = 2*((--bow_x)&1);
    if(bow_shape_tile)
    {
        _XL_DELETE(bow_x/2+2,PLAYER_Y);
    }
 
    display_bow();
}

void move_right(void)
{
    bow_shape_tile = 2*((++bow_x)&1);
    if(!bow_shape_tile)
    {
        _XL_DELETE(bow_x/2-1,PLAYER_Y);
    }

    display_bow();
}


void display_minion(void)
{
    uint8_t status = zombie_shape[zombie_x];
    uint8_t pos = zombie_y[zombie_x];
    
    if(!status)
    {
        _XL_DELETE(zombie_x, pos-1);
        _XL_DRAW(zombie_x, pos, ZOMBIE_TILE_0, _XL_WHITE);
    }
    else
    {
        _XL_DRAW(zombie_x, pos, zombie_tile[status<<1], _XL_WHITE);
        _XL_DRAW(zombie_x,1 + pos, zombie_tile[1+(status<<1)], _XL_WHITE);
    }
}

void display_boss(void)
{
    uint8_t status = zombie_shape[zombie_x];
    uint8_t pos = zombie_y[zombie_x];
    
    if(!status)
    {
        _XL_DELETE(zombie_x, pos-1);
        _XL_DRAW(zombie_x, pos, BOSS_TILE_0, _XL_GREEN);
    }
    else
    {
        _XL_DRAW(zombie_x, pos, boss_tile[status<<1], _XL_GREEN);
        _XL_DRAW(zombie_x,1 + pos, boss_tile[1+(status<<1)], _XL_GREEN);
    }
}

void display_zombie(void)
{
    if(!boss[zombie_x])
    {
        display_minion();
    }
    else
    {
        display_boss();
    }
}

void display_score(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(0,0,5,score);
}

void spawn_item(Item *item)
{
    _XL_TICK_SOUND();
    item->_active = 1;
    item->_x = zombie_x;
    item->_y = zombie_y[zombie_x]+1;
    item->_counter=20;
}


void handle_item(Item* item)
{
    if(item->_active)
    {
        if(item->_y<PLAYER_Y)
        {
            _XL_DELETE(item->_x,item->_y);
            if(_XL_RAND()&1)
            {
                ++(item->_y);
            }
            _XL_DRAW(item->_x,item->_y,item->_tile,item->_color);
        }
        
        if((item->_y==PLAYER_Y) && (item->_counter>0))
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
                _XL_ZAP_SOUND();
                item->_active=0;
                display_score();
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
    handle_item(&rechargeItem);
    handle_item(&freezeItem);
    handle_item(&powerUpItem);
    handle_item(&extraPointsItem);
    handle_item(&wallItem);
}


void zombie_spawn(void)
{
    
    if(!((_XL_RAND())&15))
    {
        boss[zombie_x]=BOSS_ENERGY;
    }
    else
    {
        boss[zombie_x]=0;
    }
    zombie_shape[zombie_x]=0;
    zombie_y[zombie_x]=ZOMBIE_INITIAL_Y;
}

void update_zombie_speed(void)
{
    if(zombie_speed<MAX_ZOMBIE_SPEED-ZOMBIE_SPEED_INCREASE)
    {
        zombie_speed+=ZOMBIE_SPEED_INCREASE;
    }
    else
    {
        if(zombie_spawn_loops<MAX_ZOMBIE_SPAWN_LOOPS)
        {
            ++zombie_spawn_loops;
            zombie_speed=INITIAL_ZOMBIE_SPEED;
        }
        else
        {
            zombie_speed=INITIAL_ZOMBIE_SPEED*2;   
        }
        _XL_TOCK_SOUND();
        
        display_level();
        _XL_PING_SOUND();
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
    if(!boss[zombie_x])
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
    uint8_t pos = zombie_y[zombie_x];
    
    uint16_t rnd;
    
    _XL_DELETE(zombie_x,pos-1);    
    _XL_DELETE(zombie_x,pos);
    _XL_DELETE(zombie_x,pos+1);
    // _XL_DRAW(zombie_x,pos, ZOMBIE_DEATH_TILE, _XL_RED);
    display_red_zombie();
    _XL_SHOOT_SOUND();
    _XL_DRAW(zombie_x,pos, ZOMBIE_DEATH_TILE, _XL_RED);
    _XL_SHOOT_SOUND();
    _XL_DELETE(zombie_x,pos);
    display_wall(BOTTOM_WALL_Y);
    
    rnd = (_XL_RAND())&15;
    
    if(rnd<ITEM_SPAWN_CHANCE)
    {
        if(rnd<6)
        {
            if(!rechargeItem._active)
            {
                spawn_item(&rechargeItem);
            }
        }
        else if(rnd<11)
        {
            if(!extraPointsItem._active)
            {
                spawn_item(&extraPointsItem);
            }
        }
        else if(rnd<13)
        {
            if(!freezeItem._active)
            {
                spawn_item(&freezeItem);
            }
        }
        else if(rnd<15)
        {
            if(!wallItem._active)
            {
                spawn_item(&wallItem);
            }
        }
        else 
        {
            if(!powerUpItem._active)
            {
                spawn_item(&powerUpItem);
            }  
        }

    }

    zombie_spawn();

    display_score();
}


uint8_t compute_next_arrow(void)
{
    uint8_t i;
    
    for(i=0;i<NUMBER_OF_ARROWS_ON_SCREEN;++i)
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
    
    for(i=0;i<NUMBER_OF_ARROWS_ON_SCREEN;++i)
    {
        if(active_arrow[i]) // ACTIVE
        {
            
            if(arrow_y[i]<(arrow_range))
            {
                active_arrow[i]=0;
                --arrows_on_screen_counter;
            }
            else
            {
                _XL_DELETE(arrow_x[i],arrow_y[i]);
                --arrow_y[i];
                if(arrow_y[i]>=(arrow_range))
                {
                    _XL_DRAW(arrow_x[i],arrow_y[i],arrow_shape[i],_XL_CYAN);
                }
            }
        }
    }
}

uint8_t zombie_hit(void)
{
    uint8_t i;
    
    for(i=0;i<NUMBER_OF_ARROWS_ON_SCREEN;++i)
    {
        if(active_arrow[i] && arrow_x[i]==zombie_x
          && zombie_y[zombie_x]>=arrow_y[i])
           {
               active_arrow[i]=0;
                --arrows_on_screen_counter;

               _XL_DELETE(arrow_x[i],arrow_y[i]);
               return 1;
           }
    }
    return 0;
}

void handle_zombie_collisions(void)
{
    for(zombie_x=0;zombie_x<XSize;++zombie_x)
    {
        if(zombie_hit())
        {
            if(boss[zombie_x])
            {
                display_red_zombie();
                _XL_TOCK_SOUND();
                
                if(powerUp>=POWER_THRESHOLD)
                {
                    --boss[zombie_x];
                }
                --boss[zombie_x];
                if(!boss[zombie_x])
                {
                    score+=BOSS_POINTS;  
                    zombie_die();
                }
                else
                {
                    display_boss();
                }
            }
            else
            {
                score+=MINION_POINTS;
                zombie_die();
            }
        }
    }
}

void move_zombies(void)
{
    uint8_t i;

    for(i=0;i<zombie_spawn_loops;++i)
    {
        zombie_x=_XL_RAND()%XSize;
        display_zombie();
        if(!freeze && (_XL_RAND()<zombie_speed))
        {
            if(zombie_y[zombie_x]<PLAYER_Y)
            {
                ++zombie_shape[zombie_x];

                (zombie_shape[zombie_x])&=3;
 
                if(!zombie_shape[zombie_x])
                    {
                        ++zombie_y[zombie_x];
                    }
            }
            else
            {
                alive = 0;
                display_red_zombie();
            }
        }
    }
    
}


void handle_bow_move(void)
{
    input = _XL_INPUT();
    
    if(_XL_LEFT(input) && bow_x>0)
    {
        move_left();
    }
    else if (_XL_RIGHT(input) && bow_x<MAX_PLAYER_X)
    {
        move_right();
    }
    else if (_XL_FIRE(input) && loaded_bow)
    {
        loaded_bow = 0;
        active_arrow[next_arrow] = 1;
        ++arrows_on_screen_counter;
        bow_load_counter = bow_reload_loops;
        arrow_shape[next_arrow] = arrow_tile[bow_x&1];
        arrow_y[next_arrow] = PLAYER_Y-1;
        _XL_SHOOT_SOUND();
        arrow_x[next_arrow] = (bow_x/2)+(bow_x&1);
        _XL_DRAW(arrow_x[next_arrow],PLAYER_Y-1,arrow_shape[next_arrow],_XL_CYAN);
        
        display_bow();
    }
}


void handle_bow_load(void)
{
    if(!loaded_bow && arrows_on_screen_counter<NUMBER_OF_ARROWS_ON_SCREEN && !bow_load_counter && arrows)
    {
        loaded_bow = 1;
        next_arrow = compute_next_arrow();
        display_bow();
        --arrows;
        display_remaining_arrows();
    }
    if(bow_load_counter)
    {
        --bow_load_counter;
    }
}

void game_over(void)
{
    _XL_EXPLOSION_SOUND();
    
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT_CENTERED(_XL_G _XL_A _XL_M _XL_E _XL_SPACE _XL_O _XL_V _XL_E _XL_R);
    
    _XL_SLEEP(1);
    
    _XL_WAIT_FOR_INPUT();
    
    _XL_CLEAR_SCREEN();
}


void initialize_vars(void)
{
    uint8_t i;

    if(score>hiscore)
    {
        hiscore=score;
    }
    score = 0;
    
    initialize_items();
    number_of_arrows = 1;
    
    freeze = 0;
    powerUp = 0;
    next_arrow = 0;
    arrows_on_screen_counter = 0;
    bow_load_counter = 0;
    bow_reload_loops = INITIAL_BOW_RELOAD_LOOPS;
    
    loaded_bow = 1;
    alive = 1;
    arrows = MAX_ARROWS;
    arrow_range = INITIAL_ARROW_RANGE;
    bow_x = XSize;
    zombie_speed=INITIAL_ZOMBIE_SPEED;
    zombie_spawn_loops=INITIAL_ZOMBIE_SPAWN_LOOPS;
    
    for(i=0;i<NUMBER_OF_ARROWS_ON_SCREEN;++i)
    {
        active_arrow[i] = 0;
    }
    
    for(zombie_x=0;zombie_x<XSize;++zombie_x)
    {
        zombie_y[zombie_x]=ZOMBIE_INITIAL_Y;
        zombie_shape[zombie_x]=0;
        boss[zombie_x]=0;
    }
}


void display_initial_screen(void)
{

    _XL_CLEAR_SCREEN();
    
    display_wall(0);
    display_wall(BOTTOM_WALL_Y);

    _XL_SET_TEXT_COLOR(_XL_CYAN);               
    _XL_PRINT_CENTERED_ON_ROW(2, _XL_H _XL_I _XL_S _XL_C _XL_O _XL_R _XL_E); 

    _XL_SET_TEXT_COLOR(_XL_WHITE);                
    _XL_PRINTD(XSize/2-3,3,5,hiscore);
    
    
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT_CENTERED_ON_ROW(YSize/3,_XL_C _XL_SPACE _XL_R _XL_SPACE _XL_O _XL_SPACE _XL_S _XL_SPACE _XL_S 
                       _XL_SPACE _XL_SPACE 
                       _XL_H _XL_SPACE _XL_O _XL_SPACE _XL_R _XL_SPACE _XL_D _XL_SPACE _XL_E);


    _XL_SET_TEXT_COLOR(_XL_WHITE);               
    _XL_PRINT_CENTERED_ON_ROW(YSize/3+2, _XL_F _XL_A _XL_B _XL_R _XL_I _XL_Z _XL_I _XL_O _XL_SPACE _XL_C _XL_A _XL_R _XL_U _XL_S _XL_O);

    _XL_SLEEP(1);
    
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT_CENTERED_ON_ROW(YSize/3+8, _XL_R _XL_E _XL_S _XL_I _XL_S _XL_T _XL_SPACE _XL_T _XL_H _XL_E _XL_SPACE _XL_H _XL_O _XL_R _XL_D _XL_E );

}

void display_stats(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    display_score();
    
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(XSize-10,0,_XL_H _XL_I);
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(XSize-8,0,5, hiscore);
    
    _XL_DRAW(6,0,ARROW_TILE_0,_XL_CYAN);
    display_remaining_arrows();
    
    display_level();
    
    display_power_ups();
}


int main(void)
{       
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();  
    _XL_INIT_SOUND();
    
    score = 0;
    hiscore = 0;
    
    while(1)
    {
        initialize_vars();
        
        display_initial_screen();
        _XL_WAIT_FOR_INPUT();
        
        _XL_CLEAR_SCREEN();
        
        display_wall(BOTTOM_WALL_Y);
        
        display_bow();
        display_stats();
        _XL_SLEEP(1);
        
        while(alive)
        {
            handle_items();
            move_zombies();
            if(freeze)
            {
                --freeze;
            }
            handle_zombie_collisions();
            handle_bow_move();
            handle_bow_load();
            handle_arrows();            
            _XL_SLOW_DOWN(SLOW_DOWN);

        }
        game_over();
    }

    return EXIT_SUCCESS;
}

