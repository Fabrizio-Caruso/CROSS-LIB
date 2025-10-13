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
                #if !defined(NO_HORIZONTAL_LEVEL)
                if(wallReached(ghostPtr) || innerVerticalWallReached(ghostPtr->_x, ghostPtr->_y) || innerHorizontalWallReached(ghostPtr->_x, ghostPtr->_y))
                #else
                if(wallReached(ghostPtr) || innerVerticalWallReached(ghostPtr->_x, ghostPtr->_y))
                #endif
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
    #if !defined(NO_HORIZONTAL_LEVEL)
    if(bulletPtr->_status && (wallReached(bulletPtr) || innerVerticalWallReached(bulletPtr->_x, bulletPtr->_y) || innerHorizontalWallReached(bulletPtr->_x, bulletPtr->_y)))
    #else
    if(bulletPtr->_status && (wallReached(bulletPtr) || innerVerticalWallReached(bulletPtr->_x, bulletPtr->_y)))
    #endif
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


