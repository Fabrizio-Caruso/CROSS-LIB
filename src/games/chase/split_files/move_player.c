
#include "settings.h"
#include "move_player.h"
#include "character.h"
#include "skull.h"
#include "definitions.h"


extern Character player;

#if !defined(TINY_GAME)
    extern uint8_t skullXCountDown;
    extern uint8_t skullYCountDown;
    extern uint8_t playerFire;
    extern Character bullet;
    extern uint8_t guns;
    extern uint8_t playerDirection;
#endif

#if defined(FULL_GAME)
    extern uint8_t player_invincibility;
    // extern uint8_t playerBlink;    
#endif

#if !defined(TINY_GAME) 
    #define _DO_MOVE_UP \
        deletePlayer(&player); \
        --player._y; \
        SHOW_UP(); \
        skullYCountDown = SKULL_COUNT_DOWN; \
        playerDirection = UP;
    #define _DO_MOVE_DOWN \
        deletePlayer(&player); \
        ++player._y; \
        SHOW_DOWN(); \
        skullYCountDown = SKULL_COUNT_DOWN; \
        playerDirection = DOWN;
    #define _DO_MOVE_LEFT \
        deletePlayer(&player); \
        --player._x; \
        SHOW_LEFT(); \
        skullXCountDown = SKULL_COUNT_DOWN; \
        playerDirection = LEFT;
    #define _DO_MOVE_RIGHT \
        deletePlayer(&player); \
        ++player._x; \
        SHOW_RIGHT(); \
        skullXCountDown = SKULL_COUNT_DOWN; \
        playerDirection = RIGHT;
#else
    #define _DO_MOVE_UP \
        deletePlayer(&player); \
        --player._y; \
        SHOW_UP();
        
    #define _DO_MOVE_DOWN \
        deletePlayer(&player); \
        ++player._y; \
        SHOW_DOWN();
        
    #define _DO_MOVE_LEFT \
        deletePlayer(&player); \
        --player._x; \
        SHOW_LEFT();
        
    #define _DO_MOVE_RIGHT \
        deletePlayer(&player); \
        ++player._x; \
        SHOW_RIGHT();
#endif

#if defined(NO_INPUT)
    void MOVE_PLAYER(void) 
    {
    }
#else
    void MOVE_PLAYER(void)
    {
        uint8_t input = _XL_INPUT();
        
        if(_XL_UP(input))
        {
            _DO_MOVE_UP
        }
        else if(_XL_DOWN(input))
        {
            _DO_MOVE_DOWN
        }
        else if(_XL_LEFT(input))
        {
            _DO_MOVE_LEFT
        }
        else if(_XL_RIGHT(input))
        {
            _DO_MOVE_RIGHT
        }
        #if !defined(TINY_GAME)
        else if(_XL_FIRE(input) && guns>0 && !bullet._status)
        {
            playerFire = 1;
        }
        #endif
    }
#endif

