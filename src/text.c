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

#include "text.h"

#include "display_macros.h"

#include "settings.h"

#include "text_strings.h"

#ifdef __ATMOS__
	#include<peekpoke.h>
#elif defined(__CPC__) && defined(CPCRSLIB)
	#include "cpc/cpcrslib.h"	
#endif

// Instructions color
#if defined(__PLUS4__) || defined(__C16__)
	#define SPLASH_COLOR COLOR_CYAN	
#elif defined(__CPC__)
	#define SPLASH_COLOR _YELLOW			
#else
	#define SPLASH_COLOR COLOR_BLUE
#endif 

// CPC
#define CPC_WHITE 1
#define CPC_YELLOW 2 
#define CPC_RED 3
#define CPC_BLACK 4

// 3 -> white
// 2 -> black 
// 1 -> black
// 0 -> yellow
// 4 -> red
#define CPC_TEXT_YELLOW 0
#define CPC_TEXT_WHITE 3
#define CPC_TEXT_RED 4

// VG5K
#define VG5K_BLACK 0
#define VG5K_RED 1
#define VG5K_GREEN 2
#define VG5K_YELLOW 3
#define VG5K_BLUE 4
#define VG5K_VIOLET 5
#define VG5K_CYAN 6
#define VG5K_WHITE 7

#if defined(__CPC__)
	#define _YELLOW CPC_TEXT_YELLOW
	#define _WHITE CPC_TEXT_WHITE
	#define _RED   CPC_TEXT_RED
	#define SCORE_COLOR _YELLOW
#elif defined(__VG5k__)
	#define _WHITE VG5K_WHITE
	#define _RED   VG5K_RED
	#define SCORE_COLOR COLOR_BLUE
#else
	#define _WHITE COLOR_WHITE
	#define _RED   COLOR_RED
	#define SCORE_COLOR COLOR_BLUE	
#endif

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

extern unsigned char level;
extern unsigned char lives;
extern unsigned char guns;
extern unsigned short points;
extern unsigned char ghostCount;
extern unsigned short ghostLevel;
extern unsigned short highScore;

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image GUN_IMAGE;
extern Image INVINCIBLE_GHOST_IMAGE;
extern Image PLAYER_IMAGE;
extern Image MISSILE_IMAGE;


#if defined(COLOR)
	void printCenteredMessageOnRow(unsigned char row, char *Text)
	{
		PRINT(((XSize - strlen(Text))>>1), row, Text);	
	}
	
	#if defined(__ATMOS__)
		void printCenteredMessageOnRowWithCol(unsigned char row, unsigned char col, char *Text)
		{
			POKE(0xBB80+3+(row+Y_OFFSET)*40,16);POKE(0xBB80+3+1+(row+Y_OFFSET)*40,col);
			printCenteredMessageOnRow(row, Text);						
			POKE(0xBB80+35+(row+Y_OFFSET)*40,16);POKE(0xBB80+35+1+(row+Y_OFFSET)*40,3);			
		}			
	#else
		void printCenteredMessageOnRowWithCol(unsigned char row, unsigned char col, char *Text)
		{
			SET_TEXT_COLOR(col);
			printCenteredMessageOnRow(row, Text);
		}		
	#endif
#else
	void printCenteredMessageOnRow(unsigned char row, char *Text)
	{
		PRINT((unsigned char)(((unsigned char) XSize - strlen (Text))>>1), row, Text);	
	}
	#define printCenteredMessageOnRowWithCol(row, col, Text) \
		printCenteredMessageOnRow(row, Text)
#endif



#if defined(FULL_GAME)
	void printKillTheSkull(void)
	{
		printCenteredMessage(KILL_THE_SKULL_STRING);	
	}
#endif


// TODO: This is SLOW
#if !defined(TINY_GAME)
	void displayStatsTitles(void)
	{	
		#if defined(WIDE)
			SET_TEXT_COLOR(SCORE_COLOR);	
			PRINT(0, -Y_OFFSET,   SCORE_STRING);
			PRINT(0, -Y_OFFSET+1, LEVEL_STRING);
	
			SET_TEXT_COLOR(_RED);	
			PRINT(XSize-11,-Y_OFFSET,  "-----------");		
			PRINT(XSize-11,-Y_OFFSET+1,"cross chase");				
		#endif

		#if defined(__CPC__) && defined(CPCRSLIB)
			SET_TEXT_COLOR(TEXT_COLOR);	
			cpc_PrintGphStrStdXY(CPC_YELLOW,")",GUN_IMAGE_X*2,0*8);gotoxy(GUN_IMAGE_X+1,0); cputc(':');
			cpc_PrintGphStrStdXY(CPC_WHITE,"%",GHOST_IMAGE_X*2,0*8);gotoxy(GHOST_IMAGE_X+1,0); cputc(':');
			cpc_PrintGphStrStdXY(CPC_YELLOW,"!",PLAYER_IMAGE_X*2,1*8);gotoxy(PLAYER_IMAGE_X+1,1); cputc(':');	
		#elif defined(__ZX81__) || defined(__ZX80__) || defined(__LAMBDA__)
			SET_TEXT_COLOR(TEXT_COLOR);		
			zx_setcursorpos(0, GUN_IMAGE_X); cputc(GUN_IMAGE._imageData);cputc(':');
			zx_setcursorpos(0, GHOST_IMAGE_X); cputc(GHOST_IMAGE._imageData);cputc(':');
			zx_setcursorpos(1, PLAYER_IMAGE_X); cputc(PLAYER_IMAGE._imageData);cputc(':');	
		#elif defined(__ATARI5200__)
			// TODO: to implement
		#elif defined(WIDE)
			SET_TEXT_COLOR(TEXT_COLOR);	
			gotoxy(GUN_IMAGE_X+X_OFFSET,0); cputc(GUN_IMAGE._imageData);cputc(':');
			gotoxy(GHOST_IMAGE_X+X_OFFSET,0); cputc(GHOST_IMAGE._imageData);cputc(':');
			gotoxy(PLAYER_IMAGE_X+X_OFFSET,1); cputc(PLAYER_IMAGE._imageData);cputc(':');	
		#elif (defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1)
			SET_TEXT_COLOR(TEXT_COLOR);	
			gotoxy(GUN_IMAGE_X,0); cputc(GUN_IMAGE._imageData+160);	
			gotoxy(GHOST_IMAGE_X,0); cputc(GHOST_IMAGE._imageData+160);
			gotoxy(PLAYER_IMAGE_X,0); cputc(PLAYER_IMAGE._imageData+64);		
		#elif defined(__CMOC__) && !defined(__WINCMOC__)
			// TODO: Implement this
		#else
			#if defined(COLOR)
				SET_TEXT_COLOR(TEXT_COLOR);
			#endif
			gotoxy(GUN_IMAGE_X+X_OFFSET,0); cputc(GUN_IMAGE._imageData);
			gotoxy(GHOST_IMAGE_X+X_OFFSET,0); cputc(GHOST_IMAGE._imageData);
			gotoxy(PLAYER_IMAGE_X+X_OFFSET,0); cputc(PLAYER_IMAGE._imageData);
		#endif
	}

	void printGunsStats(void)
	{
		#if defined(COLOR)		
			SET_TEXT_COLOR(TEXT_COLOR);	
		#endif
		#if defined(WIDE)
			PRINTF(GUN_IMAGE_X+2,0-Y_OFFSET,"%u",guns);
		#else
			PRINTF(GUN_IMAGE_X+1,0-Y_OFFSET,"%u",guns);
		#endif
	}
#endif


void printLevelStats(void)
{	
	#if defined(WIDE) && !defined(TINY_GAME)
		SET_TEXT_COLOR(TEXT_COLOR);	
		PRINTF(LEVEL_X,1-Y_OFFSET,"%02u", level);
	#else
		PRINTF(LEVEL_X,-Y_OFFSET,"%02u",level);	
	#endif	
}


void printGhostCountStats(void)
{
	#if defined(COLOR)
		SET_TEXT_COLOR(TEXT_COLOR);		
	#endif
	#if defined(WIDE) && !defined(TINY_GAME)
		PRINTF(GHOST_IMAGE_X+2,-Y_OFFSET,"%u",ghostCount);
	#else
		PRINTF(GHOST_IMAGE_X+1,-Y_OFFSET,"%u",ghostCount);	
	#endif	
}


void printLivesStats(void)
{
	#if defined(COLOR)
		SET_TEXT_COLOR(TEXT_COLOR);
	#endif
	#if defined(WIDE) && !defined(TINY_GAME)
		PRINTF(PLAYER_IMAGE_X+2,-Y_OFFSET+1,"%02u",lives);
	#else
		PRINTF(PLAYER_IMAGE_X+1,-Y_OFFSET,"%02u",lives);	
	#endif
}


void displayStats(void)
{	
	#if defined(COLOR)
		SET_TEXT_COLOR(TEXT_COLOR);
	#endif
	#if defined(WIDE) && !defined(TINY_GAME)
		PRINTF(6,-Y_OFFSET,"%05u0",points);
	#else
		PRINTF(1,0,"%05u0",points);	
	#endif	
}


#if !defined(NO_SET_SCREEN_COLORS)
	void setScreenColors(void)
	{
		SET_TEXT_COLOR(TEXT_COLOR);
		SET_BORDER_COLOR(BORDER_COLOR);
		SET_BACKGROUND_COLOR(BACKGROUND_COLOR);
	}
#endif


#if !defined(NO_TEXT)	
	void printLevel(void)
	{
		PRINTF(((XSize -7)>>1), (YSize>>1), START_LEVEL_STRING, level);
	}
#endif


#if !defined(TINY_GAME)
	void _printScoreOnRow(unsigned char row, char * text, unsigned short score)
	{
		PRINTF((unsigned char) ((XSize-strlen(text))>>1), row, text, score);
	}	
	
	void _printScore(char * text, unsigned short score)
	{
		_printScoreOnRow((YSize>>1), text, score);
	}
#endif

#if !defined(END_SCREEN)
	void gameCompleted(void)	
	{
		printCenteredMessage(YOU_MADE_IT_STRING); 
	}
#endif


#if !defined(NO_TEXT)
	void printExtraLife(void)
	{
		printCenteredMessageWithCol(_RED, EXTRA_LIFE_STRING); 
	}

	void printVictoryMessage(void)
	{
		printCenteredMessageWithCol(_RED, VICTORY_STRING);
	}	
#endif

void printPressKeyToStart(void)
{
	printCenteredMessage(PRESS_STRING);
}		


#if !defined(NO_TEXT)	
	void printGameOver(void)
	{		
		printCenteredMessageWithCol(_WHITE, GAME_OVER_STRING);
	}
	
	void printDefeatMessage(void)
	{			
		printCenteredMessageWithCol(_RED, DEFEAT_STRING);
	}	
#else
	void printGameOver(void)
	{
		printCenteredMessage(GAME_OVER_STRING);
	}	
#endif




#if defined(FULL_GAME)
	void printHints(void)
	{
		printCenteredMessageOnRowWithCol(3, _RED,  CROSS_CHASE_STRING);		

		SET_TEXT_COLOR(TEXT_COLOR);		

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
		printCenteredMessageOnRowWithCol(3, _RED,  CROSS_CHASE_STRING);
		
		SET_TEXT_COLOR(TEXT_COLOR);
		printCenteredMessageOnRow(5, AUTHOR_STRING);	

		#if !defined(TINY_GAME)
			_printTopScore();
			
			SET_TEXT_COLOR(SPLASH_COLOR);
			printCenteredMessageOnRow((YSize>>1)-1, LURE_THE_ENEMIES_STRING);
			printCenteredMessageOnRow((YSize>>1)+1, INTO_THE_MINES_STRING);
			
			SET_TEXT_COLOR(TEXT_COLOR);				
		#endif

		printCenteredMessageOnRow(YSize-3, USE_STRING);
	}
#endif

