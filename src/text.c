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

// 3 -> white
// 2 -> black 
// 1 -> black
// 0 -> yellow
// 4 -> red
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

#if defined(COLOR)
	void printCenteredMessageOnRow(unsigned char row, char *Text)
	{
		PRINTF((XSize - strlen (Text)) / 2, row,"%s", Text)		
	}
	
	void printCenteredMessageOnRowWithCol(unsigned char row, unsigned char col, char *Text)
	{
		SET_TEXT_COLOR(col);
		printCenteredMessageOnRow(row, Text);
	}
		
#else
	void printCenteredMessageOnRow(unsigned char row, char *Text)
	{
		PRINTF((XSize - strlen (Text)) / 2, row,"%s", Text)		
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
void displayStatsTitles(void)
{	
	#if defined(NO_CASE_LETTERS) && defined(WIDE)	
		SET_TEXT_COLOR(COLOR_BLUE);	
		PRINT(-X_OFFSET+2, -Y_OFFSET,   "score:");
		PRINT(-X_OFFSET+2, -Y_OFFSET+1, "level:");
	#elif defined(WIDE)
		SET_TEXT_COLOR(COLOR_BLUE);	
		PRINT(-X_OFFSET+2, -Y_OFFSET,   "SCORE:");
		PRINT(-X_OFFSET+2, -Y_OFFSET+1, "LEVEL:");	
	#else
		// Nothing
	#endif

	#if defined(WIDE) && !defined(__ZX80__) && !defined(__ZX81__) && !defined(__LAMBDA__) && !defined(__SPECTRUM__) && !defined(__MSX__)
		SET_TEXT_COLOR(_RED);	
		PRINT(24,-Y_OFFSET,"-----------");		
		PRINT(24,-Y_OFFSET+1,"cross chase");	
	#endif

	#if defined(__CPC__) && defined(CPCRSLIB)
		SET_TEXT_COLOR(TEXT_COLOR);	
		cpc_PrintGphStrStdXY(CPC_YELLOW,")",(18)*2,0*8);gotoxy(18+2,0 + ADJUST ); cputc(':');
		cpc_PrintGphStrStdXY(CPC_WHITE,"%",(18-4)*2,0*8);gotoxy(18-2,0 + ADJUST); cputc(':');
		cpc_PrintGphStrStdXY(CPC_YELLOW,"!",(18-1)*2,1*8);gotoxy(18+1,1 + ADJUST); cputc(':');	
	#elif defined(__ZX81__) || defined(__ZX80__) || defined(__LAMBDA__)
		SET_TEXT_COLOR(TEXT_COLOR);		
		zx_setcursorpos(0+ADJUST-1, 19); cputc(GUN_IMAGE._imageData);cputc(':');
		zx_setcursorpos(0+ADJUST-1, 15); cputc(GHOST_IMAGE._imageData);cputc(':');
		zx_setcursorpos(1+ADJUST-1, 18); cputc(PLAYER_IMAGE._imageData);cputc(':');			
	#elif defined(WIDE)
		SET_TEXT_COLOR(TEXT_COLOR);	
		gotoxy(19,0 + ADJUST ); cputc(GUN_IMAGE._imageData);cputc(':');
		gotoxy(15,0 + ADJUST); cputc(GHOST_IMAGE._imageData);cputc(':');
		gotoxy(18,1 + ADJUST); cputc(PLAYER_IMAGE._imageData);cputc(':');	
	#elif (defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1)
		SET_TEXT_COLOR(TEXT_COLOR);	
		gotoxy(15+1-5,0 + ADJUST); cputc(GUN_IMAGE._imageData+160);cputc(':');
		gotoxy(15-3-2-3,0 + ADJUST); cputc(GHOST_IMAGE._imageData+160);cputc(':');
		gotoxy(15+1-5+4-1,0 + ADJUST); cputc(PLAYER_IMAGE._imageData+64);cputc(':');		
	#else
		SET_TEXT_COLOR(TEXT_COLOR);	
		gotoxy(15+1-5,0 + ADJUST); cputc(GUN_IMAGE._imageData);cputc(':');
		gotoxy(15-3-2-3,0 + ADJUST); cputc(GHOST_IMAGE._imageData);cputc(':');
		gotoxy(15+1-5+4-1,0 + ADJUST); cputc(PLAYER_IMAGE._imageData);cputc(':');
	#endif


}


void printGunsStats(void)
{
	#if defined(WIDE)
		SET_TEXT_COLOR(TEXT_COLOR);
		PRINTF(18+2+1-X_OFFSET,0-Y_OFFSET,"%u",guns);
	#else
		PRINTF(15+2+1-5-1-X_OFFSET,0-Y_OFFSET,"%u",guns);
	#endif
}

void printLevelStats(void)
{	
	#if defined(WIDE)
		SET_TEXT_COLOR(TEXT_COLOR);	
		PRINTF(8-X_OFFSET,1-Y_OFFSET,"%02u", level);
	#else
		PRINTF(15+2+1-5+4-1+2-X_OFFSET,0-Y_OFFSET,"%02u",level);	
	#endif	
}

void printGhostCountStats(void)
{
	SET_TEXT_COLOR(TEXT_COLOR);		
	#if defined(WIDE)
		PRINTF(18+2-X_OFFSET-3,0-Y_OFFSET,"%u",ghostCount);
	#else
		PRINTF(15+2-X_OFFSET-3-2-3,0-Y_OFFSET,"%u",ghostCount);	
	#endif	
}

void printLivesStats(void)
{
	SET_TEXT_COLOR(TEXT_COLOR);		
	#if defined(WIDE)
		PRINTF(18+2-X_OFFSET,1-Y_OFFSET,"%02u",lives);
	#else
		PRINTF(15+2+1-5+4-1-X_OFFSET,0-Y_OFFSET,"%u",lives);	
	#endif
}

void displayStats(void)
{	
	SET_TEXT_COLOR(TEXT_COLOR);	
	#if defined(WIDE)
		PRINTF(8-X_OFFSET,0-Y_OFFSET,"%05u0",points);
	#else
		PRINTF(3-3,0-Y_OFFSET,"%05u0",points);	
	#endif	
}

void setScreenColors(void)
{
	SET_TEXT_COLOR(TEXT_COLOR);
	SET_BORDER_COLOR(BORDER_COLOR);
	SET_BACKGROUND_COLOR(BACKGROUND_COLOR);
}
	
	
#if defined (NO_CASE_LETTERS)
	void printLevel(void)
	{
		char levelString[22];

		sprintf(levelString, "level %d", level);

		printCenteredMessageWithCol(_WHITE,levelString);
	}
#elif (defined(__SVI__) && !defined(FULL_GAME)) || (defined(__LAMBDA__) && !defined(FULL_GAME))
	void printLevel(void) {}
#else
	void printLevel(void)
	{
		char levelString[22];

		sprintf(levelString, "LEVEL %d", level);

		printCenteredMessageWithCol(_WHITE,levelString);
	}
#endif

#if defined (__VG5K__)
void _printScore(char * text, unsigned int score)
{
	char levelString[22];

	sprintf(levelString, text, score);

	printCenteredMessage(levelString);	
}
#elif (defined(__SVI__) && !defined(FULL_GAME)) || (defined(__LAMBDA__) && !defined(FULL_GAME))
	void _printScore(char * text, unsigned int score) {}
#else
void _printScore(char * text, unsigned int score)
{
	char levelString[22];

	sprintf(levelString, text, score);

	printCenteredMessage(levelString);
}
#endif

#if defined(__VG5K__)
	void gameCompleted(void)	
	{
		printCenteredMessage("DONE"); 
	}
#elif defined(__C64__)
	void gameCompleted(void)
	{
		printCenteredMessage("y o u   m a d e   i t"); 
	}
#elif (defined(__SVI__) && !defined(FULL_GAME)) || (defined(__LAMBDA__) && !defined(FULL_GAME))
	void gameCompleted(void)
	{}
#elif defined(__VIC20__) && !defined(FULL_GAME)
	void gameCompleted(void)
	{
		printCenteredMessage("YOU MADE IT"); 
	}
#else
	void gameCompleted(void)
	{
		printCenteredMessage("Y O U  M A D E  I T"); 
	}
#endif


#if defined(__VG5K__) || defined(__VZ__)
		void printExtraLife(void)
		{
			printCenteredMessageWithCol(_RED,   "EXTRA LIFE"); 
		}
	
		void printPressKeyToStart(void)
		{
			printCenteredMessageWithCol(_WHITE, "PRESS A KEY");
		}

		void deleteCenteredMessage(void)
		{
			printCenteredMessage(               "           ");
		}

		void printGameOver(void)
		{
			printCenteredMessage(               "GAME OVER");
		}

		void printVictoryMessage(void)
		{
			printCenteredMessageWithCol(_WHITE, "YOU WON");
		}

		void printDefeatMessage(void)
		{
			printCenteredMessageWithCol(_WHITE, "YOU LOST");
		}	
#elif (defined(__SVI__) && !defined(FULL_GAME)) || (defined(__LAMBDA__) && !defined(FULL_GAME))
		void printExtraLife(void)
		{
		}
		
		void printPressKeyToStart(void)
		{
			
		}

		void deleteCenteredMessage(void)
		{
			printCenteredMessage("                       ");
		}

		void printGameOver(void)
		{
			printCenteredMessage("GAME OVER");
		}

		void printVictoryMessage(void)
		{
			printCenteredMessageWithCol(_WHITE, "YOU WON");
		}

		void printDefeatMessage(void)
		{
			printCenteredMessageWithCol(_WHITE, "YOU LOST");
		}	
#elif (defined(__VIC20__) || defined(__C16__) ) && defined(FULL_GAME)
		void printExtraLife(void)
		{
			printCenteredMessageWithCol(_RED, "EXTRA LIFE"); 
		}
		
		void printPressKeyToStart(void)
		{
			printCenteredMessage("PRESS ANY KEY");
		}
		
		void deleteCenteredMessage(void)
		{
			printCenteredMessage("                      ");
		}
		
		void printGameOver(void)
		{
			printCenteredMessageWithCol(_WHITE, "G A M E  O V E R");
		}

		void printVictoryMessage(void)
		{
			printCenteredMessageWithCol(_WHITE, "Y O U  W O N");
		}

		void printDefeatMessage(void)
		{
			printCenteredMessageWithCol(_WHITE, "Y O U  L O S T");
		}	
#elif (defined(__VIC20__) || defined(__C16__) ) && !defined(FULL_GAME)
		void printExtraLife(void)
		{
			printCenteredMessageWithCol(_RED,   "EXTRA LIFE"); 
		}
		
		void printPressKeyToStart(void)
		{
			printCenteredMessage(               "PRESS ANY KEY");
		}
		
		void deleteCenteredMessage(void)
		{
			printCenteredMessage(               "             ");
		}
		
		void printGameOver(void)
		{
			printCenteredMessageWithCol(_WHITE, "GAME OVER");
		}

		void printVictoryMessage(void)
		{
			printCenteredMessageWithCol(_WHITE, "YOU WON");
		}

		void printDefeatMessage(void)
		{
			printCenteredMessageWithCol(_WHITE, "YOU LOST");
		}		
#elif defined(NO_CASE_LETTERS)
		void printExtraLife(void)
		{
			printCenteredMessageWithCol(_RED, "e x t r a  l i f e"); 
		}
		
		void printPressKeyToStart(void)
		{
			printCenteredMessage("press any key");
		}	
		
		void deleteCenteredMessage(void)
		{
			printCenteredMessage("                      ");
		}	

		void printGameOver(void)
		{
			printCenteredMessageWithCol(_WHITE, "g a m e   o v e r");
		}

		void printVictoryMessage(void)
		{
			printCenteredMessageWithCol(_WHITE, "y o u   w o n");
		}

		void printDefeatMessage(void)
		{
			printCenteredMessageWithCol(_WHITE, "y o u   l o s t");
		}
#else
		void printExtraLife(void)
		{
			printCenteredMessageWithCol(_RED, "e x t r a  l i f e"); 
		}
		
		void printPressKeyToStart(void)
		{
			printCenteredMessage("PRESS ANY KEY");
		}	
		
		void deleteCenteredMessage(void)
		{
			printCenteredMessage("                      ");
		}	

		void printGameOver(void)
		{
			printCenteredMessageWithCol(_WHITE, "G A M E  O V E R");
		}

		void printVictoryMessage(void)
		{
			printCenteredMessageWithCol(_WHITE, "Y O U  W O N");
		}

		void printDefeatMessage(void)
		{
			printCenteredMessageWithCol(_WHITE, "Y O U  L O S T");
		}		
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
	#else
		printCenteredMessageOnRow(6,  "Use the gun against");
	#endif	
		
	printCenteredMessageOnRow(8,  "the skull and");

	printCenteredMessageOnRow(10, "missile bases");	
	
	printCenteredMessageOnRow(12, "for points and  ");

	printCenteredMessageOnRow(14, "extra powerups ");
	
}
 
#if (defined(__SVI__) && !defined(FULL_GAME)) || (defined(__LAMBDA__) && !defined(FULL_GAME))
void printStartMessage(void)
{
}
#else
void printStartMessage(void)
{
	#if defined(__C64__) && defined(REDEFINED_CHARS)
		c64_splash_title();
	#elif defined(__ATMOS__) && defined(FULL_GAME) && defined(REDEFINED_CHARS)
		atmos_splash_title();
	#else
		printCenteredMessageOnRowWithCol(3, _RED,  "C R O S S  C H A S E");		
		SET_TEXT_COLOR(TEXT_COLOR);		
		#if defined(NO_CASE_LETTERS)
			printCenteredMessageOnRow(5, "by fabrizio caruso");		
		#else
			printCenteredMessageOnRow(5, "by Fabrizio Caruso");
		#endif
	#endif
	
	#if !defined(__ATMOS__)
		#if defined(__PLUS4__) || defined(__C16__)
			SET_TEXT_COLOR(COLOR_CYAN);	
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
		#else
			printCenteredMessageOnRow(YSize/2-1, "Lure the enemies");
		#endif		
		printCenteredMessageOnRow(YSize/2+1, "into the mines");
	#endif

	#if !defined(__ATMOS__)
		SET_TEXT_COLOR(TEXT_COLOR);		
	#endif
	
	#if defined(JOYSTICK_CONTROL) || defined(__MSX__)
		printCenteredMessageOnRow(YSize-3, "use the joystick");
	#else		
		printCenteredMessageOnRow(YSize-3, "use i j k l space");				
	#endif	
}
#endif


