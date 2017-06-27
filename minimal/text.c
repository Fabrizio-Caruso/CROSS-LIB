/*****************************************************************************/
/*                                                                           */
/*                                		                                     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* (C) 2017      Fabrizio Caruso                                  		     */
/*                					                                         */
/*              				                                             */
/* EMail:        Fabrizio_Caruso@hotmail.com                                 */
/*                                                                           */
/*                                                                           */
/* This software is provided 'as-is', without any expressed or implied       */
/* warranty.  In no event will the authors be held liable for any damages    */
/* arising from the use of this software.                                    */
/*                                                                           */
/* Permission is granted to anyone to use this software for any purpose,     */
/* including commercial applications, and to alter it and redistribute it    */
/* freely, subject to the following restrictions:                            */
/*                                                                           */
/* 1. The origin of this software must not be misrepresented; you must not   */
/*    claim that you wrote the original software. If you use this software   */
/*    in a product, an acknowledgment in the product documentation would be  */
/*    appreciated but is not required.                                       */
/* 2. Altered source versions must be plainly marked as such, and must not   */
/*    be misrepresented as being the original software.                      */
/* 3. This notice may not be removed or altered from any source              */
/*    distribution.                                                          */
/*                                                                           */
/*****************************************************************************/
 
#include "text.h"

#include "display_macros.h"

#include "settings.h"

#include "sleep_macros.h"


extern unsigned short level;
extern unsigned char XSize;
extern unsigned char YSize;

extern unsigned short level;
extern unsigned short lives;
extern unsigned short guns;
extern unsigned long points;
extern unsigned int ghostCount;
extern unsigned int ghostLevel;

 extern Image PLAYER_IMAGE;
 extern Image GHOST_IMAGE;
 extern Image GUN_IMAGE;


// TODO: This is SLOW
void displayStatsTitles(void)
{
	SET_TEXT_COLOR(COLOR_BLACK);
	
	#if defined(__C64__) 
		PRINT(2,1,"speed:");
		PRINT(2,2,"score:");
		PRINT(2,3,"level:");
	//#elif defined(__ATARI__) || defined(__ATARIXL__) 
	#else
		PRINT(2,1,"SPEED:");
		PRINT(2,2,"SCORE:");
		PRINT(2,3,"LEVEL:");
	#endif
	
	#if defined (__ATMOS__)
		gotoxy(16,1); cputc(GUN_IMAGE._imageData);cputc(':');
		gotoxy(16,2); cputc(GHOST_IMAGE._imageData);cputc(':');
		gotoxy(16,3); cputc(PLAYER_IMAGE._imageData);cputc(':');	
	//#elif defined(__ATARI__) || defined(__ATARIXL__) 
	#else
		gotoxy(15,1); cputc(GUN_IMAGE._imageData);cputc(':');
		gotoxy(15,2); cputc(GHOST_IMAGE._imageData);cputc(':');
		gotoxy(15,3); cputc(PLAYER_IMAGE._imageData);cputc(':');
	#endif

}



// TODO: This is SLOW
void displayStats(void)
{	
	SET_TEXT_COLOR(COLOR_BLACK);
	
	//#if defined(__ATARI__) || defined(__ATARIXL__)
	//#else	
		PRINT(8,1,"      ");
		PRINTF(8,1,"%u",ghostLevel);
		PRINTF(8,2,"%lu",points);
		PRINTF(8,3,"%hu", level);
	//#endif
	
	#if defined (__ATMOS__)
		PRINTF(18,1,"%hu", guns);
		PRINT( 18,2,"    ");
		PRINTF(18,2,"%hu",ghostCount);
		PRINTF(18,3,"%hu",lives);	
	//#elif defined(__ATARI__) || defined(__ATARIXL__) 
	#else
		PRINTF(17,1,"%hu", guns);
		PRINT( 17,2,"    ");
		PRINTF(17,2,"%hu",ghostCount);
		PRINTF(17,3,"%hu",lives);
	#endif		
}



void drawBorders(void)
{
	CLEAR_SCREEN();
	
	DRAW_BORDERS();
}

void setScreenColors(void)
{
	SET_TEXT_COLOR(TEXT_COLOR);
	
	SET_BORDER_COLOR(BORDER_COLOR);
	
	SET_BACKGROUND_COLOR(BACKGROUND_COLOR);
}


void printCenteredMessage(char *Text)
{
	SET_TEXT_COLOR(TEXT_COLOR);
	PRINTF((XSize - strlen (Text)) / 2, YSize / 2,"%s", Text);
}

#ifdef __C64__
void printLevel(void)
{
	char levelString[22];

	sprintf(levelString, "level %d", level);

	printCenteredMessage(levelString);
}
#else
void printLevel(void)
{
	char levelString[22];

	sprintf(levelString, "LEVEL %d", level);

	printCenteredMessage(levelString);
}
#endif

#ifdef __C64__
void printLevelBonus(void)
{
	char levelString[22];

	sprintf(levelString, "level bonus = %d", level * 1000);

	printCenteredMessage(levelString);
	sleep(1);
}

void gameCompleted(void)
{
	printCenteredMessage("y o u   m a d e   i t !"); 
	sleep(1);
	printCenteredMessage("    t h e   e n d    "); 
	sleep(1);
}

void finalScore(void)
{
	char scoreString[22];
	clrscr();
	sprintf(scoreString, "score:  %lu", points);
	printCenteredMessage(scoreString);
}
#else
void printLevelBonus(void)
{
	char levelString[22];

	sprintf(levelString, "LEVEL BONUS = %d", level * 1000);

	printCenteredMessage(levelString);
	sleep(1);
}

void gameCompleted(void)
{
	printCenteredMessage("Y O U   M A D E   I T !"); 
	sleep(1);
	printCenteredMessage("    T H E   E N D    "); 
	sleep(1);
}

void finalScore(void)
{
	char scoreString[22];
	clrscr();
	sprintf(scoreString, "SCORE:  %lu", points);
	printCenteredMessage(scoreString);
}

#endif

#ifdef __C64__
	void printPressKeyToStart(void)
	{
		printCenteredMessage("press any key to start");
	}

	void deleteCenteredMessage(void)
	{
		PRINT((XSize - 22) / 2, YSize / 2, "                      ");
	}


	void printGameOver(void)
	{
		printCenteredMessage("g a m e   o v e r");
		sleep(1);
	}

	void printVictoryMessage(void)
	{
		printCenteredMessage("y o u   w o n ! !");
		sleep(1);
	}

	void printDefeatMessage(void)
	{
		printCenteredMessage("y o u   l o s t !");
		sleep(1);
	}
#else
	void printPressKeyToStart(void)
	{
		printCenteredMessage("PRESS ANY KEY TO START");
	}

	void deleteCenteredMessage(void)
	{
		PRINT((XSize - 22) / 2, YSize / 2, "                      ");
	}


	void printGameOver(void)
	{
		printCenteredMessage("G A M E   O V E R");
		sleep(1);
	}

	void printVictoryMessage(void)
	{
		printCenteredMessage("Y O U   W O N ! !");
		sleep(1);
	}

	void printDefeatMessage(void)
	{
		printCenteredMessage("Y O U   L O S T !");
		sleep(1);
	}

#endif


void printStartMessage(void)
{
	#ifdef __C64__
		SET_TEXT_COLOR(COLOR_BLACK);
		PRINT((XSize - 22) / 2, YSize / 2 - 9, "a s c i i   c h a s e");
		SET_TEXT_COLOR(TEXT_COLOR);
		
		SET_TEXT_COLOR(COLOR_RED);
		PRINT((XSize - 22) / 2, YSize / 2 - 7,  "by fabrizio caruso");
		SET_TEXT_COLOR(TEXT_COLOR);	
		#else
		SET_TEXT_COLOR(COLOR_BLACK);
		PRINT((XSize - 22) / 2, YSize / 2 - 9, "A S C I I   C H A S E");
		SET_TEXT_COLOR(TEXT_COLOR);
		
		SET_TEXT_COLOR(COLOR_RED);
		PRINT((XSize - 22) / 2, YSize / 2 - 7,  "by Fabrizio Caruso");
		SET_TEXT_COLOR(TEXT_COLOR);
	#endif
	
	#ifndef __PLUS4__
		SET_TEXT_COLOR(COLOR_BLUE);
	#endif // __PLUS4__

	#if defined(__VIC20__)
		PRINT((XSize - 22) / 2, YSize / 2 - 3, "You * are chased by O");
		
		PRINT((XSize - 22) / 2, YSize / 2 - 2, "Force O into X");
		
		PRINT((XSize - 22) / 2, YSize / 2 - 1, "Take S to slow O down");
		
		PRINT((XSize - 22) / 2, YSize / 2, "Catch ! for bullets!");
		
		PRINT((XSize - 22) / 2, YSize / 2 + 1, "Flee from +!");
	#elif defined(__PET__)
		PRINT(22, YSize / 2 - 3, "You * are chased by O. Force O into X");
		
		PRINT(20, YSize / 2 - 1,  "Take S to slow O down. Catch ! for bullets.");
		
		PRINT(30, YSize / 2 + 1, "Flee from +!");
	#elif defined(__C64__) || defined(__ATMOS__)
		PRINT((XSize - 22) / 2, YSize / 2 - 3, "escape the enemies");
		
		PRINT((XSize - 22) / 2, YSize / 2 - 1, "force them into the mines");
		
		PRINT((XSize - 22) / 2, YSize / 2 + 1, "catch the gun for bullets");
	#else
		PRINT(2, YSize / 2 - 3, "You * are chased by O. Force O into X");
		
		PRINT(0, YSize / 2 - 1, "Take S to slow O down. ! gives 3 bullets");
		
		PRINT(7, YSize / 2 + 1, "Flee from +!");
	#endif

	#if defined(__C64__) || defined(__C128__) || defined(__PET__)
		SET_TEXT_COLOR(COLOR_BROWN);
		PRINT((XSize - 22) / 2, YSize / 2 + 4, "use joystick in port 1");
		SET_TEXT_COLOR(TEXT_COLOR);
	#elif defined(__C16__) || defined(__PLUS4__)
		SET_TEXT_COLOR(COLOR_GRAY1);
		PRINT((XSize - 22) / 2, YSize / 2 + 4, "Use Joystick in first port");
		SET_TEXT_COLOR(TEXT_COLOR);
	#elif defined(__VIC20__)
		SET_TEXT_COLOR(COLOR_GREEN);
		PRINT((XSize - 22) / 2, YSize / 2 + 4, "Use the Joystick");
		SET_TEXT_COLOR(TEXT_COLOR);
	#elif defined(__ATMOS__) || defined(__APPLE2__) || defined(__APPLE2ENH__)
		PRINT((XSize - 22) / 2, YSize / 2 + 4, "Use W A S D <SPACE>");
	#else 
		PRINT((XSize - 22) / 2, YSize / 2 + 4, "Use the Joystick");
	#endif
	SET_TEXT_COLOR(TEXT_COLOR);
	PRINT((XSize - 22) / 2, YSize / 2 + 8, "press any key to start");
}



