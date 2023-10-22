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

// TODO: REMOVE THIS
// #define DEBUG_STRATEGY
// #define DEBUG_ITEMS
// #define DEBUG_END
// #define DEBUG_ITEMS_IN_GAME
// #define DEBUG_EASY_BOSS_LEVEL
// #define NO_CHASE

#include "settings.h"

#include "character.h"
#include "item.h"
#include "strategy.h"
#include "game_text.h"
#include "level.h"
#include "ghost.h"
#include "move_player.h"
#include "sleep.h"

#include "text_strings.h"

#include "init_images.h"

#include "display_macros.h"

#include "speed_game_settings.h"

#include "sleep_macros.h"

#include "bullet.h"
#include "skull.h"
    
#include "end_screen.h"

#include "horizontal_missile.h"
#include "rocket.h"

#include "variables.h"

// #define DEBUG_ITEMS

#if defined(DEBUG_ITEMS)


    #if defined(ANIMATE_PLAYER)
        extern Image PLAYER_DOWN_IMAGE;
        extern Image PLAYER_UP_IMAGE;
        extern Image PLAYER_RIGHT_IMAGE;
        extern Image PLAYER_LEFT_IMAGE;
    #else
        extern Image PLAYER_IMAGE;
    #endif
    extern Image GHOST_IMAGE;
    extern Image BOMB_IMAGE;
    extern Image DEAD_GHOST_IMAGE;
    extern Image SKULL_IMAGE;
    extern Image CALM_DOWN_IMAGE;
    extern Image FIRE_CHARGE_IMAGE;
    extern Image EXTRA_POINTS_IMAGE;
    extern Image BULLET_IMAGE;
    extern Image VERTICAL_BRICK_IMAGE;
    extern Image HORIZONTAL_BRICK_IMAGE;
    extern Image LEFT_HORIZONTAL_MISSILE_IMAGE;
    extern Image RIGHT_HORIZONTAL_MISSILE_IMAGE;
    extern Image ROCKET_IMAGE;
    extern Image FREEZE_IMAGE;
    extern Image SUPER_IMAGE;
    extern Image EXTRA_LIFE_IMAGE;
    extern Image INVINCIBILITY_IMAGE;
    extern Image CONFUSE_IMAGE;
    extern Image SUICIDE_IMAGE;
    extern Image BROKEN_BRICK_IMAGE;

void DO_DEBUG_ITEMS(void)
{
    #if defined(ANIMATE_PLAYER)
        _draw_stat(1, 0,&PLAYER_DOWN_IMAGE);
        _draw_stat(1, 1,&PLAYER_UP_IMAGE);
        _draw_stat(1, 2,&PLAYER_RIGHT_IMAGE);
        _draw_stat(1, 3,&PLAYER_LEFT_IMAGE);
    #else
        _draw_stat(1, 0,&PLAYER_IMAGE);
    #endif
    _draw_stat(1, 4,&GHOST_IMAGE);
    _draw_stat(1, 5,&BOMB_IMAGE);
    #if !defined(NO_DEAD_GHOST)
        _draw_stat(1, 6,&DEAD_GHOST_IMAGE);
    #endif
    #if !defined(TINY_GAME)
        _draw_stat(1, 7,&SKULL_IMAGE);
        _draw_stat(1, 8,&CALM_DOWN_IMAGE);
        _draw_stat(1, 9,&FIRE_CHARGE_IMAGE);
        _draw_stat(1,10,&EXTRA_POINTS_IMAGE);
        _draw_stat(1,11,&BULLET_IMAGE);
        _draw_stat(1,12,&VERTICAL_BRICK_IMAGE);
        _draw_stat(1,13,&HORIZONTAL_BRICK_IMAGE);
        #if defined(FULL_GAME)
            _draw_stat(1,14,&LEFT_HORIZONTAL_MISSILE_IMAGE);
            _draw_stat(1,15,&RIGHT_HORIZONTAL_MISSILE_IMAGE);
            #if (YSize+Y_OFFSET)>18
            _draw_stat(1,16,&ROCKET_IMAGE);
            _draw_stat(1,17,&FREEZE_IMAGE);
            _draw_stat(1,18,&SUPER_IMAGE);
            _draw_stat(1,19,&EXTRA_LIFE_IMAGE);
            _draw_stat(1,20,&INVINCIBILITY_IMAGE);
            _draw_stat(1,21,&CONFUSE_IMAGE);
            _draw_stat(1,22,&SUICIDE_IMAGE);
            _draw_stat(1,23,&BROKEN_BRICK_IMAGE);
            #else
            _draw_stat(3,7,&ROCKET_IMAGE);
            _draw_stat(3,8,&FREEZE_IMAGE);
            _draw_stat(3,9,&SUPER_IMAGE);
            _draw_stat(3,10,&EXTRA_LIFE_IMAGE);
            _draw_stat(3,11,&INVINCIBILITY_IMAGE);
            _draw_stat(3,12,&CONFUSE_IMAGE);
            _draw_stat(3,13,&SUICIDE_IMAGE);
            _draw_stat(3,14,&BROKEN_BRICK_IMAGE);
            #endif
        #endif
    #endif
    _XL_PRINTD(4,3,5, 1234U);
    _XL_PRINTD(4,5,5,56789U);
    _XL_PRINT(4,7,"abcdefghijklmnopqrstuvwxyz,./|-");
    _XL_PRINT(4,9,"ABCDEFGHIJKLMNOPQRSTUVWXYZ;+{}=");
    while(1){};
}
#endif


#if !defined(NO_EXTRA_TITLE) && YSize>=17
static const uint8_t item_tile[7][2] = 
{
    { _GHOST_TILE      , _XL_WHITE },
    { _FAST_GHOST_TILE , _XL_GREEN },
    { _SKULL_TILE      , _XL_YELLOW },
    { _BOSS_TILE       , _XL_RED },
    { _BULLET_TILE     , _XL_WHITE},
    { _FIRE_POWER_TILE , _FIRE_POWER_COLOR },
    { _BOMB_TILE       , _XL_RED },
};

static const char item_name[7][9] = 
{
    _XL_G _XL_H _XL_O _XL_S _XL_T,
    _XL_S _XL_P _XL_E _XL_C _XL_T _XL_E _XL_R,
    _XL_S _XL_K _XL_U _XL_L _XL_L,
    _XL_B _XL_O _XL_S _XL_S,
    _XL_B _XL_U _XL_L _XL_L _XL_E _XL_T _XL_S,
    _XL_P _XL_O _XL_W _XL_E _XL_R _XL_SPACE _XL_U _XL_P,
    _XL_M _XL_I _XL_N _XL_E,
};

#if XSize>16 && YSize>20
    #define _NEXT_ROW 2
    #define _Y_ITEMS_OFFSET 0
#else
    #define _NEXT_ROW 1
    #define _Y_ITEMS_OFFSET 1
#endif


#define display_items() \
do \
{ \
    uint8_t i; \
    \
    for(i=0;i<7;++i) \
    { \
        _XL_DRAW(XSize/2-5,YSize/3+1+i*_NEXT_ROW+_Y_ITEMS_OFFSET, item_tile[i][0], item_tile[i][1]); \
        _XL_SET_TEXT_COLOR(_XL_GREEN); \
        _XL_PRINT(XSize/2-5+3,YSize/3+1+i*_NEXT_ROW+_Y_ITEMS_OFFSET, (char *)item_name[i]); \
    } \
} while(0)

#endif


#define handle_secret_item_at_start_up(secretFlag, item, secretIndex) \
    if(secretFlag) \
    { \
        item._coolDown = 2; \
        setSecret(secretIndex); \
        secretFlag = 0; \
    }\


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
 
#include "bullet.h"
#include "level.h"
#include "game_text.h"
#include "character.h"
#include "item.h"
#include "ghost.h"
#include "strategy.h"
#include "skull.h"
#include "init_images.h"

extern uint8_t bombCount;
extern Character bombs[BOMBS_NUMBER];

extern uint8_t skullsCount;

extern uint8_t bulletStrength;

extern uint16_t points;

extern uint16_t loop;

extern uint8_t isOneMissileLevel;
extern uint8_t isMissileLevel;
extern uint8_t isBossLevel;
// extern uint8_t isRocketLevel;

extern Character ghosts[GHOSTS_NUMBER];

extern Image DEAD_GHOST_IMAGE;
extern Image VERTICAL_BRICK_IMAGE;
extern Image HORIZONTAL_BRICK_IMAGE;

extern uint8_t level;

extern Item extraPoints;

extern Character skulls[SKULLS_NUMBER];
extern uint8_t skullActive;
extern uint8_t playerFire;
extern uint8_t guns;
extern uint8_t playerDirection;
extern uint8_t bulletDirection[BULLETS_NUMBER];

extern Character bullets[BULLETS_NUMBER];
extern Character player;

extern uint8_t maxGhostsOnScreen;

extern uint8_t rocketsOnScreen;

extern Item freeze;
extern Item extraLife;
extern Item invincibility;
    
extern Character leftHorizontalMissile;
extern Character rightHorizontalMissile;
extern uint8_t rockets_x[MAX_ROCKETS_NUMBER];
extern Character rockets[MAX_ROCKETS_NUMBER];
extern uint8_t destroyed_bases;
extern uint8_t destroyed_bases_in_completed_levels;
extern uint8_t destroyerActive;

uint8_t availableBullet(void)
{
    uint8_t i;
    
    for(i=0;i<BULLETS_NUMBER;++i)
    {
        if(!bullets[i]._status)
        {
            return i;
        }
    }
    return BULLETS_NUMBER;
}


uint8_t not_stacked(uint8_t candidate_x, uint8_t candidate_y)
{
    uint8_t i;
    
    for(i=0;i<BOMBS_NUMBER;++i)
    {
        if((candidate_x==bombs[i]._x)&&(candidate_y==bombs[i]._y)&&bombs[i]._status)
        {
            return 0;
        }
    }
    return 1;
}


void handle_bomb(void)
{
    uint8_t candidate_bomb_x;
    uint8_t candidate_bomb_y;

    // Check if player has fired the gun
    if(playerFire && bombCount<BOMBS_NUMBER)
    {
        
        // deleteCharacter(&bombs[bombCount]);

        if(playerDirection==RIGHT)
        {
            candidate_bomb_x = player._x+1; 
        }
        else if(playerDirection==LEFT)
        {
            candidate_bomb_x = player._x-1;             
        }
        else
        {
            candidate_bomb_x = player._x;            
        }
        
        if(playerDirection==UP)
        {
            candidate_bomb_y = player._y-1;        
        }
        else if(playerDirection==DOWN)
        {
            candidate_bomb_y = player._y+1;        
        }
        else
        {
            candidate_bomb_y = player._y;        
        }
        
        if(not_stacked(candidate_bomb_x,candidate_bomb_y) && !onWall(candidate_bomb_x,candidate_bomb_y))
        {
            _XL_SHOOT_SOUND();

            bombs[bombCount]._x = candidate_bomb_x;
            bombs[bombCount]._y = candidate_bomb_y;
            bombs[bombCount]._status=1;
            displayBomb(&bombs[bombCount]);

            ++bombCount;
            printGunsStats();
        }
        else
        {
            _XL_TICK_SOUND();
        }
        playerFire = 0;
    }
}        


void handle_bullet_fire(uint8_t bulletIndex)
{
    // Check if player has fired the gun
    if(playerFire && bullets[bulletIndex]._status==0 && guns>0 && bombCount==BOMBS_NUMBER)
    {
        _XL_SHOOT_SOUND();
        --guns;
        printGunsStats();
        bulletDirection[bulletIndex] = playerDirection;
        bullets[bulletIndex]._status = 1;
        
        if(playerDirection==LEFT)
        {
            bullets[bulletIndex]._x = player._x-1;    
            #if !defined(NO_PLAYER_SHOOT_ANIMATION) && defined(ANIMATE_PLAYER)
            if(!destroyerActive)
            {
                _XL_DRAW(player._x, player._y, _PLAYER_RIGHT_TILE, _PLAYER_COLOR);
            }
            #endif            
        }
        else if(playerDirection==RIGHT)
        {
            bullets[bulletIndex]._x = player._x+1;  
            #if !defined(NO_PLAYER_SHOOT_ANIMATION) && defined(ANIMATE_PLAYER)
            if(!destroyerActive)
            {
                _XL_DRAW(player._x, player._y, _PLAYER_LEFT_TILE, _PLAYER_COLOR);
            }
            #endif            
        }
        else
        {
            bullets[bulletIndex]._x = player._x;                            
        }
        
        if(playerDirection==UP)
        {
            bullets[bulletIndex]._y = player._y-1;  
            #if !defined(NO_PLAYER_SHOOT_ANIMATION) && defined(ANIMATE_PLAYER)
            if(!destroyerActive)
            {
                _XL_DRAW(player._x, player._y, _PLAYER_DOWN_TILE, _PLAYER_COLOR);
            }
            #endif            
        }
        else if(playerDirection==DOWN)
        {
            bullets[bulletIndex]._y = player._y+1;    
            #if !defined(NO_PLAYER_SHOOT_ANIMATION) && defined(ANIMATE_PLAYER)
            if(!destroyerActive)
            {
                _XL_DRAW(player._x, player._y, _PLAYER_UP_TILE, _PLAYER_COLOR);
            }
            #endif            
        }
        else
        {
            bullets[bulletIndex]._y = player._y;        
        }
        displayBullet(&bullets[bulletIndex]);
        playerFire = 0;    
        checkBullet(&bullets[bulletIndex],playerDirection);
        // displayCharacter(&player);
    }    
}    


void handle_bullet(uint8_t bulletIndex)
{
    // Move bullet if fired
    if(bullets[bulletIndex]._status)
    {
        moveBullet(&bullets[bulletIndex], bulletDirection[bulletIndex]);
        checkBullet(&bullets[bulletIndex], bulletDirection[bulletIndex]);
    }
}


void handle_bullets(void)
{
    uint8_t i;
    
    if(loop&1)
    {
        for(i=0;i<BULLETS_NUMBER;++i)
        {
            handle_bullet_fire(i);
        }        
    }
    for(i=0;i<BULLETS_NUMBER;++i)
    {
        handle_bullet(i);
    }
}


void pushGhost(Character * ghostPtr, uint8_t bulletDirection)
{
    switch(bulletDirection)
    {
        case UP:
            --(ghostPtr->_y);
        break;
        case DOWN:
            ++(ghostPtr->_y);
        break;
        case LEFT:
            --(ghostPtr->_x);
        break;
        default:
        // case RIGHT:
            ++(ghostPtr->_x);
        // break;        
    }
}


void checkBullets(void)
{
    uint8_t i;
    
    for(i=0;i<BULLETS_NUMBER;++i)
    {
        checkBullet(&bullets[i], bulletDirection[i]);
    }
}


void checkBullet(Character *bulletPtr, uint8_t bulletDirection)
{
    if(bulletPtr->_status)
    {
        if(skullActive)
        {
            checkBulletVsSkulls(bulletPtr);
        }
        checkBulletVsGhosts(bulletPtr, bulletDirection);
    }
}


// TODO: Restore missile bases if still active
void restoreRocketsOnWall(uint8_t x, uint8_t y)
{
    if((y==0) || (y==YSize-1))
    {
        DRAW_CHARACTER(x,y,&HORIZONTAL_BRICK_IMAGE);
        if(ghostCount>MAX_GHOST_COUNT_FOR_ROCKETS)
        {
            uint8_t i;
            for(i=0;i<rocketsOnScreen;i++)
            {
                if(rockets[i]._status)
                {
                    displayRocket(&rockets[i]);
                }
            }
        }
    }
    else
    {
        DRAW_CHARACTER(x,y,&VERTICAL_BRICK_IMAGE);
    }
}


void checkBulletVsGhost(Character * bulletPtr,
                        uint8_t bulletDirection,
                        register Character * ghostPtr)
{
    if(ghostPtr->_status &&
       areCharctersAtSamePosition(bulletPtr, ghostPtr))
    {
        uint8_t i;
        
        deleteBullet(bulletPtr);
        bulletPtr->_status=0;

        if((ghostPtr->_status)<=bulletStrength)
        {
            ghostDiesAndSpawns(ghostPtr);
            // points+=GHOST_VS_MISSILE;
            increasePoints(GHOST_VS_MISSILE);
        }
        else
        {
            (ghostPtr->_status)-=bulletStrength;
            displayGhost(ghostPtr);
        
            for(i=0;i<bulletStrength/2;++i)
            {

                deleteGhost(ghostPtr);
                pushGhost(ghostPtr, bulletDirection);
                if(wallReached(ghostPtr) || innerVerticalWallReached(ghostPtr->_x, ghostPtr->_y) || innerHorizontalWallReached(ghostPtr->_x, ghostPtr->_y))
                {
                    // points+=GHOST_VS_WALL_BONUS;
                    increasePoints(GHOST_VS_WALL_BONUS);
                    ghostDies(ghostPtr);
                    restoreRocketsOnWall(ghostPtr->_x, ghostPtr->_y);
                    if((!isBossLevel) && (ghostCount>=maxGhostsOnScreen))
                        {
                            spawnGhost(ghostPtr,ghostCount);
                        }
                    displayStats();
                    break;
                }
                displayGhost(ghostPtr);
            }
        }
    }
}


void checkBulletVsGhosts(Character * bulletPtr, uint8_t bulletDirection)
{
    uint8_t i = 0;
    
    for(;i<maxGhostsOnScreen;++i)
    {
        if(ghosts[i]._status)
        {
            checkBulletVsGhost(bulletPtr, bulletDirection, &ghosts[i]);
        }
    };
}


void checkBulletVsSkull(register Character *bulletPtr, register Character *skullPtr)
{
    if(skullPtr->_status && 
       areCharctersAtSamePosition(bulletPtr, skullPtr))
    {
        _XL_DRAW(skullPtr->_x,skullPtr->_y,skullPtr->_imagePtr->_imageData, _XL_RED);
        _XL_PING_SOUND();
        
        bulletPtr->_status=0;
        
        deleteBullet(bulletPtr);
        // bulletPtr->_x = 0; 
        // bulletPtr->_y = 0;
        decreaseGhostLevel();
        reduceItemCoolDowns();
        
        if((skullPtr->_status)<=bulletStrength)
        {
            skullDies(skullPtr);
        }
        else
        {
            (skullPtr->_status)-=bulletStrength;
            displaySkull(skullPtr);
        }
    }    
}


void checkBulletVsSkulls(Character *bulletPtr)
{
    uint8_t i;
    
    for(i=0;i<SKULLS_NUMBER;++i)
    {
        if(skulls[i]._status)
        {
            checkBulletVsSkull(bulletPtr, &skulls[i]);
        }
    }
}


void _moveBullet(register Character *bulletPtr, uint8_t bulletDirection)
{
    deleteBullet(bulletPtr);
    switch(bulletDirection)
    {
        case RIGHT:
            ++bulletPtr->_x;
        break;
        case DOWN:
            ++bulletPtr->_y;
        break;
        case UP:
            --bulletPtr->_y;
        break;
        default:
        // case LEFT:
            --bulletPtr->_x;
        // break;
    }    
}


void destroyHorizontalMissile(Character * horizontalMissilePtr)
{
    horizontalMissilePtr->_status = 0;
    _XL_EXPLOSION_SOUND();
    deleteHorizontalMissile(horizontalMissilePtr);
    // points+=HORIZONTAL_MISSILE_BONUS;
    increasePoints(HORIZONTAL_MISSILE_BONUS);
    // displayScoreStats();                
    ++destroyed_bases;
    reduceItemCoolDowns();        
}


void moveBullet(register Character * bulletPtr, uint8_t bulletDirection)
{
    if(bulletPtr->_status && (wallReached(bulletPtr) || innerVerticalWallReached(bulletPtr->_x, bulletPtr->_y) || innerHorizontalWallReached(bulletPtr->_x, bulletPtr->_y)))
    {
        bulletPtr->_status=0;

        if(isOneMissileLevel)
        {
            if(bulletPtr->_x==XSize-1 && bulletPtr->_y==YSize/2 && rightHorizontalMissile._status)
            {
                destroyHorizontalMissile(&rightHorizontalMissile);
            }
        }                
        else if(isMissileLevel)
        {
            if(bulletPtr->_x==XSize-1 && bulletPtr->_y==HORIZONTAL_MISSILE_OFFSET && rightHorizontalMissile._status)
            {
                destroyHorizontalMissile(&rightHorizontalMissile);    
            }
            else if(bulletPtr->_x==0 && bulletPtr->_y==YSize-1-HORIZONTAL_MISSILE_OFFSET && leftHorizontalMissile._status)
            {
                destroyHorizontalMissile(&leftHorizontalMissile);    
            }
        }
        if(bulletPtr->_y==YSize-1)
        {
            uint8_t i;
            for(i=0;i<rocketsOnScreen;++i)
            {
                if((bulletPtr->_x==rockets_x[i]) && rockets[i]._status)
                {
                    rockets[i]._status = 0;
                    ++destroyed_bases;
                    _XL_EXPLOSION_SOUND();
                    deleteRocket(&rockets[i]);
                    // points+=VERTICAL_MISSILE_BONUS;
                    increasePoints(VERTICAL_MISSILE_BONUS);
                    // displayScoreStats();        
                }
            }
        }            
        restoreRocketsOnWall(bulletPtr->_x, bulletPtr->_y);
        // displayStatsTitles();
        displayStats();
    }
    else
    {
        _moveBullet(bulletPtr, bulletDirection);
        displayBullet(bulletPtr);
    }
}


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


#include "character.h"
#include "settings.h"
#include "level.h"

#include "../cross_lib/cross_lib.h"

#include "game_text.h"

extern uint16_t points;

extern uint8_t skullsCount;

extern uint8_t ghostCount;
extern uint16_t loop;
extern uint8_t level;

extern Image DEAD_GHOST_IMAGE;
extern Image GHOST_IMAGE;
extern Image BOMB_IMAGE;
extern Image BROKEN_BRICK_IMAGE;

extern Image HORIZONTAL_BRICK_IMAGE;
extern Image VERTICAL_BRICK_IMAGE;

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];
extern Character skulls[SKULLS_NUMBER];

extern Character player;

extern uint8_t maxGhostsOnScreen;

extern uint8_t isInnerHorizontalWallLevel;
extern uint8_t isInnerVerticalWallLevel;

extern uint8_t invincibilityActive;
extern uint8_t destroyerActive;

void _DRAW_PLAYER(void)
{
    if(destroyerActive)
    {
        DRAW_PLAYER(player._x, player._y, &DESTROYER_IMAGE);
    }
#if !defined(_XL_NO_COLOR)
    // Color case: player gets a different color when invincible
    else if(invincibilityActive)
    {
        player._imagePtr->_color = _XL_YELLOW;
        DRAW_PLAYER(player._x, player._y, player._imagePtr);
        player._imagePtr->_color = _XL_CYAN;
    }
#endif
    else
    {
        DRAW_PLAYER(player._x, player._y, player._imagePtr);
    }
}


#if !defined(NO_BLINKING)
void _blink_draw(uint8_t x, uint8_t y, Image * image, uint8_t *blinkCounter) 
{
    if(*blinkCounter) 
    {
        _draw(x,y,image);
        *blinkCounter=0;
    } 
    else 
    {
        _delete(x,y);
        *blinkCounter=1;
    }    
}
#endif


void displayCharacter(register Character * characterPtr)
{
    DRAW_CHARACTER(characterPtr->_x, characterPtr->_y, characterPtr->_imagePtr);
}

void deleteCharacter(Character * characterPtr)
{
    DELETE_CHARACTER(characterPtr->_x, characterPtr->_y);
}

extern uint8_t invincibilityActive;

extern uint8_t innerVerticalWallY;
extern uint8_t innerVerticalWallLength;

extern uint8_t innerHorizontalWallX;

extern uint8_t innerHorizontalWallLength;


void increasePoints(uint8_t score)
{
    points+=score;
    displayStats();
}


void playerDies(void)
{
    _XL_EXPLOSION_SOUND();
    player._status=0;
    #if !defined(LESS_TEXT)
        printDefeatMessage();
    #endif
    _XL_SLEEP(1);    
}

uint8_t playerKilledBy(Character *enemyPtr)
{
    return (!invincibilityActive) && areCharctersAtSamePosition(enemyPtr,&player);
}

uint8_t handle_player_killed(Character *enemyPtr)
{
    if(playerKilledBy(enemyPtr))
    {
        playerDies();
        return 1;
    }
    return 0;
}

void initializeCharacter(register Character* characterPtr, uint8_t x, uint8_t y, uint8_t status, Image * imagePtr)
{
    characterPtr->_x = x;
    characterPtr->_y = y;
    characterPtr->_status = status;
    characterPtr->_imagePtr = imagePtr;
    
    #if defined(DEBUG)
        displayCharacter(characterPtr);
    #endif
}


uint8_t isCharacterAtLocation(uint8_t x, uint8_t y, Character * characterPtr)
{
    return(characterPtr->_x==x) && (characterPtr->_y==y);
}


uint8_t onWall(uint8_t x, uint8_t y)
{
    return(x==0)||(x==XSize-1) || 
          (y==0)||(y==YSize-1);
}

uint8_t wallReached(Character *characterPtr)
{
   return onWall(characterPtr->_x, characterPtr->_y);
}


uint8_t sameLocationAsAnyActiveLocation(uint8_t x, uint8_t y, Character *characterList, uint8_t length)
{
    uint8_t i;

    for(i=0;i<length;++i)
    {
        if(characterList[i]._status && isCharacterAtLocation(x,y,&characterList[i]))
        {
            return i;
        }
    }    
    return length;
}


#if YSize<XSize
    #define SAFETY YSize/4
#else
    #define SAFETY XSize/4
#endif

// TODO: To be replaced with something cleaner
// also used with things different from global bombs
uint8_t safeLocation(uint8_t x, uint8_t y)
{
       return (x>=SAFETY) && (x<=XSize-SAFETY) && (y>SAFETY) && (y<=YSize-SAFETY);
}


void relocateNearBy(register Character * characterPtr)
{
    uint8_t x; 
    uint8_t y;  
    
    do
    {
        x = characterPtr->_x - (uint8_t)(RELOCATE_RANGE/2) + (uint8_t)(_XL_RAND() % RELOCATE_RANGE);
        y = characterPtr->_y - (uint8_t)(RELOCATE_RANGE/2) + (uint8_t)(_XL_RAND() % RELOCATE_RANGE);

    } while(!safeLocation(x,y));

    characterPtr->_x = x;
    characterPtr->_y = y;
}

uint8_t innerVerticalWallReached(uint8_t x, uint8_t y)
{
    return (x==XSize/2) && (y >= innerVerticalWallY) && (y <= (innerVerticalWallY + innerVerticalWallLength-1));
}


// uint8_t nearInnerVerticalWall(register Character *characterPtr)
// {
    // return isInnerVerticalWallLevel && (characterPtr->_x>=(XSize/2)-1) && (characterPtr->_x<=(XSize/2)+1) &&
           // (characterPtr->_y >= innerVerticalWallY-1) && (characterPtr->_y<= (innerVerticalWallY + innerVerticalWallLength));
// }



uint8_t innerHorizontalWallReached(uint8_t x, uint8_t y)
{
    return (y==(YSize/2)) && (x >= innerHorizontalWallX) && (x<= (innerHorizontalWallX + innerHorizontalWallLength-1));
}


// uint8_t nearInnerHorizontalWall(register Character *characterPtr)
// {
    // return isInnerHorizontalWallLevel && (characterPtr->_y>=(YSize/2)-1) && (characterPtr->_y<=(YSize/2)+1) &&
           // (characterPtr->_x >= innerHorizontalWallX-1) && (characterPtr->_x<= (innerHorizontalWallX + innerHorizontalWallLength));
// }    


// void DRAW_BROKEN_BRICK(uint8_t x, uint8_t y)
// {
    // _draw(x,y,&BROKEN_BRICK_IMAGE);        
// }


#include "settings.h"

#include "end_screen.h"
#include "game_text.h"
#include "text_strings.h"
#include "move_player.h"

#if defined(END_SCREEN)
	#include "strategy.h"
	#include "level.h"
	#include "character.h"
	#include "ghost.h"
	
	extern Character player;
	extern Character skulls[];
	extern Character ghosts[GHOSTS_NUMBER];
	extern Character bullet;
	extern uint8_t playerFire;
	extern uint8_t level;
	extern uint8_t guns;
	extern uint8_t ind;
    
    extern uint8_t maxGhostsOnScreen;
    
    extern uint8_t isBossLevel;
    extern uint8_t innerVerticalWallLength;
#endif


#if YSize < 20
	#define MESSAGE_START 3
	#define MESSAGE_RANGE 7
#else
	#define MESSAGE_START 7
	#define MESSAGE_RANGE 15
#endif

#if defined(END_SCREEN)
	void gameCompleted(void)
	{
		
		_XL_CLEAR_SCREEN();
        ghostCount = 8;
        maxGhostsOnScreen = 8;
        isBossLevel = 0;
		fillLevelWithCharacters();

		// skulls[0]._x = player._x-4;
		// skulls[0]._y = player._y;

        for(ind=0;ind<maxGhostsOnScreen;++ind)
        {
            displayCharacter(&ghosts[ind]);
        }
        
        displayPlayer(&player);
        // displaySkull(&skulls[0]);
    
        _XL_PRINT(XSize/2-3, MESSAGE_START,  YOU_MADE_IT_STRING);

        _XL_WAIT_FOR_INPUT();
        
        #if defined(BETWEEN_LEVEL)
            spiral(&player, 2*MIN_SIZE-18);
            // _XL_SLEEP(1);
        #endif
	}
#endif


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
#include "cross_lib.h"

#include "game_text.h"
#include "character.h"
// #include "settings.h"
#include "text_strings.h"
#include "sound_macros.h"
#include "sleep_macros.h"
#include "item.h"
#include "sleep.h"
#include "init_images.h"

extern Image GHOST_IMAGE;
extern Image BULLET_IMAGE;
extern Image PLAYER_IMAGE;
extern Image FIRE_POWER_IMAGE;

extern uint8_t guns; 

extern uint8_t bulletStrength;

#define SET_COLOR(c) _XL_SET_TEXT_COLOR(c)

#if YSize<=15
    #define LINE_SKIP 1
    #define INIT_HINT_LINE 1
#else
    #define LINE_SKIP 2
    #define INIT_HINT_LINE 1+(YSize/8)
#endif


extern uint8_t level;
extern uint8_t lives;

extern uint16_t points;
extern uint8_t ghostCount;
extern uint16_t ghostLevel;
extern uint16_t highScore;

extern Image BULLET_IMAGE;
extern Image BOMB_IMAGE;

extern uint8_t discoveredSecrets[];

extern uint8_t  secretLevelActivated;

extern uint8_t bombCount;



void PRINT_CENTERED_ON_ROW(uint8_t row, char *Text)
{
	_XL_PRINT(((uint8_t) (XSize - strlen(Text))>>1), row, Text);	
}


#if !defined(LESS_TEXT)

void print_destroy_missiles(uint8_t row)
{
    _XL_PRINT(XSize/2-9+1,row, DESTROY_MISSILES__STRING);
    
    _XL_DRAW(XSize/2-3, row+LINE_SKIP, _LEFT_HORIZONTAL_MISSILE_TILE, _XL_WHITE);
    _XL_DRAW(XSize/2-1, row+LINE_SKIP, _ROCKET_TILE, _XL_WHITE);
    _XL_DRAW(XSize/2+1, row+LINE_SKIP, _RIGHT_HORIZONTAL_MISSILE_TILE, _XL_WHITE); 
   
}

#endif

#if !defined(LESS_TEXT)
void printKillTheSkulls(void)
{
    uint8_t i;
    
    _XL_DRAW(XSize/2-4, YSize/2-4, _BOSS_TILE, _XL_RED);
    
    for(i=0;i<3;++i)
    {
        _XL_DRAW(XSize/2-2+(i<<1), YSize/2-4, _SKULL_TILE, _XL_CYAN);
    }

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    PRINT_CENTERED_ON_ROW(((uint8_t)YSize)/2-2,KILL_THE_BOSS);    
    PRINT_CENTERED_ON_ROW(((uint8_t)YSize)/2,KILL_THE_SKULLS_STRING);   

    print_destroy_missiles(((uint8_t)YSize)/2+2);
}
#endif


void displayStatsTitles(void)
{                
    _draw_stat(GHOST_IMAGE_X, GHOST_IMAGE_Y, &GHOST_IMAGE);
    _draw_stat(PLAYER_IMAGE_X, PLAYER_IMAGE_Y, &PLAYER_IMAGE);                    
    _draw_stat(FIRE_POWER_IMAGE_X, FIRE_POWER_IMAGE_Y, &FIRE_POWER_IMAGE); 
}


void displayStats(void)
{
    #if !defined(_XL_NO_COLOR)
    if(bulletStrength>=COLOR_BULLET_SECOND_THRESHOLD)
    {
        BULLET_IMAGE._color = SECOND_THRESHOLD_BULLET_COLOR;
    }    
    else if(bulletStrength>=COLOR_BULLET_FIRST_THRESHOLD)
    {
        BULLET_IMAGE._color = FIRST_THRESHOLD_BULLET_COLOR; 
    }
    #endif
    displayScoreStats();
    displayStatsTitles();
    #if XSize>=32
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(XSize-10,0,"HI");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(XSize-8, 0, 5, highScore );  
    #endif

    printLevelStats();
    printLivesStats();
    printGunsStats();
    printFirePowerStats();
    printGhostCountStats();
}


void printGunsStats(void)
{
    SET_COLOR(_XL_WHITE);    


    if(bombCount<BOMBS_NUMBER)
    {

        #if defined(WIDE)
            _XL_PRINTD(BULLET_IMAGE_X+1,BULLET_IMAGE_Y,2,BOMBS_NUMBER-bombCount);
        #else
            _XL_PRINTD(BULLET_IMAGE_X+1,BULLET_IMAGE_Y,2,BOMBS_NUMBER-bombCount);
        #endif
        _draw_stat(BULLET_IMAGE_X, BULLET_IMAGE_Y, &BOMB_IMAGE);
    }
    else
    {
        #if defined(WIDE)
            _XL_PRINTD(BULLET_IMAGE_X+1,BULLET_IMAGE_Y,2,guns);
        #else
            _XL_PRINTD(BULLET_IMAGE_X+1,BULLET_IMAGE_Y,2,guns);
        #endif
        _draw_stat(BULLET_IMAGE_X, BULLET_IMAGE_Y, &BULLET_IMAGE);
    }  
    

}


void printFirePowerStats(void)
{
    #if !defined(_XL_NO_TEXT_COLOR)
    if(bulletStrength<COLOR_BULLET_FIRST_THRESHOLD)
    {
        SET_COLOR(INITIAL_BULLET_COLOR);
    }
    else if(bulletStrength<COLOR_BULLET_SECOND_THRESHOLD)
    {
        SET_COLOR(FIRST_THRESHOLD_BULLET_COLOR);
    }
    else
    {
        SET_COLOR(SECOND_THRESHOLD_BULLET_COLOR);
    }
    #endif
    
    #if defined(WIDE)
        _XL_PRINTD(FIRE_POWER_IMAGE_X+2,FIRE_POWER_IMAGE_Y,1,bulletStrength-1);
    #else
        _XL_PRINTD(FIRE_POWER_IMAGE_X+1,FIRE_POWER_IMAGE_Y,1,bulletStrength-1);
    #endif
}


#if XSize>20
void printLevelStats(void)
{    
	SET_COLOR(_XL_WHITE);

	_XL_PRINTD(LEVEL_X,LEVEL_Y,2,level);
}
#endif


void printGhostCountStats(void)
{
	SET_COLOR(_XL_WHITE);        
	
	#if defined(WIDE)
		_XL_PRINTD(GHOST_IMAGE_X+1,GHOST_IMAGE_Y,2,ghostCount);
	#else
		_XL_PRINTD(GHOST_IMAGE_X+1,GHOST_IMAGE_Y,2,ghostCount);    
	#endif    
}


void printLivesStats(void)
{
	SET_COLOR(_XL_WHITE);
	
	_XL_PRINTD(PLAYER_IMAGE_X+1,PLAYER_IMAGE_Y,1,lives);    
}    



void displayScoreStats(void)
{    
    SET_COLOR(_XL_WHITE);
    _XL_PRINTD(0,0,5,points);  
}


#if !defined(LESS_TEXT)    
    void printLevel(void)
    {
        uint8_t i;
        
        SET_COLOR(_XL_YELLOW);

        if(!level)
        {
            _XL_PRINT(((XSize -7)>>1), (YSize>>1)-2, _XL_S _XL_E _XL_C _XL_R _XL_E _XL_T); 
        }
        else if(level==8)//(!(level&7))
        {
            _XL_PRINT(((XSize -7)>>1), (YSize>>1)-2, _XL_B _XL_SPACE _XL_O _XL_SPACE _XL_S _XL_SPACE _XL_S);

        }
        else if(level==16)
        {
            _XL_PRINT(((XSize -7)>>1), (YSize>>1)-2, _XL_F _XL_I _XL_N _XL_A _XL_L);
        }
        else
        {
            for(i=0;i<(level&7);++i)
            {
                _draw_stat(((XSize -7)>>1)+i,(YSize>>1)-4,&GHOST_IMAGE);
            }
            SET_COLOR(_XL_WHITE);
            
            _XL_PRINT(((XSize -7)>>1), (YSize>>1)-2, _XL_P _XL_A _XL_R _XL_T _XL_SPACE _XL_I);


            if(level>8)
            {
                _XL_CHAR(((XSize -7)>>1)+6, (YSize>>1)-2, 'I');
            }
        }
        SET_COLOR(_XL_WHITE);

        _XL_PRINT(((XSize -7)>>1), (YSize>>1), LEVEL_STRING);
        _XL_PRINTD(((XSize -7)>>1)+6, (YSize>>1), 2, level);
    }
#endif


#if !defined(END_SCREEN)
    void gameCompleted(void)    
    {
        _XL_CLEAR_SCREEN();
        PRINT_CENTERED(YOU_MADE_IT_STRING); 
        _XL_WAIT_FOR_INPUT();
    }
#endif


#if !defined(LESS_TEXT)

    void printVictoryMessage(void)
    {
        SET_COLOR(_XL_RED);
        _XL_PRINT(XSize/2-3,YSize/2-2,VICTORY_STRING);
    }    
#endif


#if !defined(LESS_TEXT)    
    void printDefeatMessage(void)
    {            
        SET_COLOR(_XL_RED);
        PRINT_CENTERED_ON_ROW(YSize/2-2,DEFEAT_STRING);
    }    
#endif
    
    
void printGameOver(void)
{
	SET_COLOR(_XL_RED);
	PRINT_CENTERED(GAME_OVER_STRING);
}    


#if !defined(NO_INITIAL_SCREEN)

void _printCrossShoot(void)
{
    PRINT_CENTERED_ON_ROW(2,  CROSS_SHOOT_STRING);        
    SET_COLOR(_XL_WHITE);
}
#endif


#if !defined(NO_HINTS) && !defined(NO_INITIAL_SCREEN) && XSize>=18
    void printHints(void)
    {
        SET_COLOR(_XL_RED);

        _printCrossShoot();
        
        _XL_PRINT(XSize/2-7+1,INIT_HINT_LINE+2*LINE_SKIP, KILL_SKULLS__STRING);
        _XL_PRINT(XSize/2-10+1, INIT_HINT_LINE+3*LINE_SKIP, BEFORE__TO_UNLOCK__STRING);  
        _XL_PRINT(XSize/2-7+1,INIT_HINT_LINE+4*LINE_SKIP, ITEMS__STRING);    
        _XL_PRINT(XSize/2-9+1,INIT_HINT_LINE+5*LINE_SKIP+1, DESTROY_MISSILES__STRING);
        
        _XL_DRAW(XSize/2-2, INIT_HINT_LINE+6*LINE_SKIP+1, _LEFT_HORIZONTAL_MISSILE_TILE, _XL_WHITE);
        _XL_DRAW(XSize/2-0, INIT_HINT_LINE+6*LINE_SKIP+1, _ROCKET_TILE, _XL_WHITE);
        _XL_DRAW(XSize/2+2, INIT_HINT_LINE+6*LINE_SKIP+1, _RIGHT_HORIZONTAL_MISSILE_TILE, _XL_WHITE);   


        _XL_DRAW(XSize/2+5+1,INIT_HINT_LINE+2*LINE_SKIP, _SKULL_TILE, _XL_YELLOW);
        _XL_DRAW(XSize/2-3+1,INIT_HINT_LINE+3*LINE_SKIP, _GHOST_TILE, _XL_WHITE);
        // _XL_DRAW(XSize/2-3+1,INIT_HINT_LINE+3*LINE_SKIP, _FAST_GHOST_TILE, _XL_WHITE);

        _XL_DRAW(XSize/2-1+1,INIT_HINT_LINE+4*LINE_SKIP, _SUPER_TILE, _XL_RED);
        _XL_DRAW(XSize/2+1+1,INIT_HINT_LINE+4*LINE_SKIP, _FREEZE_TILE, _XL_CYAN);
        _XL_DRAW(XSize/2+3+1,INIT_HINT_LINE+4*LINE_SKIP, _INVINCIBILITY_TILE, _XL_YELLOW);
    }
#endif


#if !defined(NO_INITIAL_SCREEN)
    void printStartMessage(void)
    {
        SET_COLOR(_XL_RED);

        _printCrossShoot();
        
        #if XSize>=16
        PRINT_CENTERED_ON_ROW(5, AUTHOR_STRING);    
        #endif

        _printTopScore();

        #if XSize>= 14 && defined(NO_EXTRA_TITLE)
        PRINT_CENTERED_ON_ROW((YSize>>1)+1, KILL_THEM_ALL__STRING);
        #endif
        
        SET_COLOR(_XL_WHITE);    
            


        #if !defined(NO_CONTROL_INSTRUCTIONS) && XSize>=14
            #if YSize<25
                PRINT_CENTERED_ON_ROW(YSize-1, USE_STRING);
            #else
                PRINT_CENTERED_ON_ROW(YSize-2, USE_STRING);
            #endif
        #endif
    }
#endif


#if !defined(LESS_TEXT)

#define MAX_BONUS 995

void handleLevelBonus(uint16_t bonus)
{
    uint16_t i;
    uint16_t j;
    
    const uint8_t delay = (uint8_t) (MAX_BONUS/bonus); // Minimum bonus = 20. So 995/20<255
    
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    
    for(i=0;i<=bonus;i+=5)
    {
        // delay = (MAX_DELAY+i-bonus)/(bonus-i+1);
        _XL_PRINTD(XSize/2-1,YSize/2,3,i);
        increasePoints(5);
        
        _XL_TICK_SOUND();

        for(j=0;j<(i>>DISPLAY_SPEED_RIGHT_SHIFT);++j)
        {
            SHORT_SLEEP(delay);
        }
    }
}
#endif

#if !defined(NO_ACHIEVEMENTS)
uint8_t countDiscoveredSecrets(void)
{
    uint8_t total;
    uint8_t i;
    
    total = 0;
    #if defined(DEBUG_SECRETS)
    SET_COLOR(_XL_WHITE);    
    #endif
    for(i=0;i<SECRETS_NUMBER;++i)
    {
        #if defined(DEBUG_SECRETS)
        _XL_PRINTD(3,i,2,i);
        #endif

        if(discoveredSecrets[i])
        {
            ++total;
            #if defined(DEBUG_SECRETS)
                _XL_PRINT(8,i,"done");
            #endif
        }
        #if defined(DEBUG_SECRETS)
            else
            {

                _XL_PRINT(8,i,"NOPE");
            }
        #endif
    }

    return total;
}

void printAchievements(void)
{
    uint8_t i;
    uint8_t j;
    uint8_t foundSecrets;
    
    foundSecrets = countDiscoveredSecrets();
    
    SET_COLOR(_XL_RED);    

    _XL_PRINT(1, (YSize>>1)-4,  HIGH_SCORE_STRING);

    _XL_PRINT(1, (YSize>>1)-2,  SCORE_STRING);

    if(level<=FINAL_LEVEL)
    {
        _XL_PRINT(1, (YSize>>1),    LEVEL_STRING);
        _XL_PRINTD(9, (YSize>>1), 2,   level);
    }
    else
    {
        SET_COLOR(_XL_YELLOW);
        _XL_PRINT(1, (YSize>>1), GAME_COMPLETED_STRING);
        SET_COLOR(_XL_RED);    
    }
    _XL_PRINT(1, (YSize>>1)+2,  DISCOVERED_SECRETS_STRING);
    
    _XL_PRINT(12, (YSize>>1)+2, OF_STRING );
    
    _XL_PRINTD(15, (YSize>>1)+2, 2, SECRETS_NUMBER);

    SET_COLOR(_XL_WHITE);    

    _XL_PRINTD(9, (YSize>>1)-4, 5, highScore );

    _XL_PRINTD(9, (YSize>>1)-2, 5, points);

    // if(level<=FINAL_LEVEL)
    // {
    // }

    i=0;
    do
    {
        _XL_PRINTD(9, (YSize>>1)+2, 2, i);
        for(j=0;j<i;++j)
        {
            SHORT_SLEEP(DISPLAY_SPEED_FACTOR);
        }
        _XL_SHOOT_SOUND();
        ++i;
    } while(i<=foundSecrets);
    
    // SHORT_SLEEP(10);
    if(secretLevelActivated)
    {
        SET_COLOR(_XL_YELLOW);    
        _XL_PRINT(1, (YSize>>1)+4, SECRET_LEVEL_FOUND_STRING);
    }
}
#endif

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


#include "ghost.h"
#include "settings.h"
#include "game_text.h"
#include "level.h"
#include "character.h"
#include "init_images.h"

extern Image DEAD_GHOST_IMAGE;
extern Image GHOST_IMAGE;
extern Image FAST_GHOST_IMAGE;

extern uint8_t maxGhostsOnScreen;
extern uint16_t ghostLevel;
extern uint8_t level;

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];

extern uint8_t guns;

extern uint8_t bulletStrength;

extern uint8_t isBossLevel;

extern uint8_t exploded_bombs;

extern uint8_t restart;

void checkBombsVsGhost(register Character * ghostPtr)
{
    uint8_t reachedBombInd = characterReachedBombs(ghostPtr);
    
    if(ghostPtr->_status && reachedBombInd<BOMBS_NUMBER)
    {
        // points+=GHOST_VS_BOMBS_BONUS;
        increasePoints(GHOST_VS_BOMBS_BONUS);
        bombs[reachedBombInd]._status = 0;
        ghostDiesAndSpawns(ghostPtr);
        ++exploded_bombs;
    }
}



void checkBombsVsGhosts(void)
{
    uint8_t i;
    
    for(i=0;i<maxGhostsOnScreen;++i)
      {
         checkBombsVsGhost(&ghosts[i]);
      }
}


void spawnGhost(Character *ghostPtr, uint8_t ghostIndex)
{
    if(!isBossLevel)
    {
        if((ghostCount>=FAST_GHOST_COUNT_MIN_THRESHOLD) && (!(ghostIndex % 4)) && (guns>=FAST_GHOST_GUN_THRESHOLD) && !restart)
        {
            if(!(ghostIndex % 8))
            {
                initializeCharacter(ghostPtr  ,  1      ,     1, FAST_GHOST_LIFE, &FAST_GHOST_IMAGE);
            }
            else
            {
                initializeCharacter(ghostPtr  ,XSize-2, YSize-2, FAST_GHOST_LIFE, &FAST_GHOST_IMAGE);
            }
        }
        else
        {
            switch(ghostIndex % 8)
            {
                case 0:
                    initializeCharacter(ghostPtr  , 1      ,     1, GHOST_LIFE, &GHOST_IMAGE);
                break;
                case 1:
                    initializeCharacter(ghostPtr, XSize-2, YSize-2, GHOST_LIFE, &GHOST_IMAGE);
                break;
                case 2:
                    initializeCharacter(ghostPtr, 1      , YSize-2, GHOST_LIFE, &GHOST_IMAGE);
                break;
                case 3:
                    initializeCharacter(ghostPtr, XSize-2,       1, GHOST_LIFE, &GHOST_IMAGE);
                break;
                case 4:
                    initializeCharacter(ghostPtr, XSize-2, YSize/2, GHOST_LIFE, &GHOST_IMAGE);
                break;
                case 5:
                    initializeCharacter(ghostPtr  , 1    , YSize/2, GHOST_LIFE, &GHOST_IMAGE);
                break;
                case 6:
                    initializeCharacter(ghostPtr, XSize/2,       1, GHOST_LIFE, &GHOST_IMAGE);
                break;
                case 7:
                    initializeCharacter(ghostPtr, XSize/2, YSize-2, GHOST_LIFE, &GHOST_IMAGE);
                break;    
            }
        }
    }
}


void ghostDies(register Character * ghostPtr)
{
    _XL_DRAW(ghostPtr->_x, ghostPtr->_y, _GHOST_TILE, _XL_RED);
    _XL_EXPLOSION_SOUND();
    
    if(ghostPtr->_imagePtr==&FAST_GHOST_IMAGE)
    {
        increasePoints(FAST_GHOST_EXTRA_POINTS);
    }
    
    ghostPtr->_status=0;
    // displayScoreStats();
    
    --ghostCount;
    deleteGhost(ghostPtr);
    // printGhostCountStats();
    displayStats();
}


void ghostDiesAndSpawns(Character * ghostPtr)
{
    ghostDies(ghostPtr);
    if((!isBossLevel) && (ghostCount>=maxGhostsOnScreen))
    {
        spawnGhost(ghostPtr,ghostCount);
    }
}


uint16_t computeGhostSlowDown(void)
{
    if(ghostLevel<MAX_GHOST_LEVEL)
    {
        #if !defined(WIDE)
            return INITIAL_GHOST_SLOWDOWN-ghostLevel*4;        
        #else
            return INITIAL_GHOST_SLOWDOWN-ghostLevel*8;
        #endif
    }
    return GHOST_MIN_SLOWDOWN;
}


void decreaseGhostLevel(void)
{
    if(ghostLevel>GHOST_LEVEL_DECREASE)
        ghostLevel-=GHOST_LEVEL_DECREASE;
    else
        ghostLevel=0;
}


void displayBombs(void)
{
    uint8_t i;

    for(i=0;i<BOMBS_NUMBER;++i)
    {
        if(bombs[i]._status)
        {
            displayBomb(&bombs[i]);
        }
    }
}    


uint8_t sameLocationAsAnyGhostLocation(uint8_t x, uint8_t y, Character *ghostList, uint8_t length)
{
    uint8_t i;

    for(i=0;i<length;++i)
    {
        if(ghostList[i]._status && isCharacterAtLocation(x,y,&(ghostList[i])))
        {
            return i;
        }
    }    
    return length;
}

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

#include "level.h"
#include "strategy.h"

#include "../cross_lib/display/display_macros.h"

extern uint16_t loop;
extern uint8_t level;

extern Character leftHorizontalMissile;
extern Character rightHorizontalMissile;
extern Character player;

extern uint8_t arrowRange;
extern uint8_t arrowYPosition;

extern uint8_t isOneMissileLevel;
extern uint8_t isMissileLevel;
extern uint8_t isBossLevel;

	#if !defined(NO_ARROW_RANGE_CHECK)
    uint8_t _playerInArrowRange(void)
    {
        return (loop&1 && player._y<=(arrowYPosition+arrowRange) && player._y>=(arrowYPosition-arrowRange));
    }
    #endif
    
    void _handle_from_the_left(void)
    {
        if(leftHorizontalMissile._status)
        {
            if(handle_player_killed(&leftHorizontalMissile))
            {
                return;
            }

            deleteHorizontalMissile(&leftHorizontalMissile);
            if(leftHorizontalMissile._x==XSize-2)
            {
                leftHorizontalMissile._x=0;
                leftHorizontalMissile._y =  arrowYPosition;
            }
            else
            {
                ADVANCED_LEFT_MISSILE();
				#if !defined(NO_ARROW_RANGE_CHECK)
                if(_playerInArrowRange())
                {
                    if(player._x>=leftHorizontalMissile._x)
                    {
                        (void) moveCharacter((uint8_t *)&leftHorizontalMissile+Y_MOVE, (uint8_t *)&player+Y_MOVE);			
                    }
                }
				#endif
            }
            displayHorizontalMissile(&leftHorizontalMissile);
            
            handle_player_killed(&leftHorizontalMissile);
        }        
    }
    
    void _handle_from_the_right(void)
    {
        if(rightHorizontalMissile._status)
        {
            if(handle_player_killed(&rightHorizontalMissile))
            {
                return;
            }

            deleteHorizontalMissile(&rightHorizontalMissile);
            if(rightHorizontalMissile._x==1)
            {
                rightHorizontalMissile._x= XSize-1;
                rightHorizontalMissile._y = arrowYPosition;
            }
            else
            {
                ADVANCED_RIGHT_MISSILE();

				#if !defined(NO_ARROW_RANGE_CHECK)
                if(_playerInArrowRange())
                {
                    if(player._x<= rightHorizontalMissile._x)    
                    {
                        (void) moveCharacter((uint8_t *)&rightHorizontalMissile+Y_MOVE, (uint8_t *)&player+Y_MOVE);			
                    }
                }
				#endif
            }
            displayHorizontalMissile(&rightHorizontalMissile);    
            
            handle_player_killed(&rightHorizontalMissile);
        }        
    }
    
    void handle_horizontal_missiles(void)
    {    
        if(isOneMissileLevel)
        {
            arrowYPosition = YSize/2;
            _handle_from_the_right();
        }    
        else if(isMissileLevel || isBossLevel)
        {
            arrowYPosition = HORIZONTAL_MISSILE_OFFSET;
            _handle_from_the_right();
            
            arrowYPosition = YSize-1-HORIZONTAL_MISSILE_OFFSET; 
            _handle_from_the_left();
        }

    }
    

#include "display_macros.h"

#include "tiles.h"

#include "images.h"

#include "init_images.h"

#if !defined(ANIMATE_PLAYER)
	extern Image PLAYER_IMAGE;
#else
	extern Image PLAYER_DOWN_IMAGE;
	extern Image PLAYER_UP_IMAGE;
	extern Image PLAYER_RIGHT_IMAGE;
	extern Image PLAYER_LEFT_IMAGE;	
#endif
	
extern Image GHOST_IMAGE;
extern Image BOMB_IMAGE;

extern Image DESTROYER_IMAGE;

extern Image BOSS_IMAGE;

extern Image SKULL_IMAGE;
extern Image FIRE_CHARGE_IMAGE;
extern Image FIRE_POWER_IMAGE;
extern Image BULLET_IMAGE;
extern Image EXTRA_POINTS_IMAGE;

extern Image HORIZONTAL_BRICK_IMAGE;
extern Image VERTICAL_BRICK_IMAGE;

extern Image LEFT_HORIZONTAL_MISSILE_IMAGE;
extern Image RIGHT_HORIZONTAL_MISSILE_IMAGE;

extern Image ROCKET_IMAGE;

extern Image FREEZE_IMAGE;
extern Image EXTRA_LIFE_IMAGE;
extern Image INVINCIBILITY_IMAGE;
extern Image SUPER_IMAGE;
extern Image CONFUSE_IMAGE;
extern Image SUICIDE_IMAGE;

extern Image BROKEN_BRICK_IMAGE;



void INIT_IMAGES(void)
{
# if !defined(__NO_GRAPHICS)
	// Set color data 
	#if !defined(_XL_NO_COLOR)
		
		#if defined(ANIMATE_PLAYER)
			PLAYER_DOWN_IMAGE._color = _PLAYER_COLOR;
			PLAYER_UP_IMAGE._color = _PLAYER_COLOR;
			PLAYER_RIGHT_IMAGE._color = _PLAYER_COLOR;
			PLAYER_LEFT_IMAGE._color = _PLAYER_COLOR;
		#else
			PLAYER_IMAGE._color = _PLAYER_COLOR;
		#endif	
	
        FAST_GHOST_IMAGE._color = _FAST_GHOST_COLOR;
		BOMB_IMAGE._color = _BOMB_COLOR;
		GHOST_IMAGE._color = _GHOST_COLOR;		
		
		BOSS_IMAGE._color = _DEAD_GHOST_COLOR;
		
        // SKULL_IMAGE._color = _SKULL_COLOR;
        CALM_DOWN_IMAGE._color = _CALM_DOWN_COLOR;
        FIRE_CHARGE_IMAGE._color = _FIRE_CHARGE_COLOR;
        FIRE_POWER_IMAGE._color = _FIRE_POWER_COLOR;
        EXTRA_POINTS_IMAGE._color = _EXTRA_POINTS_COLOR;
        BULLET_IMAGE._color = _BULLET_COLOR;
        
        // VERTICAL_BRICK_IMAGE._color = _BRICK_COLOR;
        // HORIZONTAL_BRICK_IMAGE._color = _BRICK_COLOR;
    
        RIGHT_HORIZONTAL_MISSILE_IMAGE._color = _MISSILE_COLOR;		
        LEFT_HORIZONTAL_MISSILE_IMAGE._color = _MISSILE_COLOR;
        
        ROCKET_IMAGE._color = _ROCKET_COLOR;

        FREEZE_IMAGE._color = _FREEZE_COLOR;
        EXTRA_LIFE_IMAGE._color = _EXTRA_LIFE_COLOR;
        
        INVINCIBILITY_IMAGE._color = _INVINCIBILITY_COLOR;
        SUPER_IMAGE._color = _SUPER_COLOR;
        CONFUSE_IMAGE._color = _CONFUSE_COLOR;
        SUICIDE_IMAGE._color = _SUICIDE_COLOR;
        
        #if !defined(_XL_NO_COLOR) && defined(NO_BLINKING)
        BROKEN_BRICK_IMAGE._color = _BROKEN_BRICK_COLOR;
        #endif
        
        DESTROYER_IMAGE._color = _DESTROYER_COLOR;
	#endif	

	// Set Image Data
	#if defined(ANIMATE_PLAYER)
		PLAYER_DOWN_IMAGE._imageData = _PLAYER_DOWN_TILE;
		PLAYER_UP_IMAGE._imageData = _PLAYER_UP_TILE;
		PLAYER_RIGHT_IMAGE._imageData = _PLAYER_RIGHT_TILE;
		PLAYER_LEFT_IMAGE._imageData = _PLAYER_LEFT_TILE;	
	#else
		PLAYER_IMAGE._imageData = _PLAYER_DOWN_TILE;
	#endif
	
	GHOST_IMAGE._imageData = _GHOST_TILE;
	BOMB_IMAGE._imageData = _BOMB_TILE;
	

	BOSS_IMAGE._imageData = _BOSS_TILE;
	
    SKULL_IMAGE._imageData = _SKULL_TILE;

    CALM_DOWN_IMAGE._imageData = _CALM_DOWN_TILE;
    FIRE_CHARGE_IMAGE._imageData = _BULLET_TILE;
    FIRE_POWER_IMAGE._imageData = _FIRE_POWER_TILE;
    EXTRA_POINTS_IMAGE._imageData = _EXTRA_POINTS_TILE;
    
    BULLET_IMAGE._imageData = _BULLET_TILE;
    
    VERTICAL_BRICK_IMAGE._imageData = _VERTICAL_BRICK_TILE;
    HORIZONTAL_BRICK_IMAGE._imageData = _HORIZONTAL_BRICK_TILE;

    LEFT_HORIZONTAL_MISSILE_IMAGE._imageData = _LEFT_HORIZONTAL_MISSILE_TILE;
    
    RIGHT_HORIZONTAL_MISSILE_IMAGE._imageData = _RIGHT_HORIZONTAL_MISSILE_TILE;
    
    ROCKET_IMAGE._imageData = _ROCKET_TILE;
    
    FREEZE_IMAGE._imageData = _FREEZE_TILE;
    SUPER_IMAGE._imageData = _SUPER_TILE;

    EXTRA_LIFE_IMAGE._imageData = _PLAYER_DOWN_TILE;
    INVINCIBILITY_IMAGE._imageData = _INVINCIBILITY_TILE;
    
    CONFUSE_IMAGE._imageData = _SKULL_TILE;
    SUICIDE_IMAGE._imageData = _GHOST_TILE;
    
    #if !defined(_XL_NO_COLOR) && defined(NO_BLINKING)
    BROKEN_BRICK_IMAGE._imageData = _BOMB_TILE;
    #endif
    
    DESTROYER_IMAGE._imageData = _DESTROYER_TILE;
    FAST_GHOST_IMAGE._imageData = _FAST_GHOST_TILE;
#endif 
}



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

#include "character.h"
#include "item.h"
#include "ghost.h"
#include "game_text.h"
#include "level.h"
#include "text_strings.h"
#include "sleep.h"

#include "cross_lib.h"

extern uint16_t points;

extern uint8_t guns;
extern uint8_t lives;

extern uint8_t level;

extern uint8_t freezeActive;
extern uint8_t freeze_count_down;

extern uint8_t bombCount;

extern uint8_t bulletStrength;

extern uint8_t invincibilityActive;
extern uint8_t confuseActive;

extern uint8_t invincibility_count_down;
extern uint8_t confuse_count_down;

extern uint8_t destroyed_bases_in_completed_levels;
extern uint8_t all_skulls_killed_in_completed_levels;    

extern uint8_t extraLife_present_on_level;
extern uint8_t suicide_present_on_level;

extern Image DEAD_GHOST_IMAGE;

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];
extern Character skull;

extern Character player;

extern Item calmDown;
extern Item fireCharge;
extern Item bombCharge;
extern Item firePower;
extern Item extraPoints;

extern uint8_t maxGhostsOnScreen;

extern uint8_t isBossLevel;

extern Character *chasedEnemyPtr;

extern Item freeze;

extern Item invincibility;

extern Item super;
extern Item extraLife;

extern Item confuse;
extern Item suicide;

extern Item destroyer;

extern uint8_t destroyed_bases;
extern uint8_t exploded_bombs;
extern uint8_t missileDestroyerActivated;
extern uint8_t bombDestroyerActivated;
extern uint8_t destroyerActive;
extern uint8_t destroyer_count_down;

extern uint8_t firePowerItemSecret;
extern uint8_t firePowerLevelSecret;
extern uint8_t fireChargeSecret;

extern uint8_t discoveredSecrets[];

extern Character skulls[];

extern Image SKULL_IMAGE;
extern Image CONFUSE_IMAGE;
extern Image PLAYER_IMAGE;
extern Image BULLET_IMAGE;

extern uint8_t skullActive;


void itemReached(Character * itemPtr)
{
    _XL_ZAP_SOUND();
    // deleteItem(itemPtr);
    // displayPlayer(&player);
    itemPtr->_status = 0;
    // displayScoreStats();
}


void relocateAwayFromWalls(Character * itemPtr)
{        
    do
    {
        relocateNearBy(itemPtr);
    } while(innerVerticalWallReached(itemPtr->_x, itemPtr->_y)||innerHorizontalWallReached(itemPtr->_x,itemPtr->_y));        
}    


void _freezeEffect(void)
{
    decreaseGhostLevel();
    freezeActive = 1;    
    increasePoints(FREEZE_BONUS);
    freeze_count_down += FREEZE_COUNT_DOWN;    
}


void _increaseBullets(uint8_t bullets)
{
    uint8_t missing = MAX_GUNS - guns;
    
    if(missing>=bullets)
    {
        guns+=bullets;
    }
    else
    {
        guns=MAX_GUNS;
        fireChargeSecret = 1;
    }
    printGunsStats();
}


void fireChargeEffect(void)
{
    _increaseBullets(BULLET_GUNS);
    // points+=FIRE_CHARGE_BONUS;
    increasePoints(FIRE_CHARGE_BONUS);
    fireCharge._coolDown = FIRE_CHARGE_COOL_DOWN;        
}


void bombChargeEffect(void)
{
    uint8_t i;
    
    for(i=0;i<BOMBS_NUMBER;++i)
    {
        deleteBomb(&bombs[i]);
    }

    initializeBombs();
    bombCount = 0;
    // TODO: delete Bombs
    bombCharge._coolDown = BOMB_CHARGE_COOL_DOWN; 
    printGunsStats();
}


void calmDownEffect(void)
{
    uint8_t i;
    
    for(i=0;i<CALM_DOWN_EFFECT_FACTOR;++i)
    {
        decreaseGhostLevel();
    }
    // points+=CALM_DOWN_BONUS;
    increasePoints(CALM_DOWN_BONUS);
    freezeActive = 1;
    freeze_count_down += FREEZE_COUNT_DOWN/4;
    calmDown._coolDown = CALM_DOWN_COOL_DOWN*2;    
}


void _firePowerEffect(void)
{
    if(bulletStrength<9)
    {
        ++bulletStrength;
    }
    if(bulletStrength==FIRE_POWER_ITEM_THRESHOLD)
    {
        firePowerItemSecret = 1;
    }
    if(bulletStrength>=FIRE_POWER_LEVEL_THRESHOLD)
    {
        firePowerLevelSecret = bulletStrength-FIRE_POWER_LEVEL_THRESHOLD+3;
    }
    // points+=FIRE_POWER_BONUS;
    increasePoints(FIRE_POWER_BONUS);
    // #if !defined(_XL_NO_COLOR)
        // displayStats();
    // #else
        // printFirePowerStats();
    // #endif
}


void firePowerEffect(void)
{
    _firePowerEffect();
    firePower._coolDown = FIRE_POWER_COOL_DOWN*2;    
}


void extraPointsEffect(void)
{
    
    if(level)
    {
        // points+=EXTRA_POINTS+level*EXTRA_POINTS_LEVEL_INCREASE;
        increasePoints(EXTRA_POINTS+level*EXTRA_POINTS_LEVEL_INCREASE);
        extraPoints._coolDown = SECOND_EXTRA_POINTS_COOL_DOWN;
    }
    else
    {
        // points+=SECRET_LEVEL_EXTRA_POINTS;
        increasePoints(SECRET_LEVEL_EXTRA_POINTS);
        extraPoints._coolDown = 4;
    }
    setSecret(EXTRA_POINTS_EFFECT_SECRET_INDEX);
}


void handle_item(register Item *itemPtr)
{
    // Manage item
    if(itemPtr->_character._status)
    {    
        if(areCharctersAtSamePosition(&player, (Character *) itemPtr))
        {
            itemPtr->_effect();
            itemReached((Character *) itemPtr);            
        }
        else
        {
            _blink_draw(itemPtr->_character._x, itemPtr->_character._y, itemPtr->_character._imagePtr, &(itemPtr->_blink));
        }        
    }
    else if (itemPtr->_coolDown == 0)
    {
        itemPtr->_character._status = 1;
        relocateAwayFromWalls((Character *) itemPtr);

        // TODO: Is this really necessary??
        // _blink_draw(itemPtr->_character._x, itemPtr->_character._y, itemPtr->_character._imagePtr, &(itemPtr->_blink));
    }
    else
    {
        --itemPtr->_coolDown;
    }
}


void handle_count_down(uint8_t * flagPtr, uint8_t * countDownPtr)
{
    if(*flagPtr)
    {
        if(*countDownPtr==0)
        {
            *flagPtr=0;
        }
        else
        {
            --(*countDownPtr);
        }
    }
}    


void reduceItemCoolDowns(void)
{
    extraPoints._coolDown-=extraPoints._coolDown/4;
    invincibility._coolDown-=invincibility._coolDown/8;
    freeze._coolDown-=freeze._coolDown/16;
    fireCharge._coolDown-=fireCharge._coolDown/32;
    _XL_TICK_SOUND();        
}


void freezeEffect(void)
{
    _freezeEffect();
    freeze._coolDown = ((uint16_t) (FREEZE_COOL_DOWN));    
}


void extraLifeEffect(void)
{
    ++lives;
    // all_skulls_killed_in_completed_levels=1;
    // destroyed_bases_in_completed_levels = 0;
    extraLife_present_on_level = 0;
    // extraLife._coolDown = EXTRA_LIFE_COOL_DOWN*10; // second time must be impossible
    printLivesStats();        
    setSecret(EXTRA_LIFE_EFFECT_SECRET_INDEX);
}


void _invincibilityEffect(void)
{
    invincibilityActive = 1;
    // Remark: The invincibility count-down could be higher than the standard threshold if the destroyer is active
    if(invincibility_count_down<INVINCIBILITY_COUNT_DOWN)
    {
        invincibility_count_down = INVINCIBILITY_COUNT_DOWN;
    }
}


void invincibilityEffect(void)
{
    _invincibilityEffect();
    if(level)
    {
        invincibility._coolDown = ((uint16_t) (INVINCIBILITY_COOL_DOWN)*4);
    }
    else
    {
        invincibility._coolDown = SECRET_LEVEL_INVINCIBILITY_COOL_DOWN;
    }
    setSecret(INVINCIBILITY_EFFECT_SECRET_INDEX);
}


void superEffect(void)
{
    _freezeEffect();
    _firePowerEffect();
    _invincibilityEffect();
    super._coolDown = ((uint16_t) (SUPER_COOL_DOWN)*8);
    setSecret(SUPER_EFFECT_SECRET_INDEX);
}


void confuseEffect(void)
{
    uint8_t i;
    uint8_t j;
    
    if(skullActive)
    {

        for(i=0;i<SKULLS_NUMBER;++i)
        {
            if(skulls[i]._status)
            {
                for(j=0;j<5;++j)
                {
                    _XL_DRAW(skulls[i]._x,skulls[i]._y,SKULL_IMAGE._imageData,_XL_GREEN);
                    SHORT_SLEEP(3);
                    _XL_DRAW(skulls[i]._x,skulls[i]._y,SKULL_IMAGE._imageData,_XL_RED);
                    _XL_TOCK_SOUND();
                    SHORT_SLEEP(2);
                }

                if(skulls[i]._status>CONFUSE_DAMAGE)
                {
                    skulls[i]._status-=CONFUSE_DAMAGE;
                }
                // SHORT_SLEEP(3);
                _XL_EXPLOSION_SOUND();
                displaySkull(&skulls[i]);
            }
        }
    }
    confuseActive = 1;
    confuse._coolDown = SECOND_CONFUSE_COOL_DOWN; //20000UL;//(CONFUSE_COOL_DOWN<<4);
    confuse_count_down = CONFUSE_COUNT_DOWN;

    setSecret(CONFUSE_EFFECT_SECRET_INDEX);
}


void suicideEffect(void)
{
    uint8_t i;
    
    destroyed_bases_in_completed_levels = 1;
    if(!level)
    {
        suicide._coolDown = SECOND_SUICIDE_COOL_DOWN/2;
    }
    else
    {
        suicide._coolDown = SECOND_SUICIDE_COOL_DOWN;
    }    
    setSecret(SUICIDE_EFFECT_SECRET_INDEX);
    for(i=0;i<maxGhostsOnScreen;++i)
    {
        if(ghosts[i]._status)
        {
            ghostDiesAndSpawns(&ghosts[i]);
            // points+=GHOST_VS_BOMBS_BONUS;
            increasePoints(GHOST_VS_BOMBS_BONUS);
        }
    }
}


void destroyerEffect(void)
{
    destroyerActive = 1;
    invincibilityActive = 1;
    invincibility_count_down= 255;
    destroyer_count_down = DESTROYER_COUNT_DOWN;
    destroyer._coolDown = DESTROYER_COOL_DOWN;
}


void handle_destroyer_triggers(void)
{
    if((destroyed_bases>=MISSILE_DESTROYER_TRIGGER)&& !missileDestroyerActivated)
    {
        missileDestroyerActivated = 1;
        destroyer._coolDown = 2;
        setSecret(MISSILE_DESTROYER_SECRET_INDEX);
    }
    
    if((exploded_bombs>=BOMB_DESTROYER_TRIGGER)&& !bombDestroyerActivated)
    {
        bombDestroyerActivated = 1;
        destroyer._coolDown = 2;
        setSecret(BOMB_DESTROYER_SECRET_INDEX);
    }
}


void setSecret(uint8_t secretIndex)
{   if(!discoveredSecrets[secretIndex])
    {   
        #if !defined(_XL_NO_COLOR)
        uint8_t i;
        
        for(i=0;i<10;++i)
        {
            _XL_SET_TEXT_COLOR(_XL_RED);
            PRINT_CENTERED_ON_ROW(YSize/2-2,SECRET_FOUND_STRING);
            _XL_TOCK_SOUND();
            SHORT_SLEEP(2);
            _XL_SET_TEXT_COLOR(_XL_YELLOW);
            PRINT_CENTERED_ON_ROW(YSize/2-2,SECRET_FOUND_STRING);
            _XL_TICK_SOUND();
        }
        #else
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        PRINT_CENTERED_ON_ROW(YSize/2-2,SECRET_FOUND_STRING); 
        #endif
        _XL_SLEEP(1);
        PRINT_CENTERED_ON_ROW(YSize/2-2,EMPTY_STRING);        
        discoveredSecrets[secretIndex] = 1;
    }
}


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
 
#include "character.h"
#include "item.h"
#include "item.h"
#include "settings.h"
#include "ghost.h"

#include "cross_lib.h"

#include "level.h"
#include "sleep.h"

extern uint8_t level;

extern uint8_t maxGhostsOnScreen;

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image BOMB_IMAGE;
extern Image BOSS_IMAGE;

extern Image SKULL_IMAGE;
extern Image BULLET_IMAGE;
extern Image FIRE_CHARGE_IMAGE;
extern Image FIRE_POWER_IMAGE;
extern Image EXTRA_POINTS_IMAGE;

extern Image DESTROYER_IMAGE;

extern Character player; 

extern Character skulls[SKULLS_NUMBER];

extern Item fireCharge;
extern Item bombCharge;
extern Item firePower;
extern Item extraPoints;

extern Character bullets[BULLETS_NUMBER];


extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];


extern Item chase;
extern Character chasingBullet;

// extern uint8_t innerVerticalWallX;
extern uint8_t innerVerticalWallY;
extern uint8_t innerVerticalWallLength;

extern uint8_t innerHorizontalWallX;
// extern uint8_t innerHorizontalWallY;
extern uint8_t innerHorizontalWallLength;

extern Image ROCKET_IMAGE;

extern Image CALM_DOWN_IMAGE;
extern Image FREEZE_IMAGE;
extern Image LEFT_HORIZONTAL_MISSILE_IMAGE;
extern Image RIGHT_HORIZONTAL_MISSILE_IMAGE;

extern Image EXTRA_LIFE_IMAGE;
extern Image INVINCIBILITY_IMAGE;
extern Image SUPER_IMAGE;
extern Image CONFUSE_IMAGE;
extern Image SUICIDE_IMAGE;
extern Image BROKEN_BRICK_IMAGE;
extern Image VERTICAL_BRICK_IMAGE;

extern Character leftHorizontalMissile;
extern Character rightHorizontalMissile;

extern Item calmDown;
extern Item freeze;
extern Item extraLife;
extern Item invincibility;
extern Item super;
extern Item confuse;
extern Item suicide;
extern Item destroyer;

extern Character rockets[MAX_ROCKETS_NUMBER];

extern uint8_t rockets_x[MAX_ROCKETS_NUMBER];

extern uint8_t isBossLevel;
extern uint8_t isOneMissileLevel;
extern uint8_t isMissileLevel;
// extern uint8_t isRocketLevel;
extern uint8_t isInnerHorizontalWallLevel;
extern uint8_t isInnerVerticalWallLevel;

extern uint8_t rocketsOnScreen;

extern uint8_t restart;


extern Image HORIZONTAL_BRICK_IMAGE;
extern Image VERTICAL_BRICK_IMAGE;

void DRAW_HORIZONTAL_LINE(uint8_t x,uint8_t y, uint8_t length) 
{
	uint8_t i;
	for(i=0;i<length;++i)
	{
		_draw(x+i,y,&HORIZONTAL_BRICK_IMAGE);
	}
}

void DRAW_VERTICAL_LINE(uint8_t x,uint8_t y, uint8_t length) 
{
	uint8_t i;
	for(i=0;i<length;++i)
	{
		_draw(x,y+i,&VERTICAL_BRICK_IMAGE);
	}        
}

void updateInnerVerticalWall(void)
{    
    #if YSize>17
    uint8_t lvmod;
    
    lvmod = level&7;
    #endif
    
    if(!isInnerVerticalWallLevel)
    {
        innerVerticalWallLength = 0;
    }
    else
    {

        #if YSize>17
            innerVerticalWallLength = YSize-11-lvmod;
        #elif YSize>9
        
            innerVerticalWallLength = 6;
        #else
            innerVerticalWallLength = 3;
        #endif
    }

    // innerVerticalWallX = (XSize>>1);
    innerVerticalWallY = (YSize>>1)-(innerVerticalWallLength>>1);
}


void updateInnerHorizontalWall(void)
{    
    #if (defined(WIDE) || YSize>12)
    uint8_t lvmod = level&7;
    #endif
    
    if(!isInnerHorizontalWallLevel)
    {
        innerHorizontalWallLength = 0;
    }
    else
    {
        #if defined(WIDE)
            innerHorizontalWallLength = XSize-13-lvmod;
        #elif YSize<=12
            innerHorizontalWallLength = 8;
        #else
            innerHorizontalWallLength = XSize-11-lvmod;            
        #endif
    }

    innerHorizontalWallX = (XSize>>1)-(innerHorizontalWallLength>>1);
    // innerHorizontalWallY = (YSize>>1);
}

uint8_t innerHorizontalWallLevel(void)
{
    // 2, 5, 7, 10, 13, 16
    return ((level&7)==2) || ((level&7)==5) || ((level&7)==7);
    // return ((level&7)==2) || ((level&7)==4) || ((level&7)==6);
}

uint8_t innerVerticalWallLevel(void)
{
    // 0, 1, 4, 8, 9, 12, 16
    return ((level&7)==1) || ((level&7)==4) || ((level&7)==0);
}    

uint8_t oneMissileLevel(void)
{
    return (level==3) || (level==4);
    // return ((level&7)==3) || ((level&7)==7) || (level==9);
}

// uint8_t rocketLevel(void)
// {
    // return !level || ((level==3) || (level>=6));
// }

uint8_t missileLevel(void)
{
    return level>=5;
    // return ((level&7)==0) || ((level&7)==6) || ((level&7)==5) || ((level&7)==4);
}    

uint8_t bossLevel(void)
{
    return level && !(level&7);
}

void initializeAwayFromWall(Character * characterPtr, uint8_t x, uint8_t y, uint8_t status, Image *imagePtr)
{
    initializeCharacter(characterPtr, x, y, status, imagePtr);
    relocateAwayFromWalls(characterPtr);
}


#if defined(BETWEEN_LEVEL)

void spiral(register Character *characterPtr, uint8_t length)
{
    uint8_t i;
    uint8_t j;
    
    characterPtr->_x = XSize/2;
    characterPtr->_y = YSize/2;
    for(i=0;i<length;++i)
    {
        for(j=0;j<i/2;++j)
            {
                displayCharacter(characterPtr);        
                if(i&2)
                {
                    ++(*((uint8_t *) characterPtr + (i&1)));
                }
                else
                {
                    --(*((uint8_t *) characterPtr + (i&1)));                
                }
                #if defined(_XL_SLOW_DOWN_FACTOR)
                // SHORT_SLEEP(DISPLAY_SPEED_FACTOR/2);    
                _XL_SLOW_DOWN(180);
                #endif
            }
    }
    _XL_SLEEP(1);
}

#endif

void initializeBombs(void)
{
    uint8_t i;
    
    INITIALIZE_BOMBS();
}

void setNumberOfRocketsOnScreen(void)
{
    if(!level)
    {
        #if MAX_ROCKETS_NUMBER<4
        rocketsOnScreen = MAX_ROCKETS_NUMBER;
        #else
        rocketsOnScreen = 4;
        #endif
    }
    else if(level<=4)
    {
        rocketsOnScreen = 0;
    }
    else if(level<=9)
    {
        rocketsOnScreen = 2;
    }
    else if(level<12)
    {
        #if MAX_ROCKETS_NUMBER<4
        rocketsOnScreen = MAX_ROCKETS_NUMBER;
        #else
        rocketsOnScreen = 4;
        #endif
    }
    else
    {
        rocketsOnScreen = MAX_ROCKETS_NUMBER;
    }    
}

// #define DEBUG_LEVEL

void fillLevelWithCharacters(void)
{
    uint8_t i;
    uint8_t count;
    
    #if !defined(_XL_NO_COLOR)
    switch(level&3)
    {
        case 0:
            HORIZONTAL_BRICK_IMAGE._color = _XL_RED;
            VERTICAL_BRICK_IMAGE._color = _XL_RED;
        break;
        case 1:
            HORIZONTAL_BRICK_IMAGE._color = _XL_GREEN;
            VERTICAL_BRICK_IMAGE._color = _XL_GREEN;
        break;
        case 2:
            HORIZONTAL_BRICK_IMAGE._color = _XL_YELLOW;
            VERTICAL_BRICK_IMAGE._color = _XL_YELLOW;
        break;
        default:
            HORIZONTAL_BRICK_IMAGE._color = _XL_CYAN;
            VERTICAL_BRICK_IMAGE._color = _XL_CYAN;  
    }
    
    #endif
    
    #if !defined(NO_BORDERS)
        DRAW_BORDERS();
    #endif
    

    #if XSize==40
        #define NEXT_ROCKET_INDEX (((i+1)*(XSize))/(rocketsOnScreen+1))
    #else
        #define NEXT_ROCKET_INDEX (i+1)*(XSize/(rocketsOnScreen+1))
    #endif        
        
    for(i=0;i<rocketsOnScreen;i++)
    {
        rockets_x[i] = (uint8_t) NEXT_ROCKET_INDEX;
        initializeCharacter(&rockets[i],(uint8_t) rockets_x[i],(uint8_t)(YSize-1),1,&ROCKET_IMAGE);
        displayRocket(&rockets[i]);
    }
    // }
    
    if(ghostCount>maxGhostsOnScreen)
    {
        count = maxGhostsOnScreen;
    }
    else
    {
        count = ghostCount;
    }
    
    restart = 1;
    for(i=0;i<count;++i)
    {
        spawnGhost(&ghosts[i],i);
        #if defined(DEBUG_LEVEL)
        // _XL_PRINT(0,i,"ghost spawned.......");        
        #endif
        #if defined(DEBUG_LEVEL)
            displayCharacter((Character *)&ghosts[i]);
            _XL_WAIT_FOR_INPUT();
        #endif          
    }
    restart = 0;
    
    for(i=count;i<GHOSTS_NUMBER;++i)
    {
        #if defined(DEBUG_LEVEL)
        PRINT(0,i,"initialized dead ghost......."); 
        #endif        
        initializeCharacter(&ghosts[i],0,0,0,0);
        #if defined(DEBUG_LEVEL)
            displayCharacter((Character *)&ghosts[i]);
        #endif        
    }
    
    initializeBombs();
    
    initializeAwayFromWall(&(calmDown._character),(XSize>>1),(YSize>>1),0,&CALM_DOWN_IMAGE);
    initializeAwayFromWall(&(fireCharge._character),(XSize>>1),(YSize>>1),1,&BULLET_IMAGE);
    
    #if defined(NO_BLINKING)
        #if !defined(_XL_NO_COLOR)
        initializeAwayFromWall(&(bombCharge._character),(XSize>>1),(YSize>>1),0,&BROKEN_BRICK_IMAGE);
        #else
        initializeAwayFromWall(&(bombCharge._character),(XSize>>1),(YSize>>1),0,&VERTICAL_BRICK_IMAGE);
        #endif
    #else
        initializeAwayFromWall(&(bombCharge._character),(XSize>>1),(YSize>>1),0,&BOMB_IMAGE);
    #endif
    initializeAwayFromWall(&(freeze._character),(XSize>>1),(YSize>>1),0,&FREEZE_IMAGE);
    initializeAwayFromWall(&(extraPoints._character), (XSize>>1), (YSize>>1), 0, &EXTRA_POINTS_IMAGE);
    initializeAwayFromWall(&(super._character), (XSize>>1), (YSize>>1), 0, &SUPER_IMAGE);
    initializeAwayFromWall(&(confuse._character), (XSize>>1), (YSize>>1), 0, &CONFUSE_IMAGE);        
    initializeAwayFromWall(&(suicide._character), (XSize>>1), (YSize>>1), 0, &SUICIDE_IMAGE); 
    
    initializeAwayFromWall(&(destroyer._character), (XSize>>1), (YSize>>1), 0, &DESTROYER_IMAGE);    

    initializeAwayFromWall(&(firePower._character),(XSize>>1), (YSize>>1), 0, &FIRE_POWER_IMAGE);

    initializeAwayFromWall(&player,(uint8_t) ((XSize>>1)+(_XL_RAND()&1)),(uint8_t) ((YSize>>1)+(_XL_RAND()&1)),1,&PLAYER_IMAGE);
            
    initializeAwayFromWall(&(extraLife._character), (XSize>>1), (YSize>>1), 0, &EXTRA_LIFE_IMAGE);

    initializeAwayFromWall(&(invincibility._character), (XSize>>1), (YSize>>1), 0, &INVINCIBILITY_IMAGE);

    if(isOneMissileLevel)
    {
        initializeCharacter(&rightHorizontalMissile,         XSize-1,                      (YSize>>1), 1,&RIGHT_HORIZONTAL_MISSILE_IMAGE);            
    }
    else if(isMissileLevel)
    {    
        initializeCharacter(&rightHorizontalMissile,         XSize-1,         HORIZONTAL_MISSILE_OFFSET, 1,&RIGHT_HORIZONTAL_MISSILE_IMAGE);
        initializeCharacter(&leftHorizontalMissile,                0, YSize-1-HORIZONTAL_MISSILE_OFFSET, 1,&LEFT_HORIZONTAL_MISSILE_IMAGE);        
    }        
        
    displayPlayer(&player);
    
    #if !defined(_XL_NO_COLOR)
    BULLET_IMAGE._color = _XL_WHITE;
    #endif
    for(i=0;i<BULLETS_NUMBER;++i)
    {
        initializeCharacter(&bullets[i], 0, 0,0,&BULLET_IMAGE);
    }
    
    if(isBossLevel)
    {
        initializeCharacter(&skulls[BOSS_INDEX],XSize-3,2, BOSS_HITS, &BOSS_IMAGE);
        #if !defined(_XL_NO_COLOR)
        SKULL_IMAGE._color = _XL_CYAN;
        #endif
    }
    else
    {
        initializeCharacter(&skulls[BOSS_INDEX],XSize-3,2, MIN_SKULL_HITS, &SKULL_IMAGE);
        #if !defined(_XL_NO_COLOR)
        SKULL_IMAGE._color = _XL_YELLOW;
        #endif
    }
    initializeCharacter(&skulls[0],XSize-3,YSize-3, MIN_SKULL_HITS+(isBossLevel<<BOSS_LEVEL_SKULL_INCREASE), &SKULL_IMAGE);
    initializeCharacter(&skulls[1],2,YSize-3, MIN_SKULL_HITS+(isBossLevel<<BOSS_LEVEL_SKULL_INCREASE), &SKULL_IMAGE);

    initializeCharacter(&skulls[3],2,2, MIN_SKULL_HITS+(isBossLevel<<BOSS_LEVEL_SKULL_INCREASE), &SKULL_IMAGE);
    
}


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
    return innerVerticalWallReached(x,y) || innerHorizontalWallReached(x,y);
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

    #if defined(NO_INPUT)
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

    #if defined(NO_INPUT)
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


#include "../cross_lib/display/display_macros.h"

#include "settings.h"
#include "character.h"
#include "level.h"

extern uint8_t rocketsOnScreen;

extern Character rockets[MAX_ROCKETS_NUMBER];

extern uint8_t ghostCount;

// extern uint8_t isRocketLevel;
extern uint8_t isBossLevel;

    void handle_rockets(void)
    {    
        if((ghostCount<=MAX_GHOST_COUNT_FOR_ROCKETS) || isBossLevel)
        { 
            uint8_t i;

            for(i=0;i<rocketsOnScreen;++i)
            {
                if(rockets[i]._status)
                {
                    if(handle_player_killed(&rockets[i]))
                    {
                        return;
                    }
                    
                    #if defined(TURNED_BASED)
                    if(1)
                    #else
                    if(_XL_RAND()&1)
                    #endif
                    {
                        deleteRocket(&rockets[i]);
                        --(rockets[i]._y);
                    }

                    displayRocket(&rockets[i]);            
                    if(rockets[i]._y<=1)
                    {    
                        deleteRocket(&rockets[i]);
                        rockets[i]._y = YSize-1;                            
                    }
                    
                    handle_player_killed(&rockets[i]);
                }
            }
        }
    }



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

#include "skull.h"
#include "level.h"
#include "strategy.h"
#include "game_text.h"
#include "item.h"
#include "init_images.h"

extern uint8_t level;
extern uint16_t loop;

extern uint16_t skullSlowDown;
extern uint8_t skullXCountDown;
extern uint8_t skullYCountDown ;

extern uint16_t ghostLevel;

extern uint8_t skullsCount;

extern Character skulls[SKULLS_NUMBER];
extern Character bombs[BOMBS_NUMBER];

extern Character skull;
extern Character player;

extern uint8_t skullActive;

extern uint8_t isBossLevel;

extern uint8_t confuseActive;

extern uint8_t freezeActive;

extern uint8_t maxGhostsOnScreen;

extern Character ghosts[]; 

extern Image GHOST_IMAGE;
extern Image FAST_GHOST_IMAGE;

extern Image BOSS_IMAGE;

// extern uint16_t levelSlowDown;

extern uint8_t exploded_bombs;


uint16_t computeSkullSlowDown(void)
{
    if(loop<MAX_SKULL_LOOP)
    {
        #if !defined(WIDE)
            return INITIAL_GHOST_SLOWDOWN - ghostLevel*4;                
        #else
            return INITIAL_GHOST_SLOWDOWN - ghostLevel*8;        
        #endif
    }
    return SKULL_MIN_SLOWDOWN; // You must die!
}

void computeSkullParameters(void)
{
    skullSlowDown = computeSkullSlowDown();
    skullXCountDown = SKULL_COUNT_DOWN;
    skullYCountDown = SKULL_COUNT_DOWN;
}


// Redability macros
#define INACTIVITY_COUNT_DOWN_REACHED ((skullXCountDown==0) || (skullYCountDown==0))
#define GHOST_COUNT_TRIGGER_REACHED (ghostCount<=SKULL_TRIGGER+(level/4))
#define BOSS_LOOP_TRIGGER_REACHED (loop>=SKULL_LOOP_TRIGGER)

#define NON_BOSS_TRIGGER_REACHED (INACTIVITY_COUNT_DOWN_REACHED || GHOST_COUNT_TRIGGER_REACHED)

#if defined(__NCURSES__)
    #define SKULL_RAND_CONDITION ((_XL_RAND())>skullSlowDown)
#else
    #define SKULL_RAND_CONDITION (_XL_RAND()>skullSlowDown)
#endif


void skullDies(register Character * skullPtr)
{
        // DRAW_BROKEN_BRICK(skullPtr->_x, skullPtr->_y);
        _XL_DRAW(skullPtr->_x, skullPtr->_y,_SKULL_TILE, _XL_RED);
        skullPtr->_status=0;
        if(!(--skullsCount))
        {
            skullActive = 0;
        }
        _XL_EXPLOSION_SOUND();
        // points+=SKULL_POINTS;
        increasePoints(SKULL_POINTS);
        if(isBossLevel)
        {
            // points+=SKULL_POINTS;
            increasePoints(BOSS_LEVEL_SKULL_EXTRA_POINTS);
            if(skullPtr->_imagePtr==&BOSS_IMAGE)
            {
                increasePoints(BOSS_EXTRA_POINTS);
            }
        }

        // displayScoreStats();
        deleteSkull(skullPtr);
}


void handle_skull(Character *skullPtr, uint8_t strategy)
{
    if(skullPtr->_status)
    {
        if(handle_player_killed(skullPtr))
        {
            return;
        }

        if(!freezeActive && SKULL_RAND_CONDITION)
        {
            skullMoveTowardCharacter(skullPtr, strategy);
        }
        displaySkull(skullPtr);
        // if (playerKilledBy(skullPtr))
        // {
            // playerDies();
        // }
        handle_player_killed(skullPtr);
    }
}

// FAST_SPAWN_SPAWN_AND_MASK: frequency (probability) of spawning a fast ghost vs normal ghost
// GHOST_SPAWN_LOOP_END_MASK: frequency of spawning a ghost (or fast ghost)
#if defined(WIDE)
    #define FAST_SPAWN_SPAWN_AND_MASK 1
    #define GHOST_SPAWN_LOOP_END_MASK 31
#else
    #define FAST_SPAWN_SPAWN_AND_MASK 3
    #define GHOST_SPAWN_LOOP_END_MASK 63
#endif

void handle_skulls(void)
{
    if(!skullActive)
    {   
        if((!isBossLevel && NON_BOSS_TRIGGER_REACHED) || 
           (isBossLevel && BOSS_LOOP_TRIGGER_REACHED))
        {
            #if !defined(DEBUG_NO_SKULL)
            skullActive = 1;
            #endif
            
            if(!isBossLevel && INACTIVITY_COUNT_DOWN_REACHED)
            {
                setSecret(SKULLS_ACTIVATED_SECRET_INDEX);
            }
        }
        else
        {
            --skullXCountDown;
            --skullYCountDown;
        }
    }
    else
    {     
        uint8_t i;

        if(!confuseActive || !(loop&3))
        {
            skullSlowDown = computeSkullSlowDown();

            for(i=0;i<SKULLS_NUMBER;++i)
            {
                handle_skull(&skulls[i],i*2);
            }
        }
        
         // Boss spawns only in boss level and: 
         // it spawns only every 32nd loop or 64th loop (non-wide)
         // it spawns only if boss is alive
         // it spawns only if not on vertical wall (always present on boss levels)
        if(isBossLevel && !(loop&GHOST_SPAWN_LOOP_END_MASK) && (ghostCount<BOSS_LEVEL_GHOSTS_NUMBER) && skulls[BOSS_INDEX]._status && skulls[BOSS_INDEX]._x!=XSize/2 )
        {
            i=0;
            while((i<BOSS_LEVEL_GHOSTS_NUMBER)&&(ghosts[i]._status))
            {
                ++i;
            }
            // if(ghostCount<BOSS_LEVEL_GHOSTS_NUMBER)
            // {
            if(!(_XL_RAND()&FAST_SPAWN_SPAWN_AND_MASK))
            {
                initializeCharacter(&ghosts[i],skulls[BOSS_INDEX]._x, skulls[BOSS_INDEX]._y,FAST_GHOST_LIFE,&FAST_GHOST_IMAGE);
            }
            else
            {
                initializeCharacter(&ghosts[i],skulls[BOSS_INDEX]._x, skulls[BOSS_INDEX]._y,GHOST_LIFE,&GHOST_IMAGE);
            }
            ++ghostCount;
            printGhostCountStats();
            // }
            
        }
    }
}

void checkBombsVsSkull(register Character * skullPtr)
{
    uint8_t reachedBombInd = characterReachedBombs(skullPtr);
    
    if(skullPtr->_status && reachedBombInd<BOMBS_NUMBER)
    {        
        // points+=GHOST_VS_BOMBS_BONUS;
        increasePoints(GHOST_VS_BOMBS_BONUS);
        
        bombs[reachedBombInd]._status = 0;
        deleteBomb(&bombs[reachedBombInd]);
        ++exploded_bombs;
        _XL_EXPLOSION_SOUND();
        
        if(skullPtr->_status<=SKULL_BOMB_DAMAGE)
        {
            skullDies(skullPtr);
        }
        else
        {
            skullPtr->_status-=SKULL_BOMB_DAMAGE;
        }
    }
}

void checkBombsVsSkulls(void)
{
    uint8_t i;
    for(i=0;i<SKULLS_NUMBER;++i)
      {
         checkBombsVsSkull(&skulls[i]);
      }
}   


void displaySkulls(void)
{
    uint8_t i;

    for(i=0;i<SKULLS_NUMBER;++i)
    {
        if (skulls[i]._status)
        {
            displaySkull(&skulls[i]);
        }
    }
}




#include "sleep.h"

#include "standard_libs.h"
#include "sleep_macros.h"
#include "cross_lib.h"

// #if _XL_SLOW_DOWN_FACTOR>0
void SHORT_SLEEP(uint8_t t)
{
    uint8_t i;
    
    for(i=0;i<t;++i)
    {
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
}
// #endif


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
#include "character.h"
#include "strategy.h"
#include "level.h"

extern uint8_t level;
extern uint16_t loop;
extern uint8_t ghostCount;

extern Character player; 
extern Character ghosts[GHOSTS_NUMBER];

extern uint8_t strategyArray[GHOSTS_NUMBER];

extern uint8_t innerVerticalWallX;
extern uint8_t innerVerticalWallY;
extern uint8_t innerVerticalWallLength;

extern uint8_t innerHorizontalWallX;
extern uint8_t innerHorizontalWallY;
extern uint8_t innerHorizontalWallLength;

extern uint8_t maxGhostsOnScreen;

extern uint8_t isInnerVerticalWallLevel;

extern uint16_t ghostSlowDown;

extern Image FAST_GHOST_IMAGE;


#define GHOST_RANDOM_CONDITION() ((_XL_RAND())>ghostSlowDown)

#define FAST_GHOST_RANDOM_CONDITION() ((_XL_RAND())>FAST_GHOST_SLOWDOWN)


uint8_t ghost_move_condition(void)
{
    return GHOST_RANDOM_CONDITION();
}

uint8_t fast_ghost_move_condition(void)
{
    return FAST_GHOST_RANDOM_CONDITION();
}


// Required by horizontal missile
uint8_t moveCharacter(register uint8_t *hunterOffsetPtr, register uint8_t *preyOffsetPtr)
{
    if((uint8_t) *((uint8_t *)hunterOffsetPtr) < (uint8_t) *((uint8_t *)preyOffsetPtr))
    {
        ++(*((uint8_t *) hunterOffsetPtr));
    }
    else if((uint8_t) *((uint8_t *) hunterOffsetPtr) > (uint8_t) *((uint8_t *)preyOffsetPtr))
    {
        --(*((uint8_t *) hunterOffsetPtr));
    }    
    else
    {
        return 0;
    }
    return 1;
}


void blindChaseCharacterXStrategy(Character* hunterPtr)
{
    deleteGhost(hunterPtr);
    if(moveCharacter((uint8_t *)hunterPtr + X_MOVE, 
                     (uint8_t *)&player + X_MOVE))
    {
        return;
    }
    else
    {
        (void) moveCharacter((uint8_t *)hunterPtr + Y_MOVE, 
                      (uint8_t *)&player + Y_MOVE);
    }
}


void blindChaseCharacterYStrategy(Character* hunterPtr)
{
    deleteGhost(hunterPtr);
    if(moveCharacter((uint8_t *)hunterPtr + Y_MOVE, 
                     (uint8_t *)&player + Y_MOVE))
    {
        return;
    }
    else
    {
        (void) moveCharacter((uint8_t *)hunterPtr + X_MOVE, 
                      (uint8_t *)&player + X_MOVE);
    }
}


uint8_t inHorizontalCorridor(Character *hunterPtr)
{
    return (hunterPtr->_y<innerVerticalWallY-1) || (hunterPtr->_y>YSize-innerVerticalWallY);
}
    
uint8_t leftSide(Character *characterPtr)
{
    return characterPtr->_x < (XSize/2);
}    

uint8_t rightSide(Character *characterPtr)
{
    return characterPtr->_x > (XSize/2);
}

uint8_t sameSide(Character *hunterPtr)
{
    return ((leftSide(&player) && leftSide(hunterPtr)) || (rightSide(&player) && rightSide(hunterPtr)));
}


// strategy: 
// 4 means do no prefer horizontal to vertical movement
// 0 means always horizontal
// 9 means always vertical
#if !defined(SIMPLE_STRATEGY)
void verticalWallMoveTowardCharacter(register Character *hunterPtr, uint8_t strategy)
{
    if(sameSide(hunterPtr)) // same side
    {
        #if defined(DEBUG_STRATEGY)
        gotoxy(4,1);cprintf("same vertical side      ");           
        #endif
        if((_XL_RAND()&7) > strategy) // Select blind chase strategy
            { // 0 - 4
                blindChaseCharacterXStrategy(hunterPtr);    
            }
            else
            { // 5 - 9
                blindChaseCharacterYStrategy(hunterPtr);
            }            
    }
    else if(inHorizontalCorridor(hunterPtr)) // hunter in vertical corridor
    {
        #if defined(DEBUG_STRATEGY)   
        gotoxy(4,1);cprintf("in horizontal corridor  ");           
        #endif
        blindChaseCharacterXStrategy(hunterPtr);            
    }
    else if((player._x)!=(XSize/2)) // hunter behind the wall
    {
        #if defined(DEBUG_STRATEGY)        
        gotoxy(4,1);cprintf("behind the wall         ");
        #endif
        deleteGhost(hunterPtr);
        if(hunterPtr->_y>(YSize/2))
        {
            ++(hunterPtr->_y);
        } 
        else 
        {
            --(hunterPtr->_y);
        }                
    }
    else // prey aligned with the wall
    {
        #if defined(DEBUG_STRATEGY)
        gotoxy(4,1);cprintf("aligned with the wall  ");          
        #endif
        if(hunterPtr->_x==XSize/2) // both aligned with the wall
        {
            deleteGhost(hunterPtr);
            ++(hunterPtr->_x);
        }
        else
        {
            blindChaseCharacterYStrategy(hunterPtr);
        }            
    }
}
#else
void verticalWallMoveTowardCharacter(register Character *hunterPtr)
{
    if(sameSide(hunterPtr)) // same side
    {
        #if defined(DEBUG_STRATEGY)
        gotoxy(4,1);cprintf("same vertical side      ");           
        #endif
        if((_XL_RAND()&7) > 3) // Select blind chase strategy
            { // 0 - 4
                blindChaseCharacterXStrategy(hunterPtr);    
            }
            else
            { // 5 - 9
                blindChaseCharacterYStrategy(hunterPtr);
            }            
    }
    else if(inHorizontalCorridor(hunterPtr)) // hunter in vertical corridor
    {
        #if defined(DEBUG_STRATEGY)   
        gotoxy(4,1);cprintf("in horizontal corridor  ");           
        #endif
        blindChaseCharacterXStrategy(hunterPtr);            
    }
    else if((player._x)!=(XSize/2)) // hunter behind the wall
    {
        #if defined(DEBUG_STRATEGY)        
        gotoxy(4,1);cprintf("behind the wall         ");
        #endif
        deleteGhost(hunterPtr);
        if(hunterPtr->_y>(YSize/2))
        {
            ++(hunterPtr->_y);
        } 
        else 
        {
            --(hunterPtr->_y);
        }                
    }
    else // prey aligned with the wall
    {
        #if defined(DEBUG_STRATEGY)
        gotoxy(4,1);cprintf("aligned with the wall  ");          
        #endif
        if(hunterPtr->_x==XSize/2) // both aligned with the wall
        {
            deleteGhost(hunterPtr);
            ++(hunterPtr->_x);
        }
        else
        {
            blindChaseCharacterYStrategy(hunterPtr);
        }            
    }
    
}
#endif

uint8_t inVerticalCorridor(Character *hunterPtr)
{
    return (hunterPtr->_x<innerHorizontalWallX-1) || (hunterPtr->_x>XSize-innerHorizontalWallX+1); // TODO: To check this
}

uint8_t topSide(Character *characterPtr)
{
    return characterPtr->_y < (YSize/2);
}    

uint8_t bottomSide(Character *characterPtr)
{
    return characterPtr->_y > (YSize/2);
}

uint8_t sameHorizontalSide(Character *hunterPtr)
{
    return ((topSide(&player) && topSide(hunterPtr)) || (bottomSide(&player) && bottomSide(hunterPtr)));
}


#if !defined(SIMPLE_STRATEGY)
void horizontalWallMoveTowardCharacter(register Character *hunterPtr, uint8_t strategy)
{
    if(sameHorizontalSide(hunterPtr))
    {
        #if defined(DEBUG_STRATEGY)
        gotoxy(4,1);cprintf("same horizontal side  ");   
        #endif
        if((_XL_RAND()&7) > strategy) // Select blind chase strategy
            { // 0 - 4
                blindChaseCharacterXStrategy(hunterPtr);    
            }
            else
            { // 5 - 9
                blindChaseCharacterYStrategy(hunterPtr);
            }            
    }
    else if(inVerticalCorridor(hunterPtr))
    {
        #if defined(DEBUG_STRATEGY) 
        gotoxy(4,1);cprintf("in vertical corridor  ");        
        #endif
        blindChaseCharacterYStrategy(hunterPtr);            
    }
    else if((player._y)!=(YSize/2))
    {
        #if defined(DEBUG_STRATEGY)        
        gotoxy(4,1);cprintf("behind the wall       ");          
        #endif
        deleteGhost(hunterPtr);
        if(hunterPtr->_x>(XSize/2))
        {
            ++(hunterPtr->_x);
        } 
        else 
        {
            --(hunterPtr->_x);
        }                
    }
    else
    {
        #if defined(DEBUG_STRATEGY)        
        gotoxy(4,1);cprintf("aligned with the wall ");         
        #endif
        if(hunterPtr->_y==YSize/2)
        {
            deleteGhost(hunterPtr);
            ++(hunterPtr->_y);
        }
        else
        {
            blindChaseCharacterXStrategy(hunterPtr);
        }            
    }
}
#else
void horizontalWallMoveTowardCharacter(register Character *hunterPtr)
{
    if(sameHorizontalSide(hunterPtr))
    {
        #if defined(DEBUG_STRATEGY)
        gotoxy(4,1);cprintf("same horizontal side  ");   
        #endif
        if((_XL_RAND()&7) > 3) // Select blind chase strategy
            { // 0 - 4
                blindChaseCharacterXStrategy(hunterPtr);    
            }
            else
            { // 5 - 9
                blindChaseCharacterYStrategy(hunterPtr);
            }            
    }
    else if(inVerticalCorridor(hunterPtr))
    {
        #if defined(DEBUG_STRATEGY) 
        gotoxy(4,1);cprintf("in vertical corridor  ");        
        #endif
        blindChaseCharacterYStrategy(hunterPtr);            
    }
    else if((player._y)!=(YSize/2))
    {
        #if defined(DEBUG_STRATEGY)        
        gotoxy(4,1);cprintf("behind the wall       ");          
        #endif
        deleteGhost(hunterPtr);
        if(hunterPtr->_x>(XSize/2))
        {
            ++(hunterPtr->_x);
        } 
        else 
        {
            --(hunterPtr->_x);
        }                
    }
    else
    {
        #if defined(DEBUG_STRATEGY)        
        gotoxy(4,1);cprintf("aligned with the wall ");         
        #endif
        if(hunterPtr->_y==YSize/2)
        {
            deleteGhost(hunterPtr);
            ++(hunterPtr->_y);
        }
        else
        {
            blindChaseCharacterXStrategy(hunterPtr);
        }            
    }
}
#endif

void skullMoveTowardCharacter(Character *hunterPtr, uint8_t strategy)
{
    if((_XL_RAND()&7) > strategy) // Select blind chase strategy
        { // 0 - 4
            blindChaseCharacterXStrategy(hunterPtr);    
        }
        else
        { // 5 - 9
            blindChaseCharacterYStrategy(hunterPtr);
        }
}


#if !defined(SIMPLE_STRATEGY)
void computeStrategy(void)
{
    uint8_t i;
    uint8_t skew = level / 8;
    
    for(i=0; i<2; ++i) // 3 (if total=8)
    {
        strategyArray[i] = 4+skew; // 5,6,7,8 prefer Y (60%, 70%, 80%, 90)
        strategyArray[3-i] = 2-skew; // 3,2,1,0 prefer X (60%, 70%, 80%, 90%)
            
    }
    for(i=4;i<maxGhostsOnScreen;++i)
    {
        strategyArray[i] = 3;
    }
}
#endif


// Ghosts move to new positions if they get their chanche
void chaseCharacter(void)
{
    uint8_t i;
    
    _XL_WAIT_VSYNC();
    #if defined(DEBUG_GHOST_DISPLAY)
        SET_DEBUG_BORDER();
    #endif
    for(i=0;i<maxGhostsOnScreen;++i)
    {
        if(ghosts[i]._status)    
        {
            if(((ghosts[i]._imagePtr==&FAST_GHOST_IMAGE)&&(fast_ghost_move_condition())) || ghost_move_condition())
            {
                if(isInnerVerticalWallLevel)
                {
                    #if !defined(SIMPLE_STRATEGY)
                        verticalWallMoveTowardCharacter((Character *)&ghosts[i], strategyArray[i]);    
                    #else
                        verticalWallMoveTowardCharacter((Character *)&ghosts[i]);        
                    #endif
                }
                else
                {
                    #if !defined(SIMPLE_STRATEGY)
                        horizontalWallMoveTowardCharacter((Character *)&ghosts[i], strategyArray[i]);    
                    #else
                        horizontalWallMoveTowardCharacter((Character *)&ghosts[i]);        
                    #endif
                }
            }
            displayGhost(&ghosts[i]);
        }
    }
    /*
    
    if(isInnerVerticalWallLevel)
    {
        for(i=0;i<maxGhostsOnScreen;++i)
        {
            if(ghosts[i]._status)    
            {
                if(((ghosts[i]._status==2)&&(fast_ghost_move_condition())) || ghost_move_condition())
                {
                    #if !defined(SIMPLE_STRATEGY)
                        verticalWallMoveTowardCharacter((Character *)&ghosts[i], strategyArray[i]);    
                    #else
                        verticalWallMoveTowardCharacter((Character *)&ghosts[i]);        
                    #endif
                }
                displayGhost(&ghosts[i]);
            }
        }
    }
    else
    {
        for(i=0;i<maxGhostsOnScreen;++i)
        {
            if(ghosts[i]._status)
            {
                if(((ghosts[i]._status==2)&&(fast_ghost_move_condition())) || ghost_move_condition())
                {
                    #if !defined(SIMPLE_STRATEGY)
                        horizontalWallMoveTowardCharacter((Character *)&ghosts[i], strategyArray[i]);    
                    #else
                        horizontalWallMoveTowardCharacter((Character *)&ghosts[i]);        
                    #endif
                }
                displayGhost(&ghosts[i]);
            }
        }  
    }
    
    */
    #if defined(DEBUG_GHOST_DISPLAY)
        UNSET_DEBUG_BORDER();
    #endif
}




void resetSecrets(void)
{
    uint8_t i;
    
    for(i=0;i<SECRETS_NUMBER;++i)
    {
        discoveredSecrets[i]=0;
    }
}


void resetItems(void)
{

    calmDown._coolDown = CALM_DOWN_COOL_DOWN;
    firePower._coolDown = FIRE_POWER_COOL_DOWN;
    fireCharge._coolDown = FIRE_CHARGE_COOL_DOWN;
    freeze._coolDown = FREEZE_COOL_DOWN;
    bombCharge._coolDown = BOMB_CHARGE_COOL_DOWN;
    
    extraPoints._coolDown = EXTRA_POINTS_COOL_DOWN;
    invincibility._coolDown = INVINCIBILITY_COOL_DOWN;

    handle_secret_item_at_start_up(calmDownSecret, calmDown, CALM_DOWN_AT_START_SECRET_INDEX);
    handle_secret_item_at_start_up(extraPointsSecret, extraPoints, EXTRA_POINTS_AT_START_SECRET_INDEX);
    handle_secret_item_at_start_up(freezeSecret, freeze, FREEZE_AT_START_SECRET_INDEX);

    handle_secret_item_at_start_up(firePowerItemSecret, firePower, FIRE_POWER_AT_START_SECRET_INDEX);

    super._coolDown = SUPER_COOL_DOWN;
    extraLife._coolDown = EXTRA_LIFE_COOL_DOWN;
    
    confuse._coolDown = CONFUSE_COOL_DOWN;
    suicide._coolDown = SUICIDE_COOL_DOWN;                
    destroyer._coolDown = DESTROYER_COOL_DOWN;
    
    if(!level)
    {
        isBossLevel = 0;
        extraPoints._coolDown = 4;
        invincibility._coolDown = 4;
    }
    
    if(isBossLevel)
    {
        firePower._coolDown<<=1;
        freeze._coolDown<<=1;
        bombCharge._coolDown<<=1;
        super._coolDown<<=1;
        confuse._coolDown<<=1;
    }
}


// Constructor for all items
void constructItems(void)
{
    calmDown._effect = &calmDownEffect;
    fireCharge._effect = &fireChargeEffect;
    bombCharge._effect = &bombChargeEffect;
    firePower._effect = &firePowerEffect;
    extraPoints._effect = &extraPointsEffect;
    freeze._effect = &freezeEffect;
    extraLife._effect = &extraLifeEffect;
    invincibility._effect = &invincibilityEffect;
    super._effect = &superEffect;
    confuse._effect = &confuseEffect;
    suicide._effect = &suicideEffect;
    destroyer._effect = &destroyerEffect;
}    


#if !defined(NO_INITIAL_SCREEN)            
void initialScreen(void)
{    
    _XL_CLEAR_SCREEN();                    
    printStartMessage();
    
    #if !defined(NO_EXTRA_TITLE) && YSize>=17
    display_items();
    #endif 
    
    _XL_WAIT_FOR_INPUT();
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _printCrossShoot();
    _XL_ZAP_SOUND();
    SHORT_SLEEP(3);
    
    #if !defined(NO_HINTS) && XSize>=18
        _XL_CLEAR_SCREEN();
        printHints();    
    #endif
}
#endif

void handle_special_triggers(void)
{
    confuse_present_on_level = !level || destroyed_bases_in_completed_levels;
    suicide_present_on_level = !level || (destroyed_bases_in_completed_levels>=2);
    super_present_on_level = all_skulls_killed_in_completed_levels>=2;
    extraLife_present_on_level = super_present_on_level && suicide_present_on_level && !discoveredSecrets[EXTRA_LIFE_EFFECT_SECRET_INDEX];
}


void handle_player_killed_by_ghost(void)
{
    uint8_t reachedByGhost = sameLocationAsAnyGhostLocation(player._x, player._y, ghosts, maxGhostsOnScreen);
    
    if(destroyerActive && (reachedByGhost < maxGhostsOnScreen))
    {
        points += GHOST_VS_BOMBS_BONUS;
        ghostDiesAndSpawns(&ghosts[reachedByGhost]);
    }
    
    
    if((!invincibilityActive && ((reachedByGhost<maxGhostsOnScreen))))
    {
        playerDies();
    }
}

int main(void)
{        
    _XL_INIT_GRAPHICS();

    _XL_INIT_INPUT();

    _XL_INIT_SOUND();

    highScore = 0;
    
    while(1)
    {
        INIT_IMAGES();
        
        
        #if !defined(NO_INITIAL_SCREEN)
            initialScreen();
            
            _XL_WAIT_FOR_INPUT();
            _XL_SET_TEXT_COLOR(_XL_YELLOW);

            _printCrossShoot();
            _XL_ZAP_SOUND();
            SHORT_SLEEP(9);
            _XL_CLEAR_SCREEN();    
        #else
            _XL_CLEAR_SCREEN();    
            
            printPressKeyToStart();                
        #endif

        #if defined(DEBUG_ITEMS)
            DO_DEBUG_ITEMS();
        #endif
        
        extraLifeThroughPointsCounter = 1;
        points = 0;
        level = INITIAL_LEVEL;     
        lives = LIVES_NUMBER;
        
        ghostCount = FIRST_LEVEL_GHOSTS_NUMBER; 
         
        destroyed_bases_in_completed_levels = 0;
        all_skulls_killed_in_completed_levels = 0;
        
        firePowerItemSecret = 0;
        calmDownSecret = 0;
        extraPointsSecret = 0;
        freezeSecret = 0;
        fireChargeSecret = 0;
        
        secretLevelActivated = 0;
        zeroLevelSecret = 0;
        
        resetSecrets();
        
        do // Level (Re-)Start
        {
            isBossLevel = bossLevel();
            // isRocketLevel = rocketLevel();
            isOneMissileLevel = oneMissileLevel();
            isMissileLevel = missileLevel();
            isInnerHorizontalWallLevel = innerHorizontalWallLevel();
            isInnerVerticalWallLevel = innerVerticalWallLevel();
            setNumberOfRocketsOnScreen();
            
            #if defined(DEBUG_STRATEGY)
            maxGhostsOnScreen = 1;
            #else
            if(!level) // TODO: Do I need this check for the zero level?
            {
                maxGhostsOnScreen = GHOSTS_NUMBER;
            }
            else if(isBossLevel)
            {
                maxGhostsOnScreen = BOSS_LEVEL_GHOSTS_NUMBER;
            }
            else if(isInnerHorizontalWallLevel || isInnerVerticalWallLevel)
            {
                maxGhostsOnScreen = GHOSTS_NUMBER-GHOST_DECREASE;
            }
            else
            {
                maxGhostsOnScreen = GHOSTS_NUMBER;
            }
            #endif
            
            skullsCount = SKULLS_NUMBER;
            
            bombCount = BOMBS_NUMBER;
            
            loop = 0;
            
            ghostLevel = 32*level;
        
            
            destroyed_bases = 0;
            exploded_bombs = 0;
            
            invincibilityActive = 1;                
            invincibility_count_down = INITIAL_INVINCIBILITY_COUNT_DOWN;
            
            destroyerActive = 0;
            missileDestroyerActivated = 0;
            bombDestroyerActivated = 0;
            
            #if !defined(INITIAL_GHOST_FREEZE)
                freezeActive = 0;
            #endif
            confuseActive = 0;

            #if defined(DEBUG_ITEMS_IN_GAME)
                destroyed_bases_in_completed_levels = 2;
                all_skulls_killed_in_completed_levels = 2;

            #endif

            handle_special_triggers();

            #if !defined(SIMPLE_STRATEGY)
                computeStrategy();            
            #endif
                            
            skullActive = 0;
            
            #if defined(INITIAL_GHOST_FREEZE)
                freezeActive = 1;
                freeze_count_down = INITIAL_FROZEN_COUNT_DOWN;
            #endif
            
            computeSkullParameters();                
            
            // levelSlowDown = INITIAL_GHOST_SLOWDOWN;
            ghostSlowDown = computeGhostSlowDown();
            
            _XL_CLEAR_SCREEN();
            #if !defined(LESS_TEXT)
                // Clear the screen, put cursor in upper left corner

                printLevel();
                _XL_SLEEP(1);
                _XL_WAIT_FOR_INPUT();
                _XL_CLEAR_SCREEN();
            #endif
                  
            arrowRange = computeArrowRange();
            
            if(isBossLevel)
            {
                printKillTheSkulls();
                _XL_SLEEP(1);
                _XL_WAIT_FOR_INPUT();
                ghostCount = 0;
            }
            _XL_CLEAR_SCREEN();
            
            updateInnerVerticalWall();    
            
            updateInnerHorizontalWall();                
            
            // printPressKeyToStart();
            // _XL_WAIT_FOR_INPUT();
            // _XL_CLEAR_SCREEN();
            
            playerFire = 0;

            if(fireChargeSecret)
            {
                guns = SECRET_GUNS;
                setSecret(FIRE_CHARGE_AT_START_SECRET_INDEX);
                fireChargeSecret = 0;
            }
            else
            {
                guns = 0;
            }

            if(firePowerLevelSecret)
            {
                bulletStrength = firePowerLevelSecret;
                setSecret(FIRE_POWER_LEVEL_SECRET_INDEX);
                firePowerLevelSecret = 0;
            }
            else
            {
                bulletStrength = 2;
            }

            fillLevelWithCharacters();            
            
            constructItems();    
            
            // displayStatsTitles();
            
            // displayStats();
            
            resetItems();
            

            displayStats();

            while(player._status && (( ((ghostCount>0)&&(skullsCount)) && !isBossLevel) || (skullsCount && isBossLevel))) // while alive && there are still ghosts
            {                
                #if defined(DEBUG_END)
                    gameCompleted();
                #endif
                    
                #if !defined(_XL_TURN_BASED)
                    MOVE_PLAYER();  
                    #if !defined(TRANSITION_ANIMATION)
                        _DRAW_PLAYER();    
                    #endif
                #endif
                        
                handle_rockets();
                handle_horizontal_missiles();
                
                ++loop;
                
                if(points>(extraLifeThroughPointsCounter*EXTRA_LIFE_THROUGH_POINTS))
                {
                    ++extraLifeThroughPointsCounter;
                    _XL_PING_SOUND();
                    ++lives;
                    printLivesStats();
                    _XL_TICK_SOUND();
                }
                
                handle_bullets();
                handle_bomb();

                handle_player_killed_by_ghost();

                #if !defined(NO_CHASE)
                if(!freezeActive)
                {
                    if(ghostCount)
                    {
                        chaseCharacter();
                    }
                    ++ghostLevel;            
                    
                }
                else // Frozen ghosts have to be displayed anyway
                {
                    for(ind=0;ind<GHOSTS_NUMBER;++ind)
                    {
                        if(ghosts[ind]._status)
                        {
                            displayGhost(&ghosts[ind]);
                        }
                    }
                }
                #else
                    for(ind=0;ind<GHOSTS_NUMBER;++ind)
                    {
                        if(ghosts[ind]._status)
                        {
                            displayGhost(&ghosts[ind]);
                        }
                    }
                #endif
                                
                
                #if !defined(DEBUG_STRATEGY) && !defined(NO_CHASE)
                handle_skulls();
                #endif
            
                // This detects collisions of ghosts that have just moved
                checkBullets();

                
                #if defined(TRANSITION_ANIMATION)
                    if(moved)
                    {
                        _XL_DELETE(old_x,old_y);
                        moved = 0;
                    }
                    _DRAW_PLAYER();
                #endif                
                
                // Check collisions bombs vs ghosts
                checkBombsVsGhosts();
                if(skullActive)
                {
                    checkBombsVsSkulls();
                }
                
                SHORT_SLEEP(3);
                if(invincibilityActive)
                {
                    #if _XL_SLOW_DOWN_FACTOR>0
                    SHORT_SLEEP(1);
                    #endif
                    _XL_DELETE(player._x, player._y);
                }

                SHORT_SLEEP(3);
                
                handle_calmDown_item();
                handle_extraPoints_item();
                handle_firePower_item();
                handle_fireCharge_item();
                handle_bombCharge_item();    
                
                handle_freeze_count_down();                    
                
                handle_freeze_item();    
                handle_invincibility_item();
                handle_invincibility_count_down();                    

                handle_destroyer_triggers();
                handle_destroyer_item();
                handle_destroyer_count_down();
                    
                if(super_present_on_level)
                {
                    handle_super_item();
                    if(extraLife_present_on_level)
                    {
                        handle_extraLife_item();
                    }
                }
                
                                    
                if(confuse_present_on_level)
                {
                    handle_confuse_item();
                    handle_confuse_count_down();
                    if(suicide_present_on_level)
                    {
                        handle_suicide_item();
                    }
                }              
                
                handle_player_killed_by_ghost();
                // reachedByGhost = sameLocationAsAnyGhostLocation(player._x, player._y, ghosts, maxGhostsOnScreen);
                
                // if(destroyerActive && (reachedByGhost < maxGhostsOnScreen))
                // {
                    // points += GHOST_VS_BOMBS_BONUS;
                    // ghostDiesAndSpawns(&ghosts[reachedByGhost]);
                // }
                
                
                // if((!invincibilityActive && ((reachedByGhost<maxGhostsOnScreen))))
                // {
                    // playerDies();
                // }

                    
                SKIP_WALL_DRAW
                {
                    ghostSlowDown = computeGhostSlowDown();
                    
                    if(isInnerVerticalWallLevel)
                    {
                        DRAW_VERTICAL_LINE(XSize/2, YSize/2-(innerVerticalWallLength/2), innerVerticalWallLength);
                    }
                    else if(isInnerHorizontalWallLevel)
                    {
                        #if defined(BUGGY_HORIZ_DRAW)
                        DRAW_HORIZONTAL_LINE(1+(XSize>>1)-(innerHorizontalWallLength>>1), YSize/2, innerHorizontalWallLength);
                        #else
                        DRAW_HORIZONTAL_LINE((XSize>>1)-(innerHorizontalWallLength>>1), YSize/2, innerHorizontalWallLength);
                        #endif
                    }
                    displayBombs();                                                        
                }
                
            }; // end inner while [while (player._alive && ghostCount>0), i.e., exit on death or end of level]

            if(player._status) // if level finished
            {
                _DRAW_PLAYER();
                #if defined(BETWEEN_LEVEL)
                    chasedEnemyPtr = &player;
                    SHOW_DOWN();
                #endif
                
                #if !defined(LESS_TEXT)
                    _XL_SLEEP(1);
                    printVictoryMessage();
                    _XL_WAIT_FOR_INPUT();

                    // _XL_CLEAR_SCREEN();
                #endif

                handleLevelBonus(((uint16_t)BASE_LEVEL_BONUS)+((uint16_t) LEVEL_BONUS)*((uint16_t) level)+ghostCount*GHOSTS_LEFT_BONUS);

                if(!isBossLevel)
                {
                    if(ghostCount>=CALM_DOWN_SECRET_THRESHOLD)
                    {
                        calmDownSecret = 1;
                        if(ghostCount>=EXTRA_POINTS_SECRET_THRESHOLD)
                        {
                            extraPointsSecret = 1;
                            if(ghostCount>=FREEZE_SECRET_THRESHOLD)
                            {
                                freezeSecret = 1;
                                if((ghostCount>=SECRET_LEVEL_THRESHOLD) && !secretLevelActivated)
                                {
                                    zeroLevelSecret = 1;
                                }
                            }
                        }
                    }
                }

                _XL_SLEEP(1);
                _XL_WAIT_FOR_INPUT();
                
                if(isBossLevel && level!=FINAL_LEVEL)
                {    
                    all_skulls_killed_in_completed_levels = 1;
                    destroyed_bases_in_completed_levels/=2;
                }
                else
                {
                    if(!skullsCount)
                    {
                        ++all_skulls_killed_in_completed_levels;
                    }
                    destroyed_bases_in_completed_levels+=destroyed_bases;
                }
                
                if(zeroLevelSecret)
                {
                    nextLevel = level+1;
                    level = 0;
                    secretLevelActivated = 1;
                    zeroLevelSecret = 0;
                    setSecret(ZERO_LEVEL_SECRET_INDEX);
                    ghostCount = ZERO_LEVEL_GHOSTS_NUMBER;
                }
                else
                {
                    if(!level)
                    {
                        // Restart at the next level after the one you left when entering the secret zero level
                        level = nextLevel;
                    }
                    else
                    {
                        ++level;
                    }
                    if(level>8)
                    {
                        ghostCount = NUMBER_OF_GHOSTS_ON_PART_II;
                    }
                    else
                    {
                        ghostCount = FIRST_LEVEL_GHOSTS_NUMBER + 2*level;
                    }
                }
                

            }
            else // if dead
            {        
                #if defined(BETWEEN_LEVEL)
                    chasedEnemyPtr = &skulls[0];    
                #endif
                
                _XL_CLEAR_SCREEN();
                --lives;
                if(lives>0)
                {
                    player._status = 1;
                }
                firePowerItemSecret = 0;
                calmDownSecret = 0;
                extraPointsSecret = 0;
                freezeSecret = 0;
                fireChargeSecret = 0;
                firePowerLevelSecret = 0;
                zeroLevelSecret = 0;
            }
            #if defined(BETWEEN_LEVEL)
                spiral(chasedEnemyPtr, SPIRAL_LOOPS);
                // _XL_SLEEP(1);
            #endif
            
        } while (player._status && (level<(FINAL_LEVEL+1))); // lives left and not completed game game     

        // GAME OVER    

        if(level==FINAL_LEVEL+1) // if completed game
        {
            // uint8_t i;
            // uint8_t lives_left = lives;
            do
            {
                points+=LIVES_LEFT_BONUS;
                --lives;
                displayScoreStats();
                printLivesStats();
                _XL_ZAP_SOUND();
                _XL_SLEEP(1);
            } while(lives);
            _XL_SLEEP(2);
            _XL_WAIT_FOR_INPUT();
            gameCompleted();
            _XL_SLEEP(1);
        }

        if(points>highScore)
        {
            highScore = points;
        }

        _XL_CLEAR_SCREEN();
		#if !defined(NO_ACHIEVEMENTS)
        printAchievements();
        _XL_SLEEP(1);
        _XL_WAIT_FOR_INPUT();
        
        _XL_CLEAR_SCREEN();
		#endif
        printGameOver();
        
        _XL_SLEEP(1);
        _XL_WAIT_FOR_INPUT();
        
    } // while(1) -> restart from the beginning

    return EXIT_SUCCESS;
}

