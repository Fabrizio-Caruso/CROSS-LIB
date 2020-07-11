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
 
#ifndef _GAME_TEXT
#define _GAME_TEXT

#include "settings.h"
#include "cross_lib.h"


#if defined(__CMOC__) && !defined(__WINCMOC__)
	#include <cmoc.h>
#else
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>	
#endif

#if defined(WIDE)
    #define BULLET_IMAGE_X 13
    #define BULLET_IMAGE_Y 0
    #define FIRE_POWER_IMAGE_X 17
    #define FIRE_POWER_IMAGE_Y 0
    #define GHOST_IMAGE_X 13
    #define GHOST_IMAGE_Y 1
    #define PLAYER_IMAGE_X 17
    #define PLAYER_IMAGE_Y 1
    #define LEVEL_X 6
#else
    #define BULLET_IMAGE_X 10
    #define BULLET_IMAGE_Y 0
    #define FIRE_POWER_IMAGE_X 17
    #define FIRE_POWER_IMAGE_Y 0
    #define GHOST_IMAGE_X 6
    #define GHOST_IMAGE_Y 0
    #define PLAYER_IMAGE_X 14
    #define PLAYER_IMAGE_Y 0
    #define LEVEL_X (XSize-2)
#endif



#if defined(NO_TITLE_LINE)
    #define TITLE_Y 0
    #define TITLE_LINE()
#else
    #define TITLE_Y 1
    #define TITLE_LINE() PRINT(XSize-11,+0,  "-----------")
#endif


#define PRINT_WIDE_TITLE() \
    do \
    { \
        SET_COLOR(COLOR_CYAN); \
        PRINT(0, +0,   SCORE_STRING); \
        PRINT(0, +0+1, LEVEL_STRING); \
        \
        SET_COLOR(COLOR_RED); \
        TITLE_LINE(); \
        PRINT(XSize-11,TITLE_Y,CROSS_SHOOT_SHORT_STRING); \
    } while(0)


// game_stat
void displayScoreStats(void);

void displayStatsTitles(void);

void printGunsStats(void);

void printFirePowerStats(void);

void displayStats(void);

#if !defined(NO_STATS)
    #if XSize>20
        void printLevelStats(void);
    #else
        #define printLevelStats()
    #endif
	void printGhostCountStats(void);
	void printLivesStats(void);		
#else
	#define printLevelStats()
	#define printGhostCountStats()
	#define printLivesStats()
#endif


// game_text
#if !defined(LESS_TEXT)
	void printLevel(void);

	void printVictoryMessage(void);

	void printExtraLife(void);
#endif

#if !defined(NO_MESSAGE)
	void printGameOver(void);
#else
	#define printGameOver()
#endif

void printDefeatMessage(void);

void printStartMessage(void);

#if !defined(END_SCREEN) && !defined(NO_MESSAGE)
	void gameCompleted(void);
#elif defined(NO_MESSAGE)
	#define gameCompleted()
#endif

#if !defined(NO_MESSAGE) && !defined(NO_PRINT)
	void printPressKeyToStart(void);
#else
	#define printPressKeyToStart()
#endif

#if !defined(NO_MESSAGE)
	void _printScoreOnRow(uint8_t row, uint16_t score);
	void _printScore(uint16_t score);
	void printKillTheSkull(void);
#else
	#define _printScoreOnRow(r,score)
	#define _printScore(s)
	#define printKillTheSkull()
#endif

#define _printTopScore() _printScoreOnRow(0,highScore);


#if defined(LESS_TEXT) 
		#define printLevelBonus(bonus) 
		#define finalScore()
		#define highScoreScreen() 
		
#else
    #define printLevelBonus(bonus) _printScore(bonus);
    #define finalScore() _printScore(points);
    #define highScoreScreen() _printScore(highScore);  	
#endif

void printHints(void);

void printAchievements(void);

	
#endif // _GAME_TEXT


