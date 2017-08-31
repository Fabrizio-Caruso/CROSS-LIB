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
	unsigned char _x;
	unsigned char _y;
	
	// _status decides whether the character is active
	unsigned char _status;
	
	Image* _imagePtr;
};

typedef struct CharacterStruct Character;

#include <stdlib.h>

#include "settings.h"

#include "display_macros.h"

extern unsigned int points;

extern unsigned char XSize;
extern unsigned char YSize;

extern unsigned char ghostCount;


void initializeCharacter(Character* characterPtr, unsigned char x, unsigned char y, unsigned char status, Image * imagePtr);

void setCharacterPosition(Character* characterPtr, unsigned char x, unsigned char y);

void setCharacterDisplay(Character* characterPtr, char ch);

unsigned char isCharacterAtLocation(unsigned char x, unsigned char y, Character * characterPtr);

unsigned char areCharctersAtSamePosition(Character* lhs, Character* rhs);

unsigned char wallReached(Character *characterPtr);

void die(Character * playerPtr);

// TODO: playerReached and playerReachedBombs should be substituted by a generic collision detection routine
unsigned char playerReached(Character* preyPtr);
				  
unsigned char playerReachedBombs(Character* ghostPtr);

unsigned char ghostsMeetAlive(unsigned char preyIndex);

unsigned char ghostsMeetDead(unsigned char preyIndex);

// TODO: check bombs routines should be substituted by a generic collision detection routine
void checkBombsVsGhost(Character * ghostPtr);
						
void checkBombsVsGhosts(void);
						
unsigned char safeLocation(unsigned char x, unsigned char y, Character *danger, unsigned char dangerSize);

void relocateCharacter(Character * characterPtr, Character *danger, unsigned char dangerSize);

unsigned char innerWallReached(Character *characterPtr);

unsigned char nearInnerWall(Character *characterPtr);

void ghostDies(Character * ghostPtr);

unsigned char sameLocationAstAnyLocation(unsigned char x, unsigned char y, Character *characterList, unsigned char length);

#if defined(FULL_GAME)
	unsigned char innerWallReached(Character *characterPtr);

	unsigned char nearInnerWall(Character *characterPtr);
#endif

#endif // _CHARACTER
