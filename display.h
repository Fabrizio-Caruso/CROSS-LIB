
#ifndef _DISPLAY
#define _DISPLAY

#include <stdlib.h>
#include <string.h>

#include <stdio.h>


void displayStatsTitles(void);

void displayStats(void);

void drawBorders(void);

void setScreenColors(void);

void printCenteredMessage(char *Text);

void printLevel(void);

void printLevelBonus(void);

void printPressKeyToStart(void);

void deleteCenteredMessage(void);

void printGameOver(void);

void printVictoryMessage(void);

void printDefeatMessage(void);

void defeat(void);

void victory(void);

void printStartMessage(void);

void gameCompleted(void);

void finalScore(void);

#endif // _DISPLAY

