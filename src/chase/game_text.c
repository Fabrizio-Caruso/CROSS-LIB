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

#include "settings.h"
#include "../cross_lib/cross_lib.h"

#include "game_text.h"
#include "character.h"
#include "settings.h"
#include "text_strings.h"

#define _YELLOW COLOR_YELLOW
#define _WHITE COLOR_WHITE
#define _RED   COLOR_RED
#define SCORE_COLOR COLOR_IF_NO_BLUE_THEN_RED

#if defined(WIDE) && !defined(TINY_GAME)
	#define GUN_IMAGE_X 17
	#define GUN_IMAGE_Y 0
	#define GHOST_IMAGE_X 13
	#define GHOST_IMAGE_Y 0
	#define PLAYER_IMAGE_X 16
	#define PLAYER_IMAGE_Y 1
	#define LEVEL_X 6
#else
	#define GUN_IMAGE_X 11
	#define GUN_IMAGE_Y 0
	#define GHOST_IMAGE_X 8
	#define GHOST_IMAGE_Y 0
	#define PLAYER_IMAGE_X 14
	#define PLAYER_IMAGE_Y 0
	#define LEVEL_X 18
#endif

#if defined(NO_TEXT_COLOR)
	#define printCenteredMessageOnRowWithCol(row,col,text) printCenteredMessageOnRow(row,text)
#endif	

extern unsigned char level;
extern unsigned char lives;

extern unsigned short points;
extern unsigned char ghostCount;
extern unsigned short ghostLevel;
extern unsigned short highScore;


#if defined(FULL_GAME) && !defined(NO_MESSAGE)
	void printKillTheSkull(void)
	{
		printCenteredMessage(KILL_THE_SKULL_STRING);	
		printCenteredMessageOnRow(((unsigned char)YSize)/2+2,DESTROY_MISSILES_STRING);
	}
	
#endif

#if defined(NO_TITLE_LINE)
	#define TITLE_Y 0
	#define TITLE_LINE()
#else
	#define TITLE_Y 1
	#define TITLE_LINE() PRINT(XSize-11,+0,  "-----------")
#endif

#define PRINT_WIDE_TITLE() \
	SET_TEXT_COLOR(SCORE_COLOR); \
	PRINT(0, +0,   SCORE_STRING); \
	PRINT(0, +0+1, LEVEL_STRING); \
	\
	SET_TEXT_COLOR(_RED); \
	TITLE_LINE(); \
	PRINT(XSize-11,TITLE_Y,"cross chase");	


// TODO: This is SLOW
#if !defined(TINY_GAME)
	void displayStatsTitles(void)
	{				
		#if defined(WIDE)
				PRINT_WIDE_TITLE();
		#endif
		
		#if !defined(NO_COLOR)
			SET_TEXT_COLOR(TEXT_COLOR);
		#endif		
		
		#if (X_OFFSET==0) && (Y_OFFSET==0)
			#define _draw_stat _draw
		#endif
	
		_draw_stat(GUN_IMAGE_X, GUN_IMAGE_Y, &GUN_IMAGE);
		_draw_stat(GHOST_IMAGE_X, GHOST_IMAGE_Y, &GHOST_IMAGE);
		_draw_stat(PLAYER_IMAGE_X, PLAYER_IMAGE_Y, &PLAYER_IMAGE);					
	}

	void printGunsStats(void)
	{
		#if !defined(NO_COLOR)		
			SET_TEXT_COLOR(TEXT_COLOR);	
		#endif
		#if defined(WIDE)
			PRINTF(GUN_IMAGE_X+2,0+0,"%u",guns);
		#else
			PRINTF(GUN_IMAGE_X+1,0+0,"%u",guns);
		#endif
	}
#endif

#if !defined(NO_STATS)
	void printLevelStats(void)
	{	
		#if defined(WIDE) && !defined(TINY_GAME)
			SET_TEXT_COLOR(TEXT_COLOR);	
			PRINTF(LEVEL_X,1+0,"%02u", level);
		#else
			PRINTF(LEVEL_X,+0,"%02u",level);	
		#endif	
	}


	void printGhostCountStats(void)
	{
		#if !defined(NO_COLOR)
			SET_TEXT_COLOR(TEXT_COLOR);		
		#endif
		#if defined(WIDE) && !defined(TINY_GAME)
			PRINTF(GHOST_IMAGE_X+2,+0,"%u",ghostCount);
		#else
			PRINTF(GHOST_IMAGE_X+1,+0,"%u",ghostCount);	
		#endif	
	}


	void printLivesStats(void)
	{
		#if !defined(NO_COLOR)
			SET_TEXT_COLOR(TEXT_COLOR);
		#endif
		#if defined(WIDE) && !defined(TINY_GAME)
			PRINTF(PLAYER_IMAGE_X+2,+0+1,"%02u",lives);
		#else
			PRINTF(PLAYER_IMAGE_X+1,+0,"%02u",lives);	
		#endif
	}	
	
#endif

#if !defined(NO_MESSAGE)
	void printPressKeyToStart(void)
	{
		printCenteredMessage(PRESS_STRING);
	}	
#endif

#if !defined(ALT_DISPLAY_STATS)
void displayStats(void)
{	
	#if !defined(NO_COLOR)
		SET_TEXT_COLOR(TEXT_COLOR);
	#endif
	#if defined(WIDE) && !defined(TINY_GAME)
		PRINTF(6,+0,"%05u0",points);
	#else
		PRINTF(1,0,"%05u0",points);	
	#endif	
}
#endif


#if !defined(LESS_TEXT)	
	void printLevel(void)
	{
		PRINTF(((XSize -7)>>1), (YSize>>1), START_LEVEL_STRING, level);
	}
#endif


#if !defined(TINY_GAME) && !defined(NO_MESSAGE)
	void _printScoreOnRow(unsigned char row, char * text, unsigned short score)
	{
		PRINTF((unsigned char) ((XSize-strlen(text))>>1), row, text, score);
	}	
	
	void _printScore(char * text, unsigned short score)
	{
		_printScoreOnRow((YSize>>1), text, score);
	}
#endif

#if !defined(END_SCREEN) && !defined(NO_MESSAGE)
	void gameCompleted(void)	
	{
		printCenteredMessage(YOU_MADE_IT_STRING); 
	}
#endif


#if !defined(LESS_TEXT)
	void printExtraLife(void)
	{
		printCenteredMessageWithCol(_RED, EXTRA_LIFE_STRING); 
	}

	void printVictoryMessage(void)
	{
		printCenteredMessageWithCol(_RED, VICTORY_STRING);
	}	
#endif


#if !defined(LESS_TEXT)	
	void printGameOver(void)
	{		
		printCenteredMessageWithCol(_WHITE, GAME_OVER_STRING);
	}
	
	void printDefeatMessage(void)
	{			
		printCenteredMessageWithCol(_RED, DEFEAT_STRING);
	}	
#elif !defined(NO_MESSAGE)
	void printGameOver(void)
	{
		printCenteredMessage(GAME_OVER_STRING);
	}	
#endif



#if (defined(FULL_GAME) && !defined(NO_HINTS)) || !defined(NO_INITIAL_SCREEN)
	void _printCrossChase(void)
	{
		printCenteredMessageOnRowWithCol(3, _RED,  CROSS_CHASE_STRING);		
		SET_TEXT_COLOR(TEXT_COLOR);			
	}
#endif


#if defined(FULL_GAME) && !defined(NO_HINTS)
	void printHints(void)
	{
		_printCrossChase();
		
		printCenteredMessageOnRow(6,  USE_THE_GUN_AGAINST_STRING);

		printCenteredMessageOnRow(8,  THE_SKULL_AND_STRING);

		printCenteredMessageOnRow(10, MISSILE_BASES_STRING);	
		
		printCenteredMessageOnRow(12, FOR_POINTS_AND___STRING);

		printCenteredMessageOnRow(14, EXTRA_POWERUPS__STRING);
	}
#endif

#if !defined(NO_INITIAL_SCREEN)
	void printStartMessage(void)
	{
		_printCrossChase();
		
		printCenteredMessageOnRow(5, AUTHOR_STRING);	

		#if !defined(TINY_GAME) && !defined(NO_TITLE_INFO)
			_printTopScore();
			
			SET_TEXT_COLOR(COLOR_IF_NO_BLUE_THEN_RED);
			printCenteredMessageOnRow((YSize>>1)-1, LURE_THE_ENEMIES_STRING);
			printCenteredMessageOnRow((YSize>>1)+1, INTO_THE_MINES_STRING);
			
			SET_TEXT_COLOR(TEXT_COLOR);				
		#endif

		printCenteredMessageOnRow(YSize-3, USE_STRING);
	}
#endif

