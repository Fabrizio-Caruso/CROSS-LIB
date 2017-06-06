
#include "display.h"

void displayStatsTitles(void)
{
	// Draw score 
	gotoxy(2,1);
	cputs("SPEED:");

	// Draw bullets 
	gotoxy(14,1);
	cputs("!:");
	
	// Draw score 
	gotoxy(2,2);
	cputs("SCORE:");
	
	// Draw ghost count
	gotoxy(14,2);
	cputs("O:");
	
	// Draw level 
	gotoxy(2,3);
	cputs("LEVEL:");

	// Draw lives
	gotoxy(14,3);
	cputs("*:");	
}


void displayStats(unsigned short level, unsigned short lives, 
				  unsigned short guns, unsigned long points, 
				  unsigned int ghostCount, unsigned int ghostLevel)
{
	/*
	// Draw score 
	gotoxy(2,1);
	cputs("SPEED:");
	*/
	
	gotoxy(8,1);
	cputs("      ");
	gotoxy(8,1);
	cprintf("%u",ghostLevel);

	/*
	// Draw bullets 
	gotoxy(14,1);
	cputs("!:");
	*/
	
	gotoxy(16,1);
	gotoxy(16,1);
	cprintf("%hu", guns);

	/*
	// Draw score 
	gotoxy(2,2);
	cputs("SCORE:");
	*/
	
	gotoxy(8,2);
	gotoxy(8,2);
	cprintf("%lu",points);

	/*
	// Draw ghost count
	gotoxy(14,2);
	cputs("O:");
	*/
	
	gotoxy(16,2);
	cputs("    ");
	gotoxy(16,2);
	cprintf("%u",ghostCount);

	/*
	// Draw level 
	gotoxy(2,3);
	cputs("LEVEL:");
	*/
	
	gotoxy(8,3);
	gotoxy(8,3);
	cprintf("%hu", level);

	/*
	// Draw lives
	gotoxy(14,3);
	cputs("*:");
	*/
	
	gotoxy(16,3);
	gotoxy(16,3);
	cprintf("%hu",lives);
}



void drawBorders(int XSize, int YSize)
{
	/* Clear the screen, put cursor in upper left corner */
    clrscr ();
	
	/* Top line */
    cputc (CH_ULCORNER);
    chline (XSize - 2);
    cputc (CH_URCORNER);

    /* Vertical line, left side */
    cvlinexy (0, 1, YSize - 2);

    /* Bottom line */
    cputc (CH_LLCORNER);
    chline (XSize - 2);
    cputc (CH_LRCORNER);

    /* Vertical line, right side */
    cvlinexy (XSize - 1, 1, YSize - 2);
}

void setScreenColors(void)
{
    (void) textcolor (COLOR_WHITE);
    (void) bordercolor (COLOR_BLACK);
    (void) bgcolor (COLOR_BLACK);
}


void printCenteredMessage(int XSize, int YSize, char *Text)
{
	gotoxy ((XSize - strlen (Text)) / 2, YSize / 2);
    cprintf ("%s", Text);
}

void printLevel(int XSize, int YSize, int level)
{
	char levelString[22];

	sprintf(levelString, "LEVEL %d", level);

	printCenteredMessage(XSize, YSize, levelString);
}

void printLevelBonus(int XSize, int YSize, int level)
{
	char levelString[22];

	sprintf(levelString, "LEVEL BONUS = %d", level * 1000);

	printCenteredMessage(XSize, YSize, levelString);
	sleep(1);
}

void printPressKeyToStart(int XSize, int YSize)
{
	printCenteredMessage(XSize, YSize, "PRESS ANY KEY TO START");
}

void deleteCenteredMessage(int XSize, int YSize)
{
	gotoxy ((XSize - 22) / 2, YSize / 2);
    cputs( "                      ");
}




void printGameOver(int XSize, int YSize)
{
	printCenteredMessage(XSize, YSize, "G A M E   O V E R");
}

void printVictoryMessage(int XSize, int YSize)
{
	printCenteredMessage(XSize, YSize, "Y O U   W O N ! !");
}

void printDefeatMessage(int XSize, int YSize)
{
	printCenteredMessage(XSize, YSize, "Y O U   L O S T !");
}

void defeat(int XSize, int YSize)
{
	printDefeatMessage(XSize, YSize);
	sleep(1);
}

void victory(int XSize, int YSize)
{
	printVictoryMessage(XSize, YSize);
	sleep(1);
}


void printStartMessage(int XSize, int YSize)
{
	gotoxy ((XSize - 22) / 2, YSize / 2 - 9);
	cprintf ("%s", "A S C I I   C H A S E");
	
	gotoxy ((XSize - 22) / 2, YSize / 2 - 7);
	cprintf ("%s", "by Fabrizio Caruso");
	
	/*
	gotoxy ((XSize - 9) / 2, YSize / 2 - 4);
	cprintf ("%s", "GAME PLAY");
	*/
	
	gotoxy ((XSize - 22) / 2, YSize / 2 - 3);
	cprintf ("%s", "You * are chased by O");
	
	gotoxy ((XSize - 22) / 2, YSize / 2 - 2);
	cprintf ("%s", "Force O into X");
	
	gotoxy ((XSize - 22) / 2, YSize / 2 - 1);	
	cprintf ("%s", "Take P to slow O down");
	
	
	gotoxy ((XSize - 22) / 2, YSize / 2);
	cprintf ("%s", "Catch ! for bullets!");
	
	gotoxy ((XSize - 22) / 2, YSize / 2 +1);
	cprintf ("%s", "Flee from +!");
	
/*	
	gotoxy ((XSize - 4) / 2, YSize / 2 + 3);
	cprintf ("%s", "KEYS");
	*/
	gotoxy ((XSize - 22) / 2, YSize / 2 + 4);
	cprintf ("%s", "Use Joystick in Port 1");

/*	
	gotoxy ((XSize - 22) / 2, YSize / 2 + 5);
	cprintf("%s",  "and shoot with SPACE");
*/

	gotoxy ((XSize - 22) / 2, YSize / 2 + 8);
	cprintf("%s",  "PRESS ANY KEY TO START");
}

void gameCompleted(int XSize, int YSize)
{
	printCenteredMessage(XSize, YSize, "Y O U  M A D E  I T !"); 
	sleep(2);
	printCenteredMessage(XSize, YSize, "    T H E   E N D    "); 
	sleep(2);
}

void finalScore(int XSize, int YSize, unsigned long points)
{
	char scoreString[22];
	clrscr();
	sprintf(scoreString, "SCORE:  %lu", points);
	printCenteredMessage(XSize, YSize, scoreString);
	sleep(3);
}

void drawInnerVerticalWall(int XSize, int YSize, unsigned short innerVerticalWallLength)
{
	cvlinexy (XSize/2, YSize/2-(innerVerticalWallLength/2), innerVerticalWallLength);
}

/*
void drawInnerVerticalWall(int XSize, int YSize, short length)
{
	innerVerticalWallLength = length;
	innerVerticalWallX = XSize / 2;
	innerVerticalWallY = YSize/2-(innerVerticalWallLength/2);
	drawInnerVerticalWall(XSize, YSize);
}
*/