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

#include "display_macros.h"

struct CharacterStruct
{
	// character coordinates
	uint8_t _x;
	uint8_t _y;
	
	// _status decides whether the character is active
	uint8_t _status;
	
	Image* _imagePtr;
};

typedef struct CharacterStruct Character;


#include "settings.h"

#if defined(ANIMATE_PLAYER)
	#define PLAYER_IMAGE PLAYER_DOWN_IMAGE
#endif

#define playerReachedGhosts() sameLocationAsAnyLocation(player._x, player._y, ghosts, GHOSTS_NUMBER)

#define characterReachedBombs(preyPtr) sameLocationAsAnyLocation((preyPtr)->_x, (preyPtr)->_y, bombs, BOMBS_NUMBER)

#define areCharctersAtSamePosition(lhs, rhs)isCharacterAtLocation((lhs)->_x, (lhs)->_y,rhs)

extern uint16_t points;

extern uint8_t ghostCount;

extern Image INVINCIBILITY_IMAGE;

#if defined(FULL_GAME)
	uint8_t playerKilledBy(Character *enemyPtr);
#endif

void displayCharacter(Character * characterPtr);

void deleteCharacter(Character * characterPtr);

//

#if defined(ANIMATE_PLAYER)
	extern Image PLAYER_LEFT_IMAGE;
	extern Image PLAYER_RIGHT_IMAGE;
	extern Image PLAYER_UP_IMAGE;
	extern Image PLAYER_DOWN_IMAGE;

	#define SHOW_LEFT() player._imagePtr = (Image *)&PLAYER_LEFT_IMAGE
	#define SHOW_RIGHT() player._imagePtr = (Image *)&PLAYER_RIGHT_IMAGE 
	#define SHOW_UP() player._imagePtr = (Image *)&PLAYER_UP_IMAGE
	#define SHOW_DOWN() player._imagePtr = (Image *)&PLAYER_DOWN_IMAGE
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
				DRAW_PLAYER(player._x, player._y, &INVINCIBILITY_IMAGE); \
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

#define DELETE_PLAYER(x,y,image) _delete(x,y)

#define DELETE_GHOST(x,y,image) _delete(x,y)
#if defined(__GAMATE__)
	#define DELETE_SKULL(x,y,image) do {SET_TEXT_COLOR(COLOR_YELLOW); _delete(x,y);} while(0)
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

void initializeCharacter(register Character* characterPtr, uint8_t x, uint8_t y, uint8_t status, Image * imagePtr);

uint8_t isCharacterAtLocation(uint8_t x, uint8_t y, Character * characterPtr);

uint8_t wallReached(register Character *characterPtr);

void playerDies(void);

void checkBombsVsGhost(register Character * ghostPtr);
						
uint8_t safeLocation(uint8_t x, uint8_t y);

void relocateCharacter(register Character * characterPtr);

void ghostDies(Character * ghostPtr);

uint8_t sameLocationAsAnyLocation(uint8_t x, uint8_t y, Character *characterList, uint8_t length);

#if defined(FULL_GAME)
	uint8_t innerWallReached(register Character *characterPtr);
	uint8_t horizontalWallsReached(void);
#endif

#endif // _CHARACTER
