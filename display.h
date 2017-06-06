
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

#endif // _DISPLAY

