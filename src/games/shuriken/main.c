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

#include "levels.h"


#define INITIAL_LEVEL 3
#define FINAL_LEVEL 7

#define INITIAL_LIVES 5

// ----------------------------------------------------------
// Objects: static walls and collectable items
extern const uint8_t objects_map[];
extern const uint16_t objects_index[];

// ----------------------------------------------------------
// Shurikens: horizontal, vertical and mini-shurikens
extern const uint8_t shurikens_map[];
extern const uint8_t shurikens_index[];


// ----------------------------------------------------------
// Walls: dynamic walls
extern const uint8_t walls_map[];
extern const uint8_t walls_index[];


// TILES

#define MAX_NUMBER_OF_SHURIKENS 16

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

#define RING_TILE         _TILE_25

#define DIAMOND_TILE        _TILE_26




#define RINGS_X XSize/2-3


#define SHURIKEN_LEFT 0
#define SHURIKEN_RIGHT 1
#define SHURIKEN_UP 0
#define SHURIKEN_DOWN 1


#define MIN_PLAYER_Y 3
#define MAX_PLAYER_Y (2*YSize-6)
#define MIN_PLAYER_X 2
#define MAX_PLAYER_X (2*XSize-5)

#define MOVE_FORCE 8U

#if XSize<32
    #define MINI_SHURIKEN_NUMBER 8
#elif XSize<64
    #define MINI_SHURIKEN_NUMBER 8
#else
    #define MINI_SHURIKEN_NUMBER 8
#endif

#define MAX_NUMBER_OF_WALLS 4


#define DIAMOND_POINTS 10
#define FREEZE_POINTS 50
#define RING_POINTS 100
#define SHURIKEN_POINTS 100

#define LEVEL_BONUS 30
#define TIME_BONUS 30
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

uint8_t mini_shuriken_x[MINI_SHURIKEN_NUMBER];
uint8_t mini_shuriken_y[MINI_SHURIKEN_NUMBER];

uint8_t map[XSize][YSize];

uint8_t force;

uint8_t player_cell[4];

uint16_t score;
uint16_t hiscore;

uint8_t player_color;   

uint8_t level_horizontal_shurikens;
uint8_t level_vertical_shurikens;
uint8_t level_mini_shurikens;

uint8_t wall_x[MAX_NUMBER_OF_WALLS];
uint8_t wall_y[MAX_NUMBER_OF_WALLS];
uint8_t wall_width[MAX_NUMBER_OF_WALLS];
uint8_t wall_height[MAX_NUMBER_OF_WALLS];
uint8_t wall_type[MAX_NUMBER_OF_WALLS];
uint8_t wall_color[MAX_NUMBER_OF_WALLS];
uint8_t wall_counter[MAX_NUMBER_OF_WALLS];
uint8_t wall_triggered[MAX_NUMBER_OF_WALLS];
uint8_t wall_threshold[MAX_NUMBER_OF_WALLS];

uint8_t number_of_walls;

uint8_t freeze_active;
uint8_t freeze_counter;

uint8_t ring_active;

uint16_t counter;

uint8_t ring_counter;

uint8_t shuriken_counter;

uint8_t border_color;
uint8_t mini_shuriken_color;

uint8_t restart_level;

uint8_t tile_group;

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

static const uint8_t border_colors[] = {_XL_YELLOW, _XL_RED, _XL_CYAN, _XL_GREEN};
// const uint8_t shuriken_colors[] = {_XL_CYAN, _XL_CYAN, _XL_RED, _XL_CYAN};
static const uint8_t mini_shuriken_colors[] = {_XL_RED, _XL_YELLOW};

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


void build_element(uint8_t type, uint8_t color, uint8_t x, uint8_t y)
{
    map[x][y] = type;
	_XL_DRAW(x,y,screen_tile[type], color);

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
    _XL_PRINTD(7,YSize-1,1,shuriken_counter);
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

/*        
		switch(cell_value)
		{
			case EMPTY:
			break;
			
			case DIAMOND:
				_XL_PING_SOUND();
                score+=DIAMOND_POINTS;
                update_score_display();
                --remaining_diamonds;
                update_remaining_display();
			break;
			
			case FREEZE:
                _XL_ZAP_SOUND();
                score+=FREEZE_POINTS;
                update_score_display();
                ++freeze_counter;
                freeze_active=freeze_counter<<4;
                update_freeze_display();
			break;
			
			case RING:
                _XL_ZAP_SOUND();
                score+=RING_POINTS;
                update_score_display();
                ++ring_counter;
                player_color = _XL_YELLOW;
                ring_active=BASE_RING_EFFECT+(ring_counter<<4);
                update_ring_display();
			break;
			
			case WALL:
				alive = 0;
			break;
			
			case SHURIKEN:
			case MINI_SHURIKEN:
                if(ring_active)
                {
                    display_player();
                }
                else
                {
                    alive=0;
                }
			break;			
		}
*/

        if(cell_value)
        {
            if(cell_value==DIAMOND)
            {
                _XL_PING_SOUND();
                score+=DIAMOND_POINTS;
                update_score_display();
                --remaining_diamonds;
                update_remaining_display();
            }
            else if(cell_value==FREEZE)
            {
                _XL_ZAP_SOUND();
                score+=FREEZE_POINTS;
                update_score_display();
                ++freeze_counter;
                freeze_active=freeze_counter<<4;
                update_freeze_display();
            }
            else if(cell_value==RING)
            {
                _XL_ZAP_SOUND();
                score+=RING_POINTS;
                update_score_display();
                ++ring_counter;
                player_color = _XL_YELLOW;
                ring_active=BASE_RING_EFFECT+(ring_counter<<4);
                update_ring_display();
            }
            else if(cell_value==WALL) 
            {
                alive = 0;
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


void update_force(uint8_t cell1, uint8_t cell2)
{
    if(((cell1==BLOCK)||(cell2==BLOCK))&&(force<MOVE_FORCE))
    {     
		++force;
    }
    else
    {
        force=0;
    }
}


uint8_t allowed(uint8_t cell1, uint8_t cell2, uint8_t beyond_cell1, uint8_t beyond_cell2)
{
    update_force(cell1,cell2);

	if((cell1==WALL)||(cell2==WALL))
	{
		return 0;
	}

    if(force<MOVE_FORCE)
    {
        return (cell1!=BLOCK) && (cell2!=BLOCK);
    }
    else
    {
		
        return ((!beyond_cell1)||(beyond_cell1==WALL)) && ((!beyond_cell2)||(beyond_cell2==WALL)) ;
    }
}


uint8_t allowed_down(void)
{
    uint8_t cell1 = map[screen_x][screen_y+2];
    uint8_t cell2 = map[screen_x+1][screen_y+2];

    uint8_t beyond_cell1 = map[screen_x][screen_y+3];
    uint8_t beyond_cell2 = map[screen_x+1][screen_y+3]; 
    
    return allowed(cell1,cell2,beyond_cell1,beyond_cell2);

}


uint8_t allowed_up(void)
{
    uint8_t cell1 = map[screen_x][screen_y-1];
    uint8_t cell2 = map[screen_x+1][screen_y-1];

    uint8_t beyond_cell1 = map[screen_x][screen_y-2];
    uint8_t beyond_cell2 = map[screen_x+1][screen_y-2]; 
    
    return allowed(cell1,cell2,beyond_cell1,beyond_cell2);
}


uint8_t allowed_left(void)
{
    uint8_t cell1 = map[screen_x-1][screen_y];
    uint8_t cell2 = map[screen_x-1][screen_y+1];    
    
    uint8_t beyond_cell1 = map[screen_x-2][screen_y];
    uint8_t beyond_cell2 = map[screen_x-2][screen_y+1]; 	
	
    return allowed(cell1,cell2,beyond_cell1,beyond_cell2);
}


uint8_t allowed_right(void)
{
    uint8_t cell1 = map[screen_x+2][screen_y];
    uint8_t cell2 = map[screen_x+2][screen_y+1]; 

    uint8_t beyond_cell1 = map[screen_x+3][screen_y];
    uint8_t beyond_cell2 = map[screen_x+3][screen_y+1]; 	
        
    return allowed(cell1,cell2,beyond_cell1,beyond_cell2);
}


void _if_block_push_down(uint8_t x)
{
    if((map[x][screen_y+2]==BLOCK)&&!map[x][screen_y+3])
    {
        build_element(BLOCK,_XL_GREEN, x,screen_y+3);
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
        build_element(BLOCK,_XL_GREEN,x,screen_y-2);
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
        build_element(BLOCK,_XL_GREEN,screen_x-2,y);
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
        build_element(BLOCK,_XL_GREEN,screen_x+3,y);
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
            build_element(MINI_SHURIKEN, mini_shuriken_color, mini_shuriken_x[i],mini_shuriken_y[i]);
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
    
    
    border_color = border_colors[level&3];
    // shuriken_color = shuriken_colors[level&3];
    mini_shuriken_color = mini_shuriken_colors[level&1];
    
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
        build_element(WALL,border_color,i,1);
        build_element(WALL,border_color,i,YSize-1);
    }
    for(i=1;i<YSize-1;++i)
    {
        build_element(WALL,border_color,0,i);
        build_element(WALL,border_color,XSize-1,i);
    }


}


void init_score_display(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
  
    update_score_display();
    _XL_PRINTD(XSize-5,0,5,hiscore);
    
    _XL_DRAW(XSize-10,0,SHURIKEN_TILE,_XL_WHITE);
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    
    _XL_PRINTD(XSize-9,0,1,lives);
    
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(XSize-7,0,"HI");
    
    _XL_DRAW(6,0,DIAMOND_TILE,_XL_GREEN);

    update_remaining_display();
    
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(XSize-2,YSize-1,2,level+1);
    
    _XL_DRAW(0,YSize-1,RING_TILE,_XL_WHITE);
    _XL_DRAW(3,YSize-1,FREEZE_TILE,_XL_CYAN);
    _XL_DRAW(6,YSize-1,SHURIKEN_TILE,_XL_CYAN);
    update_ring_display();
    update_freeze_display();
    update_shuriken_display();
}



// too many parameters
void build_rectangle(uint8_t type, uint8_t color, uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
    uint8_t i;
    uint8_t j;
    
    for(i=x;i<x+width;++i)
    {
        for(j=y;j<y+height;++j)
        {
            
            if(type)
            {
                build_element(type, color,i,j);
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
    // uint8_t j;
    // uint8_t k;
    uint8_t x;
    uint8_t y;
    uint8_t x_size;
    uint8_t y_size;
    uint8_t type;
    uint8_t color;  
    
    // _XL_PRINTD(XSize/2,YSize/2,4,index);
    
    remaining_diamonds = 0;
    for(i=0;i<no_of_objects;++i)
    {
        x = objects_map[++index];
        y = objects_map[++index];
        x_size = objects_map[++index];
        y_size = objects_map[++index];
        type = objects_map[++index];
        color = objects_map[++index];
        if(type==DIAMOND)
        {
            remaining_diamonds+=x_size*y_size;
        }
        // TODO: USE build_rectangle
        // for(j=x;j<x+x_size;++j)
        // {
            // for(k=y;k<y+y_size;++k)
            // {

                // build_element(type,color,j,k);
                
            // }
        // }
		build_rectangle(type,color,x,y,x_size,y_size);
    }
}


void build_walls(void)
{
    uint8_t index = walls_index[level];
    uint8_t i;

    number_of_walls = walls_map[index];   
    
    // _XL_PRINTD(XSize/2,YSize/2+2,3,number_of_walls);
    // _XL_WAIT_FOR_INPUT();
        
    for(i=0;i<number_of_walls;++i)
    {
        wall_x[i] = walls_map[++index];
        wall_y[i] = walls_map[++index];
        wall_width[i] = walls_map[++index];
        wall_height[i] = walls_map[++index];  
        wall_type[i] = walls_map[++index];
        wall_color[i] = walls_map[++index];       
        wall_counter[i] = walls_map[++index];       
        wall_threshold[i] = walls_map[++index];
        wall_triggered[i] = 0;
    }
}


void build_shurikens(void)
{
    uint8_t index = shurikens_index[level];
    uint8_t i;

    level_horizontal_shurikens = shurikens_map[index];
    level_vertical_shurikens = shurikens_map[++index];
    level_mini_shurikens = shurikens_map[++index];

    for(i=0;i<level_horizontal_shurikens+level_vertical_shurikens;++i)
    {
        shuriken_x[i]=shurikens_map[++index];
        shuriken_y[i]=shurikens_map[++index];
        shuriken_direction[i]=0;
        shuriken_transition[i]=0;
        build_element(SHURIKEN,SHURIKEN_COLOR,shuriken_x[i],shuriken_y[i]);
   
    }
    
    for(i=0;i<level_mini_shurikens;++i)
    {
        mini_shuriken_x[i]=shurikens_map[++index];
        mini_shuriken_y[i]=1;
        build_element(MINI_SHURIKEN,mini_shuriken_color,mini_shuriken_x[i],mini_shuriken_y[i]);
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

// TODO: to be fixed
void switch_wall_if_possible(uint8_t i)
{
    uint8_t wall;  

    if(!wall_triggered[i])
    {
        if(safe_area(wall_x[i]-1,wall_y[i]-1,wall_width[i]+2, wall_height[i]+2))
        {
            // _XL_PRINT(XSize/2,YSize/2,"TRIGGERED");
            _XL_TOCK_SOUND();
            wall = wall_type[i];
            ++wall_triggered[i];
        }
        else
        {
            return;
        }
    }
    else
    {
        // _XL_PRINT(XSize/2,YSize/2,"DELETED");
        wall = EMPTY;
        wall_triggered[i] = 0;
    }

    build_rectangle(wall,wall_color[i],wall_x[i],wall_y[i],wall_width[i], wall_height[i]);
}


void handle_wall(uint8_t i)
{
    if(wall_counter[i]<wall_threshold[i])
    {
        ++wall_counter[i];
    }
    else
    {
        wall_counter[i]=0;
        switch_wall_if_possible(i);
    }
}


void handle_walls(void)
{
    uint8_t i;
    for(i=0;i<number_of_walls;++i)
    {
        handle_wall(i);
    }
}


void init_level(void)
{
    init_map();    
            
    build_objects();
    
    init_score_display();

    build_shurikens();
    
    build_walls();
    
    // REMARK: Initialize counter *only* at level start (not after losing a life)
    counter = 0;
}


void display_horizontal_transition_shuriken(uint8_t x, uint8_t y)
{
    _XL_DRAW(x-1,y,SHURIKEN_TILE_LEFT, SHURIKEN_COLOR);
    _XL_DRAW(x,y,SHURIKEN_TILE_RIGHT, SHURIKEN_COLOR);
}


void shuriken_death(uint8_t x, uint8_t y, uint8_t index)
{
    _XL_SHOOT_SOUND();
    delete_element(x,y);
    score+=SHURIKEN_POINTS;
    update_score_display();
    ++shuriken_counter;
    update_shuriken_display();
    shuriken_status[index]=0;
}


void block_explosion(uint8_t x, uint8_t y)
{
    _XL_DRAW(x,y,BLOCK_TILE,_XL_RED);
    _XL_SHOOT_SOUND();
    delete_element(x,y);
}


void handle_horizontal_shuriken(register uint8_t index)
{
    register uint8_t x = shuriken_x[index];
    register uint8_t y = shuriken_y[index];
    
    if(shuriken_direction[index]==SHURIKEN_LEFT)
    {
        
        if(!shuriken_transition[index]) // transition not performed, yet
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
                shuriken_death(x,y,index);
                block_explosion(x-1,y);
            }
            else
            {
                shuriken_direction[index]=SHURIKEN_RIGHT;
            }
        }
        else // transition already performed
        {
            shuriken_transition[index]=0;
            delete_element(x,y);
            --shuriken_x[index];
            _XL_DRAW(shuriken_x[index],y,SHURIKEN_TILE,SHURIKEN_COLOR);
        }
    }
    else // direction is RIGHT
    {
        if(!shuriken_transition[index]) // transition not performed, yet
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
                shuriken_death(x,y,index);
                block_explosion(x+1,y);
            }
            else
            {
                shuriken_direction[index]=SHURIKEN_LEFT;
                
            }
        }
        else // transition already performed
        {
            shuriken_transition[index]=0;
            delete_element(x,y);
            ++shuriken_x[index];
            _XL_DRAW(shuriken_x[index],y,SHURIKEN_TILE,SHURIKEN_COLOR);
        }
    }
}



void delete_shurikens(void)
{
    uint8_t i;
    
    for(i=0;i<level_horizontal_shurikens;++i)
    {
        if(shuriken_status[i])
        {
            delete_element(shuriken_x[i],shuriken_y[i]);
            if(shuriken_transition[i])
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
        }
    }
    
    for(i=0+level_horizontal_shurikens;i<level_horizontal_shurikens+level_vertical_shurikens;++i)
    {
        if(shuriken_status[i])
        {
            delete_element(shuriken_x[i],shuriken_y[i]);
            
            if(shuriken_transition[i])
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
    
    for(i=0;i<level_mini_shurikens;++i)
    {
        delete_element(mini_shuriken_x[i],mini_shuriken_y[i]);
    }
}


void handle_horizontal_shurikens(void)
{
    uint8_t i;
    
    for(i=0;i<level_horizontal_shurikens;++i)
    {
        if(shuriken_status[i])
        {
            handle_horizontal_shuriken(i);
        }
    }
}


void display_vertical_transition_shuriken(uint8_t x, uint8_t y)
{
    _XL_DRAW(x,y-1,SHURIKEN_TILE_UP,SHURIKEN_COLOR);
    _XL_DRAW(x,y,SHURIKEN_TILE_DOWN,SHURIKEN_COLOR);
}


void handle_vertical_shuriken(register uint8_t index)
{
    register uint8_t x = shuriken_x[index];
    register uint8_t y = shuriken_y[index];
    
    if(shuriken_direction[index]==SHURIKEN_UP)
    {
        
        if(!shuriken_transition[index]) // transition not performed, yet
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
                shuriken_death(x,y,index);
                block_explosion(x,y-1);
            }
            else
            {
                shuriken_direction[index]=SHURIKEN_DOWN;
            }
        }
        else // transition already performed
        {
            shuriken_transition[index]=0;
            delete_element(x,y);
            --shuriken_y[index];
            _XL_DRAW(x,shuriken_y[index],SHURIKEN_TILE,SHURIKEN_COLOR);
        }
    }
    else // direction is DOWN
    {
        if(!shuriken_transition[index]) // transition not performed, yet
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
                shuriken_death(x,y,index);
                block_explosion(x,y+1);
            }
            else
            {
                shuriken_direction[index]=SHURIKEN_UP;
            }
        }
        else // transition already performed
        {
            shuriken_transition[index]=0;
            delete_element(x,y);    
            ++shuriken_y[index];
            _XL_DRAW(x,shuriken_y[index],SHURIKEN_TILE,SHURIKEN_COLOR);
        }
    }
}


void handle_vertical_shurikens(void)
{
    uint8_t i;
    
    for(i=0+level_horizontal_shurikens;i<level_horizontal_shurikens+level_vertical_shurikens;++i)
    {
        if(shuriken_status[i])
        {
            handle_vertical_shuriken(i);
        }
    }
}


void handle_player(void)
{
    uint8_t input;
    
    input = _XL_INPUT();
    
    if(_XL_UP(input) && (!(player_y&1) || allowed_up()))
    {
        // if(player_y>MIN_PLAYER_Y)
        // {
            if(player_y&1)
            {
                delete_player_down();
                if_block_push_up();
            }
            --player_y;

            update_player();
        // }
    }
    else if(_XL_DOWN(input) && ((player_y&1) || allowed_down()))
    {    
        // if(player_y<MAX_PLAYER_Y)
        // {
            if(!(player_y&1))
            {
                delete_player_up();
                if_block_push_down();
            }
            ++player_y;

            update_player();
        // }
    }
    else if(_XL_LEFT(input) && ((player_x&1) || allowed_left()))
    {
        // if(player_x>MIN_PLAYER_X)
        // {
            if(!(player_x&1))
            {
                delete_player_right();
                if_block_push_left();
            }
            --player_x;

            update_player();
        // }
    }
    else if(_XL_RIGHT(input) && (!(player_x&1) || allowed_right()))
    {    
        // if(player_x<MAX_PLAYER_X)
        // {   
            if(player_x&1)
            {
                delete_player_left();
                if_block_push_right();
            }
            ++player_x;

            update_player();
        // }
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
    player_x = XSize;
    player_y = YSize;
    
    force = 0;
    ring_active = START_RING_EFFECT;
	
    player_color = _XL_YELLOW;
    
}


void title(void)
{
    _XL_CLEAR_SCREEN();
    
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    
    _XL_PRINTD(XSize/2-2,1,5,hiscore);    
    
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    
    _XL_PRINT(XSize/2-7,4, "S H U R I K E N");
    
    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    
    _XL_WAIT_FOR_INPUT();
    
    _XL_CLEAR_SCREEN();
}


void init_variables(void)
{
    score = 0;
    lives = INITIAL_LIVES;
    // remaining_diamonds = 0;
    level = INITIAL_LEVEL;
}


void handle_shurikens(void)
{
    if((!freeze_active) || (counter&1))
    {
        handle_vertical_shurikens();
        handle_horizontal_shurikens();        
        handle_mini_shuriken();
    }
    else if(freeze_active)
    {
        --freeze_active;
    }
    // _XL_PRINTD(0,YSize-1,3,freeze_active);
}


void handle_ring(void)
{
    // _XL_PRINTD(0,1,3,ring_active);
    if(ring_active)
    {
        --ring_active;
    }
    else
    {
        player_color = _XL_WHITE;
        display_player();
    }
}


void handle_lose_life(void)
{
    uint8_t i;
    
    player_color=_XL_RED;
    display_player();
    _XL_EXPLOSION_SOUND();
    
    --lives;

    // TODO: Do I need these lines?
    init_score_display();
    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    _XL_WAIT_FOR_INPUT();
    
    delete_player();
    
    delete_shurikens();
    
    build_shurikens();

    for(i=0;i<number_of_walls;++i)
    {
        switch_wall_if_possible(i);
    }

}


void update_item_display(void)
{
	update_freeze_display();
	update_ring_display();
	update_shuriken_display();
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
		update_score_display();
		_XL_ZAP_SOUND();
		_XL_SLEEP(1);
		} while(*item_counter_ptr);
	}
}


void handle_next_level(void)
{
    ++level;
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(XSize/2-4,YSize/2,"COMPLETED");
    _XL_WAIT_FOR_INPUT();
    restart_level = 1;
    
    // _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    _XL_SLEEP(1);
    score+=LEVEL_BONUS*level;
    update_score_display();
    // _XL_PING_SOUND();
    // _XL_SLEEP(1);
    // _XL_WAIT_FOR_INPUT();
    
    if(counter<1024+128)
    {
        score+=(8-counter/128)*TIME_BONUS;
		_XL_PING_SOUND();
		update_score_display();
    }
    _XL_SLEEP(1);
    // _XL_WAIT_FOR_INPUT();
    
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

    update_freeze_display();
    update_ring_display();
    update_shuriken_display();
    
    // REMARK: counter should not be initialized 
    // otherwise the player gets more points by losing just before completing a level
}


int main(void)
{        
    _XL_INIT_GRAPHICS();

    _XL_INIT_INPUT();
    
    _XL_INIT_SOUND();

    hiscore = 0;
    
    // init_screen_tiles();
    
    while(1)
    {
        title();
        
        init_variables();
        
        restart_level = 1;
        
        while(lives && (level<FINAL_LEVEL+1))
        {            
            init_player_achievements();

            if(restart_level)
            {
                init_level();
            }
            init_player();

            update_player();
            
            restart_level = 0;
            _XL_SLEEP(1);   
            _XL_WAIT_FOR_INPUT();
            
            
            while(remaining_diamonds && alive)
            {

                handle_player();
                
                handle_shurikens();
                handle_walls();
                
                handle_ring();
                handle_collisions();
                
                ++counter;

                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR/2);
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

