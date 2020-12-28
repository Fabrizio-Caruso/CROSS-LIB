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
 
#ifndef _LEVEL
#define _LEVEL

#include "character.h"
#include "ghost.h"

// VERTICAL AND HORIZONTAL BORDER
#if !defined(TINY_GAME)
    #define DRAW_VERTICAL_BORDER(x) DRAW_VERTICAL_LINE(x,0,YSize-1)
    
    #if !defined(FULL_LOWER_BORDER)
        #define DRAW_HORIZONTAL_BORDER(y) DRAW_HORIZONTAL_LINE(0,y,XSize-1)
    #else
        #define DRAW_HORIZONTAL_BORDER(y) DRAW_HORIZONTAL_LINE(0,y,XSize)
    #endif
#else    
    #define DRAW_VERTICAL_BORDER(x)    
    #define DRAW_HORIZONTAL_BORDER(y)
#endif


#define WALL_COLOR COLOR_YELLOW

#if !defined(NO_WALL)
    #define DRAW_BORDERS() \
        SET_TEXT_COLOR(WALL_COLOR); \
        DRAW_HORIZONTAL_BORDER(0); \
        DRAW_HORIZONTAL_BORDER(YSize-1); \
        DRAW_VERTICAL_BORDER(0); \
        DRAW_VERTICAL_BORDER(XSize-1); 
#else
    #define DRAW_BORDERS()
#endif


#define PLACE_DEAD_GHOST() \
    initializeCharacter(&ghosts[count],(uint8_t) (GHOSTS_NUMBER-count),(uint8_t) 1,0,&DEAD_GHOST_IMAGE);


// Check XSize to avoid overflow
#if XSize>48
    #define ROUND_X_POS (uint8_t) ((2*j-1)*(uint8_t)(XSize/6))
#else
    #define ROUND_X_POS (uint8_t) ((2*j-1)*XSize/6)
#endif

// Check YSize to avoid overflow
#if YSize>48
    #define ROUND_Y_POS (uint8_t) ((2*i-1)*(uint8_t)(YSize/6))
#else
    #define ROUND_Y_POS (uint8_t) ((2*i-1)*YSize/6)
#endif
    
#define PLACE_ROUND_GHOST() \
    initializeCharacter(&ghosts[count], \
        ROUND_X_POS, \
        ROUND_Y_POS, \
        GHOST_LIFE ,&GHOST_IMAGE);

#define PLACE_FLAT_GHOST() \
    initializeCharacter(&ghosts[count],(uint8_t) (j*(XSize/5)),(i*(YSize/5)),GHOST_LIFE,&GHOST_IMAGE);

#define PLACE_FLAT_GHOST_6() \
    initializeCharacter(&ghosts[count],(uint8_t) (j*(XSize>>2)),(i*(YSize/5)),GHOST_LIFE,&GHOST_IMAGE);    
    
#define PLACE_NINTH_GHOST() \
    initializeCharacter(&ghosts[4],(uint8_t) (XSize-4),(uint8_t) (YSize-4),GHOST_LIFE,&GHOST_IMAGE);

    
#define ROUND_NINE_GHOSTS() \
    for(i=1;i<=3;++i) \
    { \
        for(j=1;j<=3;++j) \
        { \
            if(nGhosts>count) \
            { \
                if(count!=4) \
                { \
                    PLACE_ROUND_GHOST(); \
                } \
                else \
                { \
                    PLACE_NINTH_GHOST(); \
                } \
            } \
            else \
            { \
                PLACE_DEAD_GHOST(); \
            } \
            ++count; \
        } \
    }
    
    
#define ROUND_EIGHT_GHOSTS() \
    for(i=1;i<=3;++i) \
    { \
        for(j=1;j<=3;++j) \
        { \
            if(i!=2 || j !=2) \
            { \
                if(nGhosts>count) \
                { \
                    PLACE_ROUND_GHOST(); \
                } \
                else \
                { \
                    PLACE_DEAD_GHOST(); \
                } \
                ++count; \
            } \
        } \
    }


#define FLAT_EIGHT_GHOSTS() \
    for(j=1;j<=4;++j) \
    { \
        for(i=1;i<=4;i+=3) \
        { \
            if(nGhosts>count) \
            { \
                PLACE_FLAT_GHOST(); \
            } \
            else \
            { \
                PLACE_DEAD_GHOST(); \
            } \
            ++count; \
        } \
    }

    
#define FLAT_SIX_GHOSTS() \
    for(j=1;j<=3;++j) \
    { \
        for(i=1;i<=4;i+=3) \
        { \
            if(nGhosts>count) \
            { \
                PLACE_FLAT_GHOST_6(); \
            } \
            else \
            { \
                PLACE_DEAD_GHOST(); \
            } \
            ++count; \
        } \
    }
    
#define PLACE_BOMBS() \
    do \
    { \
        for(i=0;i<BOMBS_NUMBER;++i) \
        { \
            initializeCharacter(&bombs[i], 1+i, YSize-2,0,&BOMB_IMAGE); \
        } \
    } \
    while(0)

    
#if defined(NO_RANDOM_LEVEL)
    #define THREE_BOMBS() \
        do \
        { \
            initializeCharacter(&bombs[0],(XSize>>1), ((YSize/3)<<1),0,&BOMB_IMAGE); \
            initializeCharacter(&bombs[1],XSize/3, (YSize/3),0,&BOMB_IMAGE); \
            initializeCharacter(&bombs[2],((XSize/3)<<1), (YSize/3),0,&BOMB_IMAGE);    \
        } \
        while(0)
#else
    #define THREE_BOMBS() \
        do \
        { \
            uint8_t rnd = rand()&1; \
            initializeCharacter(&bombs[0],XSize/3+rnd, (YSize/3)+rnd,0,&BOMB_IMAGE); \
            initializeCharacter(&bombs[1],(XSize>>1)+rnd, ((YSize/3)*2)+rnd,0,&BOMB_IMAGE); \
            initializeCharacter(&bombs[2],2*(XSize/3)+rnd, (YSize/3)-rnd,0,&BOMB_IMAGE); \
        } \
        while(0)
#endif    


#if defined(NO_RANDOM_LEVEL)
    #define TWO_BOMBS() \
        do \
        { \
            initializeCharacter(&bombs[0],(XSize>>1), ((YSize/3)),0,&BOMB_IMAGE); \
            initializeCharacter(&bombs[1],(XSize>>1), ((YSize/3)<<1),0,&BOMB_IMAGE); \
        } \
        while(0)
#else
    #define TWO_BOMBS() \
        do \
        { \
            initializeCharacter(&bombs[0],(XSize>>1), ((YSize/3))+rand()%3,0,&BOMB_IMAGE); \
            initializeCharacter(&bombs[1],(XSize>>1)-1+rand()%3, ((YSize/3)*2)-1+rand()%3,0,&BOMB_IMAGE); \
        } \
        while(0)
#endif    


#if defined(NO_RANDOM_LEVEL)
    #define ONE_BOMB() \
        initializeCharacter(&bombs[0],(XSize>>1), (YSize>>1),0,&BOMB_IMAGE);            
#else
    #define ONE_BOMB() \
        initializeCharacter(&bombs[0],(XSize>>1)+rand()&1, (YSize>>1)+rand()&1,0,&BOMB_IMAGE);
#endif

#if defined(BETWEEN_LEVEL)
    void spiral(register Character *characterPtr, uint8_t length);
#endif

void fillLevelWithCharacters(void);

void updateInnerVerticalWall(void);

void updateInnerHorizontalWall(void);

uint8_t innerHorizontalWallLevel(void);

uint8_t oneMissileLevel(void);

uint8_t rocketLevel(void);

uint8_t missileLevel(void);    

uint8_t innerVerticalWallLevel(void);

uint8_t bossLevel(void);

void initializeAwayFromWall(Character * characterPtr, uint8_t x, uint8_t y, uint8_t status, Image *imagePtr);

void placeBombs(void);

#endif // _LEVEL
