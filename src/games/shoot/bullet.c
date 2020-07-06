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

extern uint8_t bombCount;
extern Character bombs[BOMBS_NUMBER];

extern uint8_t skullsCount;

extern uint8_t bulletStrength;

extern uint16_t points;

extern uint16_t loop;

extern uint8_t isOneMissileLevel;
extern uint8_t isMissileLevel;
extern uint8_t isBossLevel;
extern uint8_t isRocketLevel;

extern Character ghosts[GHOSTS_NUMBER];
#if !defined(NO_DEAD_GHOSTS)
extern Image DEAD_GHOST_IMAGE;
#else
extern Image SKULL_IMAGE;    
#endif
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

extern uint8_t ghostsOnScreen;

extern uint8_t rocketsOnScreen;

extern Item freeze;
extern Item extraLife;
extern Item invincibility;
    
extern Character leftHorizontalMissile;
extern Character rightHorizontalMissile;
extern uint8_t rockets_x[ROCKETS_NUMBER];
extern Character rockets[ROCKETS_NUMBER];
extern uint8_t dead_rockets;
extern uint8_t missileBasesDestroyed;


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


void handle_bomb(void)
{
    // Check if player has fired the gun
    if(playerFire && bombCount<BOMBS_NUMBER)
    {
        SHOOT_SOUND();
        
        deleteCharacter(&bombs[bombCount]);
        
        bombs[bombCount]._status=1;
        
        if(playerDirection==RIGHT)
        {
            bombs[bombCount]._x = player._x+1; 
        }
        else if(playerDirection==LEFT)
        {
            bombs[bombCount]._x = player._x-1;             
        }
        else
        {
            bombs[bombCount]._x = player._x;            
        }
        
        if(playerDirection==UP)
        {
            bombs[bombCount]._y = player._y-1;        
        }
        else if(playerDirection==DOWN)
        {
            bombs[bombCount]._y = player._y+1;        
        }
        else
        {
            bombs[bombCount]._y = player._y;        
        }
        
        displayBomb(&bombs[bombCount]);

        ++bombCount;

        playerFire = 0;    
    }
}        


void handle_bullet_fire(uint8_t bulletIndex)
{
    // Check if player has fired the gun
    if(playerFire && bullets[bulletIndex]._status==0 && guns>0 && bombCount==BOMBS_NUMBER)
    {
        SHOOT_SOUND();
        --guns;
        printGunsStats();
        bulletDirection[bulletIndex] = playerDirection;
        bullets[bulletIndex]._status = 1;
        
        if(playerDirection==LEFT)
        {
            bullets[bulletIndex]._x = player._x-1;             
        }
        else if(playerDirection==RIGHT)
        {
            bullets[bulletIndex]._x = player._x+1;                
        }
        else
        {
            bullets[bulletIndex]._x = player._x;                            
        }
        
        if(playerDirection==UP)
        {
            bullets[bulletIndex]._y = player._y-1;        
        }
        else if(playerDirection==DOWN)
        {
            bullets[bulletIndex]._y = player._y+1;        
        }
        else
        {
            bullets[bulletIndex]._y = player._y;        
        }
        displayBullet(&bullets[bulletIndex]);
        playerFire = 0;    
        checkBullet(&bullets[bulletIndex],playerDirection);
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
        case RIGHT:
            ++(ghostPtr->_x);
        break;        
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


void checkBulletVsGhost(Character * bulletPtr,
                        uint8_t bulletDirection,
                        Character * ghostPtr)
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
            points+=GHOST_VS_MISSILE;
        }
        else
        {
            (ghostPtr->_status)-=bulletStrength;
            displayGhost(ghostPtr);
        
            for(i=0;i<bulletStrength/2;++i)
            {

                deleteGhost(ghostPtr);
                pushGhost(ghostPtr, bulletDirection);
                if(wallReached(ghostPtr) || innerVerticalWallReached(ghostPtr) || innerHorizontalWallReached(ghostPtr))
                {
                    ghostDies(ghostPtr);
                    points+=GHOST_VS_WALL_BONUS;
                    DRAW_BROKEN_BRICK(ghostPtr->_x, ghostPtr->_y);
                    if((!isBossLevel) && (ghostCount>=ghostsOnScreen))
                        {
                            spawnGhost(ghostPtr,ghostCount);
                        }
                    #if !defined(WIDE) && !defined(FORCE_NARROW)
                        displayStatsTitles();
                        displayStats();
                    #endif
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
    
    for(;i<ghostsOnScreen;++i)
    {
        if(ghosts[i]._status)
        {
            checkBulletVsGhost(bulletPtr, bulletDirection, &ghosts[i]);
        }
    };
}


void checkBulletVsSkull(register Character *bulletPtr, Character *skullPtr)
{
    if(skullPtr->_status && 
       areCharctersAtSamePosition(bulletPtr, skullPtr))
    {
        PING_SOUND();
        
        bulletPtr->_status=0;
        
        deleteBullet(bulletPtr);
        bulletPtr->_x = 0; bulletPtr->_y = 0;
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
        case LEFT:
            --bulletPtr->_x;
        break;
    }    
}


void destroyHorizontalMissile(Character * horizontalMissilePtr)
{
    horizontalMissilePtr->_status = 0;
    EXPLOSION_SOUND();
    deleteHorizontalMissile(horizontalMissilePtr);
    points+=HORIZONTAL_MISSILE_BONUS;
    displayScoreStats();                
    ++dead_rockets;
    reduceItemCoolDowns();        
}


void moveBullet(register Character * bulletPtr, uint8_t bulletDirection)
{
    if((wallReached(bulletPtr) || innerVerticalWallReached(bulletPtr) || innerHorizontalWallReached(bulletPtr)) && bulletPtr->_status)
    {
        bulletPtr->_status=0;

        if(isOneMissileLevel)
        {
            if(bulletPtr->_x==XSize-1 && bulletPtr->_y==YSize/2 && rightHorizontalMissile._status)
            {
                destroyHorizontalMissile(&rightHorizontalMissile);
            }
        }                
        else if(isMissileLevel || isBossLevel)
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
        if((isRocketLevel || isBossLevel) && bulletPtr->_y==YSize-1)
        {
            uint8_t i;
            for(i=0;i<rocketsOnScreen;++i)
            {
                if(bulletPtr->_x==rockets_x[i] && rockets[i]._status)
                {
                    rockets[i]._status = 0;
                    ++dead_rockets;
                    EXPLOSION_SOUND();
                    deleteRocket(&rockets[i]);
                    points+=VERTICAL_MISSILE_BONUS;
                    displayScoreStats();        
                }
            }
        }            
        DRAW_BROKEN_BRICK(bulletPtr->_x, bulletPtr->_y);
        #if !defined(WIDE) && !defined(FORCE_NARROW)
            displayStatsTitles();
            displayStats();
        #endif
    }
    else
    {
        _moveBullet(bulletPtr, bulletDirection);
        displayBullet(bulletPtr);
    }
}


