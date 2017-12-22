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

#ifdef __ATMOS__
	#include<peekpoke.h>
#elif defined(__CPC__) && defined(CPCRSLIB)
	#include "cpc/cpcrslib.h"	
#elif defined(__ZX81__) || defined(__LAMBDA__) || defined(__ZX80__) 
#endif

// CPC
#define CPC_WHITE 1
#define CPC_YELLOW 2 
#define CPC_RED 3
#define CPC_BLACK 4

#if defined(NO_CASE_LETTERS)
	#if defined(JOYSTICK_CONTROL) || defined(__MSX__)
		#define PRESS_STRING "press fire"
	#else
		#define PRESS_STRING "press any key"
	#endif
	#define GAME_OVER_STRING "game over"
	#define DEFEAT_STRING "you lost"
	#define VICTORY_STRING "you won"
	#define EXTRA_LIFE_STRING "extra life"
	#define YOU_MADE_IT_STRING "you made it"
#else
	#if defined(JOYSTICK_CONTROL) || defined(__MSX__)
		#define PRESS_STRING "PRESS FIRE"
	#else
		#define PRESS_STRING "PRESS ANY KEY"
	#endif
	#define GAME_OVER_STRING "GAME OVER"
	#define DEFEAT_STRING "YOU LOST"	
	#define VICTORY_STRING "YOU WON"
	#define EXTRA_LIFE_STRING "EXTRA LIFE"
	#define YOU_MADE_IT_STRING "YOU MADE IT"	
#endif

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
#elif defined(__VG5k__)
	#define _WHITE VG5K_WHITE
	#define _RED   VG5K_RED
#else
	#define _WHITE COLOR_WHITE
	#define _RED   COLOR_RED
#endif


extern unsigned char XSize;
extern unsigned char YSize;

extern unsigned char level;
extern unsigned char lives;
extern unsigned char guns;
extern unsigned int points;
extern unsigned char ghostCount;
extern unsigned short ghostLevel;
extern unsigned int highScore;

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image GUN_IMAGE;
extern Image INVINCIBLE_GHOST_IMAGE;
extern Image PLAYER_IMAGE;
extern Image MISSILE_IMAGE;


#if defined(COLOR)
	void printCenteredMessageOnRow(unsigned char row, char *Text)
	{
		PRINT((XSize - strlen (Text)) / 2, row, Text);	
	}
	
	void printCenteredMessageOnRowWithCol(unsigned char row, unsigned char col, char *Text)
	{
		SET_TEXT_COLOR(col);
		printCenteredMessageOnRow(row, Text);
	}
		
#else
	void printCenteredMessageOnRow(unsigned char row, char *Text)
	{
		PRINT((XSize - strlen (Text)) / 2, row, Text)		
	}
	#define printCenteredMessageOnRowWithCol(row, col, Text) \
		printCenteredMessageOnRow(row, Text)
#endif

#if defined(COLOR)
	#define printCenteredMessage(Text) \
		printCenteredMessageOnRow(YSize / 2, Text)

	#define printCenteredMessageWithCol(col, Text) \
		printCenteredMessageOnRowWithCol(YSize / 2, col, Text)	
#else
	#define printCenteredMessage(Text) \
		printCenteredMessageOnRow(YSize/2, Text)
		
	#define printCenteredMessageWithCol(col, Text) \
		printCenteredMessageOnRow(YSize/2, Text)
#endif
	

#if defined(FULL_GAME)
	#if defined(NO_CASE_LETTERS)
		void printKillTheSkull(void)
		{
			printCenteredMessage("kill the skull");	
		}
	#else
		void printKillTheSkull(void)
		{
			printCenteredMessage("Kill the skull");
		}	
	#endif	
#endif


// TODO: This is SLOW
#if !defined(TINY_GAME)
	void displayStatsTitles(void)
	{	
		#if defined(NO_CASE_LETTERS) && defined(WIDE)	
			SET_TEXT_COLOR(COLOR_BLUE);	
			PRINT(2-2, -Y_OFFSET,   "score:");
			PRINT(2-2, -Y_OFFSET+1, "level:");
		#elif defined(WIDE)
			#if defined(__CPC__)
				SET_TEXT_COLOR(_YELLOW);			
			#else 
				SET_TEXT_COLOR(COLOR_BLUE);	
			#endif
			#if defined(__MSX__) || (defined(__SVI__) && defined(MSX_MODE0))
				PRINT(2-1, -Y_OFFSET,   "SCORE:");
				PRINT(2-1, -Y_OFFSET+1, "LEVEL:");			
			#else
				PRINT(2-2, -Y_OFFSET,   "SCORE:");
				PRINT(2-2, -Y_OFFSET+1, "LEVEL:");	
			#endif
		#else
			// Nothing
		#endif

		// && !defined(__ZX80__) && !defined(__ZX81__) && !defined(__LAMBDA__) && !defined(__SPECTRUM__) && !defined(__MSX__)
		#if defined(WIDE)
			#if !defined(__MSX__)
				SET_TEXT_COLOR(_RED);	
				PRINT(24-3,-Y_OFFSET,"-----------");		
				PRINT(24-3,-Y_OFFSET+1,"cross chase");	
			#else
				PRINT(24-3-1,-Y_OFFSET,  "cross-----");		
				PRINT(24-3-1,-Y_OFFSET+1,"----chase");			
			#endif
		#endif

		#if defined(__CPC__) && defined(CPCRSLIB)
			SET_TEXT_COLOR(TEXT_COLOR);	
			cpc_PrintGphStrStdXY(CPC_YELLOW,")",(18+1-2)*2,0*8);gotoxy(18+2-2,0); cputc(':');
			cpc_PrintGphStrStdXY(CPC_WHITE,"%",(18-4+1-2)*2,0*8);gotoxy(18-2-2,0); cputc(':');
			cpc_PrintGphStrStdXY(CPC_YELLOW,"!",(18-1+1-2)*2,1*8);gotoxy(18+1-2,1); cputc(':');	
		#elif defined(__ZX81__) || defined(__ZX80__) || defined(__LAMBDA__)
			SET_TEXT_COLOR(TEXT_COLOR);		
			zx_setcursorpos(0-1+1, 19-2); cputc(GUN_IMAGE._imageData);cputc(':');
			zx_setcursorpos(0-1+1, 15-2); cputc(GHOST_IMAGE._imageData);cputc(':');
			zx_setcursorpos(1-1+1, 18-2); cputc(PLAYER_IMAGE._imageData);cputc(':');			
		#elif defined(WIDE)
			SET_TEXT_COLOR(TEXT_COLOR);	
			gotoxy(19-2+X_OFFSET,0); cputc(GUN_IMAGE._imageData);cputc(':');
			gotoxy(15-2+X_OFFSET,0); cputc(GHOST_IMAGE._imageData);cputc(':');
			gotoxy(18-2+X_OFFSET,1); cputc(PLAYER_IMAGE._imageData);cputc(':');	
		#elif (defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1)
			SET_TEXT_COLOR(TEXT_COLOR);	
			gotoxy(15+1-5,0); cputc(GUN_IMAGE._imageData+160);cputc(':');
			gotoxy(15-3-2-3,0); cputc(GHOST_IMAGE._imageData+160);cputc(':');
			gotoxy(15+1-5+4-1,0); cputc(PLAYER_IMAGE._imageData+64);cputc(':');		
		#elif defined(__GAMATE__)
			SET_TEXT_COLOR(3);	
			gotoxy(11,0); cputc(GUN_IMAGE._imageData);cputc(':');
			gotoxy(7,0); cputc(GHOST_IMAGE._imageData);cputc(':');
			gotoxy(14,0); cputc(PLAYER_IMAGE._imageData);cputc(':');	
		#else
			#if defined(COLOR)
			SET_TEXT_COLOR(TEXT_COLOR);
			#endif
			gotoxy(11+X_OFFSET,0); cputc(GUN_IMAGE._imageData);cputc(':');
			gotoxy(7+X_OFFSET,0); cputc(GHOST_IMAGE._imageData);cputc(':');
			gotoxy(14+X_OFFSET,0); cputc(PLAYER_IMAGE._imageData);cputc(':');
		#endif
	}

	void printGunsStats(void)
	{
		#if defined(COLOR)		
			SET_TEXT_COLOR(TEXT_COLOR);	
		#endif
		#if defined(WIDE)
			PRINTF(18+2+1-2,0-Y_OFFSET,"%u",guns);
		#else
			PRINTF(15+2+1-5-1,0-Y_OFFSET,"%u",guns);
		#endif
	}
#endif

void printLevelStats(void)
{	
	#if defined(WIDE) && !defined(TINY_GAME)
		SET_TEXT_COLOR(TEXT_COLOR);	
		PRINTF(6,1-Y_OFFSET,"%02u", level);
	#else
		PRINTF(18,0-Y_OFFSET,"%02u",level);	
	#endif	
}

void printGhostCountStats(void)
{
	#if defined(COLOR)
		SET_TEXT_COLOR(TEXT_COLOR);		
	#endif
	#if defined(WIDE) && !defined(TINY_GAME)
		PRINTF(18+2-2-3,0-Y_OFFSET,"%u",ghostCount);
	#else
		PRINTF(15+2-3-2-3,0-Y_OFFSET,"%u",ghostCount);	
	#endif	
}

void printLivesStats(void)
{
	#if defined(COLOR)
		SET_TEXT_COLOR(TEXT_COLOR);
	#endif
	#if defined(WIDE) && !defined(TINY_GAME)
		PRINTF(18+2-2,-Y_OFFSET+1,"%02u",lives);
	#else
		PRINTF(15+2+1-5+4-1,-Y_OFFSET,"%u",lives);	
	#endif
}

void displayStats(void)
{	
	#if defined(COLOR)
		SET_TEXT_COLOR(TEXT_COLOR);
	#endif
	#if defined(WIDE) && !defined(TINY_GAME)
		PRINTF(8-2,-Y_OFFSET,"%05u0",points);
	#else
		PRINTF(3-3,0,"%05u0",points);	
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
	#if defined (NO_CASE_LETTERS)
		void printLevel(void)
		{
			char levelString[22];

			sprintf(levelString, "level %u", level);

			printCenteredMessageWithCol(_WHITE,levelString);
		}
	#else
		void printLevel(void)
		{
			char levelString[22];

			sprintf(levelString, "LEVEL %u", level);

			printCenteredMessageWithCol(_WHITE,levelString);
		}
	#endif
#endif
	


#if !defined(TINY_GAME)
	void _printScoreOnRow(unsigned char row, char * text, unsigned int score)
	{
		char levelString[22];

		sprintf(levelString, text, score);

		printCenteredMessageOnRow(row, levelString);	
	}	
	
	void _printScore(char * text, unsigned int score)
	{
		_printScoreOnRow(YSize/2, text, score);
	}
#else
	void _printScore(char * text, unsigned int score)
	{
		char levelString[22];

		sprintf(levelString, text, score);

		printCenteredMessage(levelString);	
	}	
#endif

void gameCompleted(void)	
{
	printCenteredMessage(YOU_MADE_IT_STRING); 
}

#if !defined(NO_TEXT)
	void printExtraLife(void)
	{
		printCenteredMessageWithCol(_RED, EXTRA_LIFE_STRING); 
	}

	void printVictoryMessage(void)
	{
		printCenteredMessageWithCol(_WHITE, VICTORY_STRING);
	}	
#endif

#if !defined(NO_TEXT)
	void printPressKeyToStart(void)
	{		
		printCenteredMessage(               PRESS_STRING);
	}
				
	void printGameOver(void)
	{		
		printCenteredMessageWithCol(_WHITE, GAME_OVER_STRING);
	}
	
	void printDefeatMessage(void)
	{			
		printCenteredMessageWithCol(_WHITE, DEFEAT_STRING);
	}	
#else
	#if defined(NO_CASE_LETTERS)
		void printPressKeyToStart(void)
		{
			printCenteredMessage("go");
		}		
		
		void printGameOver(void)
		{
			printCenteredMessage(" game over ");
		}	
	#else
		void printPressKeyToStart(void)
		{
			printCenteredMessage("GO");
		}		
		
		void printGameOver(void)
		{
			printCenteredMessage(" GAME OVER ");
		}	
	#endif
#endif




// SPLASH SCREEN

#if defined(__C64__) && defined(REDEFINED_CHARS)
	void c64_splash_title(void)
	{
		SET_TEXT_COLOR(_RED);
		PRINT((XSize - 22) / 2, 2, "c r o s s   c h a s e");
		SET_TEXT_COLOR(COLOR_BROWN);
		PRINT((XSize - 22) / 2, YSize / 2 - 7,  "by fabrizio caruso");
		SET_TEXT_COLOR(TEXT_COLOR);	
	}
#endif

	
#if defined(__ATMOS__) && defined(FULL_GAME)
	void atmos_splash_title(void)
	{
		POKE(0xBB80+3*40,16);POKE(0xBB80+1+3*40,1); POKE(0xBB80+2+3*40,14);
		POKE(0xBB80+4*40,16);POKE(0xBB80+1+4*40,1); POKE(0xBB80+2+4*40,14);
		POKE(0xBB80+5*40,16);POKE(0xBB80+1+5*40,1); POKE(0xBB80+2+5*40,14);
		
		POKE(0xBB80+6*40,16);POKE(0xBB80+1+6*40,3); POKE(0xBB80+2+6*40,10);
		POKE(0xBB80+7*40,16);POKE(0xBB80+1+7*40,3); POKE(0xBB80+2+7*40,10);
		
		POKE(0xBB80+8*40,16);POKE(0xBB80+1+8*40,1); POKE(0xBB80+2+8*40,10);
		POKE(0xBB80+9*40,16);POKE(0xBB80+1+9*40,1); POKE(0xBB80+2+9*40,10);
		POKE(0xBB80+10*40,16);POKE(0xBB80+1+10*40,3); POKE(0xBB80+2+10*40,10);
		POKE(0xBB80+11*40,16);POKE(0xBB80+1+11*40,3); POKE(0xBB80+2+11*40,10);
		
		POKE(0xBB80+12*40,16);POKE(0xBB80+1+12*40,4); 
		POKE(0xBB80+13*40,16);POKE(0xBB80+1+13*40,4); 
		POKE(0xBB80+14*40,16);POKE(0xBB80+1+14*40,4); 
		POKE(0xBB80+15*40,16);POKE(0xBB80+1+15*40,4); 
		
		gotoxy(19,6); cputc(PLAYER_IMAGE._imageData);
		gotoxy(19,7); cputc(PLAYER_IMAGE._imageData);
		
		gotoxy(15,10); cputc(GHOST_IMAGE._imageData);
		gotoxy(15,11); cputc(GHOST_IMAGE._imageData);
		
		gotoxy(19,10); cputc(INVINCIBLE_GHOST_IMAGE._imageData);
		gotoxy(19,11); cputc(INVINCIBLE_GHOST_IMAGE._imageData);
		
		gotoxy(23,10); cputc('>');
		gotoxy(23,11); cputc('>');
		
		gotoxy(9, 5); 
		cputc('C'); 
		cputc(' ');
		cputc('R');
		cputc(' ');
		cputc('O');
		cputc(' ');		
		cputc('S');
		cputc(' ');		
		cputc('S');
		cputc(' ');		
		cputc(' ');
		cputc(' ');		
		cputc('C');
		cputc(' ');		
		cputc('H');
		cputc(' ');		
		cputc('A');
		cputc(' ');		
		cputc('S');
		cputc(' ');		
		cputc('E'); 
		
		gotoxy(9, 4); 
		cputc('C'); 
		cputc(' ');
		cputc('R');
		cputc(' ');
		cputc('O');
		cputc(' ');		
		cputc('S');
		cputc(' ');		
		cputc('S');
		cputc(' ');		
		cputc(' ');
		cputc(' ');		
		cputc('C');
		cputc(' ');		
		cputc('H');
		cputc(' ');		
		cputc('A');
		cputc(' ');		
		cputc('S');
		cputc(' ');		
		cputc('E'); 
		
		POKE(0xBB80+(YSize / 2 + 4 + 1)*40,16);POKE(0xBB81+(YSize / 2 + 4 +1)*40,4);
		POKE(0xBB80+(YSize / 2 + 4 + 2)*40,16);POKE(0xBB81+(YSize / 2 + 4 +2)*40,4);
		POKE(0xBB80+(YSize / 2 + 4 + 3)*40,16);POKE(0xBB81+(YSize / 2 + 4 +3)*40,4);	

		POKE(0xBB80+(YSize / 2 + 4 + 4)*40,16);POKE(0xBB81+(YSize / 2 + 4 +4)*40,12);
		POKE(0xBB80+(YSize / 2 + 4 + 5)*40,16);POKE(0xBB81+(YSize / 2 + 4 +5)*40,12);
		POKE(0xBB80+(YSize / 2 + 4 + 6)*40,16);POKE(0xBB81+(YSize / 2 + 4 +6)*40,12);

		gotoxy(19,8); cputc(MISSILE_IMAGE._imageData);
		gotoxy(19,9); cputc(MISSILE_IMAGE._imageData);			
	}
#endif


#if defined(__C64__)
	void c64_splash_instructions(void)
	{
		PRINT((XSize - 22) / 2, YSize / 2 - 3, "escape the enemies");
		
		PRINT((XSize - 22) / 2, YSize / 2 - 1, "force them into the mines");
		
		PRINT((XSize - 22) / 2, YSize / 2 + 1, "catch the gun for bullets");
	}
#endif


#if defined(__ATMOS__) && defined(FULL_GAME)
	void atmos_splash_instructions(void)
	{
		PRINT(7, YSize / 2 - 1, "Escape from the enemies");
		
		PRINT(7, YSize / 2, "Force them into the mines");
	}
#endif

#if defined(FULL_GAME)
void printHints(void)
{
	#if defined(__CPC__)
		printCenteredMessageOnRowWithCol(3, _RED,  "c r o s s  c h a s e");	
	#elif defined(NO_CASE_LETTERS) 
		printCenteredMessageOnRowWithCol(3, _RED, "c r o s s  c h a s e");		
	#else
		printCenteredMessageOnRowWithCol(3, _RED, "C R O S S  C H A S E");	
	#endif
	SET_TEXT_COLOR(TEXT_COLOR);		

	#if defined(NO_CASE_LETTERS)
		printCenteredMessageOnRow(6,  "use the gun against");
	#elif defined(__PC6001__)
		printCenteredMessageOnRow(6,  "USE THE GUN AGAINST");	
	#else
		printCenteredMessageOnRow(6,  "Use the gun against");
	#endif	
		
	#if !defined(__PC6001__)
		printCenteredMessageOnRow(8,  "the skull and");

		printCenteredMessageOnRow(10, "missile bases");	
		
		printCenteredMessageOnRow(12, "for points and  ");

		printCenteredMessageOnRow(14, "extra powerups ");
	#else
		printCenteredMessageOnRow(8,  "THE SKULL AND");

		printCenteredMessageOnRow(10, "MISSILE BASES");	
		
		printCenteredMessageOnRow(12, "FOR POINTS  ");

		printCenteredMessageOnRow(14, "EXTRA POWERUPS  ");		
	#endif
	
}
#endif

#if !defined(NO_INITIAL_SCREEN)
void printStartMessage(void)
{
	#if defined(__C64__) && defined(REDEFINED_CHARS)
		c64_splash_title();
	#elif defined(__ATMOS__) && defined(FULL_GAME) && defined(REDEFINED_CHARS)
		atmos_splash_title();
	#elif !defined(TINY_GAME)
		_printTopScore();
		#if defined(NO_CASE_LETTERS)
			printCenteredMessageOnRowWithCol(3, _RED,  "c r o s s  c h a s e");				
		#else
			printCenteredMessageOnRowWithCol(3, _RED,  "C R O S S  C H A S E");		
		#endif
		SET_TEXT_COLOR(TEXT_COLOR);		
		#if defined(NO_CASE_LETTERS)
			printCenteredMessageOnRow(5, "by fabrizio caruso");		
		#elif defined(__PC6001__)
			printCenteredMessageOnRow(5, "BY FABRIZIO CARUSO");				
		#else
			printCenteredMessageOnRow(5, "by Fabrizio Caruso");
		#endif
	#else
		printCenteredMessageOnRowWithCol(3, _RED,  "CROSS CHASE");	
		#if defined(NO_CASE_LETTERS)
			printCenteredMessageOnRow(5, "fabrizio caruso");	
		#elif  defined(__PC6001__)
			printCenteredMessageOnRow(5, "FABRIZIO CARUSO");				
		#else
			printCenteredMessageOnRow(5, "Fabrizio Caruso");
		#endif		
	#endif
	
	#if !defined(TINY_GAME)
		#if !defined(__ATMOS__)
			#if defined(__PLUS4__) || defined(__C16__)
				SET_TEXT_COLOR(COLOR_CYAN);	
			#elif defined(__CPC__)
				SET_TEXT_COLOR(_YELLOW);			
			#else
				SET_TEXT_COLOR(COLOR_BLUE);
			#endif 
		#endif

		#if defined(__C64__) 
			c64_splash_instructions();
		#elif defined(__ATMOS__) && defined(FULL_GAME) && defined(REDEFINED_CHARS)
			atmos_splash_instructions();
		#else
			#if defined(NO_CASE_LETTERS)
				printCenteredMessageOnRow(YSize/2-1, "lure the enemies");
			#elif defined(__PC6001__)
				printCenteredMessageOnRow(YSize/2-1, "LURE THE ENEMIES");					
			#else
				printCenteredMessageOnRow(YSize/2-1, "Lure the enemies");
			#endif		
			#if !defined(__PC6001__)
				printCenteredMessageOnRow(YSize/2+1, "into the mines");
			#else
				printCenteredMessageOnRow(YSize/2+1, "INTO THE MINES");
			#endif
		#endif

		#if !defined(__ATMOS__) && !defined(NO_TEXT)
			SET_TEXT_COLOR(TEXT_COLOR);		
		#endif
	#endif
	
	#if defined(JOYSTICK_CONTROL) || defined(__MSX__)
		printCenteredMessageOnRow(YSize-3, "use the joystick");
	#elif !defined(TINY_GAME)		
		#if !defined(__PC6001__)
			printCenteredMessageOnRow(YSize-3, "use i j k l space");
		#else
			printCenteredMessageOnRow(YSize-3, "USE I J K L SPACE");
		#endif
	#else
		#if !defined(__PC6001__)
			printCenteredMessageOnRow(YSize-3, "use ijkl");		
		#else
			printCenteredMessageOnRow(YSize-3, "USE IJKL");
		#endif			
	#endif	
}
#endif

