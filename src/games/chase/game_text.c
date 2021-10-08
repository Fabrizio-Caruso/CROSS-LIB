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
#include "game_text.h"
#include "character.h"
#include "settings.h"
#include "text_strings.h"

#include "cross_lib.h"


extern uint8_t guns;
extern Image GUN_IMAGE;
extern Image SKULL_IMAGE;

#if !defined(_XL_NO_TEXT_COLOR) || defined(NO_GRAPHICS)
	#define SET_COLOR(c) _XL_SET_TEXT_COLOR(c)
#else
	#define SET_COLOR(c)
#endif

#if XSize<20
	#define EXTRA_TINY 1
#else
	#define EXTRA_TINY 0
#endif


#if Y_OFFSET==3
	#define SKIP_ROW 1
#else
	#define SKIP_ROW 0
#endif

#if defined(WIDE) && !defined(TINY_GAME)
	#define GUN_IMAGE_X 17
	#define GUN_IMAGE_Y 0
	#define GHOST_IMAGE_X 13
	#define GHOST_IMAGE_Y 0
	#define PLAYER_IMAGE_X 16
	#define PLAYER_IMAGE_Y (1+SKIP_ROW)
	#define LEVEL_X 6
	#define LEVEL_Y (1+SKIP_ROW)
#else
	#define GUN_IMAGE_X (11-EXTRA_TINY)
	#define GUN_IMAGE_Y 0
	#define GHOST_IMAGE_X (8-EXTRA_TINY)
	#define GHOST_IMAGE_Y 0
	#define PLAYER_IMAGE_X (14-EXTRA_TINY)
	#define PLAYER_IMAGE_Y 0
	#define LEVEL_X 18
	#define LEVEL_Y 0
#endif


#ifndef _XL_PRINT_CENTERED_ON_ROW
    #if defined(ALT_DISPLAY_STATS)
        #define _XL_PRINT_CENTERED_ON_ROW(row, text) _XL_PRINT(2,row,text)
    #endif
#endif

extern uint8_t level;
extern uint8_t lives;

extern uint16_t points;
extern uint8_t ghostCount;
extern uint16_t ghostLevel;
extern uint16_t highScore;

extern Image GHOST_IMAGE;
extern Image PLAYER_IMAGE;

#if defined(FULL_GAME) && !defined(NO_MESSAGE)
	void printKillTheSkull(void)
	{
		_XL_PRINT_CENTERED(KILL_THE_SKULL_STRING);	
		_XL_PRINT_CENTERED_ON_ROW(((uint8_t)YSize)/2+2,DESTROY_MISSILES_STRING);
	}
	
#endif

#if defined(NO_TITLE_LINE)
	#define TITLE_Y 0
	#define TITLE_LINE()
#else
	#define TITLE_Y 1
	#define TITLE_LINE() 
#endif


#define PRINT_WIDE_TITLE() \
	SET_COLOR( _XL_CYAN); \
	_XL_PRINT(0,       0, SCORE_STRING); \
	_XL_PRINT(0, LEVEL_Y, LEVEL_STRING); \
	\
	SET_COLOR( _XL_RED); \
	TITLE_LINE(); \
	_XL_PRINT(XSize-11,TITLE_Y,TITLE_LINE_STRING);	


// TODO: This is SLOW
#if !defined(TINY_GAME) && !defined(NO_STATS)
	void displayStatsTitles(void)
	{				
		// #if defined(WIDE)
				// PRINT_WIDE_TITLE();
		// #endif
		
		SET_COLOR(TEXT_COLOR);

		_draw_stat(GUN_IMAGE_X, GUN_IMAGE_Y, &GUN_IMAGE);
		_draw_stat(GHOST_IMAGE_X, GHOST_IMAGE_Y, &GHOST_IMAGE);
		_draw_stat(PLAYER_IMAGE_X, PLAYER_IMAGE_Y, &PLAYER_IMAGE);
	}

	
	void printGunsStats(void)
	{
		SET_COLOR(TEXT_COLOR);
		
		#if defined(WIDE)
			_XL_PRINTD(GUN_IMAGE_X+2,GUN_IMAGE_Y,1,guns);
		#else
			_XL_PRINTD(GUN_IMAGE_X+1,GUN_IMAGE_Y,1,guns);
		#endif
	}
#endif

#if !defined(NO_STATS)
	void printLevelStats(void)
	{	
		SET_COLOR(TEXT_COLOR);
	
		#if defined(WIDE) && !defined(TINY_GAME)
			_XL_PRINTD(LEVEL_X,LEVEL_Y,2,level);
		#elif XSize>16
			_XL_PRINTD(LEVEL_X,LEVEL_Y,2,level);
		#else
			// No space for level
		#endif	
	}


	void printGhostCountStats(void)
	{
		SET_COLOR(TEXT_COLOR);		
		
		#if defined(WIDE) && !defined(TINY_GAME)
			_XL_PRINTD(GHOST_IMAGE_X+2,GHOST_IMAGE_Y,1,ghostCount);
		#else
			_XL_PRINTD(GHOST_IMAGE_X+1,GHOST_IMAGE_Y,1,ghostCount);	
		#endif	
	}


	void printLivesStats(void)
	{
		SET_COLOR(TEXT_COLOR);
		
		#if defined(WIDE) && !defined(TINY_GAME)
			_XL_PRINTD(PLAYER_IMAGE_X+2,PLAYER_IMAGE_Y,2,lives);
		#else
			_XL_PRINTD(PLAYER_IMAGE_X+1,PLAYER_IMAGE_Y,2,lives);	
		#endif
	}	
	
#endif

#if !defined(NO_MESSAGE) && !defined(NO_PRINT)
	void printPressKeyToStart(void)
	{
		_XL_PRINT_CENTERED(PRESS_STRING);
	}	
#endif


void displayStats(void)
{	
	SET_COLOR(TEXT_COLOR);
	
	// #if defined(WIDE) && !defined(TINY_GAME)
		// _XL_PRINTD(6,+0,5,points);
	// #else
		_XL_PRINTD(!EXTRA_TINY,0,5,points);	
	// #endif	
}

#if !defined(LESS_TEXT)	
	void printLevel(void)
	{
		_XL_PRINT(((XSize -7)>>1), (YSize>>1), START_LEVEL_STRING);
        _XL_PRINTD(((XSize -7)>>1)+6, (YSize>>1), 2, level);
	}
#endif


#if !defined(TINY_GAME) && !defined(NO_MESSAGE)
	void _printScoreOnRow(uint8_t row, uint16_t score)
	{
		_XL_PRINTD((uint8_t) ((XSize)>>1)-2, row, 5, score);
	}	
	
	#if !defined(LESS_TEXT)
	void _printScore(uint16_t score)
	{
		_printScoreOnRow((YSize>>1), score);
	}
	#endif
#endif

#if !defined(END_SCREEN) && !defined(NO_MESSAGE)
	void gameCompleted(void)	
	{
		_XL_CLEAR_SCREEN();
		_XL_PRINT_CENTERED(YOU_MADE_IT_STRING); 
	}
#endif


#if !defined(LESS_TEXT)
	void printExtraLife(void)
	{
		_XL_PRINT_CENTERED_WITH_COLOR(_XL_RED, EXTRA_LIFE_STRING); 
	}

	void printVictoryMessage(void)
	{
		_XL_PRINT_CENTERED_WITH_COLOR(_XL_RED, VICTORY_STRING);
	}	
#endif


#if !defined(LESS_TEXT)	
	void printDefeatMessage(void)
	{			
		_XL_PRINT_CENTERED_WITH_COLOR(_XL_RED, DEFEAT_STRING);
	}	
#endif
	
	
#if !defined(NO_MESSAGE)
	void printGameOver(void)
	{
		_XL_PRINT_CENTERED_WITH_COLOR(_XL_RED, GAME_OVER_STRING);
	}	
#endif



#if YSize>=20
	#define EXTRA_Y 1
	#define AUTHOR_Y 5
	#define CROSS_CHASE_Y 2
	#define INTERLINE 2
	#define INSTR_Y_OFFSET 3
#else
	#define EXTRA_Y 0
	#define AUTHOR_Y 3
	#define CROSS_CHASE_Y 1
	#if defined(Z88DK_SPRITES)
		#define INTERLINE 2
	#else
		#define INTERLINE 1
	#endif
	#define INSTR_Y_OFFSET 1
#endif

 


#if (defined(FULL_GAME) && !defined(NO_HINTS)) || !defined(NO_INITIAL_SCREEN)
	#if defined(FULL_GAME) && !defined(NO_HINTS)
	void _printCrossChase(void)
	{
		_XL_PRINT_CENTERED_ON_ROW_WITH_COLOR(CROSS_CHASE_Y, _XL_RED,  CROSS_CHASE_STRING);		
		SET_COLOR(TEXT_COLOR);
		
	}
	#else
		#define _printCrossChase() \
			_XL_PRINT_CENTERED_ON_ROW_WITH_COLOR(CROSS_CHASE_Y, _XL_RED,  CROSS_CHASE_STRING);	\
			SET_COLOR(TEXT_COLOR);	
	#endif
#endif


#if defined(FULL_GAME) && !defined(NO_HINTS)
	void printHints(void)
	{
		_printCrossChase();
		
		_XL_PRINT_CENTERED_ON_ROW(AUTHOR_Y+1*INTERLINE+EXTRA_Y,  USE_THE_GUN_AGAINST_STRING);

		_XL_PRINT_CENTERED_ON_ROW(AUTHOR_Y+2*INTERLINE+EXTRA_Y,  THE_SKULL_AND_STRING);

		_XL_PRINT_CENTERED_ON_ROW(AUTHOR_Y+3*INTERLINE+EXTRA_Y, MISSILE_BASES_STRING);	
		
		#if YSize>=14
			_XL_PRINT_CENTERED_ON_ROW(AUTHOR_Y+4*INTERLINE+EXTRA_Y, FOR_POINTS_AND___STRING);

			_XL_PRINT_CENTERED_ON_ROW(AUTHOR_Y+5*INTERLINE+EXTRA_Y, EXTRA_POWERUPS__STRING);
		#endif
	}
#endif

#if !defined(NO_INITIAL_SCREEN)
	void printStartMessage(void)
	{
		_printCrossChase();
		
		_XL_PRINT_CENTERED_ON_ROW(AUTHOR_Y, AUTHOR_STRING);	

		#if !defined(TINY_GAME) && !defined(NO_TITLE_INFO)
			_printTopScore();
			
			SET_COLOR(_XL_CYAN);
			
			_XL_PRINT_CENTERED_ON_ROW(AUTHOR_Y+1*INTERLINE+EXTRA_Y,  LURE_THE_ENEMIES_STRING);
			_XL_PRINT_CENTERED_ON_ROW(AUTHOR_Y+2*INTERLINE+EXTRA_Y,  INTO_THE_MINES_STRING);			
			
			SET_COLOR(TEXT_COLOR);	
			
		#endif

		#if !defined(NO_CONTROL_INSTRUCTIONS)
			_XL_PRINT_CENTERED_ON_ROW(YSize-INSTR_Y_OFFSET, USE_STRING);
		#endif
	}
#endif



