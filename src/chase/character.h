/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 
 
#ifndef _CHARACTER
#define _CHARACTER

#include "../cross_lib/display/display_macros.h"

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


#if defined(__CMOC__) && !defined(__WINCMOC__)
	#include <cmoc.h>
#else
	#include <stdlib.h>
#endif

#include "settings.h"

#if defined(REDEFINED_CHARS)
	#define PLAYER_IMAGE PLAYER_DOWN
#endif

#define playerReached(preyPtr) sameLocationAsAnyLocation((preyPtr)->_x, (preyPtr)->_y, ghosts, GHOSTS_NUMBER)

#define  playerReachedBombs(preyPtr) sameLocationAsAnyLocation((preyPtr)->_x, (preyPtr)->_y, bombs, BOMBS_NUMBER)

#define areCharctersAtSamePosition(lhs, rhs)isCharacterAtLocation((lhs)->_x, (lhs)->_y,rhs)

extern unsigned short points;

extern unsigned char ghostCount;

#if defined(FULL_GAME)
	unsigned char playerKilledBy(Character *enemyPtr);
#endif

void displayCharacter(Character * characterPtr);

void deleteCharacter(Character * characterPtr);

//

#if defined(REDEFINED_CHARS)
	extern Image PLAYER_LEFT;
	extern Image PLAYER_RIGHT;
	extern Image PLAYER_UP;
	extern Image PLAYER_DOWN;

	#define SHOW_LEFT() player._imagePtr = (Image *)&PLAYER_LEFT
	#define SHOW_RIGHT() player._imagePtr = (Image *)&PLAYER_RIGHT 
	#define SHOW_UP() player._imagePtr = (Image *)&PLAYER_UP
	#define SHOW_DOWN() player._imagePtr = (Image *)&PLAYER_DOWN
#else
	#define SHOW_LEFT() { }
	#define SHOW_RIGHT() { }
	#define SHOW_UP() { }
	#define SHOW_DOWN() { }		
#endif

#define DRAW_CHARACTER(x,y,image) _draw(x,y,image)

#define DRAW_PLAYER(x,y,image) DRAW_CHARACTER(x,y,image)
#define DRAW_GHOST(x,y,image) DRAW_CHARACTER(x,y,image)
#define DRAW_SKULL(x,y,image) DRAW_CHARACTER(x,y,image)
#define DRAW_BOMB(x,y,image) DRAW_CHARACTER(x,y,image)
#define DRAW_MISSILE(x,y,image) DRAW_CHARACTER(x,y,image)



#if defined(FULL_GAME) 
	#if defined(NO_BLINKING)
		#define _DRAW_PLAYER() \
			if(invincibilityActive) \
			{ \
				DRAW_PLAYER(player._x, player._y, skull._imagePtr); \
			} \
			else \
			{ \
				DRAW_PLAYER(player._x, player._y, player._imagePtr); \
			}	
	#else
		#define _DRAW_PLAYER() \
			if(invincibilityActive) \
			{ \
				DRAW_BLINKING_PLAYER(player._x, player._y, player._imagePtr); \
			} \
			else \
			{ \
				DRAW_PLAYER(player._x, player._y, player._imagePtr); \
			}
	#endif
#else
	#define _DRAW_PLAYER() \
		DRAW_PLAYER(player._x, player._y, player._imagePtr); 
#endif

#define DRAW_BLINKING_PLAYER(x, y, image) _blink_draw(x,y,image, &playerBlink)


#define DELETE_CHARACTER(x,y) _delete(x,y)

#if defined(__GAMATE__)
	#define DELETE_PLAYER(x,y,image) do {textcolor(COLOR_CYAN); _delete(x,y);} while(0)
#else
	#define DELETE_PLAYER(x,y,image) _delete(x,y)
#endif
#define DELETE_GHOST(x,y,image) _delete(x,y)
#if defined(__GAMATE__)
	#define DELETE_SKULL(x,y,image) do {textcolor(COLOR_YELLOW); _delete(x,y);} while(0)
#else
	#define DELETE_SKULL(x,y,image) _delete(x,y)	
#endif
#define DELETE_BOMB(x,y,image) _delete(x,y)
#define DELETE_POWERUP(x,y,image) _delete(x,y)
#define DELETE_GUN(x,y,image) _delete(x,y)
#define DELETE_MISSILE(x,y,image) _delete(x,y)
#define DELETE_EXTRA_POINTS(x,y,image) _delete(x,y)
#define DELETE_EXTRA_LIFE(x,y,image) _delete(x,y)
#define DELETE_INVINCIBILITY(x,y,image) _delete(x,y)	


//

#define displayPlayer(characterPtr) displayCharacter(characterPtr);
#define displayGhost(characterPtr) displayCharacter(characterPtr);
#define displayBomb(characterPtr) displayCharacter(characterPtr);
#define displayHorizontalMissile(characterPtr) displayCharacter(characterPtr);
#define displayRocket(characterPtr) displayCharacter(characterPtr);
#define displayBullet(characterPtr) displayCharacter(characterPtr);
#define displayChasingBullet(characterPtr) displayCharacter(characterPtr);
#define displaySkull(characterPtr) displayCharacter(characterPtr);

#define deletePlayer(characterPtr) deleteCharacter(characterPtr);
#define deleteGhost(characterPtr) deleteCharacter(characterPtr);
#define deleteBomb(characterPtr) deleteCharacter(characterPtr);
#define deleteHorizontalMissile(characterPtr) deleteCharacter(characterPtr);
#define deleteRocket(characterPtr) deleteCharacter(characterPtr);
#define deleteBullet(characterPtr) deleteCharacter(characterPtr);
#define deleteChasingBullet(characterPtr) deleteCharacter(characterPtr);
#define deleteSkull(characterPtr) deleteCharacter(characterPtr);
#define deletePowerUp(characterPtr) deleteCharacter(characterPtr);

void initializeCharacter(register Character* characterPtr, unsigned char x, unsigned char y, unsigned char status, Image * imagePtr);

void setCharacterPosition(Character* characterPtr, unsigned char x, unsigned char y);

unsigned char isCharacterAtLocation(unsigned char x, unsigned char y, Character * characterPtr);

unsigned char wallReached(register Character *characterPtr);

void die(Character * playerPtr);

void playerDies(void);

void checkBombsVsGhost(register Character * ghostPtr);
						
void checkBombsVsGhosts(void);
						
unsigned char safeLocation(unsigned char x, unsigned char y, Character *danger, unsigned char dangerSize);

void relocateCharacter(register Character * characterPtr, Character *danger, unsigned char dangerSize);

void ghostDies(Character * ghostPtr);

unsigned char sameLocationAsAnyLocation(unsigned char x, unsigned char y, Character *characterList, unsigned char length);

#if defined(FULL_GAME)
	unsigned char innerWallReached(register Character *characterPtr);
	unsigned char horizontalWallsReached(register Character *characterPtr);
	unsigned char nearInnerWall(register Character *characterPtr);
#endif

#endif // _CHARACTER
