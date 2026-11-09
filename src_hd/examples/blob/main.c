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

#define INITIAL_LEVEL 1
#define FINAL_LEVEL 8

uint16_t hiscore;
uint16_t score;
uint8_t level;
uint8_t alive;
uint8_t lives;
uint8_t input;
uint8_t level_completed;
uint8_t counter;

uint8_t direction;

uint8_t x;
uint8_t y;


uint8_t enemy_x;
uint8_t enemy_y;

#define RIGHT_TAIL              _TILE_0
#define LEFT_TAIL               _TILE_1
#define RIGHT_PART              _TILE_2
#define LEFT_PART               _TILE_3
#define RIGHT_PLAYER_HEAD       _TILE_4
#define LEFT_PLAYER_HEAD        _TILE_5
#define RIGHT_BOSS_HEAD         _TILE_6
#define LEFT_BOSS_HEAD          _TILE_7
#define RIGHT_ENEMY_HEAD        _TILE_8
#define LEFT_ENEMY_HEAD         _TILE_9
#define RIGHT_PLAYER_HEAD_PART  _TILE_10
#define LEFT_PLAYER_HEAD_PART   _TILE_11
#define RIGHT_BOSS_HEAD_PART    _TILE_12
#define LEFT_BOSS_HEAD_PART     _TILE_13
#define RIGHT_ENEMY_HEAD_PART   _TILE_14
#define LEFT_ENEMY_HEAD_PART    _TILE_15
#define TERRAIN                 _TILE_16
#define JUMP                    _TILE_17
#define FIRE                    _TILE_18
#define STOP                    _TILE_19
#define EXIT                    _TILE_20
#define LEFT_MINE               _TILE_21
#define RIGHT_MINE              _TILE_22
#define UP_MINE                 _TILE_23
#define DOWN_MINE               _TILE_24
#define CENTER_MINE             _TILE_25
#define PILL                    _TILE_26

#define MAX_X                   (XSize-2)
#define MIN_X                   2

struct BlobStruct
{
    uint8_t x;
    uint8_t y;
    uint8_t direction;
    uint8_t left_head;
    uint8_t left_head_part;
    uint8_t right_head;
    uint8_t right_head_part;   
};

typedef struct BlobStruct Blob;


void PRINT_CENTERED_ON_ROW(uint8_t row, char *Text)
{
	_XL_PRINT(((uint8_t) (XSize - strlen(Text))>>1), row, Text);	
}

#define PRINT_CENTERED(Text) \
	PRINT_CENTERED_ON_ROW((YSize>>1), Text)


void display_player_right(void)
{
    uint8_t rx;
    if((x&1))
    {
        rx = (x+1)>>1;
        _XL_DELETE(rx-1,y); 
        _XL_DRAW(rx,y,RIGHT_PART,_XL_GREEN);
        _XL_DRAW(rx+1,y,RIGHT_PLAYER_HEAD_PART,_XL_GREEN);
    }
    else
    {
        rx = x>>1;
        _XL_DELETE(rx-1,y);
        _XL_DRAW(rx,y,RIGHT_TAIL,_XL_GREEN);
        _XL_DRAW(rx+1,y,RIGHT_PLAYER_HEAD,_XL_GREEN);
    }
}

void display_player_left(void)
{
    uint8_t rx;
    if((x&1))
    {
        rx = (x-1)>>1;
        _XL_DELETE(rx+2,y);
        _XL_DRAW(rx,y,LEFT_PLAYER_HEAD_PART,_XL_GREEN);
        _XL_DRAW(rx+1,y,LEFT_PART,_XL_GREEN);
    }
    else
    {
        rx = x>>1;
        _XL_DELETE(rx+2,y);
        _XL_DRAW(rx,y,LEFT_PLAYER_HEAD,_XL_GREEN);
        _XL_DRAW(rx+1,y,LEFT_TAIL,_XL_GREEN);
    }
}


void display_enemy_right(void)
{
    uint8_t rx;
    if((x&1))
    {
        rx = (x+1)>>1;
        _XL_DELETE(rx-1,y); 
        _XL_DRAW(rx,y,RIGHT_PART,_XL_GREEN);
        _XL_DRAW(rx+1,y,RIGHT_ENEMY_HEAD_PART,_XL_GREEN);
    }
    else
    {
        rx = x>>1;
        _XL_DELETE(rx-1,y);
        _XL_DRAW(rx,y,RIGHT_TAIL,_XL_GREEN);
        _XL_DRAW(rx+1,y,RIGHT_ENEMY_HEAD,_XL_GREEN);
    }
}

void display_enemy_left(void)
{
    uint8_t rx;
    if((x&1))
    {
        rx = (x-1)>>1;
        _XL_DELETE(rx+2,y);
        _XL_DRAW(rx,y,LEFT_ENEMY_HEAD_PART,_XL_GREEN);
        _XL_DRAW(rx+1,y,LEFT_PART,_XL_GREEN);
    }
    else
    {
        rx = x>>1;
        _XL_DELETE(rx+2,y);
        _XL_DRAW(rx,y,LEFT_ENEMY_HEAD,_XL_GREEN);
        _XL_DRAW(rx+1,y,LEFT_TAIL,_XL_GREEN);
    }
}


void display_boss_right(void)
{
    uint8_t rx;
    if((x&1))
    {
        rx = (x+1)>>1;
        _XL_DELETE(rx-1,y); 
        _XL_DRAW(rx,y,RIGHT_PART,_XL_GREEN);
        _XL_DRAW(rx+1,y,RIGHT_BOSS_HEAD_PART,_XL_GREEN);
    }
    else
    {
        rx = x>>1;
        _XL_DELETE(rx-1,y);
        _XL_DRAW(rx,y,RIGHT_TAIL,_XL_GREEN);
        _XL_DRAW(rx+1,y,RIGHT_BOSS_HEAD,_XL_GREEN);
    }
}

void display_boss_left(void)
{
    uint8_t rx;
    if((x&1))
    {
        rx = (x-1)>>1;
        _XL_DELETE(rx+2,y);
        _XL_DRAW(rx,y,LEFT_BOSS_HEAD_PART,_XL_GREEN);
        _XL_DRAW(rx+1,y,LEFT_PART,_XL_GREEN);
    }
    else
    {
        rx = x>>1;
        _XL_DELETE(rx+2,y);
        _XL_DRAW(rx,y,LEFT_BOSS_HEAD,_XL_GREEN);
        _XL_DRAW(rx+1,y,LEFT_TAIL,_XL_GREEN);
    }
}


void display_player(void)
{
    if(direction)
    {
        display_player_right();
    }
    else
    {
        display_player_left();
    }
}



void display_enemy(void)
{
    if(direction)
    {
        display_enemy_right();
    }
    else
    {
        display_enemy_left();
    }
}


void display_boss(void)
{
    if(direction)
    {
        display_boss_right();
    }
    else
    {
        display_boss_left();
    }
}


void draw_level(void)
{
    uint8_t i;
    for(i=MIN_X; i<=MAX_X/2;  ++i)
    {
        _XL_DRAW(i,YSize/2+1,TERRAIN, _XL_WHITE);
    }
}


void handle_player(void)
{
    input = _XL_INPUT();

    if (_XL_RIGHT(input) && x<MAX_X)
    {
        direction = 1;
        ++x;
        display_player();
    } 
    else if (_XL_LEFT(input) && x>MIN_X)
    {
        direction = 0;
        --x;
        display_player();
    } 
}


int main(void)
{        

    _XL_INIT_GRAPHICS();

    _XL_INIT_INPUT();
    
    _XL_INIT_SOUND();
    
    hiscore = 0;

    // MAIN END-LESS LOOP
    while(1)
    {
        score = 0;
        level = INITIAL_LEVEL;
        lives = 3;

        _XL_CLEAR_SCREEN();
            
        _XL_SET_TEXT_COLOR(_XL_RED);
        PRINT_CENTERED_ON_ROW(4, "BLOB");
        _XL_SET_TEXT_COLOR(_XL_CYAN);
        PRINT_CENTERED_ON_ROW(6, "FABRIZIO CARUSO");
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        
        PRINT_CENTERED_ON_ROW(0, "HISCORE");
        _XL_PRINTD(XSize/2-3,1,5,hiscore);
        
        #if !defined(_XL_NO_JOYSTICK)
        PRINT_CENTERED_ON_ROW(YSize-1, "PRESS FIRE");
        #else
        PRINT_CENTERED_ON_ROW(YSize-1, "PRESS A KEY");
        #endif
        _XL_WAIT_FOR_INPUT();
        
        _XL_CLEAR_SCREEN();
        
        // GAME LOOP
        while(lives && (level<FINAL_LEVEL+1))
        {
            x = XSize/4;
            y = YSize/2;
            // (RE-)START LEVEL
            level_completed = 0;
            alive = 1;
            counter = 0;
            direction = 1;
            
            _XL_CLEAR_SCREEN();
            
            _XL_PRINT(0,0,"LEVEL");
            _XL_PRINTD(7,0,1,level);
            
            _XL_PRINT(0,2,"LIVES");
            _XL_PRINTD(7,2,1,lives);       
            
            _XL_SLEEP(1);
            PRINT_CENTERED_ON_ROW(14, "PRESS FIRE");
            _XL_WAIT_FOR_INPUT();
            
            _XL_CLEAR_SCREEN();
            
            _XL_PRINT(0,0, "SCORE");
            _XL_PRINTD(7,0,5,score);
            _XL_PRINTD(XSize-6,YSize-1,5,0);
            display_player();
            draw_level();

            // LEVEL LOOP
            while(!level_completed && alive)
            {
                
                handle_player();
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);

            }
                        
            // ALIVE OR LEVEL_COMPLETED?
            if(alive)
            {
                ++level;
                PRINT_CENTERED("LEVEL COMPLETED");
            }
            else
            {
                --lives;
                PRINT_CENTERED("YOU DIED");
                _XL_SLEEP(1);
            }
        } // (lives && (level<FINAL_LEVEL+1)) 
        if(lives) // Game completed
        {
            // GAME COMPLETED
            _XL_CLEAR_SCREEN();
            PRINT_CENTERED("GAME COMPLETED");
            _XL_SLEEP(1);
            _XL_WAIT_FOR_INPUT();
        }
        _XL_CLEAR_SCREEN();
        PRINT_CENTERED("GAME OVER");
        _XL_WAIT_FOR_INPUT();
        if(score>hiscore) 
        {
            hiscore = score;
        }
    } // while(1) -> restart from level 1

    return EXIT_SUCCESS;
}

