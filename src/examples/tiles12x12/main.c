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

#if XSize<17
	#define MAX_NUMBER_OF_HORIZONTAL_SHURIKENS 5
#elif XSize<24
	#define MAX_NUMBER_OF_HORIZONTAL_SHURIKENS 6
#elif XSize<33
	#define MAX_NUMBER_OF_HORIZONTAL_SHURIKENS 8
#else
	#define MAX_NUMBER_OF_HORIZONTAL_SHURIKENS 9
#endif

#if YSize<13
	#define MAX_NUMBER_OF_VERTICAL_SHURIKENS 3
#elif YSize<19
	#define MAX_NUMBER_OF_VERTICAL_SHURIKENS 4
#elif YSize<24
	#define MAX_NUMBER_OF_VERTICAL_SHURIKENS 4
#elif YSize<26
	#define MAX_NUMBER_OF_VERTICAL_SHURIKENS 5
#else
	#define MAX_NUMBER_OF_VERTICAL_SHURIKENS 6
#endif

// TILES

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

#define SHIELD_TILE         _TILE_23

#define MINI_SHURIKEN_TILE  _TILE_24

#define RING_TILE           _TILE_25

#define DIAMOND_TILE        _TILE_26


#define EMPTY 0
#define SHIELD 1

#define RING 2
#define DIAMOND 3

#define BLOCK 4

#define SHURIKEN 5
#define MINI_SHURIKEN 6

#define WALL  7

#define DEADLY 5


const uint8_t screen_tile[7+1] =
{
    0, // unused
    SHIELD_TILE,
    RING_TILE,
    DIAMOND_TILE,
    BLOCK_TILE,
    SHURIKEN_TILE,
    MINI_SHURIKEN_TILE,
    WALL_TILE,
};  



#define SHURIKEN_RIGHT 0
#define SHURIKEN_LEFT 1
#define SHURIKEN_UP 2
#define SHURIKEN_DOWN 3


#define MIN_PLAYER_Y 3
#define MAX_PLAYER_Y (2*YSize-6)
#define MIN_PLAYER_X 2
#define MAX_PLAYER_X (2*XSize-5)

#define MOVE_FORCE 8

#define MINI_SHURIKEN_NUMBER 2

#define INITIAL_LIVES 3
#define FINAL_LEVEL 1

uint8_t player_x;
uint8_t player_y;

uint8_t screen_x;
uint8_t screen_y;

uint8_t alive;
uint8_t lives;
uint8_t level;
uint8_t remaining_diamonds;

uint8_t horizontal_shuriken_x[MAX_NUMBER_OF_HORIZONTAL_SHURIKENS];
uint8_t horizontal_shuriken_y[MAX_NUMBER_OF_HORIZONTAL_SHURIKENS];
uint8_t horizontal_shuriken_direction[MAX_NUMBER_OF_HORIZONTAL_SHURIKENS];
uint8_t horizontal_shuriken_transition[MAX_NUMBER_OF_HORIZONTAL_SHURIKENS];

uint8_t vertical_shuriken_x[MAX_NUMBER_OF_VERTICAL_SHURIKENS];
uint8_t vertical_shuriken_y[MAX_NUMBER_OF_VERTICAL_SHURIKENS];
uint8_t vertical_shuriken_direction[MAX_NUMBER_OF_VERTICAL_SHURIKENS];
uint8_t vertical_shuriken_transition[MAX_NUMBER_OF_VERTICAL_SHURIKENS];

uint8_t mini_shuriken_x[MINI_SHURIKEN_NUMBER];
uint8_t mini_shuriken_y[MINI_SHURIKEN_NUMBER];

uint8_t map[XSize][YSize];

uint8_t force;
// uint8_t move_threshold;

uint8_t player_cell[4];

uint16_t score;
uint16_t hiscore;

uint8_t player_color;

uint8_t level_horizontal_shurikens;
uint8_t level_vertical_shurikens;
uint8_t level_mini_shurikens;

uint8_t chasing_transition;
uint8_t chasing_direction;
uint8_t chasing_x;
uint8_t chasing_y;


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


static const uint8_t objects_map[] =
{
    6, // rectangles
    XSize/8,2,3,YSize-2-2,SHIELD,_XL_WHITE,
    
    XSize/4,4,1,YSize-1-2-4,BLOCK,_XL_GREEN,
    
    XSize/2+4,4,4,YSize-1-2-4,SHURIKEN,_XL_CYAN,

    XSize-2,4,1,YSize-1-2-4,DIAMOND,_XL_GREEN,
    XSize-3,4,1,YSize-1-2-4,RING,_XL_WHITE,
    XSize-4,4,1,YSize-1-2-4,WALL,_XL_CYAN,
    
};


static const uint8_t objects_index[] = 
{
    0,
    0,
    0,
    // TODO: ....
};


static const uint8_t shurikens_map[] =
{
    6,
    3,4,_XL_CYAN,
    3,7,_XL_CYAN,
    3,10,_XL_CYAN,
    3,YSize-10,_XL_CYAN,
    3,YSize-4,_XL_CYAN,
    3,YSize-7,_XL_CYAN,
};


static const uint8_t shurikens_index[] = 
{
    0,
    0,
    0,
    // TODO: ....
};


static const uint8_t mini_shurikens_map[] =
{
    4, // horizontal
    0, // vertical
    0, // mini
};


static const uint8_t mini_shurikens_index[] = 
{
    0,
    0,
    0,
    // TODO: ....
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
}


// void build_rectangle(uint8_t type, uint8_t color, uint8_t width, uint8_t height)
// {
    // uint8_t i;
    // uint8_t j;
    
    // for(i=0;i<width;++i)
    // {
        // for(j=0;j<height;++j)
        // {
            // build_element(type, color, i,j);
        // }
    // }
// }


void update_screen_xy(void)
{
	screen_x = player_x>>1;
	screen_y = (player_y+1)>>1;
}
    

void update_score_display(void)
{
    _XL_PRINTD(0,0,5,score);
}


void handle_collisions(void)
{
    uint8_t i;
    
    player_cell[0] = map[screen_x][screen_y];
    player_cell[1] = map[screen_x+1][screen_y];
    player_cell[2] = map[screen_x][screen_y+1];
    player_cell[3] = map[screen_x+1][screen_y+1];
    
    for(i=0;i<4;++i)
    {
        if(player_cell[i]==DIAMOND)
        {
            _XL_PING_SOUND();
            // TODO: score and effects
            score+=50;
            update_score_display();
        }
        else if(player_cell[i]==RING)
        {
            _XL_ZAP_SOUND();
            // TODO: score and effects
            score+=250;
            update_score_display();
        }
        else if(player_cell[i]>=DEADLY)
        {
            alive = 0;
        }
    }
    
}


void update_player(void)
{
	uint8_t tile_group = (player_x&1)+2*(player_y&1);
	
	update_screen_xy();
    
	
    _XL_DRAW(screen_x,screen_y,player_tile[tile_group][2],player_color);
    _XL_DRAW(screen_x+1,screen_y,player_tile[tile_group][3],player_color);  
    _XL_DRAW(screen_x,screen_y+1,player_tile[tile_group][0],player_color);
    _XL_DRAW(screen_x+1,screen_y+1,player_tile[tile_group][1],player_color);  
    
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


void update_force(uint8_t cell1, uint8_t cell2)
{
    if((cell1==BLOCK)||(cell2==BLOCK))
    {     
        if(force<MOVE_FORCE)
        {
            ++force;
            // _XL_PRINTD(8,0,3,force);
            // move_threshold=SHIELD;
            // player_color = _XL_WHITE;
            // update_player();
        }
        else
        {
            // force=0;
            // _XL_PRINTD(8,0,3,force);

            // move_threshold=BLOCK;
            player_color = _XL_RED;
            update_player();
        }  
    }
    else
    {
        force=0;
        player_color = _XL_WHITE;
        // move_threshold=BLOCK-1;
    }
}


uint8_t allowed(uint8_t cell1, uint8_t cell2)
{
    update_force(cell1,cell2);

	if(force<MOVE_FORCE)
	{
		return (cell1!=WALL) && (cell2!=WALL) && (cell1!=BLOCK) && (cell2!=BLOCK);
	}
    else 
	{
		return (cell1!=WALL) && (cell2!=WALL);
	}
}


uint8_t allowed_down(void)
{
    uint8_t cell1 = map[screen_x][screen_y+2];
    uint8_t cell2 = map[screen_x+1][screen_y+2];
    
    return allowed(cell1,cell2);

}


uint8_t allowed_up(void)
{
    uint8_t cell1 = map[screen_x][screen_y-1];
    uint8_t cell2 = map[screen_x+1][screen_y-1];
    
    return allowed(cell1,cell2);
}


uint8_t allowed_left(void)
{
    uint8_t cell1 = map[screen_x-1][screen_y];
    uint8_t cell2 = map[screen_x-1][screen_y+1];    
    
    return allowed(cell1,cell2);
}


uint8_t allowed_right(void)
{
    uint8_t cell1 = map[screen_x+2][screen_y];
    uint8_t cell2 = map[screen_x+2][screen_y+1];    
        
    return allowed(cell1,cell2);
}


void _if_block_move_down(uint8_t screen_x)
{
    if((map[screen_x][screen_y+2]==BLOCK)&&map[screen_x][screen_y+3]<=SHIELD)
    {
        // map[screen_x][screen_y+3]=BLOCK;
        // _XL_DRAW(screen_x,screen_y+3,BLOCK_TILE,_XL_GREEN);
        build_element(BLOCK,_XL_GREEN, screen_x,screen_y+3);
    }
    
    // if((map[screen_x+1][screen_y+2]==BLOCK)&&map[screen_x+1][screen_y+3]<=SHIELD)
    // {
        // map[screen_x+1][screen_y+3]=BLOCK;
        // _XL_DRAW(screen_x+1,screen_y+3,BLOCK_TILE,_XL_GREEN);
    // }
}


void if_block_move_down(void)
{
    _if_block_move_down(screen_x);
    _if_block_move_down(screen_x+1);
    // if((map[screen_x][screen_y+2]==BLOCK)&&map[screen_x][screen_y+3]<=SHIELD)
    // {
        // map[screen_x][screen_y+3]=BLOCK;
        // _XL_DRAW(screen_x,screen_y+3,BLOCK_TILE,_XL_GREEN);
    // }
    
    // if((map[screen_x+1][screen_y+2]==BLOCK)&&map[screen_x+1][screen_y+3]<=SHIELD)
    // {
        // map[screen_x+1][screen_y+3]=BLOCK;
        // _XL_DRAW(screen_x+1,screen_y+3,BLOCK_TILE,_XL_GREEN);
    // }
}


void _if_block_move_up(uint8_t screen_x)
{
    if((map[screen_x][screen_y-1]==BLOCK)&&map[screen_x][screen_y-2]<=SHIELD)
    {
        // map[screen_x][screen_y-2]=BLOCK;
        // _XL_DRAW(screen_x,screen_y-2,BLOCK_TILE,_XL_GREEN);
        build_element(BLOCK,_XL_GREEN,screen_x,screen_y-2);
    }
    
    // if((map[screen_x+1][screen_y-1]==BLOCK)&&map[screen_x+1][screen_y-2]<=SHIELD)
    // {
        // map[screen_x+1][screen_y-2]=BLOCK;
        // _XL_DRAW(screen_x+1,screen_y-2,BLOCK_TILE,_XL_GREEN);
    // }
}


void if_block_move_up(void)
{
    _if_block_move_up(screen_x);
    _if_block_move_up(screen_x+1);
    
    // if((map[screen_x][screen_y-1]==BLOCK)&&map[screen_x][screen_y-2]<=SHIELD)
    // {
        // map[screen_x][screen_y-2]=BLOCK;
        // _XL_DRAW(screen_x,screen_y-2,BLOCK_TILE,_XL_GREEN);
    // }
    
    // if((map[screen_x+1][screen_y-1]==BLOCK)&&map[screen_x+1][screen_y-2]<=SHIELD)
    // {
        // map[screen_x+1][screen_y-2]=BLOCK;
        // _XL_DRAW(screen_x+1,screen_y-2,BLOCK_TILE,_XL_GREEN);
    // }
}


void _if_block_move_left(uint8_t screen_y)
{
    if((map[screen_x-1][screen_y]==BLOCK)&&map[screen_x-2][screen_y]<=SHIELD)
    {
        // map[screen_x-2][screen_y]=BLOCK;
        // _XL_DRAW(screen_x-2,screen_y,BLOCK_TILE,_XL_GREEN);
        build_element(BLOCK,_XL_GREEN,screen_x-2,screen_y);
    }
    
    // if((map[screen_x-1][screen_y+1]==BLOCK)&&map[screen_x-2][screen_y+1]<=SHIELD)
    // {
        // map[screen_x-2][screen_y+1]=BLOCK;
        // _XL_DRAW(screen_x-2,screen_y+1,BLOCK_TILE,_XL_GREEN);
    // }
}

void if_block_move_left(void)
{
    _if_block_move_left(screen_y);
    _if_block_move_left(screen_y+1);    
}

void _if_block_move_right(uint8_t screen_y)
{
    if((map[screen_x+2][screen_y]==BLOCK)&&map[screen_x+3][screen_y]<=SHIELD)
    {
        // map[screen_x+3][screen_y]=BLOCK;
        // _XL_DRAW(screen_x+3,screen_y,BLOCK_TILE,_XL_GREEN);
        build_element(BLOCK,_XL_GREEN,screen_x+3,screen_y);
    }
    
    // if((map[screen_x+2][screen_y+1]==BLOCK)&&map[screen_x+3][screen_y+1]<=SHIELD)
    // {
        // map[screen_x+3][screen_y+1]=BLOCK;
        // _XL_DRAW(screen_x+3,screen_y+1,BLOCK_TILE,_XL_GREEN);
    // }
}


void if_block_move_right(void)
{
    _if_block_move_right(screen_y);
    _if_block_move_right(screen_y+1);
}


// void init_mini_shuriken(void)
// {
    // uint8_t i;

    // for(i=0;i<level_mini_shurikens;++i)
    // {
        // mini_shuriken_x[i] = (1+i)*(XSize/(MINI_SHURIKEN_NUMBER+1));
        // mini_shuriken_y[i] = 1;
        // _XL_DRAW(mini_shuriken_x[i],mini_shuriken_y[i],MINI_SHURIKEN_TILE,_XL_RED);			

    // }
// }


void handle_mini_shuriken(void)
{	
    uint8_t i;

    for(i=0;i<level_mini_shurikens;++i)
    {
        
        // _XL_DELETE(mini_shuriken_x[i],mini_shuriken_y[i]);
        // map[mini_shuriken_x[i]][mini_shuriken_y[i]]=EMPTY;
        delete_element(mini_shuriken_x[i],mini_shuriken_y[i]);
        
        ++(mini_shuriken_y[i]);

        if(!map[mini_shuriken_x[i]][mini_shuriken_y[i]] && mini_shuriken_y[i]<YSize-2)
        {
            // _XL_DRAW(mini_shuriken_x[i],mini_shuriken_y[i],MINI_SHURIKEN_TILE,_XL_RED);
            // map[mini_shuriken_x[i]][mini_shuriken_y[i]]=MINI_SHURIKEN;
            build_element(MINI_SHURIKEN, _XL_RED, mini_shuriken_x[i],mini_shuriken_y[i]);
        }
        else
        {	
            if(map[mini_shuriken_x[i]][mini_shuriken_y[i]]<=SHIELD)
            {
                // _XL_DELETE(mini_shuriken_x[i],mini_shuriken_y[i]);
                // map[mini_shuriken_x[i]][mini_shuriken_y[i]]=EMPTY;
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
    
    for(i=0;i<XSize;++i)
    {
        build_element(WALL,_XL_YELLOW,i,1);
        build_element(WALL,_XL_YELLOW,i,YSize-1);
    }
    for(i=1;i<YSize-1;++i)
    {
        build_element(WALL,_XL_YELLOW,0,i);
        build_element(WALL,_XL_YELLOW,XSize-1,i);
    }
    for(i=1;i<XSize-1;++i)
    {
        for(j=2;j<YSize-1;++j)
        {
            map[i][j]=EMPTY;
        }
    }

}


void init_score_display(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
  
    update_score_display();
    _XL_PRINTD(XSize-5,0,5,hiscore);
    
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(XSize-7,0,"HI");
}


void build_objects(void)
{
    uint8_t index = objects_index[level];
    uint8_t no_of_objects = objects_map[index];
    uint8_t i;
    uint8_t j;
    uint8_t k;
    uint8_t x;
    uint8_t y;
    uint8_t x_size;
    uint8_t y_size;
    uint8_t type;
    uint8_t color;  
    
    for(i=0;i<no_of_objects;++i)
    {
        x = objects_map[++index];
        y = objects_map[++index];
        x_size = objects_map[++index];
        y_size = objects_map[++index];
        type = objects_map[++index];
        color = objects_map[++index];
        for(j=x;j<x+x_size;++j)
        {
            for(k=y;k<y+y_size;++k)
            {
                build_element(type,color,j,k);
            }
        }
    }
}


void build_shurikens(void)
{
    uint8_t index = shurikens_index[level];
    uint8_t i;

    level_horizontal_shurikens = shurikens_map[index];
    
    
    for(i=0;i<level_horizontal_shurikens;++i)
    {
        horizontal_shuriken_x[i]=shurikens_map[++index];
        horizontal_shuriken_y[i]=shurikens_map[++index];
        horizontal_shuriken_direction[i]=SHURIKEN_LEFT;
        horizontal_shuriken_transition[i]=0;
        build_element(SHURIKEN,shurikens_map[++index],horizontal_shuriken_x[i],horizontal_shuriken_y[i]);
    }


}

void init_level(void)
{
    // uint8_t i;
    
    // uint8_t x;
    // uint8_t y;
        
    init_map();    
        
    init_score_display();
    
    // TODO: BOGUS
    // level_mini_shurikens = 0;
    // level_horizontal_shurikens = 1;
    // level_vertical_shurikens = 1;
    //
    
    build_objects();
    
    build_shurikens();

	chasing_transition = 0;
    chasing_direction = SHURIKEN_RIGHT;
	chasing_x = 12;
	chasing_y = 4;
	build_element(SHURIKEN,_XL_YELLOW, chasing_x, chasing_y);

    // level_horizontal_shurikens = 4;
    
    // init_horizontal_shurikens();
    // init_vertical_shurikens();
    // init_mini_shuriken();
    
    
    // for(i=0;i<MAX_NUMBER_OF_HORIZONTAL_SHURIKENS;++i)
    // {
        // horizontal_shuriken_y[i]=2+i*3;
        // horizontal_shuriken_transition[i]=0;
        // horizontal_shuriken_x[i]=2;
        // horizontal_shuriken_direction[i]=SHURIKEN_RIGHT;
    // }

    // for(i=0;i<MAX_NUMBER_OF_VERTICAL_SHURIKENS;++i)
    // {
        // vertical_shuriken_x[i]=2+i*3;
        // vertical_shuriken_transition[i]=0;
        // vertical_shuriken_y[i]=3;
        // vertical_shuriken_direction[i]=SHURIKEN_DOWN;
    // }
    
    // level_horizontal_shurikens = MAX_NUMBER_OF_HORIZONTAL_SHURIKENS;
    // level_vertical_shurikens = MAX_NUMBER_OF_VERTICAL_SHURIKENS;
    
    // for(i=0;i<20;++i)
    // {
        
        // x = _XL_RAND()%(XSize-2)+1; 
        // y = _XL_RAND()%(YSize-3)+2;
        // _XL_DRAW(x,y,WALL_TILE,_XL_YELLOW);
        // map[x][y] = WALL;

    // }
    
    // for(i=0;i<50;++i)
    // {
        // x = _XL_RAND()%(XSize-2)+1; 
        // y = _XL_RAND()%(YSize-3)+2;
        // _XL_DRAW(x,y,SHIELD_TILE,_XL_WHITE);
        // map[x][y] = SHIELD;
    // }
    
    // for(i=0;i<50;++i)
    // {
        // x = _XL_RAND()%(XSize-2)+1; 
        // y = _XL_RAND()%(YSize-3)+2;
        // _XL_DRAW(x,y,BLOCK_TILE,_XL_GREEN);
        // map[x][y] = BLOCK;
    // }
    
    // for(i=0;i<50;++i)
    // {
        // x = _XL_RAND()%(XSize-2)+1; 
        // y = _XL_RAND()%(YSize-3)+2;
        // _XL_DRAW(x,y,RING_TILE,_XL_WHITE);
        // map[x][y] = RING;
    // } 
    
    // for(i=0;i<50;++i)
    // {
        // x = _XL_RAND()%(XSize-2)+1; 
        // y = _XL_RAND()%(YSize-3)+2;
        // _XL_DRAW(x,y,DIAMOND_TILE,_XL_GREEN);
        // map[x][y] = DIAMOND;
    // }     
    
}


void display_horizontal_transition_shuriken(uint8_t x, uint8_t y)
{
    _XL_DRAW(x-1,y,SHURIKEN_TILE_LEFT, _XL_CYAN);
    _XL_DRAW(x,y,SHURIKEN_TILE_RIGHT, _XL_CYAN);
}

void if_shield_destroy_it(uint8_t x, uint8_t y)
{
    if(map[x][y]==SHIELD)
    {
        _XL_TICK_SOUND();
        _XL_DELETE(x,y);
        map[x][y]=EMPTY;  
    }
}


void handle_chasing_shuriken(void)
{    
    if(chasing_direction==SHURIKEN_LEFT)
    {
        _XL_PRINT(0,0,"LEFT");
    }
    else
    {
        _XL_PRINT(0,0,"RIGHT");
    }
    if(chasing_direction==SHURIKEN_LEFT)
    {
            if(!chasing_transition) // transition not performed, yet
            {
                if(screen_x>chasing_x)
                {
                    chasing_direction=SHURIKEN_RIGHT;
                    _XL_PRINT(8,0,"CHANGE TO RIGHT");
                }
                else
                {
                    if(!map[chasing_x-1][chasing_y])
                    {
                        // Do left transition
                        display_horizontal_transition_shuriken(chasing_x,chasing_y);
                        map[chasing_x-1][chasing_y]=SHURIKEN;
                        ++chasing_transition;
                    }
                    else
                    {				
                        if_shield_destroy_it(chasing_x-1,chasing_y);
                    }
                }
            }
            else // transition already performed
            {
                chasing_transition=0;
                delete_element(chasing_x,chasing_y);
                // delete_element(chasing_x+1,chasing_y);
                --chasing_x;
                _XL_DRAW(chasing_x,chasing_y,SHURIKEN_TILE,_XL_YELLOW);
            }
    }
    else // direction is RIGHT
    {
        if(!chasing_transition) // transition not performed, yet
        {
            if(screen_x<=chasing_x)
            {
                chasing_direction=SHURIKEN_LEFT;
                _XL_PRINT(8,0,"CHANGE TO LEFT");

            }
            else
            {
                if(!map[chasing_x+1][chasing_y])
                {
                    // Do right transition
                    display_horizontal_transition_shuriken(chasing_x+1,chasing_y);
                    map[chasing_x+1][chasing_y]=SHURIKEN;
                    ++chasing_transition;
                }
                else
                {				
                    if_shield_destroy_it(chasing_x+1,chasing_y);
                }
            }
        }
        else // transition already performed
        {
            chasing_transition=0;
            // map[x][y]=EMPTY;
            // _XL_DELETE(x,y);
            delete_element(chasing_x,chasing_y);
            // delete_element(chasing_x-1,chasing_y);
            ++chasing_x;
            _XL_DRAW(chasing_x,chasing_y,SHURIKEN_TILE,_XL_YELLOW);
        }
    }
}


void handle_horizontal_shuriken(register uint8_t index)
{
    register uint8_t x = horizontal_shuriken_x[index];
    register uint8_t y = horizontal_shuriken_y[index];
    
    if(horizontal_shuriken_direction[index]==SHURIKEN_LEFT)
    {
        
        if(!horizontal_shuriken_transition[index]) // transition not performed, yet
        {
            if(!map[x-1][y])
            {
                // Do left transition
                display_horizontal_transition_shuriken(x,y);
                map[x-1][y]=SHURIKEN;
                ++horizontal_shuriken_transition[index];
            }
            else
            {
                horizontal_shuriken_direction[index]=SHURIKEN_RIGHT;
				
                if_shield_destroy_it(x-1,y);
            }
        }
        else // transition already performed
        {
            horizontal_shuriken_transition[index]=0;
            // map[x][y]=EMPTY;
            // _XL_DELETE(x,y);
            delete_element(x,y);
            --horizontal_shuriken_x[index];
            _XL_DRAW(horizontal_shuriken_x[index],y,SHURIKEN_TILE,_XL_CYAN);
        }
    }
    else // direction is RIGHT
    {
        if(!horizontal_shuriken_transition[index]) // transition not performed, yet
        {
            if(!map[x+1][y])
            {
                // Do right transition
                display_horizontal_transition_shuriken(x+1,y);
                map[x+1][y]=SHURIKEN;
                ++horizontal_shuriken_transition[index];
            }
            else
            {
                horizontal_shuriken_direction[index]=SHURIKEN_LEFT;
				
                if_shield_destroy_it(x+1,y);

            }
        }
        else // transition already performed
        {
            horizontal_shuriken_transition[index]=0;
            // map[x][y]=EMPTY;
            // _XL_DELETE(x,y);
            delete_element(x,y);
            ++horizontal_shuriken_x[index];
            _XL_DRAW(horizontal_shuriken_x[index],y,SHURIKEN_TILE,_XL_CYAN);
        }
    }
}


void handle_horizontal_shurikens(void)
{
    uint8_t i;
    
    for(i=0;i<level_horizontal_shurikens;++i)
    {
        handle_horizontal_shuriken(i);
    }
}


void display_vertical_transition_shuriken(uint8_t x, uint8_t y)
{
    _XL_DRAW(x,y-1,SHURIKEN_TILE_UP,_XL_CYAN);
    _XL_DRAW(x,y,SHURIKEN_TILE_DOWN,_XL_CYAN);
}


void handle_vertical_shuriken(register uint8_t index)
{
    register uint8_t x = vertical_shuriken_x[index];
    register uint8_t y = vertical_shuriken_y[index];
    
    if(vertical_shuriken_direction[index]==SHURIKEN_UP)
    {
        
        if(!vertical_shuriken_transition[index]) // transition not performed, yet
        {
            if(!map[x][y-1])
            {
                // Do up transition
                display_vertical_transition_shuriken(x,y);
                map[x][y-1]=SHURIKEN;
                ++vertical_shuriken_transition[index];
            }
            else
            {
                vertical_shuriken_direction[index]=SHURIKEN_DOWN;
                if_shield_destroy_it(x,y-1);
            }
        }
        else // transition already performed
        {
            vertical_shuriken_transition[index]=0;
            // map[x][y]=EMPTY;
            // _XL_DELETE(x,y);
            delete_element(x,y);
            --vertical_shuriken_y[index];
            _XL_DRAW(x,vertical_shuriken_y[index],SHURIKEN_TILE,_XL_CYAN);
        }
    }
    else // direction is DOWN
    {
        if(!vertical_shuriken_transition[index]) // transition not performed, yet
        {
            if(!map[x][vertical_shuriken_y[index]+1])
            {
                // Do right transition
                display_vertical_transition_shuriken(x,y+1);
                map[x][y+1]=SHURIKEN;
                ++vertical_shuriken_transition[index];
            }
            else
            {
                vertical_shuriken_direction[index]=SHURIKEN_UP;
                if_shield_destroy_it(x,y+1);
            }
        }
        else // transition already performed
        {
            vertical_shuriken_transition[index]=0;
            // map[x][y]=EMPTY;
            // _XL_DELETE(x,y);
            delete_element(x,y);    
            ++vertical_shuriken_y[index];
            _XL_DRAW(x,vertical_shuriken_y[index],SHURIKEN_TILE,_XL_CYAN);
        }
    }
}


void handle_vertical_shurikens(void)
{
    uint8_t i;
    
    for(i=0;i<level_vertical_shurikens;++i)
    {
        handle_vertical_shuriken(i);
    }
}


void handle_player(void)
{
    uint8_t input;
    
    input = _XL_INPUT();
    
    if(_XL_UP(input) && (!(player_y&1) || allowed_up()))
    {
        if(player_y>MIN_PLAYER_Y)
        {
            if(player_y&1)
            {
                delete_player_down();
                if_block_move_up();
            }
            --player_y;

            update_player();
        }
    }
    else if(_XL_DOWN(input) && ((player_y&1) || allowed_down()))
    {	
        if(player_y<MAX_PLAYER_Y)
        {
            if(!(player_y&1))
            {
                delete_player_up();
                if_block_move_down();
            }
            ++player_y;

            update_player();
        }
    }
    else if(_XL_LEFT(input) && ((player_x&1) || allowed_left()))
    {
        if(player_x>MIN_PLAYER_X)
        {
            if(!(player_x&1))
            {
                delete_player_right();
                if_block_move_left();
            }
            --player_x;

            update_player();
        }
    }
    else if(_XL_RIGHT(input) && (!(player_x&1) || allowed_right()))
    {	
        if(player_x<MAX_PLAYER_X)
        {   
            if(player_x&1)
            {
                delete_player_left();
                if_block_move_right();
            }
            ++player_x;

            update_player();
        }
    }
}


void init_player(void)
{
    alive = 1;
	player_x = XSize;
	player_y = YSize;
    
    force = 0;
    // move_threshold = SHIELD; // TODO: Necessary
    
    player_color = _XL_WHITE;
    
}

// const uint8_t screen_type_color[7+1] =
// {
    // 0, // unused
    // _XL_WHITE,
    // _XL_GREEN,
    // _XL_WHITE,
    // _XL_GREEN,
    // _XL_CYAN,
    // _XL_RED,
    // _XL_YELLOW,
// }

// void init_screen_tiles(void)
// {
    // screen_tile[SHIELD] = SHIELD_TILE;
    // screen_tile[DIAMOND] = DIAMOND_TILE;
    // screen_tile[RING] = RING_TILE;
    // screen_tile[BLOCK] = BLOCK_TILE;
    // screen_tile[SHURIKEN] = SHURIKEN_TILE;
    // screen_tile[MINI_SHURIKEN] = MINI_SHURIKEN_TILE;
    // screen_tile[WALL] = WALL_TILE;
// }


void title(void)
{
    _XL_CLEAR_SCREEN();
    
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    
    _XL_PRINTD(XSize/2-2,1,5,hiscore);    
    
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    
    _XL_PRINT(XSize/2-7,4, "S H U R I K E N");
    
    _XL_SLEEP(1);
    
    _XL_WAIT_FOR_INPUT();
    
    _XL_CLEAR_SCREEN();
}


void init_variables(void)
{
    score = 0;
    lives = INITIAL_LIVES;
    remaining_diamonds = 8;
    level = 0;
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
        
        while(lives && (level<FINAL_LEVEL+1))
        {            
            init_level();
            
            init_player();

            update_player();
            
            _XL_SLEEP(1);   
            _XL_WAIT_FOR_INPUT();
            
            while(remaining_diamonds && alive)
            {

                handle_player();
                
                handle_horizontal_shurikens();
                handle_vertical_shurikens();
                handle_mini_shuriken();
				handle_chasing_shuriken();
                
                handle_collisions();
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR/2);
            }
            if(alive)
            {
                ++level;
                _XL_SET_TEXT_COLOR(_XL_YELLOW);
                _XL_PRINT(XSize/2-5,YSize/2,"COMPLETED");
                _XL_WAIT_FOR_INPUT();
            }
            else
            {
                --lives;
                _XL_SET_TEXT_COLOR(_XL_RED);
                _XL_PRINT(XSize/2-5,YSize/2,"YOU LOST");
                _XL_WAIT_FOR_INPUT();
            }
        };
        
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(XSize/2-5,YSize/2,"GAME OVER");
        
        _XL_SLEEP(1);
        _XL_WAIT_FOR_INPUT();
    }
    
    return EXIT_SUCCESS;
}

