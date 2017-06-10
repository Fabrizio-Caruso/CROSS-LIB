#ifndef _CHARACTER
#define _CHARACTER

struct CharacterStruct
{
	// character coordinates
	short _x;
	short _y;
	
	// how to display the character (i.e., which ASCII character to use
	char _ch;
	
	// _status decides whether the character is active
	char _status;
	
	//_alive decides whether it is dead or alive
	char _alive;
	
};

typedef struct CharacterStruct Character;

#include <stdlib.h>



#include "character.h"
#include "settings.h"

#include "display_macros.h"

extern unsigned long points;
extern unsigned short innerVerticalWallX;
extern unsigned short innerVerticalWallY;
extern unsigned short innerVerticalWallLength;

extern unsigned char XSize;
extern unsigned char YSize;

extern unsigned short ghostCount;


void initializeCharacter(Character* characterPtr, int x, int y, char ch, short status);

void setCharacterPosition(Character* characterPtr, short x, short y);

void setCharacterDisplay(Character* characterPtr, char ch);

void deleteCharacter(Character* characterPtr);

void displayCharacter(Character* characterPtr);

int isCharacterAtLocation(short x, short y, Character * characterPtr);

int areCharctersAtSamePosition(Character* lhs, Character* rhs);

int leftDanger(Character* characterPtr, Character* bombPtr);

int rightDanger(Character* characterPtr, Character* bombPtr);

int upDanger(Character* characterPtr, Character* bombPtr);

int downDanger(Character* characterPtr, Character* bombPtr);

int leftBombs(Character* characterPtr, 
              Character** bombs);

int rightBombs(Character* characterPtr, 
              Character** bombs);
			  
int upBombs(Character* characterPtr, 
              Character** bombs);

int downBombs(Character* characterPtr, 
              Character** bombs);
			  
int leftGhosts(Character* characterPtr, 
              Character** ghosts);
			  
int rightGhosts(Character* characterPtr, 
              Character** ghosts);
			  
int upGhosts(Character* characterPtr, 
              Character** ghosts);
			  
int downGhosts(Character* characterPtr, 
              Character** ghosts);

int wallReached(Character *characterPtr);

void die(Character * playerPtr);

int playerReached(Character ** ghosts, 
                  Character* preyPtr);
				  
int playerReachedBombs(Character ** bombs,  
					   Character* ghostPtr);

int charactersMeet(short preyIndex, Character **ghosts);

void checkBombsVsGhost(Character ** bombs,
					   Character * ghostPtr);
						

void checkBombsVsGhosts(Character ** bombs,
						Character ** ghosts);


int safeLocation(int x, int y, 
				Character ** bombs,
				Character ** ghosts);


void relocateCharacter(Character * characterPtr, 
						Character ** bombs,
						Character ** ghosts);


short innerWallReached(Character *characterPtr);

short nearInnerWall(Character *characterPtr);


void checkGhostsVsGhosts(Character ** ghosts);

#endif