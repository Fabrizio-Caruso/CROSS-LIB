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

#include "sleep_macros.h"

#ifdef __ATMOS__
	#include<peekpoke.h>
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

#if defined(FULL_GAME)
	#if defined(__C64__)
		void printKillTheSkull(void)
		{
			printCenteredMessage("kill the skull!");	
		}
	#else
		void printKillTheSkull(void)
		{
			printCenteredMessage("Kill the skull!");
		}	
	#endif	
#endif


// TODO: This is SLOW
void displayStatsTitles(void)
{	
	#if defined(CC65) && defined(WIDE)
		SET_TEXT_COLOR(COLOR_BLUE);		
		PRINT(2-X_OFFSET,0-Y_OFFSET,"score:");
		PRINT(2-X_OFFSET,1-Y_OFFSET,"level:");
	#elif defined(Z88DK) && defined(WIDE)
		PRINT(2,-Y_OFFSET,"SCORE:");
		PRINT(2,-Y_OFFSET+1,"LEVEL:");	
	#else
		// Nothing
	#endif
	
	
	#if defined(CC65) && !defined(NARROW)
		SET_TEXT_COLOR(COLOR_RED);	
		PRINT(24,0-Y_OFFSET,"-----------");
		PRINT(24,1-Y_OFFSET,"cross chase");	
	#endif	


	#if defined(WIDE)
		SET_TEXT_COLOR(TEXT_COLOR);	
		gotoxy(18+1,0 + ADJUST ); cputc(GUN_IMAGE._imageData);cputc(':');
		gotoxy(18-3,0 + ADJUST); cputc(GHOST_IMAGE._imageData);cputc(':');
		gotoxy(18,1 + ADJUST); cputc(PLAYER_IMAGE._imageData);cputc(':');	
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
	#if defined(WIDE)
		SET_TEXT_COLOR(TEXT_COLOR);	
		PRINTF(18+2-X_OFFSET-3,0-Y_OFFSET,"%u",ghostCount);
	#else
		PRINTF(15+2-X_OFFSET-3-2-3,0-Y_OFFSET,"%u",ghostCount);	
	#endif	
}

void printLivesStats(void)
{
	#if defined(WIDE)
		SET_TEXT_COLOR(TEXT_COLOR);	
		PRINTF(18+2-X_OFFSET,1-Y_OFFSET,"%02u",lives);
	#else
		PRINTF(15+2+1-5+4-1-X_OFFSET,0-Y_OFFSET,"%u",lives);	
	#endif	
}

void displayStats(void)
{	
	#if defined(WIDE)
		SET_TEXT_COLOR(TEXT_COLOR);	
		PRINTF(8-X_OFFSET,0-Y_OFFSET,"%05u0",points);
	#else
		PRINTF(3-3,0-Y_OFFSET,"%05u0",points);	
	#endif	
}

void setScreenColors(void)
{
	#if !defined(CC65) && !defined(__ATMOS__)
	#else
		SET_TEXT_COLOR(TEXT_COLOR);
		
		SET_BORDER_COLOR(BORDER_COLOR);
		
		SET_BACKGROUND_COLOR(BACKGROUND_COLOR);
	#endif
}



void printCenteredMessageOnRow(unsigned char row, char *Text)
{
	PRINTF((XSize - strlen (Text)) / 2, row,"%s", Text);
}

void printCenteredMessage(char *Text)
{
	printCenteredMessageOnRow(YSize / 2, Text);
	
}

#if defined (__C64__)
	void printLevel(void)
	{
		char levelString[22];

		sprintf(levelString, "level %d", level);

		printCenteredMessage(levelString);
	}
#elif defined(__CPC__)
	void printLevel(void) {}
#elif defined(__ZX81__)
	void printLevel(void) {}
#elif defined(__VZ__)
	void printLevel(void) {}
#else
	void printLevel(void)
	{
		char levelString[22];

		sprintf(levelString, "LEVEL %d", level);

		printCenteredMessage(levelString);
	}
#endif

#if defined (__VG5K__)
void _printScore(char * text, unsigned int score)
{
	char levelString[22];

	sprintf(levelString, text, score);

	printCenteredMessage(levelString);	
}
#elif defined(__VZ__)
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
		printCenteredMessage("DONE!"); 
	}
#elif defined(__C64__)
	void gameCompleted(void)
	{
		printCenteredMessage("y o u   m a d e   i t !"); 
	}
#elif defined(__VZ__)
	void gameCompleted(void)
	{}
#else
	void gameCompleted(void)
	{
		printCenteredMessage("Y O U  M A D E  I T!"); 
	}
#endif


#if defined(__VG5K__)
		void printPressKeyToStart(void)
		{
			printCenteredMessage("PRESS ANY KEY");
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
			printCenteredMessage("YOU WON!");
		}

		void printDefeatMessage(void)
		{
			printCenteredMessage("YOU LOST!");
		}	
#elif defined(__VZ__)
		void printPressKeyToStart(void)
		{}

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
			printCenteredMessage("YOU WON!");
		}

		void printDefeatMessage(void)
		{
			printCenteredMessage("YOU LOST!");
		}	
#elif defined(__VIC20__) || defined(__ATARI__) || defined(__ATARIXL__)
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
			printCenteredMessage("G A M E  O V E R");
		}

		void printVictoryMessage(void)
		{
			printCenteredMessage("Y O U  W O N !");
		}

		void printDefeatMessage(void)
		{
			printCenteredMessage("Y O U  L O S T !");
		}		
#elif defined(__C16__)
		void printPressKeyToStart(void)
		{
			printCenteredMessage("PRESS ANY KEY TO START");
		}	
		
		void deleteCenteredMessage(void)
		{                                           
			printCenteredMessage("                      ");
		}
		
		void printGameOver(void)
		{
			printCenteredMessage("G A M E  O V E R");
		}

		void printVictoryMessage(void)
		{
			printCenteredMessage("Y O U  W O N !");
		}

		void printDefeatMessage(void)
		{
			printCenteredMessage("Y O U  L O S T !");
		}		
#elif defined(__C64__)
		void printPressKeyToStart(void)
		{
			printCenteredMessage("press any key to start");
		}	
		
		void deleteCenteredMessage(void)
		{
			printCenteredMessage("                      ");
		}	

		void printGameOver(void)
		{
			printCenteredMessage("g a m e   o v e r");
		}

		void printVictoryMessage(void)
		{
			printCenteredMessage("y o u   w o n !");
		}

		void printDefeatMessage(void)
		{
			printCenteredMessage("y o u   l o s t !");
		}
#else
		void printPressKeyToStart(void)
		{
			printCenteredMessage("PRESS ANY KEY TO START");
		}	
		
		void deleteCenteredMessage(void)
		{
			printCenteredMessage("                      ");
		}	

		void printGameOver(void)
		{
			printCenteredMessage("G A M E  O V E R");
		}

		void printVictoryMessage(void)
		{
			printCenteredMessage("Y O U  W O N !");
		}

		void printDefeatMessage(void)
		{
			printCenteredMessage("Y O U  L O S T !");
		}		
#endif



// SPLASH SCREEN

#if defined(__C64__) && defined(REDEFINED_CHARS)
	void c64_splash_title(void)
	{
		SET_TEXT_COLOR(COLOR_RED);
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

 
void printStartMessage(void)
{
	#if defined(__C64__) && defined(REDEFINED_CHARS)
		c64_splash_title();
	#elif defined(__ATMOS__) && defined(FULL_GAME) && defined(REDEFINED_CHARS)
		// atmos_splash_title();
		printCenteredMessageOnRow(3, "C R O S S  C H A S E");		
		printCenteredMessageOnRow(5, "by Fabrizio Caruso");		
	#else
		SET_TEXT_COLOR(COLOR_RED);
		printCenteredMessageOnRow(3, "C R O S S  C H A S E");
		SET_TEXT_COLOR(TEXT_COLOR);			
		printCenteredMessageOnRow(5, "by Fabrizio Caruso");
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
		// atmos_splash_instructions();
	#else
		printCenteredMessageOnRow(YSize/2-1, "Lure the enemies");
		printCenteredMessageOnRow(YSize/2+1, "into the mines");
	#endif

	#if !defined(__ATMOS__)
		SET_TEXT_COLOR(TEXT_COLOR);		
	#endif
	
	#if defined(JOYSTICK_CONTROL) || defined(__MSX__)
		printCenteredMessageOnRow(YSize-2, "use the joystick");
	#else
		#if defined(__ATMOS__)
			printCenteredMessageOnRow(YSize-4, "use w a s d space");
		#else
			printCenteredMessageOnRow(YSize-4, "use i j k l space");			
		#endif
	#endif	

}



