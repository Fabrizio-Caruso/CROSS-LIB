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

#if defined(__CMOC__) && !defined(__WINCMOC__)
    #include <cmoc.h>
#else
    #include <stdlib.h>
#endif

#include "../cross_lib/cross_lib.h"

#include "level.h"

// #define DEBUG

extern uint8_t level;

extern uint8_t ghostsOnScreen;

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image BOMB_IMAGE;
extern Image DEAD_GHOST_IMAGE;

extern Image SKULL_IMAGE;
extern Image BULLET_IMAGE;
extern Image FIRE_CHARGE_IMAGE;
extern Image FIRE_POWER_IMAGE;
extern Image EXTRA_POINTS_IMAGE;

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

extern uint8_t innerVerticalWallX;
extern uint8_t innerVerticalWallY;
extern uint8_t innerVerticalWallLength;

extern uint8_t innerHorizontalWallX;
extern uint8_t innerHorizontalWallY;
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

extern Character leftHorizontalMissile;
extern Character rightHorizontalMissile;

extern Item calmDown;
extern Item freeze;
extern Item extraLife;
extern Item invincibility;
extern Item super;
extern Item confuse;
extern Item suicide;

extern Character rockets[ROCKETS_NUMBER];

extern char rockets_x[ROCKETS_NUMBER];

extern char skullsKilled;

extern uint8_t isBossLevel;
extern uint8_t isOneMissileLevel;
extern uint8_t isMissileLevel;
extern uint8_t isRocketLevel;
extern uint8_t isInnerHorizontalWallLevel;
extern uint8_t isInnerVerticalWallLevel;


void updateInnerVerticalWall(void)
{    
    uint8_t lvmod;

    lvmod = level&7;
    
    if(!isInnerVerticalWallLevel)
    {
        innerVerticalWallLength = 0;
    }
    else if(isBossLevel)
    {
        innerVerticalWallLength = 6;
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

    innerVerticalWallX = (XSize>>1);
    innerVerticalWallY = (YSize>>1)-(innerVerticalWallLength>>1);
}


void updateInnerHorizontalWall(void)
{    
    uint8_t lvmod = level&7;
    
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
    innerHorizontalWallY = (YSize>>1);
}

uint8_t innerHorizontalWallLevel(void)
{
    return ((level&7)==2) || ((level&7)==4) || ((level&7)==6);
}

uint8_t innerVerticalWallLevel(void)
{
    return ((level&7)==1) || ((level&7)==3) || ((level&7)==5) || bossLevel();
}    

uint8_t oneMissileLevel(void)
{
    return ((level&7)==3) || ((level&7)==7);
}

uint8_t rocketLevel(void)
{
    return ((level&7)==3) || ((level&7)==6) || ((level&7)==7) || ((level&7)==0);
}

uint8_t missileLevel(void)
{
    return ((level&7)==0) || ((level&7)==6) || ((level&7)==5) || ((level&7)==4);
}    

uint8_t bossLevel(void)
{
    return !(level&7);
}

void initializeAwayFromWall(Character * characterPtr, uint8_t x, uint8_t y, uint8_t status, Image *imagePtr)
{
    initializeCharacter(characterPtr, x, y, status, imagePtr);
    relocateAwayFromWalls(characterPtr);
}


#if defined(BETWEEN_LEVEL)

void _spiral_slow_down()
{
    uint8_t k;
    
    for(k=0;k<254;++k){};
}

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
                #if defined(SLOW_DOWN)
                _spiral_slow_down();    
                #endif
            }
    }
}

#endif

void placeBombs(void)
{
    uint8_t i;
    
    PLACE_BOMBS();
}


void fillLevelWithCharacters(void)
{
    uint8_t i;
    uint8_t count;
    
    #if defined(DEBUG)
    gotoxy(1,1);cprintf("filling level.........");
    #endif
    if(isRocketLevel || isBossLevel)
    {
        for(i=0;i<ROCKETS_NUMBER;i++)
        {
            rockets_x[i] = (uint8_t) (i+1)*(XSize/(ROCKETS_NUMBER+1));
            initializeCharacter(&rockets[i],(uint8_t) rockets_x[i],(uint8_t)(YSize-1),1,&ROCKET_IMAGE);
            displayRocket(&rockets[i]);
        }
    }
    
    if(ghostCount>ghostsOnScreen)
    {
        count = ghostsOnScreen;
    }
    else
    {
        count = ghostCount;
    }
    
    for(i=0;i<count;++i)
    {
        #if defined(DEBUG)
        gotoxy(0,0);cprintf("count %d", count);SLEEP(2);
        gotoxy(1,1);cprintf("spawning ghost......");        
        #endif
        spawnGhost(&ghosts[i],i);
        #if defined(DEBUG)
        gotoxy(1,1);cprintf("ghost spawned.......");        
        #endif
        #if defined(DEBUG)
            displayCharacter((Character *)&ghosts[i]);
        #endif          
    }
    
    for(i=count;i<GHOSTS_NUMBER;++i)
    {
        #if defined(DEBUG)
        gotoxy(1,1);cprintf("initialized dead ghost......."); 
        #endif        
        initializeCharacter(&ghosts[i],0,0,0,NULL);
        #if defined(DEBUG)
            displayCharacter((Character *)&ghosts[i]);
        #endif        
    }
    
    
    placeBombs();
    
        initializeAwayFromWall(&(calmDown._character),(XSize>>1),(YSize>>1),0,&CALM_DOWN_IMAGE);
        initializeAwayFromWall(&(fireCharge._character),(XSize>>1),(YSize>>1),1,&BULLET_IMAGE);
        initializeAwayFromWall(&(bombCharge._character),(XSize>>1),(YSize>>1),0,&BOMB_IMAGE);
        initializeAwayFromWall(&(freeze._character),(XSize>>1),(YSize>>1),0,&FREEZE_IMAGE);
        initializeAwayFromWall(&(extraPoints._character), (XSize>>1), (YSize>>1), 0, &EXTRA_POINTS_IMAGE);
        initializeAwayFromWall(&(super._character), (XSize>>1), (YSize>>1), 0, &SUPER_IMAGE);
        initializeAwayFromWall(&(confuse._character), (XSize>>1), (YSize>>1), 0, &CONFUSE_IMAGE);        
        initializeAwayFromWall(&(suicide._character), (XSize>>1), (YSize>>1), 0, &SUICIDE_IMAGE);    
        
        initializeAwayFromWall(&(firePower._character),(XSize>>1), (YSize>>1), 0, &FIRE_POWER_IMAGE);

        initializeAwayFromWall(&player,(uint8_t) ((XSize>>1)+(rand()&1)),(uint8_t) ((YSize>>1)+(rand()&1)),1,&PLAYER_IMAGE);
                
        initializeAwayFromWall(&(extraLife._character), (XSize>>1), (YSize>>1), 0, &EXTRA_LIFE_IMAGE);

        initializeAwayFromWall(&(invincibility._character), (XSize>>1), (YSize>>1), 0, &INVINCIBILITY_IMAGE);

        if(isOneMissileLevel)
        {
            initializeCharacter(&rightHorizontalMissile,         XSize-1,                      (YSize>>1), 1,&RIGHT_HORIZONTAL_MISSILE_IMAGE);            
        }
        else if(isMissileLevel || isBossLevel)
        {    
            initializeCharacter(&rightHorizontalMissile,         XSize-1,         HORIZONTAL_MISSILE_OFFSET, 1,&RIGHT_HORIZONTAL_MISSILE_IMAGE);
            initializeCharacter(&leftHorizontalMissile,                0, YSize-1-HORIZONTAL_MISSILE_OFFSET, 1,&LEFT_HORIZONTAL_MISSILE_IMAGE);        
        }        
        

    displayPlayer(&player);
        
    for(i=0;i<BULLETS_NUMBER;++i)
    {
        initializeCharacter(&bullets[i], 0, 0,0,&BULLET_IMAGE);
    }
    

    initializeCharacter(&skulls[0],XSize-3,YSize-3, MIN_SKULL_HITS+(isBossLevel<<BOSS_LEVEL_SKULL_INCREASE), &SKULL_IMAGE);
    initializeCharacter(&skulls[1],2,YSize-3, MIN_SKULL_HITS+(isBossLevel<<BOSS_LEVEL_SKULL_INCREASE), &SKULL_IMAGE);
    if(isBossLevel)
    {
        initializeCharacter(&skulls[BOSS_INDEX],XSize-3,2, BOSS_HITS, &DEAD_GHOST_IMAGE);
    }
    else
    {
        initializeCharacter(&skulls[BOSS_INDEX],XSize-3,2, MIN_SKULL_HITS, &SKULL_IMAGE);
    }
    initializeCharacter(&skulls[3],2,2, MIN_SKULL_HITS+(isBossLevel<<BOSS_LEVEL_SKULL_INCREASE), &SKULL_IMAGE);
    
}

