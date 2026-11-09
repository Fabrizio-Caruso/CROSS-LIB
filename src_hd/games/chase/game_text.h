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
 
#ifndef _GAME_TEXT
#define _GAME_TEXT

#include "settings.h"
#include "cross_lib.h"

void PRINT_CENTERED_ON_ROW(uint8_t row, const char *Text);


#define PRINT_CENTERED(Text) \
	PRINT_CENTERED_ON_ROW((YSize>>1), Text)



// game_stat
void displayScore(void);

#if !defined(TINY_GAME)
	void displayStatsTitles(void);

	void printGunsStats(void);
#endif


#if !defined(NO_STATS)
	void printLevelStats(void);
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

#endif

#if !defined(NO_PRINT)
	void printGameOver(void);
#else
	#define printGameOver()
#endif

void printDefeatMessage(void);

void printStartMessage(void);

#if !defined(NO_PRINT)
	void gameCompleted(void);
#else
	#define gameCompleted()
#endif

#if !defined(NO_PRINT) && !defined(TINY_GAME)
	void printPressKeyToStart(void);
#else
	#define printPressKeyToStart()
#endif

#if !defined(TINY_GAME)
	void _printScoreOnRow(uint8_t row, uint16_t score);
	void _printScore(uint16_t score);
	void printKillTheSkull(void);
#else
	#define _printScoreOnRow(row, score)
	#define _printScore(s)
	#define printKillTheSkull()
#endif

#if !defined(TINY_GAME)
	#define _printTopScore() _printScoreOnRow(0,highScore)
#endif

#if defined(TINY_GAME)
	#if !defined(NO_PRINT)
		#define highScoreScreen() _XL_PRINTD((XSize-6)/2, 0, 5, highScore)
	#else
		#define highScoreScreen()
	#endif
#elif defined(LESS_TEXT) 
		#define highScoreScreen() 
		
#else
    #define highScoreScreen() _printScore(highScore);
#endif

#if defined(FULL_GAME) && XSize>=14
	void printHints(void);
#endif

	
#endif // _GAME_TEXT


