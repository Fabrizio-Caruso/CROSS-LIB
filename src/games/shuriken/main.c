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

#include "cross_lib.h"

#include "screen_types.h"
#include "levels.h"


#define INITIAL_LEVEL 3
#define FINAL_LEVEL 11

#define INITIAL_LIVES 5

// DEBUG
// #define SHOW_LEVELS
// #define INVINCIBLE

// TILES

#define MAX_NUMBER_OF_SHURIKENS 16

#define MAX_TIME 9

// Left low player in the 2x2 multi-tile
#define LEFT_LOW_TILE0      _TILE_2
#define LEFT_LOW_TILE1      _TILE_6
#define LEFT_LOW_TILE2      _TILE_10
#define LEFT_LOW_TILE3      _TILE_14

#define RIGHT_LOW_TILE0     _TILE_3
#define RIGHT_LOW_TILE1     _TILE_7
#define RIGHT_LOW_TILE2     _TILE_11
#define RIGHT_LOW_TILE3     _TILE_15

#define LEFT_UP_TILE0       _TILE_0
#define LEFT_UP_TILE1       _TILE_4
#define LEFT_UP_TILE2       _TILE_8
#define LEFT_UP_TILE3       _TILE_12

#define RIGHT_UP_TILE0      _TILE_1
#define RIGHT_UP_TILE1      _TILE_5
#define RIGHT_UP_TILE2      _TILE_9
#define RIGHT_UP_TILE3      _TILE_13

#define SHURIKEN_TILE       _TILE_16
#define SHURIKEN_TILE_DOWN  _TILE_17
#define SHURIKEN_TILE_UP    _TILE_18
#define SHURIKEN_TILE_LEFT  _TILE_19
#define SHURIKEN_TILE_RIGHT _TILE_20

#define WALL_TILE           _TILE_21

#define BLOCK_TILE          _TILE_22

#define FREEZE_TILE         _TILE_23

#define MINI_SHURIKEN_TILE  _TILE_24

#define RING_TILE           _TILE_25

#define DIAMOND_TILE        _TILE_26




#define RINGS_X XSize/2-3


#define SHURIKEN_LEFT 0
#define SHURIKEN_RIGHT 1
#define SHURIKEN_UP 0
#define SHURIKEN_DOWN 1

#define PLAYER_LEFT 0
#define PLAYER_RIGHT 1
#define PLAYER_UP 2
#define PLAYER_DOWN 3

#define MIN_PLAYER_Y 3
#define MAX_PLAYER_Y (2*YSize-6)
#define MIN_PLAYER_X 2
#define MAX_PLAYER_X (2*XSize-5)

#define MOVE_FORCE 3U
#define DESTROY_FORCE 20U

#if XSize<32
    #define MAX_NUMBER_OF_MINI_SHURIKENS 6
#elif XSize<64
    #define MAX_NUMBER_OF_MINI_SHURIKENS 6
#else
    #define MAX_NUMBER_OF_MINI_SHURIKENS 6
#endif

#define MAX_NUMBER_OF_WALLS 4


#define DIAMOND_POINTS 10
#define BONUS_DIAMOND_POINTS 30
#define FREEZE_POINTS 50
#define RING_POINTS 100
#define SHURIKEN_POINTS 100

#define LEVEL_BONUS 100
// #define TIME_BONUS 30
// #define FREEZE_BONUS 50
// #define RING_BONUS   100
// #define SHURIKEN_BONUS 100
#define ITEM_BONUS 50


#if XSize<32
    #define BASE_RING_EFFECT 30
#else
    #define BASE_RING_EFFECT 20
#endif

#define START_RING_EFFECT 25

#define EXTRA_LIFE_THRESHOLD 5000

#define WALL_COLOR _XL_YELLOW
#define WALL_THRESHOLD 25

uint8_t player_x;
uint8_t player_y;

uint8_t screen_x;
uint8_t screen_y;

uint8_t alive;
uint8_t lives;
uint8_t level;
uint8_t remaining_diamonds;

uint8_t shuriken_x[MAX_NUMBER_OF_SHURIKENS];
uint8_t shuriken_y[MAX_NUMBER_OF_SHURIKENS];
uint8_t shuriken_direction[MAX_NUMBER_OF_SHURIKENS];
uint8_t shuriken_transition[MAX_NUMBER_OF_SHURIKENS];
#define shuriken_status shuriken_x

uint8_t shuriken_axis[MAX_NUMBER_OF_SHURIKENS];

#define SHURIKEN_HORIZONTAL 0
#define SHURIKEN_VERTICAL   1

uint8_t mini_shuriken_x[MAX_NUMBER_OF_MINI_SHURIKENS];
uint8_t mini_shuriken_y[MAX_NUMBER_OF_MINI_SHURIKENS];

uint8_t map[XSize][YSize];

uint8_t force;

uint8_t player_cell[4];

uint16_t score;
uint16_t hiscore;

uint8_t player_color;   

// uint8_t level_horizontal_shurikens;
// uint8_t level_vertical_shurikens;
uint8_t level_shurikens;
uint8_t level_mini_shurikens;

uint8_t barrier_x[MAX_NUMBER_OF_WALLS];
uint8_t barrier_y[MAX_NUMBER_OF_WALLS];
uint8_t barrier_width[MAX_NUMBER_OF_WALLS];
uint8_t barrier_height[MAX_NUMBER_OF_WALLS];
uint8_t barrier_counter;
uint8_t barrier_triggered[MAX_NUMBER_OF_WALLS];

uint8_t number_of_walls;

uint8_t freeze_active;
uint8_t freeze_counter;

uint8_t ring_active;

uint8_t counter;

uint8_t ring_counter;

uint8_t shuriken_counter;

uint8_t restart_level;

uint8_t tile_group;

uint8_t time_counter;

uint8_t player_direction;

uint8_t extra_life_counter;

uint16_t slowdown;

uint8_t shuriken_chase[MAX_NUMBER_OF_SHURIKENS];

uint8_t shuriken_challenge;

uint8_t barrier_type;

uint8_t challenge_level;

uint8_t remaining_shurikens;

uint8_t barrier_threshold;

static const uint8_t screen_tile[7+1] =
{
    0, // unused
    RING_TILE,
    FREEZE_TILE,
    DIAMOND_TILE,
    BLOCK_TILE,
    WALL_TILE,
    SHURIKEN_TILE,
    MINI_SHURIKEN_TILE,
};  


static uint8_t screen_color[7+1] =
{
    0, // unused
    _XL_WHITE,
    _XL_CYAN,
    _XL_GREEN,
    _XL_GREEN,
    _XL_RED,
    _XL_CYAN,
    _XL_YELLOW,
};  


static const uint8_t border_colors[] = {_XL_YELLOW, _XL_CYAN, _XL_RED, _XL_GREEN};

#define SHURIKEN_COLOR _XL_CYAN

static const uint8_t player_tile[4][4] =
{
    { // left lower 12x12 multi-tile inside a 16x16 quad tile
        LEFT_UP_TILE0,
        RIGHT_UP_TILE0,
        LEFT_LOW_TILE0,
        RIGHT_LOW_TILE0,
    },
    { // right lower 12x12 multi-tile inside a 16x16 quad tile
        LEFT_UP_TILE1,
        RIGHT_UP_TILE1,
        LEFT_LOW_TILE1,
        RIGHT_LOW_TILE1,
    },
    { // left upper 12x12 multi-tile inside a 16x16 quad tile
        LEFT_UP_TILE2,
        RIGHT_UP_TILE2,
        LEFT_LOW_TILE2,
        RIGHT_LOW_TILE2,
    },
    { // right upper 12x12 multi-tile inside a 16x16 quad tile 
        LEFT_UP_TILE3,
        RIGHT_UP_TILE3,
        LEFT_LOW_TILE3,
        RIGHT_LOW_TILE3,
    },
};


void build_element(uint8_t type, uint8_t x, uint8_t y)
{
    map[x][y] = type;
    _XL_DRAW(x,y,screen_tile[type], screen_color[type]);

}


void delete_element(uint8_t x, uint8_t y)
{
    map[x][y] = EMPTY;
    _XL_DELETE(x,y);
    // _XL_CHAR(x,y,'X');
}


void update_screen_xy(void)
{
    screen_x = player_x>>1;
    screen_y = (player_y+1)>>1;
}
    

void update_score_display(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(0,0,5,score);
}


void update_remaining_display(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(7,0,2,remaining_diamonds);
}


void display_player(void)
{
    _XL_DRAW(screen_x,screen_y,player_tile[tile_group][2],player_color);
    _XL_DRAW(screen_x+1,screen_y,player_tile[tile_group][3],player_color);  
    _XL_DRAW(screen_x,screen_y+1,player_tile[tile_group][0],player_color);
    _XL_DRAW(screen_x+1,screen_y+1,player_tile[tile_group][1],player_color);  
}


void update_freeze_display(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(4,YSize-1,1,freeze_counter);
}


void update_ring_display(void)
{
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINTD(1,YSize-1,1,ring_counter);
}


void update_shuriken_display(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(7,YSize-1,2,shuriken_counter);
}


void update_time_counter_display(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(11,YSize-1,1,time_counter);    
}


void update_item_display(void)
{
    update_score_display();
    
    update_remaining_display();
    update_time_counter_display();
    update_freeze_display();
    update_ring_display();
    update_shuriken_display();
}


void increase_time_counter_if_not_max(void)
{
	if(time_counter<MAX_TIME)
	{
		++time_counter;
	}
}


void handle_collisions(void)
{
    uint8_t i;
    uint8_t cell_value;
    
    player_cell[0] = map[screen_x][screen_y];
    player_cell[1] = map[screen_x+1][screen_y];
    player_cell[2] = map[screen_x][screen_y+1];
    player_cell[3] = map[screen_x+1][screen_y+1];
    
    for(i=0;i<4;++i)
    {
        cell_value = player_cell[i];

        if(cell_value)
        {
            if(cell_value==DIAMOND)
            {
                _XL_PING_SOUND();
				if(challenge_level)
				{
					score+=BONUS_DIAMOND_POINTS;
				}
				else
				{
					score+=DIAMOND_POINTS;
				}
                // update_score_display();
                --remaining_diamonds;
                // update_remaining_display();
                update_item_display();
            }
            else if(cell_value==FREEZE)
            {
                _XL_ZAP_SOUND();
                score+=FREEZE_POINTS;
                // update_score_display();
                ++freeze_counter;
				increase_time_counter_if_not_max();
                freeze_active=freeze_counter<<4;
                update_item_display();
            }
            else if(cell_value==RING)
            {
                _XL_ZAP_SOUND();
                score+=RING_POINTS;
                // update_score_display();
                ++ring_counter;
				increase_time_counter_if_not_max();
                player_color = _XL_YELLOW;
                ring_active=BASE_RING_EFFECT+(ring_counter<<4);
                update_item_display();
            }
            else if(cell_value>=DEADLY)
            {
                if(ring_active)
                {
                    display_player();
                }
                else
                {
                    alive=0;
                }
            }
        }
    } 
}


void update_player(void)
{
    tile_group = (player_x&1)+2*(player_y&1);
    
    update_screen_xy();
    
    display_player();
    
    handle_collisions();
    
    map[screen_x][screen_y]=EMPTY;
    map[screen_x+1][screen_y]=EMPTY;
    map[screen_x][screen_y+1]=EMPTY;
    map[screen_x+1][screen_y+1]=EMPTY;
    
}


void delete_player_down(void)
{
    _XL_DELETE(screen_x,screen_y+1);
    _XL_DELETE(screen_x+1,screen_y+1);      
}


void delete_player_up(void)
{
    _XL_DELETE(screen_x,screen_y);
    _XL_DELETE(screen_x+1,screen_y);   
}


void delete_player_left(void)
{
    _XL_DELETE(screen_x,screen_y);
    _XL_DELETE(screen_x,screen_y+1);
}


void delete_player_right(void)
{
    _XL_DELETE(screen_x+1,screen_y);  
    _XL_DELETE(screen_x+1,screen_y+1);  
}


void delete_player(void)
{
    delete_player_down();
    delete_player_up();
}


// void increase_force_if_on_block(uint8_t cell1, uint8_t cell2)
// {

// }


uint8_t allowed(uint8_t cell1, uint8_t cell2, uint8_t beyond_cell1, uint8_t beyond_cell2)
{
    if((cell1==BLOCK)||(cell2==BLOCK))
    {     
        ++force;
    }

    if((cell1==WALL)||(cell2==WALL))
    {
        return 0;
    }

    // From here on, the player is not against any wall
    if(force>=DESTROY_FORCE)
    {
        force=0;
        _XL_EXPLOSION_SOUND();
        return 1;
    }

    // If no wall and no block, you can always move
    if((cell1!=BLOCK) && (cell2!=BLOCK))
    {
        return 1;
    }

    // From here on, the player is against at least one block and no wall and force<DESTROY_FORCE
    if(force<MOVE_FORCE)
    {
        // If there is a block, you can't move
        return 0;
    }
    else 
    {   // You can push blocks but you cannot destroy them
        return ((!cell1)||(!beyond_cell1)) && ((!cell2)||(!beyond_cell2));
    }
}


uint8_t allowed_down(void)
{
    // uint8_t cell1 = ;
    // uint8_t cell2 = ;

    // uint8_t beyond_cell1 = ;
    // uint8_t beyond_cell2 = ; 
    
    return allowed(map[screen_x][screen_y+2],map[screen_x+1][screen_y+2],map[screen_x][screen_y+3],map[screen_x+1][screen_y+3]);

}


uint8_t allowed_up(void)
{
    // uint8_t cell1 = ;
    // uint8_t cell2 = ;

    // uint8_t beyond_cell1 = ;
    // uint8_t beyond_cell2 = ; 
    
    return allowed(map[screen_x][screen_y-1],map[screen_x+1][screen_y-1],map[screen_x][screen_y-2],map[screen_x+1][screen_y-2]);
}


uint8_t allowed_left(void)
{
    // uint8_t cell1 = ;
    // uint8_t cell2 = ;    
    
    // uint8_t beyond_cell1 = ;
    // uint8_t beyond_cell2 = ;     
    
    return allowed(map[screen_x-1][screen_y],map[screen_x-1][screen_y+1],map[screen_x-2][screen_y],map[screen_x-2][screen_y+1]);
}


uint8_t allowed_right(void)
{
    // uint8_t cell1 = ;
    // uint8_t cell2 = ; 

    // uint8_t beyond_cell1 = ;
    // uint8_t beyond_cell2 = ;     
        
    return allowed(map[screen_x+2][screen_y],map[screen_x+2][screen_y+1],map[screen_x+3][screen_y],map[screen_x+3][screen_y+1]);
}


void _if_block_push_down(uint8_t x)
{
    if((map[x][screen_y+2]==BLOCK)&&!map[x][screen_y+3])
    {
        build_element(BLOCK, x,screen_y+3);
        force=0;    
    }
}


void if_block_push_down(void)
{
    _if_block_push_down(screen_x);
    _if_block_push_down(screen_x+1);
}


void _if_block_push_up(uint8_t x)
{
    if((map[x][screen_y-1]==BLOCK)&&!map[x][screen_y-2])
    {
        build_element(BLOCK, x,screen_y-2);
        force=0;
    }
}


void if_block_push_up(void)
{
    _if_block_push_up(screen_x);
    _if_block_push_up(screen_x+1);

}


void _if_block_push_left(uint8_t y)
{
    if((map[screen_x-1][y]==BLOCK)&&!map[screen_x-2][y])
    {
        build_element(BLOCK,screen_x-2,y);
        force=0;
    }
}


void if_block_push_left(void)
{
    _if_block_push_left(screen_y);
    _if_block_push_left(screen_y+1);    
}


void _if_block_push_right(uint8_t y)
{
    if((map[screen_x+2][y]==BLOCK)&&!map[screen_x+3][y])
    {
        build_element(BLOCK,screen_x+3,y);
        force=0;
    }
}


void if_block_push_right(void)
{
    _if_block_push_right(screen_y);
    _if_block_push_right(screen_y+1);
}


void handle_mini_shuriken(void)
{    
    uint8_t i;

    for(i=0;i<level_mini_shurikens;++i)
    {

        delete_element(mini_shuriken_x[i],mini_shuriken_y[i]);
        
        ++(mini_shuriken_y[i]);

        if(!map[mini_shuriken_x[i]][mini_shuriken_y[i]] && mini_shuriken_y[i]<YSize-2)
        {
            build_element(MINI_SHURIKEN, mini_shuriken_x[i],mini_shuriken_y[i]);
        }
        else
        {    
            if(!map[mini_shuriken_x[i]][mini_shuriken_y[i]])
            {
                delete_element(mini_shuriken_x[i],mini_shuriken_y[i]);
            }
            mini_shuriken_y[i] = 2;                            
        }
        
    }
}


void init_map(void)
{
    uint8_t i;
    uint8_t j;

    _XL_CLEAR_SCREEN();
    for(i=0;i<XSize-1;++i)
    {
        for(j=0;j<YSize-1;++j)
        {
            map[i][j]=EMPTY;
        }
    }
    
    for(i=0;i<XSize;++i)
    {
        build_element(WALL,i,1);
        build_element(WALL,i,YSize-1);
    }
    for(i=1;i<YSize-1;++i)
    {
        build_element(WALL,0,i);
        build_element(WALL,XSize-1,i);
    }


}


void update_lives_display(void)
{     
	_XL_SET_TEXT_COLOR(_XL_CYAN);
    
    _XL_PRINTD(XSize-9,0,1,lives);
}


void init_score_display(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
  
    update_score_display();
    _XL_PRINTD(XSize-5,0,5,hiscore);
    
    _XL_DRAW(XSize-10,0,SHURIKEN_TILE,_XL_WHITE);
    
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(XSize-7,0,"HI");
    
	build_element(DIAMOND,6,0);

	update_lives_display();

    // update_remaining_display();
    
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(XSize-2,YSize-1,2,level+1);
    
    _XL_DRAW(0,YSize-1,RING_TILE,_XL_WHITE);
    _XL_DRAW(3,YSize-1,FREEZE_TILE,_XL_CYAN);
    _XL_DRAW(6,YSize-1,SHURIKEN_TILE,_XL_CYAN);
    
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_CHAR(10,YSize-1,'T');

    update_item_display();
}



// too many parameters
void build_rectangle(uint8_t type, uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
    uint8_t i;
    uint8_t j;
    
    for(i=x;i<x+width;++i)
    {
        for(j=y;j<y+height;++j)
        {
            
            if(type)
            {
                build_element(type,i,j);
            }
            else // REMARK: Necessary to switch walls
            {
                delete_element(i,j);
            }
        }
    }
}


void build_objects(void)
{
    uint16_t index = objects_index[level];
    uint8_t no_of_objects = objects_map[index];
    uint8_t i;
    uint8_t x;
    uint8_t y;
    uint8_t x_size;
    uint8_t y_size;
    uint8_t type;
    // uint8_t color;  

    screen_color[WALL]=border_colors[level&3];
    
    remaining_diamonds = 0;
    for(i=0;i<no_of_objects;++i)
    {
        x = objects_map[++index];
        y = objects_map[++index];
        x_size = objects_map[++index];
        y_size = objects_map[++index];
        type = objects_map[++index];
        // color = objects_map[++index];
        if(type==DIAMOND)
        {
            remaining_diamonds+=x_size*y_size;
        }

        build_rectangle(type,x,y,x_size,y_size);
    }
}


void build_barriers(void)
{   
    uint8_t index = walls_index[level];
    uint8_t i;

    number_of_walls = walls_map[index];   
        
    for(i=0;i<number_of_walls;++i)
    {
        barrier_x[i] = walls_map[++index];
        barrier_y[i] = walls_map[++index];
        barrier_width[i] = walls_map[++index];
        barrier_height[i] = walls_map[++index];  	
        barrier_triggered[i] = 0;
    }
	if(challenge_level)
	{
		barrier_counter = WALL_THRESHOLD*4;
	}
	else
	{
		barrier_counter = 0;
	}	
}


void build_shurikens(void)
{
    uint8_t index = shurikens_index[level];
    uint8_t i;

	uint8_t level_horizontal_shurikens;
	// uint8_t level_vertical_shurikens;
	// _XL_PRINTD(1,1,4,index);
	// _XL_WAIT_FOR_INPUT();

    level_horizontal_shurikens = shurikens_map[index];
	// _XL_PRINTD(1,1,4,level_horizontal_shurikens);
	// _XL_WAIT_FOR_INPUT();
    // level_vertical_shurikens = shurikens_map[++index];
	// _XL_PRINTD(1,1,4,level_vertical_shurikens);
	// _XL_WAIT_FOR_INPUT();
	
	level_shurikens = level_horizontal_shurikens + shurikens_map[++index];;
	
    level_mini_shurikens = shurikens_map[++index];
	// _XL_PRINTD(1,1,4,level_mini_shurikens);
	// _XL_WAIT_FOR_INPUT();

    for(i=0;i<level_shurikens;++i)
    {
		if(shuriken_status[i])
		{
			shuriken_x[i]=shurikens_map[++index];
			shuriken_y[i]=shurikens_map[++index];
			shuriken_direction[i]=0;
			shuriken_transition[i]=0;
			build_element(SHURIKEN,shuriken_x[i],shuriken_y[i]);
			if(i<level_horizontal_shurikens)
			{
				shuriken_axis[i]=SHURIKEN_HORIZONTAL;
			}
			else
			{
				shuriken_axis[i]=SHURIKEN_VERTICAL;
			}
		}
		else
		{
			index+=2;
		}
    }
    for(i=level_shurikens;i<MAX_NUMBER_OF_SHURIKENS;++i)
    {
        shuriken_status[i]=0;
    }
	
    for(i=0;i<level_mini_shurikens;++i)
    {
        mini_shuriken_x[i]=shurikens_map[++index];
        mini_shuriken_y[i]=2;
        build_element(MINI_SHURIKEN,mini_shuriken_x[i],mini_shuriken_y[i]);
    }
}


uint8_t safe_area(uint8_t x, uint8_t y, uint8_t x_size, uint8_t y_size)
{
    uint8_t j;
    uint8_t k;
    
    for(j=x;j<x+x_size;++j)
    {
        for(k=y;k<y+y_size;++k)
        {
            if(map[j][k])
            {
                return 0;
            }
        }
    }
    return 1;
}


void switch_barrier_if_possible(uint8_t i)
{
    uint8_t barrier;  

    if(!barrier_triggered[i])
    {
        // if(safe_area(barrier_x[i]-1,barrier_y[i]-1,barrier_width[i]+2, barrier_height[i]+2))
		if(safe_area(barrier_x[i],barrier_y[i],barrier_width[i], barrier_height[i]))
        {
            _XL_TOCK_SOUND();
            barrier = barrier_type;
            ++barrier_triggered[i];
        }
        else
        {
            return;
        }
    }
    else if(!challenge_level)
    {
        barrier = EMPTY;
        barrier_triggered[i] = 0;
    }
	else
	{
		barrier = barrier_type;
	}

    build_rectangle(barrier,barrier_x[i],barrier_y[i],barrier_width[i], barrier_height[i]);
}


void handle_barriers(void)
{
	uint8_t i;
	
	// _XL_PRINTD(0,i,3,barrier_counter[i]);
    if(barrier_counter<barrier_threshold)
    {
        ++barrier_counter;
    }
    else
    {
        barrier_counter=0;
		for(i=0;i<number_of_walls;++i)
		{
			switch_barrier_if_possible(i);
		}
	}
}


// void handle_barriers(void)
// {
    // uint8_t i;
    // for(i=0;i<number_of_walls;++i)
    // {
        // handle_barrier(i);
    // }
// }


uint8_t is_challenge_level(void)
{
	return !((level+1)&3);
}


void activate_shurikens(void)
{
	uint8_t i;
	
	for(i=0;i<MAX_NUMBER_OF_SHURIKENS;++i)
	{
		shuriken_status[i]=1;
	}

}


void use_block_against_shurikens(void)
{
    _XL_PRINT(XSize/2-7,YSize/2+4, "USE   AGAINST");

	_XL_DRAW(XSize/2-7+4,YSize/2+4,BLOCK_TILE, _XL_GREEN);

	_XL_DRAW(XSize/2-7+14,YSize/2+4,SHURIKEN_TILE, _XL_CYAN);
}


void init_level(void)
{
    uint8_t i;
    
	if(is_challenge_level())
	{
		challenge_level = 1;
		barrier_type = BLOCK;
		screen_color[DIAMOND]=_XL_YELLOW;
		barrier_threshold=WALL_THRESHOLD*5U;
		
		_XL_SET_TEXT_COLOR(_XL_WHITE);
		_XL_CLEAR_SCREEN();
		use_block_against_shurikens();
		_XL_WAIT_FOR_INPUT();
		
	}
	else
	{
		challenge_level = 0;
		barrier_type = SHURIKEN;
		screen_color[DIAMOND]=_XL_GREEN;
		barrier_threshold=WALL_THRESHOLD;
	}
	
	init_map();    

    build_objects();
    
    time_counter = MAX_TIME;

    init_score_display();

	activate_shurikens();

    build_shurikens();
    
	remaining_shurikens = level_shurikens;
	
    build_barriers();
    
    // REMARK: Initialize counter *only* at level start (not after losing a life)
    counter = 0;
    
    // _XL_PRINTD(0,0,5,level_horizontal_shurikens);
    // _XL_PRINTD(0,1,5,level_vertical_shurikens);
    // _XL_PRINTD(0,2,5,level_mini_shurikens);

    // _XL_PRINTD(0,3,5,_XL_SLOW_DOWN_FACTOR);
    // _XL_PRINTD(0,4,5,(_XL_SLOW_DOWN_FACTOR/((MAX_NUMBER_OF_SHURIKENS+MAX_NUMBER_OF_MINI_SHURIKENS)*2)));    
    
    slowdown = _XL_SLOW_DOWN_FACTOR-((_XL_SLOW_DOWN_FACTOR/((MAX_NUMBER_OF_SHURIKENS+MAX_NUMBER_OF_MINI_SHURIKENS)*2))*(level_shurikens+level_mini_shurikens));
    
    // _XL_PRINTD(0,5,5,(_XL_SLOW_DOWN_FACTOR/((MAX_NUMBER_OF_SHURIKENS+MAX_NUMBER_OF_MINI_SHURIKENS)*2))*(level_horizontal_shurikens+level_vertical_shurikens+level_mini_shurikens));    
    
    
    // _XL_PRINTD(0,6,5,slowdown);
    // _XL_WAIT_FOR_INPUT();
	
    for(i=0;i<MAX_NUMBER_OF_SHURIKENS;++i)
    {
        if(challenge_level && (i<level))
        {
			
            shuriken_chase[i] = 1;
        }
        else
        {
            shuriken_chase[i] = 0;
        }
    }
}


void display_horizontal_transition_shuriken(uint8_t x, uint8_t y)
{
    _XL_DRAW(x-1,y,SHURIKEN_TILE_LEFT, SHURIKEN_COLOR);
    _XL_DRAW(x,y,SHURIKEN_TILE_RIGHT, SHURIKEN_COLOR);
}


void shuriken_death(uint8_t index)
{
    _XL_SHOOT_SOUND();
    // delete_element(x,y);
    score+=SHURIKEN_POINTS;
    ++shuriken_counter;
	--remaining_shurikens;
	increase_time_counter_if_not_max();
    update_item_display();
    shuriken_status[index]=0;
}


void block_explosion(uint8_t x, uint8_t y)
{
    _XL_DRAW(x,y,BLOCK_TILE,_XL_RED);
    _XL_SHOOT_SOUND();
    delete_element(x,y);
}


uint8_t player_chased_by(uint8_t index)
{
    return shuriken_chase[index]&&(!(_XL_RAND()&7));
}


void display_shuriken(uint8_t x, uint8_t y, uint8_t index)
{
    uint8_t shuriken_color;
    
    if(player_chased_by(index))
    {
        shuriken_color = _XL_YELLOW;
    }
    else
    {
        shuriken_color = _XL_CYAN;
    }
    _XL_DRAW(x,y,SHURIKEN_TILE,shuriken_color);
}


void chase_vertically(uint8_t index)
{
    shuriken_axis[index]=SHURIKEN_VERTICAL;
    
    if(screen_y<shuriken_y[index])
    {
        shuriken_direction[index]=SHURIKEN_UP;
    }
    else
    {
        shuriken_direction[index]=SHURIKEN_DOWN; 
    }
}


void handle_horizontal_shuriken(register uint8_t index)
{
    register uint8_t x = shuriken_x[index];
    register uint8_t y = shuriken_y[index];
    
    if(shuriken_direction[index]==SHURIKEN_LEFT)
    {
        if(!shuriken_transition[index]) // transition not performed, yet
        {
            
            if(player_chased_by(index))
            {
                chase_vertically(index);
            }                
            else
            {
                if(!map[x-1][y])
                {
                    // Do left transition
                    display_horizontal_transition_shuriken(x,y);
                    map[x-1][y]=SHURIKEN;
                    ++shuriken_transition[index];
                }
                else if(map[x-1][y]==BLOCK)
                {
                    delete_element(x,y);
                    shuriken_death(index);
                    block_explosion(x-1,y);
                }
                else
                {
                    shuriken_direction[index]=SHURIKEN_RIGHT;
                }
            }
        }
        else // transition already performed
        {
            shuriken_transition[index]=0;
            delete_element(x,y);
            --shuriken_x[index];
            display_shuriken(shuriken_x[index],y,index);
        }

    }
    else // direction is RIGHT
    {
        if(!shuriken_transition[index]) // transition not performed, yet
        {
            if(player_chased_by(index))
            {
                chase_vertically(index);
            }                
            else
            {
                if(!map[x+1][y])
                {
                    // Do right transition
                    display_horizontal_transition_shuriken(x+1,y);
                    map[x+1][y]=SHURIKEN;
                    ++shuriken_transition[index];
                }
                else if(map[x+1][y]==BLOCK)
                {
                    delete_element(x,y);
                    shuriken_death(index);
                    block_explosion(x+1,y);
                }
                else
                {
                    shuriken_direction[index]=SHURIKEN_LEFT;
                    
                }
            }
        }
        else // transition already performed
        {
            shuriken_transition[index]=0;
            delete_element(x,y);
            ++shuriken_x[index];
            display_shuriken(shuriken_x[index],y,index);
        }
    }
}



void delete_shurikens(void)
{
    uint8_t i;
    
    for(i=0;i<level_shurikens;++i)
    {
        if(shuriken_status[i])
        {
            delete_element(shuriken_x[i],shuriken_y[i]);
            
            if(shuriken_transition[i])
            {
                if(shuriken_axis[i]==SHURIKEN_HORIZONTAL)
                {
                    if(shuriken_direction[i]==SHURIKEN_RIGHT)
                    {
                        delete_element(shuriken_x[i]+1,shuriken_y[i]);
                    }
                    else
                    {
                        delete_element(shuriken_x[i]-1,shuriken_y[i]);
                    }
                }
                else
                {
                    if(shuriken_direction[i]==SHURIKEN_DOWN)
                    {
                        delete_element(shuriken_x[i],shuriken_y[i]+1);
                    }
                    else
                    {
                        delete_element(shuriken_x[i],shuriken_y[i]-1);
                    }
                }
            }
        }
    }
    
    for(i=0;i<level_mini_shurikens;++i)
    {
        delete_element(mini_shuriken_x[i],mini_shuriken_y[i]);
    }
}



void display_vertical_transition_shuriken(uint8_t x, uint8_t y)
{
    _XL_DRAW(x,y-1,SHURIKEN_TILE_UP,SHURIKEN_COLOR);
    _XL_DRAW(x,y,SHURIKEN_TILE_DOWN,SHURIKEN_COLOR);
}


void chase_horizontally(uint8_t index)
{
    shuriken_axis[index] = SHURIKEN_HORIZONTAL;
        
    if(screen_x<shuriken_x[index])
    {
        shuriken_direction[index]=SHURIKEN_LEFT;
    }
    else
    {
        shuriken_direction[index]=SHURIKEN_RIGHT; 
    }    
}


void handle_vertical_shuriken(register uint8_t index)
{
    register uint8_t x = shuriken_x[index];
    register uint8_t y = shuriken_y[index];
    
    if(shuriken_direction[index]==SHURIKEN_UP)
    {
        if(!shuriken_transition[index]) // transition not performed, yet
        {
            if(player_chased_by(index))
            {
                chase_horizontally(index);
            }                
            else
            {
                if(!map[x][y-1])
                {
                    // Do up transition
                    display_vertical_transition_shuriken(x,y);
                    map[x][y-1]=SHURIKEN;
                    ++shuriken_transition[index];
                }
                else if(map[x][y-1]==BLOCK)
                {
                    delete_element(x,y);
                    shuriken_death(index);
                    block_explosion(x,y-1);
                }
                else
                {
                    shuriken_direction[index]=SHURIKEN_DOWN;
                }
            }
        }
        else // transition already performed
        {
            shuriken_transition[index]=0;
            delete_element(x,y);
            --shuriken_y[index];
            display_shuriken(x,shuriken_y[index],index);
        }
    }
    else // direction is DOWN
    {
        if(!shuriken_transition[index]) // transition not performed, yet
        {
            if(player_chased_by(index))
            {
                chase_horizontally(index);
            }                
            else
            {
                if(!map[x][y+1])
                {
                    // Do right transition
                    display_vertical_transition_shuriken(x,y+1);
                    map[x][y+1]=SHURIKEN;
                    ++shuriken_transition[index];
                }
                else if(map[x][y+1]==BLOCK)
                {
                    delete_element(x,y);
                    shuriken_death(index);
                    block_explosion(x,y+1);
                }
                else
                {
                    shuriken_direction[index]=SHURIKEN_UP;
                }
            }
        }
        else // transition already performed
        {
            shuriken_transition[index]=0;
            delete_element(x,y);    
            ++shuriken_y[index];
            display_shuriken(x,shuriken_y[index],index);
        }
    }
}



void handle_big_shurikens(void)
{
    uint8_t i;
    
    for(i=0;i<MAX_NUMBER_OF_SHURIKENS;++i)
    {
        if(shuriken_status[i])
        {   
            if(!shuriken_axis[i])
            {
                handle_horizontal_shuriken(i);
            }
            else
            {
                handle_vertical_shuriken(i);
            }
        }
    }
}


void update_player_direction(uint8_t direction)
{
    update_player();
    if(player_direction!=direction)
    {
        force=0;
    }
    player_direction=direction;
}


void handle_player(void)
{
    uint8_t input;
    
    input = _XL_INPUT();
    
    if(_XL_UP(input) && (!(player_y&1) || allowed_up()))
    {
        if(player_y&1)
        {
            delete_player_down();
            if_block_push_up();
        }
        --player_y;

        update_player_direction(PLAYER_UP);
    }
    else if(_XL_DOWN(input) && ((player_y&1) || allowed_down()))
    {    
        if(!(player_y&1))
        {
            delete_player_up();
            if_block_push_down();
        }
        ++player_y;

        update_player_direction(PLAYER_DOWN);

    }
    else if(_XL_LEFT(input) && ((player_x&1) || allowed_left()))
    {
        if(!(player_x&1))
        {
            delete_player_right();
            if_block_push_left();
        }
        --player_x;

        update_player_direction(PLAYER_LEFT);

    }
    else if(_XL_RIGHT(input) && (!(player_x&1) || allowed_right()))
    {    
        if(player_x&1)
        {
            delete_player_left();
            if_block_push_right();
        }
        ++player_x;

        update_player_direction(PLAYER_RIGHT);

    }
    // REMARK: We need this because shuriken do delete the player despite hand_collision
    else 
    {
        // force=0;
        if(ring_active)
        {
            display_player();
        }
    }
}


void init_player(void)
{
    alive = 1;
    player_x = XSize-1;
    player_y = YSize-1;
    
    force = 0;
    ring_active = START_RING_EFFECT;
    
    player_color = _XL_YELLOW;
    
}


void title(void)
{
    _XL_CLEAR_SCREEN();
    
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    
    _XL_PRINT(XSize/2-7,6, "FABRIZIO CARUSO");
	
    _XL_PRINTD(XSize/2-2,1,5,hiscore);    
	
	screen_x=XSize/2;
	screen_y=8;
	player_color=_XL_WHITE;
	
	display_player();

	_XL_SET_TEXT_COLOR(_XL_YELLOW);
	
    _XL_PRINT(XSize/2-7+3,YSize/2+1, "COLLECT");
	
	use_block_against_shurikens();
 
	_XL_DRAW(XSize/2-7+8+3,YSize/2+1,DIAMOND_TILE, _XL_GREEN);
		
    _XL_SET_TEXT_COLOR(_XL_RED);
    
    _XL_PRINT(XSize/2-7,4, "S H U R I K E N");
    
    _XL_WAIT_FOR_INPUT();
    
    // _XL_CLEAR_SCREEN();
}


void init_global_game(void)
{
    score = 0;
    lives = INITIAL_LIVES;
    // remaining_diamonds = 0;
    level = INITIAL_LEVEL;
    extra_life_counter = 1;
    
    restart_level = 1;

}


void handle_shurikens(void)
{
    if((!freeze_active) || (counter&1))
    {
        handle_big_shurikens();
        handle_mini_shuriken();
    }
    else if(freeze_active)
    {
        --freeze_active;
    }
}


void handle_ring(void)
{
    if(ring_active)
    {
        #if !defined(INVINCIBLE)
        --ring_active;
        #endif
    }
    else
    {
        player_color = _XL_WHITE;
        display_player();
    }
}


void handle_lose_life(void)
{
    // uint8_t i;
    
    player_color=_XL_RED;
    display_player();
    _XL_EXPLOSION_SOUND();
    
    --lives;

    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    _XL_WAIT_FOR_INPUT();
    
    delete_player();
    
    delete_shurikens();
    
    // build_shurikens();

    // for(i=0;i<number_of_walls;++i)
    // {
        // switch_barrier_if_possible(i);
    // }
}


void item_bonus(uint8_t *item_counter_ptr)
{
    if(*item_counter_ptr)
    {
        do
        {
        score+=ITEM_BONUS;
        --(*item_counter_ptr);
        update_item_display();
        // update_score_display();
        _XL_ZAP_SOUND();
        _XL_SLOW_DOWN(4*_XL_SLOW_DOWN_FACTOR);
        } while(*item_counter_ptr);
		_XL_SLEEP(1);
    }
}


void handle_next_level(void)
{
    ++level;
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(XSize/2-4,YSize/2,"COMPLETED");
    _XL_WAIT_FOR_INPUT();
    restart_level = 1;
    
    _XL_SLEEP(1);
    score+=LEVEL_BONUS*level;
    update_score_display();
	_XL_TOCK_SOUND();

    _XL_SLEEP(1);
    
    item_bonus(&time_counter);
    item_bonus(&shuriken_counter);
    item_bonus(&freeze_counter);
    item_bonus(&ring_counter);
}


void init_player_achievements(void)
{
    freeze_counter=0;
    freeze_active=0;
    
    ring_counter=0;
    ring_active=0;

    shuriken_counter=0;

    // update_item_display();

    // REMARK: counter should not be initialized 
    // otherwise the player gets more points by losing just before completing a level
}


void handle_time(void)
{
    if(!(counter&63))
    {
        if(time_counter)
        {
            --time_counter;
            update_time_counter_display();
        }
    }
}


void handle_extra_life(void)
{
	if(score>=EXTRA_LIFE_THRESHOLD*extra_life_counter)
	{
		++extra_life_counter;
        _XL_PING_SOUND();
		++lives;
		update_lives_display();
        _XL_PING_SOUND();
	}
}


uint8_t continue_condition(void)
{
	
	#if defined(SHOW_LEVELS)
		return 0;
	#endif
	// _XL_PRINTD(0,1,3,remaining_shurikens);
	if(challenge_level)
	{
		return remaining_shurikens && alive;
	}
	else
	{
		return remaining_diamonds && alive;
	}
}


int main(void)
{        
    _XL_INIT_GRAPHICS();

    _XL_INIT_INPUT();
    
    _XL_INIT_SOUND();

    hiscore = 0;
        
    while(1)
    {
        title();
        
        init_global_game();
                
        while(lives && (level<FINAL_LEVEL+1))
        {            
            init_player_achievements();

            if(restart_level)
            {
                init_level();
            }
			init_score_display();
			
            init_player();

            update_player();
            
            restart_level = 0;
            _XL_SLEEP(1);   
            _XL_WAIT_FOR_INPUT();
            
            
            while(continue_condition())
            {
                // _XL_PRINTD(1,1,4,force);
                handle_player();
                
                if(alive)
                {
                    handle_shurikens();
                    handle_barriers();
                    
                    handle_ring();
                    handle_collisions();
                    
                    ++counter;

                    _XL_SLOW_DOWN(slowdown);
                    
					handle_extra_life();
					
					handle_time();
	
                }
            }
            if(alive)
            {
                handle_next_level();
            }
            else
            {
                handle_lose_life();
            }
        };
        
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(XSize/2-4,YSize/2,"GAME OVER");
        
        if(score>hiscore)
        {
            hiscore = score;
        }
        
        _XL_SLEEP(1);
        _XL_WAIT_FOR_INPUT();
    }
    
    return EXIT_SUCCESS;
}

