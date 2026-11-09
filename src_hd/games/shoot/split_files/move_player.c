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

#include "settings.h"

#include "move_player.h"

#include "character.h"
#include "skull.h"

#include "definitions.h"

#include "bullet.h"

extern uint8_t bombCount;

extern Character player;

extern uint8_t skullXCountDown;
extern uint8_t skullYCountDown;
extern uint8_t playerFire;
extern Character bullet;
extern uint8_t guns;
extern uint8_t playerDirection;

extern uint8_t player_invincibility;
extern uint8_t playerBlink;    


uint8_t innerWallReached(uint8_t x, uint8_t y)
{
    #if !defined(NO_HORIZONTAL_LEVEL)
    return innerVerticalWallReached(x,y) || innerHorizontalWallReached(x,y);
    #else
    return innerVerticalWallReached(x,y);
    #endif
}


#if defined(PLAYER_DIRECTION_CHANGE)    

    void up_direction(void)
    {
        up_transiction();
        playerDirection = UP;
        SHOW_UP();  
    }

    void down_direction(void)
    {
        playerDirection = DOWN;
        SHOW_DOWN();  
    }

    void left_direction(void)
    {
        playerDirection = LEFT;
        SHOW_LEFT();  
    }

    void right_direction(void)
    {
        playerDirection = RIGHT;
        SHOW_RIGHT();  
    }

    #define _DO_MOVE_UP \
        deletePlayer(&player); \
        --player._y; \
        skullYCountDown = SKULL_COUNT_DOWN; \
        playerFire = 0; \
        up_direction();
    #define _DO_MOVE_DOWN \
        deletePlayer(&player); \
        ++player._y; \
        skullYCountDown = SKULL_COUNT_DOWN; \
        playerFire = 0; \
        down_direction();
    #define _DO_MOVE_LEFT \
        deletePlayer(&player); \
        --player._x; \
        skullXCountDown = SKULL_COUNT_DOWN; \
        playerFire = 0; \
        left_direction();
    #define _DO_MOVE_RIGHT \
        deletePlayer(&player); \
        ++player._x; \
        skullXCountDown = SKULL_COUNT_DOWN; \
        playerFire = 0; \
        right_direction();

    #if defined(__NO_PRINT)
        void MOVE_PLAYER(void) {}
    #else
        void MOVE_PLAYER(void)
        {
            uint8_t input = _XL_INPUT();
            
            
            if(_XL_UP(input))
            {
                if(playerDirection != UP)
                {
                    up_direction();
                }
                else if((player._y>1) && !innerWallReached(player._x,player._y-1))
                {
                    _DO_MOVE_UP
                }
            }
            else if(_XL_DOWN(input))
            {
                if(playerDirection != DOWN)
                {
                    down_direction();
                }
                else if((player._y<YSize-2) && !innerWallReached(player._x,player._y+1))
                {
                    _DO_MOVE_DOWN
                }
            }
            else if(_XL_LEFT(input))
            {
                if(playerDirection != LEFT)
                {
                    left_direction();
                }
                else if((player._x>1) && !innerWallReached(player._x-1,player._y))
                {
                    _DO_MOVE_LEFT
                }
            }
            else if(_XL_RIGHT(input))
            {
                if(playerDirection != RIGHT)
                {
                    right_direction();
                }
                else if((player._x<XSize-2) && !innerWallReached(player._x+1,player._y))
                {
                    _DO_MOVE_RIGHT
                }
            }
            else if(_XL_FIRE(input) && (bombCount<BOMBS_NUMBER || (guns>0 && availableBullet()<BULLETS_NUMBER)))
            {
                playerFire = 1;
            }
        }
    #endif
#else
    extern uint8_t old_x;
    extern uint8_t old_y;
    extern uint8_t moved;
    #if defined(TRANSITION_ANIMATION)
    
        void _set_old(void)
        {
            old_x = player._x;
            old_y = player._y;
            moved = 1;
        }
        
    
        #include "init_images.h"
        
        #define _DO_MOVE_UP \
            _set_old(); \
            _XL_DRAW(old_x, old_y-1, _PLAYER_UP_TOP_TILE, _XL_CYAN); \
            _XL_DRAW(old_x, old_y, _PLAYER_BOTTOM_TILE, _XL_CYAN); \
            --player._y; \
            SHOW_UP(); \
            skullYCountDown = SKULL_COUNT_DOWN; \
            playerDirection = UP; \
            playerFire = 0;

        #define _DO_MOVE_DOWN \
            _set_old(); \
            _XL_DRAW(old_x, old_y, _PLAYER_DOWN_TOP_TILE, _XL_CYAN); \
            _XL_DRAW(old_x, old_y+1, _PLAYER_BOTTOM_TILE, _XL_CYAN); \
            ++player._y; \
            SHOW_DOWN(); \
            skullYCountDown = SKULL_COUNT_DOWN; \
            playerDirection = DOWN; \
            playerFire = 0;

        #define _DO_MOVE_LEFT \
            _set_old(); \
            _XL_DRAW(old_x-1, old_y, _PLAYER_LEFT_MOVE_TILE, _XL_CYAN); \
            _XL_DRAW(old_x, old_y, _PLAYER_RIGHT_MOVE_TILE, _XL_CYAN); \
            --player._x; \
            SHOW_LEFT(); \
            skullXCountDown = SKULL_COUNT_DOWN; \
            playerDirection = LEFT; \
            playerFire = 0;

        #define _DO_MOVE_RIGHT \
            _set_old(); \
            _XL_DRAW(old_x, old_y, _PLAYER_LEFT_MOVE_TILE, _XL_CYAN); \
            _XL_DRAW(old_x+1, old_y, _PLAYER_RIGHT_MOVE_TILE, _XL_CYAN); \
            ++player._x; \
            SHOW_RIGHT(); \
            skullXCountDown = SKULL_COUNT_DOWN; \
            playerDirection = RIGHT; \
            playerFire = 0;

    #else
        #define _DO_MOVE_UP \
            deletePlayer(&player); \
            --player._y; \
            SHOW_UP(); \
            skullYCountDown = SKULL_COUNT_DOWN; \
            playerDirection = UP; \
            playerFire = 0;
        #define _DO_MOVE_DOWN \
            deletePlayer(&player); \
            ++player._y; \
            SHOW_DOWN(); \
            skullYCountDown = SKULL_COUNT_DOWN; \
            playerDirection = DOWN; \
            playerFire = 0;
        #define _DO_MOVE_LEFT \
            deletePlayer(&player); \
            --player._x; \
            SHOW_LEFT(); \
            skullXCountDown = SKULL_COUNT_DOWN; \
            playerDirection = LEFT; \
            playerFire = 0;
        #define _DO_MOVE_RIGHT \
            deletePlayer(&player); \
            ++player._x; \
            SHOW_RIGHT(); \
            skullXCountDown = SKULL_COUNT_DOWN; \
            playerDirection = RIGHT; \
            playerFire = 0;
    #endif

    #if defined(__NO_PRINT)
        void MOVE_PLAYER(void) {}
    #else
        void MOVE_PLAYER(void)
        {
            uint8_t input = _XL_INPUT();
            
            if(_XL_UP(input) && (player._y>1) && !innerWallReached(player._x,player._y-1))
            {
                _DO_MOVE_UP
            }
            else if(_XL_DOWN(input) && (player._y<YSize-2) && !innerWallReached(player._x,player._y+1))
            {
                _DO_MOVE_DOWN
            }
            else if(_XL_LEFT(input) && (player._x>1) && !innerWallReached(player._x-1,player._y))
            {
                _DO_MOVE_LEFT
            }
            else if(_XL_RIGHT(input) && (player._x<XSize-2) && !innerWallReached(player._x+1,player._y))
            {
                _DO_MOVE_RIGHT
            }
            else if(_XL_FIRE(input) && (bombCount<BOMBS_NUMBER || (guns>0 && availableBullet()<BULLETS_NUMBER)))
            {
                playerFire = 1;
            }
        }
    #endif
#endif

