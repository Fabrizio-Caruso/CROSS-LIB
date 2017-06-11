#ifndef _CHARACTER
#define _CHARACTER

struct CharacterStruct
{
	// character coordinates
	short _x;
	short _y;
	
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


void initializeCharacter(Character* characterPtr, int x, int y, short status);//char ch, short status);

void setCharacterPosition(Character* characterPtr, short x, short y);

void setCharacterDisplay(Character* characterPtr, char ch);

int isCharacterAtLocation(short x, short y, Character * characterPtr);

int areCharctersAtSamePosition(Character* lhs, Character* rhs);

int wallReached(Character *characterPtr);

void die(Character * playerPtr);

// TODO: playerReached and playerReachedBombs should be substituted by a generic collision detection routine
int playerReached(Character ** ghosts, 
                  Character* preyPtr);
				  
int playerReachedBombs(Character ** bombs,  
					   Character* ghostPtr);


int charactersMeet(short preyIndex, Character **ghosts);

// TODO: check bombs routines should be substituted by a generic collision detection routine
void checkBombsVsGhost(Character ** bombs,
					   Character * ghostPtr);
						

void checkBombsVsGhosts(Character ** bombs,
						Character ** ghosts);

void checkGhostsVsGhosts(Character ** ghosts);
						
						
int safeLocation(int x, int y, 
				Character ** bombs,
				Character ** ghosts);


void relocateCharacter(Character * characterPtr, 
						Character ** bombs,
						Character ** ghosts);


short innerWallReached(Character *characterPtr);

short nearInnerWall(Character *characterPtr);



#endif