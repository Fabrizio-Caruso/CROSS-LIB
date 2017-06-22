/*****************************************************************************/
/*                                                                           */
/*                                		                             */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* (C) 2017      Fabrizio Caruso                                             */
/*                					                     */
/*              				                             */
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
 
#ifndef _CHARACTER
#define _CHARACTER

#include "display_macros.h"

struct CharacterStruct
{
	// character coordinates
	short _x;
	short _y;
	
	// _status decides whether the character is active
	char _status;
	
	Image* _imagePtr;
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


void initializeCharacter(Character* characterPtr, int x, int y, short status, Image * imagePtr);

void setCharacterPosition(Character* characterPtr, short x, short y);

void setCharacterDisplay(Character* characterPtr, char ch);

int isCharacterAtLocation(short x, short y, Character * characterPtr);

int areCharctersAtSamePosition(Character* lhs, Character* rhs);

int wallReached(Character *characterPtr);

void die(Character * playerPtr);

//void ghost_die(Character * playerPtr);

// TODO: playerReached and playerReachedBombs should be substituted by a generic collision detection routine
int playerReached(Character* preyPtr);
				  
int playerReachedBombs(Character* ghostPtr);


int charactersMeet(unsigned char preyIndex);

// TODO: check bombs routines should be substituted by a generic collision detection routine
void checkBombsVsGhost(Character * ghostPtr);
						

void checkBombsVsGhosts(void);

void checkGhostsVsGhosts(void);
						
						
int safeLocation(unsigned char x, unsigned char y, Character **danger, unsigned char dangerSize);


void relocateCharacter(Character * characterPtr, Character **danger, unsigned char dangerSize);


short innerWallReached(Character *characterPtr);

short nearInnerWall(Character *characterPtr);


#endif
