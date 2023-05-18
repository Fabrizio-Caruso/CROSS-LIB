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



#define EMPTY 0
#define SHIELD 1
#define RING 2
#define DIAMOND 3
#define BLOCK 4
#define DEADLY 5

#define WALL  6


#define SHURIKEN_RIGHT 0
#define SHURIKEN_LEFT 1
#define SHURIKEN_UP 2
#define SHURIKEN_DOWN 3

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

#define MIN_PLAYER_Y 3
#define MAX_PLAYER_Y (2*YSize-6)
#define MIN_PLAYER_X 2
#define MAX_PLAYER_X (2*XSize-5)

#define MOVE_FORCE 8

#define MINI_SHURIKEN_NUMBER 2

uint8_t player_x;
uint8_t player_y;

uint8_t screen_x;
uint8_t screen_y;

uint8_t alive;

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

uint8_t horizontal_shurikens_on_current_level;
uint8_t vertical_shurikens_on_current_level;

uint8_t force;
uint8_t move_threshold;

uint8_t player_cell[4];

uint16_t score;
uint16_t hiscore;

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
        else if(player_cell[i]==DEADLY)
        {
            alive = 0;
        }
    }
    
}


void update_player(void)
{
	uint8_t tile_group = (player_x&1)+2*(player_y&1);
	
	update_screen_xy();
	
    _XL_DRAW(screen_x,screen_y,player_tile[tile_group][2],_XL_WHITE);
    _XL_DRAW(screen_x+1,screen_y,player_tile[tile_group][3],_XL_WHITE);  
    _XL_DRAW(screen_x,screen_y+1,player_tile[tile_group][0],_XL_WHITE);
    _XL_DRAW(screen_x+1,screen_y+1,player_tile[tile_group][1],_XL_WHITE);  
    
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
    if(((cell1==BLOCK)||(cell2==BLOCK)) && force<MOVE_FORCE)
    {
        ++force;
        move_threshold=SHIELD;
    }
    else
    {
        force=0;
        move_threshold=BLOCK;
    }  
}


uint8_t allowed(uint8_t cell1, uint8_t cell2)
{
    update_force(cell1,cell2);

    return (cell1<=move_threshold) && (cell2<=move_threshold);    
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


void if_block_move_down(void)
{
    if((map[screen_x][screen_y+2]==BLOCK)&&!map[screen_x][screen_y+3])
    {
        map[screen_x][screen_y+3]=BLOCK;
        _XL_DRAW(screen_x,screen_y+3,BLOCK_TILE,_XL_GREEN);
    }
    
    if((map[screen_x+1][screen_y+2]==BLOCK)&&!map[screen_x+1][screen_y+3])
    {
        map[screen_x+1][screen_y+3]=BLOCK;
        _XL_DRAW(screen_x+1,screen_y+3,BLOCK_TILE,_XL_GREEN);
    }
}


void if_block_move_up(void)
{
    if((map[screen_x][screen_y-1]==BLOCK)&&!map[screen_x][screen_y-2])
    {
        map[screen_x][screen_y-2]=BLOCK;
        _XL_DRAW(screen_x,screen_y-2,BLOCK_TILE,_XL_GREEN);
    }
    
    if((map[screen_x+1][screen_y-1]==BLOCK)&&!map[screen_x+1][screen_y-2])
    {
        map[screen_x+1][screen_y-2]=BLOCK;
        _XL_DRAW(screen_x+1,screen_y-2,BLOCK_TILE,_XL_GREEN);
    }
}


void if_block_move_left(void)
{
    if((map[screen_x-1][screen_y]==BLOCK)&&!map[screen_x-2][screen_y])
    {
        map[screen_x-2][screen_y]=BLOCK;
        _XL_DRAW(screen_x-2,screen_y,BLOCK_TILE,_XL_GREEN);
    }
    
    if((map[screen_x-1][screen_y+1]==BLOCK)&&!map[screen_x-2][screen_y+1])
    {
        map[screen_x-2][screen_y+1]=BLOCK;
        _XL_DRAW(screen_x-2,screen_y+1,BLOCK_TILE,_XL_GREEN);
    }
}


void if_block_move_right(void)
{
    if((map[screen_x+2][screen_y]==BLOCK)&&!map[screen_x+3][screen_y])
    {
        map[screen_x+3][screen_y]=BLOCK;
        _XL_DRAW(screen_x+3,screen_y,BLOCK_TILE,_XL_GREEN);
    }
    
    if((map[screen_x+2][screen_y+1]==BLOCK)&&!map[screen_x+3][screen_y+1])
    {
        map[screen_x+3][screen_y+1]=BLOCK;
        _XL_DRAW(screen_x+3,screen_y+1,BLOCK_TILE,_XL_GREEN);
    }
}


void init_mini_shuriken(void)
{
    uint8_t i;

    for(i=0;i<MINI_SHURIKEN_NUMBER;++i)
    {
        mini_shuriken_x[i] = (1+i)*(XSize/(MINI_SHURIKEN_NUMBER+1));
        mini_shuriken_y[i] = 1;
        _XL_DRAW(mini_shuriken_x[i],mini_shuriken_y[i],MINI_SHURIKEN_TILE,_XL_RED);			

    }
}


void handle_mini_shuriken(void)
{	
    uint8_t i;

    for(i=0;i<MINI_SHURIKEN_NUMBER;++i)
    {
        
        _XL_DELETE(mini_shuriken_x[i],mini_shuriken_y[i]);
        map[mini_shuriken_x[i]][mini_shuriken_y[i]]=EMPTY;
        
        ++(mini_shuriken_y[i]);

        if(!map[mini_shuriken_x[i]][mini_shuriken_y[i]] && mini_shuriken_y[i]<YSize-2)
        {
            _XL_DRAW(mini_shuriken_x[i],mini_shuriken_y[i],MINI_SHURIKEN_TILE,_XL_RED);
            map[mini_shuriken_x[i]][mini_shuriken_y[i]]=DEADLY;
        }
        else
        {	
            if(map[mini_shuriken_x[i]][mini_shuriken_y[i]]<=SHIELD)
            {
                _XL_DELETE(mini_shuriken_x[i],mini_shuriken_y[i]);
                map[mini_shuriken_x[i]][mini_shuriken_y[i]]=EMPTY;
            }
            mini_shuriken_y[i] = 2;							
        }
        
    }
}


void init_map(void)
{
    uint8_t i;
    uint8_t j;
    
    for(i=0;i<XSize;++i)
    {
        map[i][1]=WALL;       
        map[i][YSize-1]=WALL;
        _XL_DRAW(i,1,WALL_TILE,_XL_YELLOW);
        _XL_DRAW(i,YSize-1,WALL_TILE,_XL_YELLOW);
    }
    for(i=1;i<YSize-1;++i)
    {
        map[0][i]=WALL;
        map[XSize-1][i]=WALL;
        _XL_DRAW(0,i,WALL_TILE,_XL_YELLOW);
        _XL_DRAW(XSize-1,i,WALL_TILE,_XL_YELLOW);
    }
    for(i=1;i<XSize-1;++i)
    {
        for(j=2;j<YSize-1;++j)
        {
            map[i][j]=EMPTY;
        }
    }

}


void init_level(void)
{
    uint8_t i;
    
    uint8_t x;
    uint8_t y;
    
    _XL_SET_TEXT_COLOR(_XL_WHITE);
  
    update_score_display();
    _XL_PRINTD(XSize-5,0,5,hiscore);
    
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(XSize-7,0,"HI");
    
    for(i=0;i<MAX_NUMBER_OF_HORIZONTAL_SHURIKENS;++i)
    {
        horizontal_shuriken_y[i]=2+i*3;
        horizontal_shuriken_transition[i]=0;
        horizontal_shuriken_x[i]=2;
        horizontal_shuriken_direction[i]=SHURIKEN_RIGHT;
    }

    for(i=0;i<MAX_NUMBER_OF_VERTICAL_SHURIKENS;++i)
    {
        vertical_shuriken_x[i]=2+i*3;
        vertical_shuriken_transition[i]=0;
        vertical_shuriken_y[i]=3;
        vertical_shuriken_direction[i]=SHURIKEN_DOWN;
    }
    
    horizontal_shurikens_on_current_level = MAX_NUMBER_OF_HORIZONTAL_SHURIKENS;
    vertical_shurikens_on_current_level = MAX_NUMBER_OF_VERTICAL_SHURIKENS;
    
    // horizontal_shurikens_on_current_level = 2;
    // vertical_shurikens_on_current_level = 2;
    
    for(i=0;i<20;++i)
    {
        // _XL_DRAW(_XL_RAND()%(XSize-2)+1,_XL_RAND()%(YSize-3)+2,DIAMOND_TILE,_XL_GREEN);
        
        x = _XL_RAND()%(XSize-2)+1; 
        y = _XL_RAND()%(YSize-3)+2;
        _XL_DRAW(x,y,WALL_TILE,_XL_YELLOW);
        map[x][y] = WALL;
        
        // _XL_DRAW(_XL_RAND()%(XSize-2)+1,_XL_RAND()%(YSize-3)+2,MINI_SHURIKEN_TILE,_XL_RED);
        // _XL_DRAW(_XL_RAND()%(XSize-2)+1,_XL_RAND()%(YSize-3)+2,RING_TILE,_XL_WHITE);
    }
    
    for(i=0;i<50;++i)
    {
        x = _XL_RAND()%(XSize-2)+1; 
        y = _XL_RAND()%(YSize-3)+2;
        _XL_DRAW(x,y,SHIELD_TILE,_XL_WHITE);
        map[x][y] = SHIELD;
    }
    
    for(i=0;i<50;++i)
    {
        x = _XL_RAND()%(XSize-2)+1; 
        y = _XL_RAND()%(YSize-3)+2;
        _XL_DRAW(x,y,BLOCK_TILE,_XL_GREEN);
        map[x][y] = BLOCK;
    }
    
    for(i=0;i<50;++i)
    {
        x = _XL_RAND()%(XSize-2)+1; 
        y = _XL_RAND()%(YSize-3)+2;
        _XL_DRAW(x,y,RING_TILE,_XL_WHITE);
        map[x][y] = RING;
    } 
    
    for(i=0;i<50;++i)
    {
        x = _XL_RAND()%(XSize-2)+1; 
        y = _XL_RAND()%(YSize-3)+2;
        _XL_DRAW(x,y,DIAMOND_TILE,_XL_GREEN);
        map[x][y] = DIAMOND;
    }     
    
    force = 0;
    move_threshold = SHIELD; // TODO: Necessary
    
    init_mini_shuriken();
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
                map[x-1][y]=DEADLY;
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
            map[x][y]=EMPTY;
            _XL_DELETE(x,y);
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
                map[x+1][y]=DEADLY;
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
            map[x][y]=EMPTY;
            _XL_DELETE(x,y);
            ++horizontal_shuriken_x[index];
            _XL_DRAW(horizontal_shuriken_x[index],y,SHURIKEN_TILE,_XL_CYAN);
        }
    }
}


void handle_horizontal_shurikens(void)
{
    uint8_t i;
    
    for(i=0;i<horizontal_shurikens_on_current_level;++i)
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
                map[x][y-1]=DEADLY;
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
            map[x][y]=EMPTY;
            _XL_DELETE(x,y);
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
                map[x][y+1]=DEADLY;
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
            map[x][y]=EMPTY;
            _XL_DELETE(x,y);
            ++vertical_shuriken_y[index];
            _XL_DRAW(x,vertical_shuriken_y[index],SHURIKEN_TILE,_XL_CYAN);
        }
    }
}


void handle_vertical_shurikens(void)
{
    uint8_t i;
    
    for(i=0;i<vertical_shurikens_on_current_level;++i)
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
}


int main(void)
{        
    _XL_INIT_GRAPHICS();

    _XL_INIT_INPUT();
    
    _XL_INIT_SOUND();

    hiscore = 0;
    score = 0;
    
    while(1)
    {
        _XL_CLEAR_SCREEN();
        
        _XL_SET_TEXT_COLOR(_XL_RED);
        
        _XL_PRINT(XSize/2-7,4, "S H U R I K E N");
        
        _XL_SLEEP(1);
        
        _XL_WAIT_FOR_INPUT();
        
        _XL_CLEAR_SCREEN();

        init_map();
        
        init_level();
        
        init_player();

        update_player();
        
        while(alive)
        {
            handle_player();
            
            handle_horizontal_shurikens();
            handle_vertical_shurikens();
            handle_mini_shuriken();
            
            handle_collisions();
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR/2);
        };
        
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(XSize/2-5,YSize/2,"GAME OVER");
        
        _XL_SLEEP(1);
        _XL_WAIT_FOR_INPUT();
    }
    
    return EXIT_SUCCESS;
}

