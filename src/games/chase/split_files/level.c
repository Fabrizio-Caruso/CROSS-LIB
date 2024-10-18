/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
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
#include "level.h"


#include "cross_lib.h"


extern uint8_t level;

#if defined(FULL_GAME)
    extern uint8_t isBossLevel;
    extern uint8_t isOneMissileLevel;
    extern uint8_t isMissileLevel;
    extern uint8_t isRocketLevel;
    extern uint8_t isHorizontalWallsLevel;
#endif

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image BOMB_IMAGE;
extern Image DEAD_GHOST_IMAGE;

#if !defined(TINY_GAME)
    extern Image SKULL_IMAGE;
    extern Image BULLET_IMAGE;
    extern Image POWERUP_IMAGE;
    extern Image GUN_IMAGE;
    extern Image EXTRA_POINTS_IMAGE;
#endif

extern Character player; 

#if !defined(TINY_GAME)
    extern Character skull;

    extern Item powerUp;
    extern Item gun;
    extern Item extraPoints;

    extern Character bullet;

#endif

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];


#if defined(FULL_GAME) 
    extern Item powerUp2;

    extern Item chase;
    extern Character chasingBullet;

    extern uint8_t verticalWallY;
    extern uint8_t verticalWallLength;

    extern Image ROCKET_IMAGE;

    extern Image FREEZE_IMAGE;
    extern Image LEFT_HORIZONTAL_MISSILE_IMAGE;
    extern Image RIGHT_HORIZONTAL_MISSILE_IMAGE;

    extern Image EXTRA_LIFE_IMAGE;
    extern Image INVINCIBILITY_IMAGE;
    extern Image SUPER_IMAGE;
    extern Image CONFUSE_IMAGE;
    extern Image ZOMBIE_IMAGE;

    extern Character leftHorizontalMissile;
    extern Character rightHorizontalMissile;

    extern Item freeze;
    extern Item extraLife;
    extern Item invincibility;
    extern Item super;
    extern Item confuse;
    extern Item zombie;
    
    extern Character rockets[ROCKETS_NUMBER];

    extern uint8_t rockets_x[ROCKETS_NUMBER];
    
    extern uint8_t all_skulls_killed_in_completed_levels;
#endif


#if !defined(NO_WALL) && !defined(TINY_GAME)

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
#endif

#if defined(FULL_GAME)
    void updateInnerWallVerticalData(void)
    {    
        uint8_t lvmod = level%5;

        if((lvmod==1)||(lvmod==0))
        {
            verticalWallLength = 0;
        }
        else
        {
            #if defined(WIDE)
                verticalWallLength = YSize-10+(lvmod-2)*2;
            #elif YSize<=14
                verticalWallLength = 4;
            #else
                verticalWallLength = YSize-14+(lvmod-2)*2;        
            #endif
        }

        verticalWallY = (YSize>>1)-(verticalWallLength>>1);
    }

    uint8_t oneMissileLevel(void)
    {
        return ((level%5)==3) || (level==5);  
    }

    uint8_t rocketLevel(void)
    {
        return (level >= FIRST_ROCKETS_LEVEL) && ((level%5)==2 || (level%5)==3);
    }

    uint8_t missileLevel(void)
    {
        return (level%5)==4;
    }    

    uint8_t bossLevel(void)
    {        
        return !(level%5);
    }
    
    uint8_t horizWallsLevel(void)
    {
        return ((level >= FIRST_HORIZONTAL_WALLS_LEVEL) && ((level%5==1) || (level%5==4)));
    }    

    void initializeAwayFromWall(Character * characterPtr, uint8_t x, uint8_t y, uint8_t status, Image *imagePtr)
    {
        do{
            initializeCharacter(characterPtr, x, y, status, imagePtr);
            relocateCharacter(characterPtr);
        } while(innerWallReached(characterPtr));
    }
#endif 


#if defined(BETWEEN_LEVEL)

#if XSize<YSize
    #define MIN_SIZE XSize
#else
    #define MIN_SIZE YSize
#endif

#if MIN_SIZE>12
    #define SPIRAL_LOOPS (2*MIN_SIZE-21)
#else
    #define SPIRAL_LOOPS (2*MIN_SIZE-18)
#endif

void spiral(register Character *characterPtr)
{
    uint8_t i;
    uint8_t j;
    
    characterPtr->_x = XSize/2;
    characterPtr->_y = YSize/2;
    for(i=0;i<SPIRAL_LOOPS;++i)
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
                #if _XL_SLOW_DOWN_FACTOR>0
                    _XL_SLOW_DOWN(55);
                #endif
            }
    }
}

#endif

#if defined(TINY_GAME)
    #define DRAW_BORDERS() \
    do \
    { \
        uint8_t i; \
        uint8_t j; \
        for(i=0;i<YSize;++i) \
        { \
            for(j=0;j<XSize;j+=XSize-1) \
            { \
                _XL_DRAW(j,i,_BOMB_TILE,_XL_YELLOW); \
            } \
        } \
    } \
    while(0)
#else
    #define DRAW_BORDERS() \
        _XL_SET_TEXT_COLOR(WALL_COLOR); \
        DRAW_HORIZONTAL_BORDER(0); \
        DRAW_HORIZONTAL_BORDER(YSize-1); \
        DRAW_VERTICAL_BORDER(0); \
        DRAW_VERTICAL_BORDER(XSize-1); 
#endif

void fillLevelWithCharacters(void)
{
    uint8_t i;
    uint8_t j;
    uint8_t count = 0;
    
    #if defined(FULL_GAME) && !defined(_XL_NO_COLOR)
    if(isBossLevel)
    {
            HORIZONTAL_BRICK_IMAGE._color = _XL_RED;
            VERTICAL_BRICK_IMAGE._color = _XL_RED;
    }
    else if(level&1)
    {
        HORIZONTAL_BRICK_IMAGE._color = _XL_YELLOW;
        VERTICAL_BRICK_IMAGE._color = _XL_YELLOW;
    }
    else
    {
        HORIZONTAL_BRICK_IMAGE._color = _XL_CYAN;
        VERTICAL_BRICK_IMAGE._color = _XL_CYAN;
    }
    #endif
    
    // #if !defined(TINY_GAME) && !defined(NO_BORDERS)
        DRAW_BORDERS();
    // #endif    
    
    #if defined(FULL_GAME)

        if(isBossLevel && level) // level 0 is the final screen and ghostCount should remain at GHOSTS_NUMBER
        {
            ghostCount = 2+(level>>2);
            #if !defined(_XL_NO_COLOR)
                SKULL_IMAGE._color = _XL_CYAN;
                BOMB_IMAGE._color = _XL_YELLOW;
            #endif
        }
        #if !defined(_XL_NO_COLOR)
        else
        {
            SKULL_IMAGE._color = _XL_YELLOW;
            BOMB_IMAGE._color = _XL_RED;
        }
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
    #endif
            
    #if GHOSTS_NUMBER >= 9
        ROUND_NINE_GHOSTS();
    #elif GHOSTS_NUMBER==8 
        #if (!defined(TINY_GAME) || defined(ROUND_ENEMIES)) && !defined(FLAT_ENEMIES)
            ROUND_EIGHT_GHOSTS();    
        #else
            FLAT_EIGHT_GHOSTS();
        #endif
    #elif GHOSTS_NUMBER==7
        ROUND_SEVEN_GHOSTS();
    #else
        FLAT_SIX_GHOSTS();
    #endif
  
    #if BOMBS_NUMBER==4
        FOUR_BOMBS();
    #elif BOMBS_NUMBER==3    
        THREE_BOMBS();
    #elif BOMBS_NUMBER==2
        TWO_BOMBS();
    #elif BOMBS_NUMBER==1
        ONE_BOMB();
    #endif
            
    #if defined(FULL_GAME)
        initializeAwayFromWall(&(powerUp._character),(XSize>>1),(YSize>>1),1,&POWERUP_IMAGE);
        initializeAwayFromWall(&(powerUp2._character),(XSize>>1),(YSize>>1),0,&POWERUP_IMAGE);
        initializeAwayFromWall(&(freeze._character),(XSize>>1),(YSize>>1),0,&FREEZE_IMAGE);
        initializeAwayFromWall(&(extraPoints._character), (XSize>>1), (YSize>>1), 0, &EXTRA_POINTS_IMAGE);
        initializeAwayFromWall(&(super._character), (XSize>>1), (YSize>>1), 0, &SUPER_IMAGE);
        initializeAwayFromWall(&(confuse._character), (XSize>>1), (YSize>>1), 0, &CONFUSE_IMAGE);        
        initializeAwayFromWall(&(zombie._character), (XSize>>1), (YSize>>1), 0, &ZOMBIE_IMAGE);    
        
        initializeAwayFromWall(&(gun._character),(XSize>>1), (YSize>>1), (isBossLevel ? 1 : 0), &GUN_IMAGE);

        initializeAwayFromWall(&player,(uint8_t) ((XSize>>1)+(_XL_RAND()&1)),(uint8_t) ((YSize>>1)+(_XL_RAND()&1)),1,&PLAYER_IMAGE);
                
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
        
        initializeAwayFromWall(&(chase._character), (XSize>>1), (YSize>>1),0,&BULLET_IMAGE);
        initializeCharacter(&chasingBullet, 0,0, 0, &BULLET_IMAGE);
    #else
        #if !defined(TINY_GAME)
            initializeCharacter(&(powerUp._character),(XSize>>1),(YSize>>1),1,&POWERUP_IMAGE);
            initializeCharacter(&(gun._character),(XSize>>1), (YSize>>1), 0, &GUN_IMAGE);    
            initializeCharacter(&(extraPoints._character), (XSize>>1), (YSize>>1), 0, &EXTRA_POINTS_IMAGE);    
        #endif
        
        #if defined(NO_RANDOM_LEVEL) || defined(TINY_GAME) || defined(SIMPLE_RANDOM_LEVEL)
            initializeCharacter(&player,(uint8_t) ((XSize>>1)),(uint8_t) ((YSize>>1)),1,&PLAYER_IMAGE);            
        #else
            initializeCharacter(&player,(uint8_t) ((XSize>>1)+(uint8_t) (_XL_RAND()&1)),
                                        (uint8_t) ((YSize>>1)+(uint8_t) (_XL_RAND()&1)),1,&PLAYER_IMAGE);    
        #endif
    #endif
    #if !defined(TINY_GAME)
        displayPlayer(&player);
            
        initializeCharacter(&bullet, 0, 0,0,&BULLET_IMAGE);

        #if defined(FULL_GAME)
            initializeCharacter(&skull,XSize-2,YSize-2, NON_BOSS_SKULL_HITS + (isBossLevel<<3), &SKULL_IMAGE);        
        #else
            initializeCharacter(&skull,XSize-2,YSize-2, NON_BOSS_SKULL_HITS, &SKULL_IMAGE);                    
        #endif
        
    #endif
}

