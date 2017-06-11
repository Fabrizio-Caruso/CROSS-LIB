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
 
#include "display.h"

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


void displayStatsTitles(void)
{
	SET_TEXT_COLOR(COLOR_BLACK);
	
	PRINT(2,1,"SPEED:");

	PRINT(15,1,"!:");

	PRINT(2,2,"SCORE:");
	
	PRINT(15,2,"O:");

	PRINT(2,3,"LEVEL:");

	PRINT(15,3,"*:");
}



void displayStats(void)
{	
	SET_TEXT_COLOR(COLOR_BLACK);
	
	PRINT(8,1,"      ");
	PRINTF(8,1,"%u",ghostLevel);

	PRINTF(17,1,"%hu", guns);
	
	PRINTF(8,2,"%lu",points);

	PRINT(17,2,"    ");
	PRINTF(17,2,"%hu",ghostCount);

	PRINTF(8,3,"%hu", level);
	
	PRINTF(17,3,"%hu",lives);
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

void printLevel(void)
{
	char levelString[22];

	sprintf(levelString, "LEVEL %d", level);

	printCenteredMessage(levelString);
	sleep(1);
}

void printLevelBonus(void)
{
	char levelString[22];

	sprintf(levelString, "LEVEL BONUS = %d", level * 1000);

	printCenteredMessage(levelString);
	sleep(1);
}

void printPressKeyToStart(void)
{
	printCenteredMessage("PRESS ANY KEY TO START");
	sleep(1);
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

void printStartMessage(void)
{
	SET_TEXT_COLOR(COLOR_BLACK);
	PRINT((XSize - 22) / 2, YSize / 2 - 9, "A S C I I   C H A S E");
	SET_TEXT_COLOR(TEXT_COLOR);
	
	SET_TEXT_COLOR(COLOR_RED);
	PRINT((XSize - 22) / 2, YSize / 2 - 7,  "by Fabrizio Caruso");
	SET_TEXT_COLOR(TEXT_COLOR);

	#ifndef __PLUS4__
	SET_TEXT_COLOR(COLOR_BLUE);
	#endif // __PLUS4__

	#if defined(__VIC20__)
		PRINT((XSize - 22) / 2, YSize / 2 - 3, "You * are chased by O");
		
		PRINT((XSize - 22) / 2, YSize / 2 - 2, "Force O into X");
		
		PRINT((XSize - 22) / 2, YSize / 2 - 1, "Take P to slow O down");
		
		PRINT((XSize - 22) / 2, YSize / 2, "Catch ! for bullets!");
		
		PRINT((XSize - 22) / 2, YSize / 2 + 1, "Flee from +!");
	#elif defined(__PET__)
		PRINT(22, YSize / 2 - 3, "You * are chased by O. Force O into X");
		
		PRINT(20, YSize / 2 - 1,  "Take P to slow O down. Catch ! for bullets.");
		
		PRINT(30, YSize / 2 + 1, "Flee from +!");	
	#else
		PRINT(2, YSize / 2 - 3, "You * are chased by O. Force O into X");
		
		PRINT(0, YSize / 2 - 1, "Take P to slow O down. ! gives 3 bullets");
		
		PRINT(7, YSize / 2 + 1, "Flee from +!");
	#endif

	#if defined(__C64__) || defined(__C128__) || defined(__PET__)
		SET_TEXT_COLOR(COLOR_BROWN);
		PRINT((XSize - 22) / 2, YSize / 2 + 4, "Use Joystick in Port 1");
		SET_TEXT_COLOR(TEXT_COLOR);
	#elif defined(__C16__) || defined(__PLUS4__)
		SET_TEXT_COLOR(COLOR_GRAY1);
		PRINT((XSize - 22) / 2, YSize / 2 + 4, "Use Joystick in first port");
		SET_TEXT_COLOR(TEXT_COLOR);
	#elif defined(__VIC20__)
		SET_TEXT_COLOR(COLOR_GREEN);
		PRINT((XSize - 22) / 2, YSize / 2 + 4, "Use the Joystick");
		SET_TEXT_COLOR(TEXT_COLOR);
	#else
		PRINT((XSize - 22) / 2, YSize / 2 + 4, "Use the Joystick");
	#endif
	SET_TEXT_COLOR(TEXT_COLOR);
	PRINT((XSize - 22) / 2, YSize / 2 + 8, "PRESS ANY KEY TO START");
	sleep(1);
}

void gameCompleted(void)
{
	printCenteredMessage("Y O U  M A D E  I T !"); 
	sleep(2);
	printCenteredMessage("    T H E   E N D    "); 
	sleep(2);
}

void finalScore(void)
{
	char scoreString[22];
	clrscr();
	sprintf(scoreString, "SCORE:  %lu", points);
	printCenteredMessage(scoreString);
}

