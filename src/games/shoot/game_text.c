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
#include "settings.h"
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

extern uint8_t level;
extern uint8_t lives;

extern uint16_t points;
extern uint8_t ghostCount;
extern uint16_t ghostLevel;
extern uint16_t highScore;

extern Image BULLET_IMAGE;

extern uint8_t discoveredSecrets[];

extern uint8_t  secretLevelActivated;

#if !defined(LESS_TEXT)
void printKillTheSkulls(void)
{
    _XL_PRINT_CENTERED(KILL_THE_BOSS);    
    _XL_PRINT_CENTERED_ON_ROW(((uint8_t)YSize)/2+2,KILL_THE_SKULLS_STRING);    
    _XL_PRINT_CENTERED_ON_ROW(((uint8_t)YSize)/2+4,DESTROY_MISSILES_STRING);
}
#endif


void displayStatsTitles(void)
{                
    SET_COLOR(TEXT_COLOR);

    _draw_stat(BULLET_IMAGE_X, BULLET_IMAGE_Y, &BULLET_IMAGE);
    _draw_stat(GHOST_IMAGE_X, GHOST_IMAGE_Y, &GHOST_IMAGE);
    _draw_stat(PLAYER_IMAGE_X, PLAYER_IMAGE_Y, &PLAYER_IMAGE);                    
    _draw_stat(FIRE_POWER_IMAGE_X, FIRE_POWER_IMAGE_Y, &FIRE_POWER_IMAGE); 
}


void displayStats(void)
{
    displayScoreStats();            
    printLevelStats();
    printLivesStats();
    printGunsStats();
    printFirePowerStats();
    printGhostCountStats();
}


void printGunsStats(void)
{
    SET_COLOR(TEXT_COLOR);    
    
    #if defined(WIDE)
        _XL_PRINTD(BULLET_IMAGE_X+1,BULLET_IMAGE_Y,2,guns);
    #else
        _XL_PRINTD(BULLET_IMAGE_X+1,BULLET_IMAGE_Y,2,guns);
    #endif
}

void printFirePowerStats(void)
{
    #if !defined(_XL_NO_TEXT_COLOR)
    if(bulletStrength<4)
    {
        SET_COLOR(TEXT_COLOR);
    }
    else
    {
        SET_COLOR(_XL_RED);
    }
    #endif
    
    #if defined(WIDE)
        _XL_PRINTD(FIRE_POWER_IMAGE_X+2,FIRE_POWER_IMAGE_Y,1,bulletStrength-1);
    #else
        _XL_PRINTD(FIRE_POWER_IMAGE_X+1,FIRE_POWER_IMAGE_Y,1,bulletStrength-1);
    #endif
}

#if !defined(NO_STATS) || defined(LESS_TEXT)
    #if XSize>20
    void printLevelStats(void)
    {    
        SET_COLOR(TEXT_COLOR);
    
        _XL_PRINTD(LEVEL_X,LEVEL_Y,2,level);
    }
    #endif


    void printGhostCountStats(void)
    {
        SET_COLOR(TEXT_COLOR);        
        
        #if defined(WIDE)
            _XL_PRINTD(GHOST_IMAGE_X+1,GHOST_IMAGE_Y,2,ghostCount);
        #else
            _XL_PRINTD(GHOST_IMAGE_X+1,GHOST_IMAGE_Y,2,ghostCount);    
        #endif    
    }


    void printLivesStats(void)
    {
        SET_COLOR(TEXT_COLOR);
        
        #if defined(WIDE)
            _XL_PRINTD(PLAYER_IMAGE_X+1,PLAYER_IMAGE_Y,2,lives);
        #else
            _XL_PRINTD(PLAYER_IMAGE_X+1,PLAYER_IMAGE_Y,1,lives);    
        #endif
    }    
    
#endif

#if !defined(NO_MESSAGE) && !defined(NO_PRINT)
    void printPressKeyToStart(void)
    {
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT_CENTERED(PRESS_STRING);
    }    
#endif


void displayScoreStats(void)
{    
    SET_COLOR(TEXT_COLOR);
    
    _XL_PRINTD(0,0,5,points);    
}

#if !defined(LESS_TEXT)    
    void printLevel(void)
    {
        SET_COLOR(TEXT_COLOR);
        _XL_PRINT(((XSize -7)>>1), (YSize>>1), LEVEL_STRING);
        _XL_PRINTD(((XSize -7)>>1)+6, (YSize>>1), 2, level);
    }
#endif


#if !defined(NO_MESSAGE)
    void _printScoreOnRow(uint8_t row, uint16_t score)
    {
        _XL_PRINTD((uint8_t) ((XSize)>>1)-3, row, 5, score);
    }    
    
    void _printScore(uint16_t score)
    {
        _printScoreOnRow((YSize>>1), score);
    }
#endif

#if !defined(END_SCREEN) && !defined(NO_MESSAGE)
    void gameCompleted(void)    
    {
        _XL_CLEAR_SCREEN();
        _XL_PRINT_CENTERED(YOU_MADE_IT_STRING); 
        _XL_WAIT_FOR_INPUT();
    }
#endif


#if !defined(LESS_TEXT)
    void printExtraLife(void)
    {
        SET_COLOR(_XL_RED);
        _XL_PRINT_CENTERED(EXTRA_LIFE_STRING); 
    }

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
        _XL_PRINT_CENTERED_ON_ROW(YSize/2-2,DEFEAT_STRING);
    }    
#endif
    
    
#if !defined(NO_MESSAGE)
    void printGameOver(void)
    {
        SET_COLOR(_XL_RED);
        _XL_PRINT_CENTERED(GAME_OVER_STRING);
    }    
#endif


#if !defined(NO_INITIAL_SCREEN)

void _printCrossShoot(void)
{
    SET_COLOR(_XL_RED);
    _XL_PRINT_CENTERED_ON_ROW(2,  CROSS_SHOOT_STRING);        
    SET_COLOR(TEXT_COLOR);
    
}
#endif

#if YSize<=15
    #define LINE_SKIP 1
    #define INIT_HINT_LINE 1
#else
    #define LINE_SKIP 2
    #define INIT_HINT_LINE 1+(YSize/8)
#endif

#if !defined(NO_HINTS) && XSize>=18
    void printHints(void)
    {
        _printCrossShoot();
        
        _XL_PRINT(XSize/2-9,INIT_HINT_LINE+2*LINE_SKIP, KILL_ALL_SKULLS__STRING);
        _XL_PRINT(XSize/2-10, INIT_HINT_LINE+3*LINE_SKIP, BEFORE__TO_UNLOCK__STRING);  
        _XL_PRINT(XSize/2-7,INIT_HINT_LINE+4*LINE_SKIP, ITEMS__STRING);    
        _XL_PRINT(XSize/2-9,INIT_HINT_LINE+5*LINE_SKIP+1, DESTROY_MISSILES__STRING);

        _XL_DRAW(XSize/2+7,INIT_HINT_LINE+2*LINE_SKIP, _SKULL_TILE, _XL_YELLOW);
        _XL_DRAW(XSize/2-3,INIT_HINT_LINE+3*LINE_SKIP, _GHOST_TILE, _XL_WHITE);
        _XL_DRAW(XSize/2-1,INIT_HINT_LINE+4*LINE_SKIP, _SUPER_TILE, _XL_RED);
        _XL_DRAW(XSize/2+1,INIT_HINT_LINE+4*LINE_SKIP, _FREEZE_TILE, _XL_CYAN);
        _XL_DRAW(XSize/2+3,INIT_HINT_LINE+4*LINE_SKIP, _INVINCIBILITY_TILE, _XL_YELLOW);
        // _XL_DRAW(XSize/2+5,INIT_HINT_LINE+4*LINE_SKIP, _SUPER_TILE, _XL_RED);

        // _XL_DRAW(XSize/2-3,INIT_HINT_LINE+6*LINE_SKIP+1, _RIGHT_HORIZONTAL_MISSILE_TILE, _XL_WHITE);
        // _XL_DRAW(XSize/2-2,INIT_HINT_LINE+6*LINE_SKIP+1, _ROCKET_TILE, _XL_WHITE);
        // _XL_DRAW(XSize/2+0,INIT_HINT_LINE+6*LINE_SKIP+1, _LEFT_HORIZONTAL_MISSILE_TILE, _XL_WHITE);
    }
#endif

#if !defined(NO_INITIAL_SCREEN)
    void printStartMessage(void)
    {
        _printCrossShoot();
        
        #if XSize>=16
        _XL_PRINT_CENTERED_ON_ROW(5, AUTHOR_STRING);    
        #endif

        #if !defined(NO_TITLE_INFO)
            _printTopScore();
            
            // SET_COLOR(_XL_CYAN);
            
            #if XSize>= 14 && !defined(EXTRA_TITLE)
            _XL_PRINT_CENTERED_ON_ROW((YSize>>1)+1, KILL_THEM_ALL__STRING);
            #endif
            
            SET_COLOR(TEXT_COLOR);    
            
        #endif

        #if !defined(NO_CONTROL_INSTRUCTIONS) && XSize>=14
            _XL_PRINT_CENTERED_ON_ROW(YSize-2, USE_STRING);
        #endif
    }
#endif

#if !defined(LESS_TEXT)
void handleLevelBonus(uint16_t bonus)
{
    uint16_t i;
    
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    
    for(i=0;i<=bonus;i+=5)
    {
        _XL_PRINTD(XSize/2-1,YSize/2,3,i);
        points+=5;
        SHORT_SLEEP(1+(i>>6));
        displayScoreStats();
        _XL_TICK_SOUND();
    }
}
#endif

uint8_t countDiscoveredSecrets(void)
{
    uint8_t total;
    uint8_t i;
    
    total = 0;
    #if defined(DEBUG_SECRETS)
    SET_COLOR(TEXT_COLOR);    
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

    SET_COLOR(TEXT_COLOR);    

    _XL_PRINTD(9, (YSize>>1)-4, 5, highScore );

    _XL_PRINTD(9, (YSize>>1)-2, 5, points);

    if(level<=FINAL_LEVEL)
    {
        _XL_PRINTD(9, (YSize>>1), 2,   level);
    }

    i=0;
    do
    {
        _XL_PRINTD(9, (YSize>>1)+2, 2, i);
        for(j=0;j<i;++j)
        {
            SHORT_SLEEP(2);
        }
        _XL_SHOOT_SOUND();
        ++i;
    } while(i<=foundSecrets);
    
    
    SHORT_SLEEP(10);
    if(secretLevelActivated)
    {
        SET_COLOR(_XL_YELLOW);    
        _XL_PRINT(1, (YSize>>1)+4, SECRET_LEVEL_FOUND_STRING);
    }

}


