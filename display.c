
#include "display.h"

#include "display_macros.h"

#include "settings.h"


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
	PRINT(2,1,"SPEED:");

	PRINT(15,1,"!:");

	PRINT(2,2,"SCORE:");
	
	PRINT(15,2,"O:");

	PRINT(2,3,"LEVEL:");

	PRINT(15,3,"*:");
}



void displayStats(void)
{	
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
	PRINTF((XSize - strlen (Text)) / 2, YSize / 2,"%s", Text);
}

void printLevel(void)
{
	char levelString[22];

	sprintf(levelString, "LEVEL %d", level);

	printCenteredMessage(levelString);
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
}

void deleteCenteredMessage(void)
{
	PRINT((XSize - 22) / 2, YSize / 2, "                      ");
}


void printGameOver(void)
{
	printCenteredMessage("G A M E   O V E R");
}

void printVictoryMessage(void)
{
	printCenteredMessage("Y O U   W O N ! !");
}

void printDefeatMessage(void)
{
	printCenteredMessage("Y O U   L O S T !");
}

void defeat(void)
{
	printDefeatMessage();
	sleep(1);
}

void victory(void)
{
	printVictoryMessage();
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
	PRINT((XSize - 22) / 2, YSize / 2 - 3, "You * are chased by O");
	
	PRINT((XSize - 22) / 2, YSize / 2 - 2, "Force O into X");
	
	PRINT((XSize - 22) / 2, YSize / 2 - 1, "Take P to slow O down");
	
	PRINT((XSize - 22) / 2, YSize / 2, "Catch ! for bullets!");
	
	PRINT((XSize - 22) / 2, YSize / 2 + 1, "Flee from +!");
	
	PRINT((XSize - 22) / 2, YSize / 2 + 4, "Use Joystick in Port 1");
	
	SET_TEXT_COLOR(TEXT_COLOR);
	PRINT((XSize - 22) / 2, YSize / 2 + 8, "PRESS ANY KEY TO START");
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
	sleep(3);
}

