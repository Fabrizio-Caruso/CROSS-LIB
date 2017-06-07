
#ifndef _DISPLAY
#define _DISPLAY

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include <conio.h>

#include <unistd.h>

#include <time.h>

void displayStatsTitles(void);

void displayStats(unsigned short level, unsigned short lives, unsigned short guns, 
				  unsigned long points, unsigned int ghostCount, unsigned int ghostLevel);


void drawBorders(int XSize, int YSize);

void setScreenColors(void);

void printCenteredMessage(int XSize, int YSize, char *Text);

void printLevel(int XSize, int YSize, int level);

void printLevelBonus(int XSize, int YSize, int level);

void printPressKeyToStart(int XSize, int YSize);

void deleteCenteredMessage(int XSize, int YSize);

void printGameOver(int XSize, int YSize);

void printVictoryMessage(int XSize, int YSize);

void printDefeatMessage(int XSize, int YSize);

void defeat(int XSize, int YSize);

void victory(int XSize, int YSize);

void printStartMessage(int XSize, int YSize);

void gameCompleted(int XSize, int YSize);

void finalScore(int XSize, int YSize, unsigned long points);

#endif // _DISPLAY

