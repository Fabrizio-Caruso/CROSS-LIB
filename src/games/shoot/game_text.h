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


// #if defined(WIDE) && XSize>64
    // #define BULLET_IMAGE_X 13
    // #define BULLET_IMAGE_Y 0
    // #define FIRE_POWER_IMAGE_X 18
    // #define FIRE_POWER_IMAGE_Y 0
    // #define GHOST_IMAGE_X 23
    // #define GHOST_IMAGE_Y 0
    // #define PLAYER_IMAGE_X 28
    // #define PLAYER_IMAGE_Y 0
    // #define LEVEL_X (XSize-16)
    // #define LEVEL_Y 0
// #elif defined(WIDE)
    // #define BULLET_IMAGE_X 13
    // #define BULLET_IMAGE_Y 0
    // #define FIRE_POWER_IMAGE_X 17
    // #define FIRE_POWER_IMAGE_Y 0
    // #define GHOST_IMAGE_X 13
    // #define GHOST_IMAGE_Y 0
    // #define PLAYER_IMAGE_X 17
    // #define PLAYER_IMAGE_Y 0
    // #define LEVEL_X 6
    // #define LEVEL_Y 0
// #else
    #define BULLET_IMAGE_X 10
    #define BULLET_IMAGE_Y 0
    #define FIRE_POWER_IMAGE_X 17
    #define FIRE_POWER_IMAGE_Y 0
    #define GHOST_IMAGE_X 6
    #define GHOST_IMAGE_Y 0
    #define PLAYER_IMAGE_X 14
    #define PLAYER_IMAGE_Y 0
    #define LEVEL_X (XSize-2)
    #define LEVEL_Y 0
// #endif



#if defined(NO_TITLE_LINE)
    #define TITLE_Y 0
    #define TITLE_LINE()
#elif XSize>64
    #define TITLE_Y 0
    #define TITLE_LINE()
#else
    #define TITLE_Y 1
    #define TITLE_LINE() 
    //PRINT(XSize-11,+0,  "-----------")
#endif


void PRINT_CENTERED_ON_ROW(uint8_t row, char *Text);

// #define PRINT_CENTERED_ON_ROW(row, Text, len) \
// do \
// { \
	// _XL_PRINT(((uint8_t) (XSize - len)/2), row, Text);	\
// } while(0)


#define PRINT_CENTERED(Text) \
	PRINT_CENTERED_ON_ROW((YSize>>1), Text)


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


void _printCrossShoot(void);

// game_text
#if !defined(LESS_TEXT)
	void printLevel(void);

	void printVictoryMessage(void);

    // void printSecondRound(void);
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
	// void printPressKeyToStart(void);
    #define printPressKeyToStart() \
        do \
        { \
            _XL_SET_TEXT_COLOR(_XL_WHITE); \
            PRINT_CENTERED(PRESS_STRING); \
        } while(0)
#else
	#define printPressKeyToStart()
#endif

#if !defined(LESS_TEXT)
	void printKillTheSkulls(void);
#else
	#define printKillTheSkulls()
#endif


#if !defined(_XL_NO_TEXT_COLOR)
    #define _printTopScore() \
        do \
        { \
            _XL_PRINTD((uint8_t) ((XSize)>>1)-1, 0, 5, highScore); \
            _XL_SET_TEXT_COLOR(_XL_CYAN); \
            _XL_PRINT(((XSize)>>1)-3,0,"HI"); \
        } while(0)
#else
    #define _printTopScore() \
        do \
        { \
            _XL_PRINTD((uint8_t) ((XSize)>>1)-2, 0, 5, highScore); \
        } while(0)
#endif


#if defined(LESS_TEXT) 
		#define handleLevelBonus(bonus) \
            do \
            { \
                points+=(bonus); \
            } while(0) 
                
		#define finalScore()
		#define highScoreScreen() 
		
#else
    void handleLevelBonus(uint16_t score); // _XL_PRINTD(XSize/2-2,YSize/2,3,bonus)

    // #define printLevelBonus(bonus) _XL_PRINTD(XSize/2-2,YSize/2,3,bonus)
    // #define finalScore() _printScore(points)
    // #define highScoreScreen() _printScore(highScore)  	
#endif

#if XSize>=14
void printHints(void);
#endif

void printAchievements(void);

	
#endif // _GAME_TEXT


