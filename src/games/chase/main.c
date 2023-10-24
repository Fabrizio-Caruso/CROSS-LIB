/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
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

/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
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


#include "tiles.h"
#include "images.h"
#include "init_images.h"

#include "settings.h"

#include "cross_lib.h"


#if !defined(ANIMATE_PLAYER)
	extern Image PLAYER_IMAGE;
#else
	extern Image PLAYER_DOWN_IMAGE;
	extern Image PLAYER_UP_IMAGE;
	extern Image PLAYER_RIGHT_IMAGE;
	extern Image PLAYER_LEFT_IMAGE;	
#endif
	
extern Image GHOST_IMAGE;
extern Image BOMB_IMAGE;

#if !defined(NO_DEAD_GHOSTS)
	extern Image DEAD_GHOST_IMAGE;
#endif

#if !defined(TINY_GAME)
	extern Image SKULL_IMAGE;
	extern Image POWERUP_IMAGE;
	extern Image GUN_IMAGE;
	extern Image BULLET_IMAGE;
	extern Image EXTRA_POINTS_IMAGE;
	
	extern Image HORIZONTAL_BRICK_IMAGE;
	extern Image VERTICAL_BRICK_IMAGE;
#endif

#if defined(FULL_GAME)
	extern Image LEFT_HORIZONTAL_MISSILE_IMAGE;
	extern Image RIGHT_HORIZONTAL_MISSILE_IMAGE;

	extern Image ROCKET_IMAGE;

	extern Image FREEZE_IMAGE;
	extern Image EXTRA_LIFE_IMAGE;
	extern Image INVINCIBILITY_IMAGE;
	extern Image SUPER_IMAGE;	
	extern Image CONFUSE_IMAGE;
	extern Image ZOMBIE_IMAGE;
	
	extern Image BROKEN_BRICK_IMAGE;
#endif

void INIT_IMAGES(void)
{
# if !defined(__NO_GRAPHICS)
	// Set color data 
	#if !defined(_XL_NO_COLOR)
		#if defined(ANIMATE_PLAYER)
			PLAYER_DOWN_IMAGE._color = _PLAYER_COLOR;
			PLAYER_UP_IMAGE._color = _PLAYER_COLOR;
			PLAYER_RIGHT_IMAGE._color = _PLAYER_COLOR;
			PLAYER_LEFT_IMAGE._color = _PLAYER_COLOR;
		#else
			PLAYER_IMAGE._color = _PLAYER_COLOR;
		#endif	
	
		BOMB_IMAGE._color = _BOMB_COLOR;
		GHOST_IMAGE._color = _GHOST_COLOR;		
		
		#if !defined(NO_DEAD_GHOSTS)
			DEAD_GHOST_IMAGE._color = _DEAD_GHOST_COLOR;
		#endif
		
		#if !defined(TINY_GAME)
			SKULL_IMAGE._color = _SKULL_COLOR;
			POWERUP_IMAGE._color = _POWERUP_COLOR;
			GUN_IMAGE._color = _GUN_COLOR;
			EXTRA_POINTS_IMAGE._color = _EXTRA_POINTS_COLOR;
			BULLET_IMAGE._color = _BULLET_COLOR;
			
			VERTICAL_BRICK_IMAGE._color = _BRICK_COLOR;
			HORIZONTAL_BRICK_IMAGE._color = _BRICK_COLOR;
		#endif
		
		#if defined(FULL_GAME)
			RIGHT_HORIZONTAL_MISSILE_IMAGE._color = _MISSILE_COLOR;
			LEFT_HORIZONTAL_MISSILE_IMAGE._color = _MISSILE_COLOR;
			
			ROCKET_IMAGE._color = _ROCKET_COLOR;

			FREEZE_IMAGE._color = _FREEZE_COLOR;
			EXTRA_LIFE_IMAGE._color = _EXTRA_LIFE_COLOR;
			
			INVINCIBILITY_IMAGE._color = _INVINCIBILITY_COLOR;
			SUPER_IMAGE._color = _SUPER_COLOR;
			CONFUSE_IMAGE._color = _CONFUSE_COLOR;
			ZOMBIE_IMAGE._color = _ZOMBIE_COLOR;
			
			BROKEN_BRICK_IMAGE._color = _BRICK_COLOR;
		#endif	
	#endif	

	// Set Image Data
	#if defined(ANIMATE_PLAYER)
		PLAYER_DOWN_IMAGE._imageData = _PLAYER_DOWN_TILE; //_TILE_0;
		PLAYER_UP_IMAGE._imageData = _PLAYER_UP_TILE; // _TILE_1;
		PLAYER_RIGHT_IMAGE._imageData = _PLAYER_RIGHT_TILE; //_TILE_2;
		PLAYER_LEFT_IMAGE._imageData = _PLAYER_LEFT_TILE; // _TILE_3;	
	#else
		PLAYER_IMAGE._imageData = _PLAYER_DOWN_TILE;
	#endif
	
	GHOST_IMAGE._imageData = _GHOST_TILE;
	BOMB_IMAGE._imageData = _BOMB_TILE;
	
	#if !defined(NO_DEAD_GHOSTS)
		DEAD_GHOST_IMAGE._imageData = _DEAD_GHOST_TILE;
	#endif
	
	#if !defined(TINY_GAME)
		SKULL_IMAGE._imageData = _SKULL_TILE;

		POWERUP_IMAGE._imageData = _POWERUP_TILE;
		GUN_IMAGE._imageData = _GUN_TILE;
		EXTRA_POINTS_IMAGE._imageData = _EXTRA_POINTS_TILE;
		
		BULLET_IMAGE._imageData = _BULLET_TILE;
		
		VERTICAL_BRICK_IMAGE._imageData = _VERTICAL_BRICK_TILE;
		HORIZONTAL_BRICK_IMAGE._imageData = _HORIZONTAL_BRICK_TILE;
	#endif
	
	#if defined(FULL_GAME)
		LEFT_HORIZONTAL_MISSILE_IMAGE._imageData = _LEFT_MISSILE_TILE;
		
		RIGHT_HORIZONTAL_MISSILE_IMAGE._imageData = _RIGHT_MISSILE_TILE;
		
		ROCKET_IMAGE._imageData = _ROCKET_TILE;
		
        FREEZE_IMAGE._imageData = _FREEZE_TILE;
        SUPER_IMAGE._imageData = _SUPER_TILE;

		EXTRA_LIFE_IMAGE._imageData = _PLAYER_DOWN_TILE;
		INVINCIBILITY_IMAGE._imageData = _INVINCIBILITY_TILE;
        
        CONFUSE_IMAGE._imageData = _SKULL_TILE;
        ZOMBIE_IMAGE._imageData = _GHOST_TILE;
		
		BROKEN_BRICK_IMAGE._imageData = _BOMB_TILE;
	#endif
#endif 
}






#if !defined(TINY_GAME) 

#include "settings.h"
 
#include "bullet.h"
#include "level.h"
#include "game_text.h"
#include "character.h"
#include "item.h"
#include "ghost.h"
#include "strategy.h"

#include "init_images.h"

extern uint16_t points;
extern uint8_t ghostCount;
extern Character ghosts[GHOSTS_NUMBER];

extern uint8_t level;

extern Item extraPoints;

extern Character skull;
extern uint8_t skullActive;
extern uint8_t playerFire;
extern uint8_t guns;
extern uint8_t playerDirection;
extern uint8_t bulletDirection;

extern Character bullet;
extern Character player;


	
#if defined(FULL_GAME) 
	extern Item freeze;
	extern Item extraLife;
	extern Item invincibility;
	
	extern Character chasingBullet;
	
	extern Character leftHorizontalMissile;
	extern Character rightHorizontalMissile;
	extern uint8_t rockets_x[ROCKETS_NUMBER];
	extern Character rockets[ROCKETS_NUMBER];
	extern uint8_t bases;
	extern uint8_t bases_in_completed_levels;

	extern Character *chasedEnemyPtr;	
    
    extern uint8_t isBossLevel;
    extern uint8_t isMissileLevel;
    extern uint8_t isOneMissileLevel;
    extern uint8_t isRocketLevel;
#endif

#if defined(FULL_GAME)
	void handle_chasing_bullet(void)
	{
		if(chasingBullet._status)
		{
			deleteChasingBullet(&chasingBullet);
			#if !defined(SIMPLE_STRATEGY)
				moveTowardCharacter(chasedEnemyPtr, &chasingBullet, 4);
			#else
				moveTowardCharacter(chasedEnemyPtr, &chasingBullet);				
			#endif
			displayChasingBullet(&chasingBullet);
			checkBullet(&chasingBullet);
		}	
	}
#endif
	
void handle_bullet(void)
{
	// Check if player has fired the gun
	if(playerFire && bullet._status==0 && guns>0)
	{
		_XL_SHOOT_SOUND();
		--guns;
		#if !defined(NO_STATS)
		printGunsStats();
		#endif
		bulletDirection = playerDirection;
		bullet._status = 1;
		bullet._x = player._x; 
		bullet._y = player._y;		
		playerFire = 0;	
	}
	
	// Move bullet if fired
	if(bullet._status==1)
	{
		moveBullet(&bullet);
		checkBullet(&bullet);
	}
}


void checkBullet(Character *bulletPtr)
{
	bulletVsSkull(bulletPtr);
	bulletVsGhosts(bulletPtr);
}

void bulletVsGhost(Character * bulletPtr,
						Character * ghostPtr)
{
	if(ghostPtr->_status && areCharctersAtSamePosition(bulletPtr, ghostPtr))
	{
		points+=GHOST_VS_MISSILE;
		ghostDies(ghostPtr);
		
		bulletPtr->_status=0;
	}
}
	
void bulletVsGhosts(Character * bulletPtr)
{
	uint8_t i = 0;
	for(;i<GHOSTS_NUMBER;++i)
	{
		bulletVsGhost(bulletPtr, &ghosts[i]);
	};
}

void bulletVsSkull(register Character *bulletPtr)
{
	if(skullActive && skull._status &&
	   areCharctersAtSamePosition(bulletPtr, &skull))
	{
        _XL_DRAW(skull._x,skull._y,_SKULL_TILE, _XL_RED);
		_XL_PING_SOUND();
		bulletPtr->_status=0;
		#if defined(FULL_GAME)
		decreaseGhostLevel();
		#endif
		reducePowerUpsCoolDowns();
		
		if(!(--skull._status))
		{
			_XL_EXPLOSION_SOUND();
            deleteSkull(&skull);
			points+=SKULL_POINTS;
			displayScore();
		}
	}	
}


void _moveBullet(register Character *bulletPtr)
{
	deleteBullet(bulletPtr);
	switch(bulletDirection)
	{
		case RIGHT:
			++bulletPtr->_x;
		break;
		case DOWN:
			++bulletPtr->_y;
		break;
		case UP:
			--bulletPtr->_y;
		break;
        default:
		// case LEFT:
			--bulletPtr->_x;
		// break;
	}	
}


#if defined(FULL_GAME)
	void destroyHorizontalMissile(Character * horizontalMissilePtr)
	{
		horizontalMissilePtr->_status = 0;
		_XL_EXPLOSION_SOUND();
		deleteHorizontalMissile(horizontalMissilePtr);
		points+=HORIZONTAL_MISSILE_BONUS;
		displayScore();				
		++bases;
		reducePowerUpsCoolDowns();		
	}
#endif


void moveBullet(register Character * bulletPtr)
{
	_moveBullet(bulletPtr);
	if(wallReached(bulletPtr) && bulletPtr->_status)
	{
		bulletPtr->_status=0;
		
		deleteBullet(bulletPtr);
		#if defined(FULL_GAME)
			
			if(isOneMissileLevel)
			{
				if(bulletPtr->_x==XSize-1 && bulletPtr->_y==YSize/2 && rightHorizontalMissile._status)
				{
					goto _destroy;
				}
			}				
			else if(isMissileLevel || isBossLevel)
			{
				if(bulletPtr->_x==XSize-1 && bulletPtr->_y==HORIZONTAL_MISSILE_OFFSET && rightHorizontalMissile._status)
				{
					_destroy: destroyHorizontalMissile(&rightHorizontalMissile);	
				}
				else if(bulletPtr->_x==0 && bulletPtr->_y==YSize-1-HORIZONTAL_MISSILE_OFFSET && leftHorizontalMissile._status)
				{
					destroyHorizontalMissile(&leftHorizontalMissile);	
				}
			}
			if((isRocketLevel || isBossLevel) && bulletPtr->_y==YSize-1)
			{
				uint8_t i;
				for(i=0;i<ROCKETS_NUMBER;++i)
				{
					if(bulletPtr->_x==rockets_x[i] && rockets[i]._status)
					{
						rockets[i]._status = 0;
						++bases;
						_XL_EXPLOSION_SOUND();
						deleteRocket(&rockets[i]);
						points+=VERTICAL_MISSILE_BONUS;
						displayScore();		
					}
				}
			}			
		DRAW_BROKEN_BRICK(bulletPtr->_x, bulletPtr->_y);
		#endif
	}
	else
	{
		displayBullet(bulletPtr);
	}
}

#endif


/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
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

#include "character.h"
#include "settings.h"
#include "game_text.h"

#include "cross_lib.h"

extern uint16_t points;

extern uint8_t ghostCount;
#if !defined(TINY_GAME) || defined(_XL_TURN_BASED)
extern uint16_t loop;
#endif
extern uint8_t level;

extern Image DEAD_GHOST_IMAGE;
extern Image GHOST_IMAGE;
extern Image BOMB_IMAGE;
extern Image SKULL_IMAGE;
extern Image BROKEN_BRICK_IMAGE;

extern Image HORIZONTAL_BRICK_IMAGE;
extern Image VERTICAL_BRICK_IMAGE;

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];
extern Character player;

extern uint8_t invincibilityActive;

#if !defined(NO_BLINKING)
void _blink_draw(uint8_t x, uint8_t y, Image * image, uint8_t *blinkCounter) 
{
    if(*blinkCounter) 
    {
        _draw(x,y,image);
        *blinkCounter=0;
    } 
    else 
    {
        _delete(x,y);
        *blinkCounter=1;
    }    
}
#endif

#if defined(FULL_GAME) && !defined(_XL_NO_COLOR)
void _DRAW_PLAYER(void)
{
    if(invincibilityActive)
    {
        player._imagePtr->_color = _XL_YELLOW;
    }
    DRAW_PLAYER(player._x, player._y, player._imagePtr);
    player._imagePtr->_color = _XL_CYAN;
}
#endif

void displayCharacter(register Character * characterPtr)
{
	DRAW_CHARACTER(characterPtr->_x, characterPtr->_y, characterPtr->_imagePtr);
}

void deleteCharacter(Character * characterPtr)
{
	DELETE_CHARACTER(characterPtr->_x, characterPtr->_y);
}

#if defined(FULL_GAME)

    void DRAW_BROKEN_BRICK(uint8_t x, uint8_t y)
    {
        _draw(x,y,&BROKEN_BRICK_IMAGE);        
    }
#endif

#if defined(FULL_GAME)
	extern uint8_t invincibilityActive;
	extern uint8_t verticalWallY;
	extern uint8_t verticalWallLength;
	
	extern uint8_t horizWallsLength;
	extern uint8_t zombieActive;	
#endif 

void playerDies(void)
{
	_XL_EXPLOSION_SOUND();
	player._status=0;
	#if !defined(LESS_TEXT)
		printDefeatMessage();
	#endif
	_XL_SLEEP(1);
}

#if defined(FULL_GAME)
	uint8_t playerKilledBy(Character *enemyPtr)
	{
		return !invincibilityActive && areCharctersAtSamePosition(enemyPtr,&player);
	}
#endif

void initializeCharacter(register Character* characterPtr, uint8_t x, uint8_t y, uint8_t status, Image * imagePtr)
{
	characterPtr->_x = x;
	characterPtr->_y = y;
	characterPtr->_status = status;
	characterPtr->_imagePtr = imagePtr;
}


uint8_t isCharacterAtLocation(uint8_t x, uint8_t y, Character * characterPtr)
{
	return(characterPtr->_y==y) && (characterPtr->_x==x);
}


uint8_t wallReached(register Character *characterPtr)
{
	return (characterPtr->_x==0)||(characterPtr->_x==XSize-1) || 
		   (characterPtr->_y==0)||(characterPtr->_y==YSize-1);
}


void ghostDies(Character * ghostPtr)
{
	_XL_EXPLOSION_SOUND();
	
	ghostPtr->_status=0;
	displayScore();
	
	#if defined(FULL_GAME)
		if(level>=FIRST_MOVING_BOMBS_LEVEL)
			{
				uint8_t i;

				for(i=0;i<BOMBS_NUMBER;++i)
				{
					deleteCharacter(&bombs[i]);
					if(ghostCount&1)
					{
						--bombs[i]._x;
						--bombs[i]._y;
					}
					else
					{
						++bombs[i]._x;
						++bombs[i]._y;
					}
				}
			}		
	#endif
		
	#if !defined(NO_DEAD_GHOSTS)
		ghostPtr->_imagePtr = (Image *)&DEAD_GHOST_IMAGE;
        #if defined(FULL_GAME)
        displayCharacter(ghostPtr);
        #endif
	#elif !defined(TINY_GAME)
		ghostPtr->_imagePtr = (Image *)&SKULL_IMAGE;			
	#endif
	
	--ghostCount;
	printGhostCountStats();
}

void checkBombsVsGhost(register Character * ghostPtr)
{
	
	if(ghostPtr->_status && characterReachedBombs(ghostPtr))
	{		
		points+=GHOST_VS_BOMBS_BONUS;
		

		#	if defined(TINY_GAME) && defined(MOVE_DEAD_GHOST)
			ghostPtr->_y=1;
		#elif defined(TINY_GAME) && defined(BOMB_DEAD_GHOST)
			ghostPtr->_imagePtr = &BOMB_IMAGE;			
		#elif defined(TINY_GAME) && defined(HIDE_DEAD_GHOST)
			//
		#elif defined(TINY_GAME)
			ghostPtr->_x=ghostCount;
			ghostPtr->_y=1;				
		#else
			ghostPtr->_x=1+GHOSTS_NUMBER-ghostCount;
			ghostPtr->_y=1;	
		#endif
		
		ghostDies(ghostPtr);
	}
	
}
						

uint8_t sameLocationAsAnyLocation(uint8_t x, uint8_t y, Character *characterList, uint8_t length)
{
	uint8_t i;

	for(i=0;i<length;++i)
	{
		if(isCharacterAtLocation(x,y,&characterList[i]))
			return 1;
	}	
	return 0;
}


#if YSize<XSize
	#define SAFETY YSize/4
#else
	#define SAFETY XSize/4
#endif

// TODO: To be replaced with something cleaner
// also used with things different from global bombs
uint8_t safeLocation(uint8_t x, uint8_t y)
{
	return !(sameLocationAsAnyLocation(x,y,ghosts,GHOSTS_NUMBER) 
	      || sameLocationAsAnyLocation(x,y,bombs, BOMBS_NUMBER)
		  || (x<SAFETY) || (x>XSize-SAFETY) || (y<=SAFETY) || (y>YSize-SAFETY));
}

#if !defined(TINY_GAME)
void relocateCharacter(register Character * characterPtr)
{
	uint8_t x; 
	uint8_t y;  
	
	do
	{
		x = characterPtr->_x  + (uint8_t)(_XL_RAND() % RELOCATE_RANGE) - (uint8_t)(RELOCATE_RANGE/2);
		y = characterPtr->_y  + (uint8_t)(_XL_RAND() % RELOCATE_RANGE) - (uint8_t)(RELOCATE_RANGE/2);

	} while(!safeLocation(x,y));

	characterPtr->_x = x;
	characterPtr->_y = y;
}
#endif

#if defined(FULL_GAME)
	uint8_t innerWallReached(register Character *characterPtr)
	{
		return (characterPtr->_x==XSize/2) && (characterPtr->_y >= verticalWallY) && (characterPtr->_y<= (verticalWallY + verticalWallLength-1));
	}

	uint8_t horizWallsReached(void)
	{
		return (player._y==YSize/2) && 
		       ((player._x<=horizWallsLength) ||
			    (player._x>=-1+XSize-horizWallsLength));
	}	
	
#endif


#if !defined(TINY_GAME)

#include "settings.h"

#include "end_screen.h"
#include "game_text.h"
#include "text_strings.h"
#include "move_player.h"

#if defined(END_SCREEN)
	#include "strategy.h"
	#include "level.h"
	#include "character.h"
	#include "ghost.h"
	
	extern Character player;
	extern Character skull;
	extern Character ghosts[GHOSTS_NUMBER];
	extern Character bullet;
	extern uint8_t playerFire;
	extern uint8_t level;
	extern uint8_t guns;
	extern uint8_t ind;
#endif



#if defined(END_SCREEN) || defined(DANCE)
	extern uint8_t bulletDirection;

	void dance(Character * characterPtr)
	{
		deleteCharacter(characterPtr);
		if(!(bulletDirection&3))
		{
			++(characterPtr->_x);
		}
		else if((bulletDirection&3)==1)
		{
			++(characterPtr->_y);			
		}
		else if ((bulletDirection&3)==2)
		{
			--(characterPtr->_x);			
		}
		else
		{
			--(characterPtr->_y);			
		}
		displayCharacter(characterPtr);	
	}

#endif

#if YSize < 20
	#define MESSAGE_START 5
#else
	#define MESSAGE_START 6
#endif

#if defined(END_SCREEN)
	void gameCompleted(void)
	{
		level = 0;
		
		_XL_CLEAR_SCREEN();
        ghostCount = GHOSTS_NUMBER;
		fillLevelWithCharacters();
        displayScore();

		playerFire = 0;
		skull._x = player._x-4;
		skull._y = player._y;	
		bullet._status = 0;
		guns = 1;
		while(!playerFire && !wallReached(&player))
		{
			++bulletDirection;
			displayBombs();
			for(ind=0;ind<GHOSTS_NUMBER;++ind)
			{
				dance(&ghosts[ind]);
			}
			
			displayPlayer(&player);
			
			dance(&skull);
		
			PRINT_CENTERED_ON_ROW(MESSAGE_START,  YOU_MADE_IT_STRING);		
			#if _XL_SLOW_DOWN_FACTOR>0
				for(ind=0;ind<253U;++ind){};
			#endif
					
			MOVE_PLAYER();
		}
	}
#endif

#endif


/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
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

#include "settings.h"
#include "game_text.h"
#include "character.h"
#include "settings.h"
#include "text_strings.h"

#include "cross_lib.h"
#include "init_images.h"

#define TEXT_COLOR _XL_WHITE

extern uint8_t guns;
extern Image GUN_IMAGE;
extern Image SKULL_IMAGE;

#if !defined(_XL_NO_TEXT_COLOR) || defined(__NO_GRAPHICS)
	#define SET_COLOR(c) _XL_SET_TEXT_COLOR(c)
#else
	#define SET_COLOR(c)
#endif

#if XSize<20
	#define EXTRA_TINY 1
#else
	#define EXTRA_TINY 0
#endif


#if Y_OFFSET==3
	#define SKIP_ROW 1
#else
	#define SKIP_ROW 0
#endif

#if defined(WIDE) && !defined(TINY_GAME)
	#define GUN_IMAGE_X 17
	#define GUN_IMAGE_Y 0
	#define GHOST_IMAGE_X 13
	#define GHOST_IMAGE_Y 0
	#define PLAYER_IMAGE_X 16
	#define PLAYER_IMAGE_Y (1+SKIP_ROW)
	#define LEVEL_X (XSize-2)
	#define LEVEL_Y (1+SKIP_ROW)
#else
	#define GUN_IMAGE_X (11-EXTRA_TINY)
	#define GUN_IMAGE_Y 0
	#define GHOST_IMAGE_X (8-EXTRA_TINY)
	#define GHOST_IMAGE_Y 0
	#define PLAYER_IMAGE_X (14-EXTRA_TINY)
	#define PLAYER_IMAGE_Y 0
	#define LEVEL_X (XSize-2)
	#define LEVEL_Y 0
#endif



extern uint8_t level;
extern uint8_t lives;

extern uint16_t points;
extern uint8_t ghostCount;
extern uint16_t ghostLevel;
extern uint16_t highScore;

extern Image GHOST_IMAGE;
extern Image PLAYER_IMAGE;


void PRINT_CENTERED_ON_ROW(uint8_t row, const char *Text)
{
	_XL_PRINT(((uint8_t) (XSize - strlen((char *)Text))>>1), row, Text);	
}


#if defined(FULL_GAME)
	void printKillTheSkull(void)
	{
		PRINT_CENTERED(KILL_THE_SKULL_STRING);	
		PRINT_CENTERED_ON_ROW(((uint8_t)YSize)/2+2,DESTROY_MISSILES_STRING);
	}
	
#endif



// TODO: This is SLOW
#if !defined(TINY_GAME) && !defined(NO_STATS)
	void displayStatsTitles(void)
	{				
		SET_COLOR(TEXT_COLOR);

		_draw_stat(GUN_IMAGE_X, GUN_IMAGE_Y, &GUN_IMAGE);
		_draw_stat(GHOST_IMAGE_X, GHOST_IMAGE_Y, &GHOST_IMAGE);
		_draw_stat(PLAYER_IMAGE_X, PLAYER_IMAGE_Y, &PLAYER_IMAGE);
	}

	
	void printGunsStats(void)
	{
		SET_COLOR(TEXT_COLOR);
		
		#if defined(WIDE)
			_XL_PRINTD(GUN_IMAGE_X+2,GUN_IMAGE_Y,1,guns);
		#else
			_XL_PRINTD(GUN_IMAGE_X+1,GUN_IMAGE_Y,1,guns);
		#endif
	}
#endif

#if !defined(NO_STATS)
	void printLevelStats(void)
	{	
		SET_COLOR(TEXT_COLOR);
	
		#if defined(WIDE) && !defined(TINY_GAME)
			_XL_PRINTD(LEVEL_X,LEVEL_Y,2,level);
		#elif XSize>16
			_XL_PRINTD(LEVEL_X,LEVEL_Y,2,level);
		#else
			// No space for level
		#endif	
	}


	void printGhostCountStats(void)
	{
		SET_COLOR(TEXT_COLOR);		
		
		#if defined(WIDE) && !defined(TINY_GAME)
			_XL_PRINTD(GHOST_IMAGE_X+2,GHOST_IMAGE_Y,1,ghostCount);
		#else
			_XL_PRINTD(GHOST_IMAGE_X+1,GHOST_IMAGE_Y,1,ghostCount);	
		#endif	
	}


	void printLivesStats(void)
	{
		SET_COLOR(TEXT_COLOR);
		
		#if defined(WIDE) && !defined(TINY_GAME)
			_XL_PRINTD(PLAYER_IMAGE_X+2,PLAYER_IMAGE_Y,2,lives);
		#else
			_XL_PRINTD(PLAYER_IMAGE_X+1,PLAYER_IMAGE_Y,2,lives);	
		#endif
	}	
	
#endif

#if !defined(NO_PRINT)
	void printPressKeyToStart(void)
	{
        _XL_SET_TEXT_COLOR(_XL_WHITE);
		PRINT_CENTERED(PRESS_STRING);
	}	
#endif


void displayScore(void)
{	
	SET_COLOR(TEXT_COLOR);
	
    _XL_PRINTD(!EXTRA_TINY,0,5,points);	
}

#if !defined(LESS_TEXT)	
	void printLevel(void)
	{
        _XL_SET_TEXT_COLOR(_XL_WHITE);
		_XL_PRINT(((XSize -7)>>1), (YSize>>1)-2, START_LEVEL_STRING);
        _XL_PRINTD(((XSize -7)>>1)+6, (YSize>>1)-2, 2, level);
	}
#endif


#if !defined(TINY_GAME)
	void _printScoreOnRow(uint8_t row, uint16_t score)
	{
		_XL_PRINTD((uint8_t) ((XSize)>>1)-3, row, 5, score);
	}	
	
	#if !defined(LESS_TEXT)
	void _printScore(uint16_t score)
	{
		_printScoreOnRow((YSize>>1), score);
	}
	#endif
#endif

#if !defined(END_SCREEN)
	void gameCompleted(void)	
	{
		_XL_CLEAR_SCREEN();
		PRINT_CENTERED(YOU_MADE_IT_STRING); 
	}
#endif


#if !defined(LESS_TEXT)
	void printVictoryMessage(void)
	{
        SET_COLOR(_XL_RED);
		PRINT_CENTERED(VICTORY_STRING);
	}	
#endif


#if !defined(LESS_TEXT)	
	void printDefeatMessage(void)
	{		
        SET_COLOR(_XL_RED);
		PRINT_CENTERED(DEFEAT_STRING);
	}	
#endif
	
	

void printGameOver(void)
{
	SET_COLOR(_XL_RED);
	PRINT_CENTERED(GAME_OVER_STRING);
}	



#if YSize>=21
    #define AUTHOR_Y_SPACE 1
#else
    #define AUTHOR_Y_SPACE 0
#endif

#if YSize>=20
	#define EXTRA_Y 1
	#define AUTHOR_Y 4
	#define CROSS_CHASE_Y 2
	#define INTERLINE 2
	#define INSTR_Y_OFFSET 2
#else
	#define EXTRA_Y 0
	#define AUTHOR_Y 3
	#define CROSS_CHASE_Y 1
	#if defined(__Z88DK_SPRITES_GRAPHICS)
		#define INTERLINE 2
	#else
		#define INTERLINE 1
	#endif
	#define INSTR_Y_OFFSET 1
#endif

 


#if (defined(FULL_GAME) && !defined(NO_HINTS)) || !defined(NO_INITIAL_SCREEN)
	#if defined(FULL_GAME) && !defined(NO_HINTS)
	void _printCrossChase(void)
	{
        SET_COLOR(_XL_RED);
		PRINT_CENTERED_ON_ROW(CROSS_CHASE_Y, CROSS_CHASE_STRING);		
		SET_COLOR(TEXT_COLOR);
		
	}
	#else
		#define _printCrossChase() \
            SET_COLOR(_XL_RED); \
			PRINT_CENTERED_ON_ROW(CROSS_CHASE_Y, CROSS_CHASE_STRING);	\
			SET_COLOR(TEXT_COLOR);	
	#endif
#endif


#if defined(FULL_GAME) && !defined(NO_HINTS) && XSize>=14
	void printHints(void)
	{
		_printCrossChase();
        
        PRINT_CENTERED_ON_ROW(AUTHOR_Y+1*INTERLINE+EXTRA_Y,  LURE_THE_ENEMIES_STRING);
        PRINT_CENTERED_ON_ROW(AUTHOR_Y+2*INTERLINE+EXTRA_Y,  INTO_THE_MINES_STRING);
                    
		PRINT_CENTERED_ON_ROW(AUTHOR_Y+3*INTERLINE+EXTRA_Y,  USE_THE_GUN_AGAINST_STRING);

		PRINT_CENTERED_ON_ROW(AUTHOR_Y+4*INTERLINE+EXTRA_Y,  THE_SKULL_AND_STRING);

		PRINT_CENTERED_ON_ROW(AUTHOR_Y+5*INTERLINE+EXTRA_Y, MISSILE_BASES_STRING);	
		
		#if YSize>=14
			PRINT_CENTERED_ON_ROW(AUTHOR_Y+6*INTERLINE+EXTRA_Y, FOR_POINTS_AND___STRING);

			PRINT_CENTERED_ON_ROW(AUTHOR_Y+7*INTERLINE+EXTRA_Y, EXTRA_POWERUPS__STRING);
		#endif
	}
#endif

#if !defined(NO_EXTRA_TITLE)

    #define ITEMS_TO_DISPLAY 6

    static const uint8_t item_tile[ITEMS_TO_DISPLAY][2] = 
    {
        { _GHOST_TILE, _GHOST_COLOR },
        { _SKULL_TILE, _SKULL_COLOR },
        { _BOMB_TILE, _BOMB_COLOR },
        { _GUN_TILE, _GUN_COLOR},
        { _POWERUP_TILE, _POWERUP_COLOR},
        { _FREEZE_TILE, _FREEZE_COLOR},
    };

    static const char item_name[ITEMS_TO_DISPLAY][7] = 
    {
        _XL_E _XL_N _XL_E _XL_M _XL_Y,
        _XL_B _XL_O _XL_S _XL_S,
        _XL_M _XL_I _XL_N _XL_E,
        _XL_G _XL_U _XL_N,
        _XL_S _XL_L _XL_O _XL_W,
        _XL_F _XL_R _XL_E _XL_E _XL_Z _XL_E,
    };


    #define display_items() \
    do \
    { \
        uint8_t i; \
        \
        for(i=0;i<ITEMS_TO_DISPLAY;++i) \
        { \
            _XL_DRAW(XSize/2-5,AUTHOR_Y+AUTHOR_Y_SPACE+2+i*INTERLINE, item_tile[i][0], item_tile[i][1]); \
            _XL_SET_TEXT_COLOR(_XL_GREEN); \
            _XL_PRINT(XSize/2-5+3,AUTHOR_Y+AUTHOR_Y_SPACE+2+i*INTERLINE, (char *)item_name[i]); \
        } \
    } while(0)
    
#endif

#if !defined(NO_INITIAL_SCREEN)
	void printStartMessage(void)
	{
		_printCrossChase();
		
        #if XSize>=16
		PRINT_CENTERED_ON_ROW(AUTHOR_Y, AUTHOR_STRING);	
        #endif

		#if !defined(TINY_GAME)
			_printTopScore();
			
			
            #if XSize>=16
                #if !defined(NO_EXTRA_TITLE)
                    display_items();
                #else
                    SET_COLOR(_XL_CYAN);

                    PRINT_CENTERED_ON_ROW(AUTHOR_Y+1*INTERLINE+EXTRA_Y,  LURE_THE_ENEMIES_STRING);
                    PRINT_CENTERED_ON_ROW(AUTHOR_Y+2*INTERLINE+EXTRA_Y,  INTO_THE_MINES_STRING);			
                #endif
            #endif
			SET_COLOR(TEXT_COLOR);	
			
		#endif

		#if !defined(NO_CONTROL_INSTRUCTIONS) && XSize>=14
			PRINT_CENTERED_ON_ROW(YSize-INSTR_Y_OFFSET, USE_STRING);
		#endif
	}
#endif


/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
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


#include "ghost.h"
#include "settings.h"

extern uint16_t ghostLevel;
extern uint8_t level;
extern uint16_t loop;

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];


#if defined(FULL_GAME)

extern uint16_t levelSlowDown;

uint16_t computeGhostSlowDown(void)
{
	if((loop<MAX_GHOST_LOOP) && (ghostLevel<MAX_GHOST_LEVEL))
	{
		#if defined(_XL_TURN_BASED)
			return levelSlowDown-ghostLevel*8;		
		#else
			return levelSlowDown-ghostLevel*16;
		#endif
	}
	return GHOST_MIN_SLOWDOWN;
}
#endif

#if defined(FULL_GAME)
	void decreaseGhostLevel(void)
	{
		if(ghostLevel>GHOST_LEVEL_DECREASE)
			ghostLevel-=GHOST_LEVEL_DECREASE;
		else
			ghostLevel=0;
	}
#endif

void displayBombs(void)
{
	uint8_t i;

	for(i=0;i<BOMBS_NUMBER;++i)
	{
		displayBomb(&bombs[i]);
	}
}	

#if defined(FULL_GAME)
	uint8_t firstAlive(void)
	{
		uint8_t i;
		
		i=0;
		while(i<GHOSTS_NUMBER)
		{
			if(ghosts[i]._status)
				return i;
			++i;
		}
		return GHOSTS_NUMBER;
	}
#endif



#include "settings.h"

#if defined(FULL_GAME) 

#include "level.h"
#include "strategy.h"

#include "cross_lib.h"

#if !defined(TINY_GAME) || defined(_XL_TURN_BASED)
extern uint16_t loop;
#endif
extern uint8_t level;

extern Character leftHorizontalMissile;
extern Character rightHorizontalMissile;
extern Character player;

extern uint8_t arrowRange;
extern uint8_t arrowYPosition;

extern uint8_t isBossLevel;
extern uint8_t isMissileLevel;
extern uint8_t isOneMissileLevel;

	uint8_t _playerInArrowRange(void)
	{
		return (loop&1 && player._y<=(arrowYPosition+arrowRange) && player._y>=(arrowYPosition-arrowRange));
	}
	
	void handle_from_left(void)
	{
		if(leftHorizontalMissile._status)
		{
			deleteHorizontalMissile(&leftHorizontalMissile);
			if(leftHorizontalMissile._x==XSize-2)
			{
				leftHorizontalMissile._x=0;
				leftHorizontalMissile._y =  arrowYPosition;
			}
			else
			{
				ADVANCED_LEFT_MISSILE();
				if(_playerInArrowRange())
				{
					if(player._x>=leftHorizontalMissile._x)
					{
						(void) moveCharacter((uint8_t *)&leftHorizontalMissile+Y_MOVE, (uint8_t *)&player+Y_MOVE);			
					}
				}
			}
			displayHorizontalMissile(&leftHorizontalMissile);
			
			if(playerKilledBy(&leftHorizontalMissile))
			{
				playerDies();
			}
		}		
	}
	
	void handle_from_right(void)
	{
		if(rightHorizontalMissile._status)
		{
			deleteHorizontalMissile(&rightHorizontalMissile);
			if(rightHorizontalMissile._x==1)
			{
				rightHorizontalMissile._x= XSize-1;
				rightHorizontalMissile._y = arrowYPosition;
			}
			else
			{
				ADVANCED_RIGHT_MISSILE();
				if(_playerInArrowRange())
				{
					if(player._x<= rightHorizontalMissile._x)	
					{
						(void) moveCharacter((uint8_t *)&rightHorizontalMissile+Y_MOVE, (uint8_t *)&player+Y_MOVE);			
					}
				}
			}
			displayHorizontalMissile(&rightHorizontalMissile);	
			
			if(playerKilledBy(&rightHorizontalMissile))
			{
				playerDies();
			}
		}		
	}
	
	void handle_missiles(void)
	{	
		if(isOneMissileLevel)
		{
			arrowYPosition = YSize/2;
			handle_from_right();
		}	
		else if(isMissileLevel || isBossLevel)
		{
			arrowYPosition = HORIZONTAL_MISSILE_OFFSET;
			handle_from_right();
			
			arrowYPosition = YSize-1-HORIZONTAL_MISSILE_OFFSET; 
			handle_from_left();
		}

	}
	
#endif // defined(FULL_GAME)


#include "settings.h"

#include "character.h"
#include "item.h"
#include "ghost.h"
#include "game_text.h"

extern uint16_t points;

extern uint8_t guns;
extern uint8_t lives;

extern uint8_t level;

extern uint8_t freezeActive;
extern uint8_t freeze_count_down;


#if defined(FULL_GAME)
	extern uint8_t invincibilityActive;
	extern uint8_t confuseActive;
	extern uint8_t zombieActive;
	
	extern uint8_t invincibility_count_down;
	extern uint8_t confuse_count_down;
	extern uint8_t zombie_count_down;
	
	extern uint8_t bases_in_completed_levels;
	extern uint8_t all_skulls_killed_in_completed_levels;	
	
	extern uint8_t extraLife_present_on_level;
	extern uint8_t zombie_present_on_level;
#endif

extern Image DEAD_GHOST_IMAGE;

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];
extern Character skull;

extern Character player;

extern Item powerUp;
extern Item gun;
extern Item extraPoints;


#if defined(FULL_GAME)
	extern Character *chasedEnemyPtr;
	extern Character chasingBullet;

	extern Item powerUp2;	
	
	extern Item freeze;

	extern Item invincibility;

	extern Item chase;

	extern Item super;
	extern Item extraLife;

	extern Item confuse;
	extern Item zombie;
#endif
	
#if !defined(TINY_GAME)

	void itemReached(Character * itemPtr)
	{
		_XL_ZAP_SOUND();
		#if defined(_XL_TURN_BASED)
			displayPlayer(&player);
		#endif
		itemPtr->_status = 0;
		displayScore();
	}

	void relocateItem(Character * itemPtr)
	{
			itemPtr->_status = 1;
			
			#if defined(FULL_GAME)
			do
			{
				relocateCharacter(itemPtr);
			} while(innerWallReached(itemPtr));		
			#else
				relocateCharacter(itemPtr);
			#endif	
	}	
	
	#if defined(FULL_GAME)
		void _commonPowerUpEffect(void)
		{
			points+=POWER_UP_BONUS;
			decreaseGhostLevel();
			freezeActive = 1;	
			freeze_count_down += FROZEN_COUNT_DOWN;	
		}

		void powerUpEffect(void)
		{
			_commonPowerUpEffect();
			powerUp._coolDown = POWER_UP_COOL_DOWN;		
		}

		void _gunEffect(void)
		{
			guns = GUNS_NUMBER;
			#if !defined(NO_STATS)
			printGunsStats();	
			#endif
			points+=GUN_BONUS;		
		}

		void gunEffect(void)
		{
			_gunEffect();
			gun._coolDown = GUN_COOL_DOWN;	
		}
	#else

		void powerUpEffect(void)
		{
			points+=POWER_UP_BONUS;
			freezeActive = 1;	
			freeze_count_down += FROZEN_COUNT_DOWN;	
			powerUp._coolDown = POWER_UP_COOL_DOWN;		
		}

		void gunEffect(void)
		{
			guns = GUNS_NUMBER;
			#if !defined(NO_STATS)
			printGunsStats();	
			#endif
			points+=GUN_BONUS;	
			gun._coolDown = GUN_COOL_DOWN;	
		}		
	#endif

	void extraPointsEffect(void)
	{
		points+=EXTRA_POINTS+level*EXTRA_POINTS_LEVEL_INCREASE;
		extraPoints._coolDown = SECOND_EXTRA_POINTS_COOL_DOWN;//(EXTRA_POINTS_COOL_DOWN<<4); // second time is harder		
	}

	void handle_item(register Item *itemPtr)
	{
		// Manage item
		if(itemPtr->_character._status == 1)
		{	
			if(areCharctersAtSamePosition(&player, (Character *) itemPtr))
			{
				itemPtr->_effect();
				itemReached((Character *) itemPtr);			
			}
			else
			{
				_blink_draw(itemPtr->_character._x, itemPtr->_character._y, itemPtr->_character._imagePtr, &(itemPtr->_blink));
			}		
		}
		else if (itemPtr->_coolDown == 0)
		{
			relocateItem((Character *) itemPtr);
		}
		else
		{
			--(itemPtr->_coolDown);
		}
	}

	void handle_count_down(uint8_t * activeItemFlagPtr, uint8_t * countDownPtr)
	{
		if(*activeItemFlagPtr)
		{
			if(*countDownPtr<=0)
			{
				*activeItemFlagPtr=0;
			}
			else
			{
				--(*countDownPtr);
			}
		}
	}	
	
#endif // !defined(TINY_GAME)

	
#if defined(FULL_GAME)
	void reducePowerUpsCoolDowns(void)
	{
		extraPoints._coolDown/=2;
		invincibility._coolDown/=2;
		freeze._coolDown/=2;
		_XL_TICK_SOUND();		
	}
#elif !defined(TINY_GAME)
	void reducePowerUpsCoolDowns(void)
	{
		extraPoints._coolDown/=2;
		_XL_TICK_SOUND();		
	}
#else	
#endif	


	
#if defined(FULL_GAME)
	void powerUp2Effect(void)
	{
		_commonPowerUpEffect();
		powerUp2._coolDown = POWER_UP2_COOL_DOWN;	
	}

	void _freezeEffect(void)
	{
		_commonPowerUpEffect();
		_commonPowerUpEffect();
		_commonPowerUpEffect();
	}

	void freezeEffect(void)
	{
		_freezeEffect();
		freeze._coolDown = ((uint16_t) (FREEZE_COOL_DOWN)*2);	
	}
	
	
	void extraLifeEffect(void)
	{
		++lives;
		all_skulls_killed_in_completed_levels=1;
		extraLife_present_on_level = 0;
		printLivesStats();		
	}

	void _invincibilityEffect(void)
	{
		invincibilityActive = 1;
		invincibility_count_down = INVINCIBILITY_COUNT_DOWN;			
	}
	
	void invincibilityEffect(void)
	{
		_invincibilityEffect();
		invincibility._coolDown = ((uint16_t) (INVINCIBILITY_COOL_DOWN)*4);
	}
	
	void superEffect(void)
	{
		_freezeEffect();
		_gunEffect();
		_invincibilityEffect();
		super._coolDown = ((uint16_t) (SUPER_COOL_DOWN)*8);
	}

	void confuseEffect(void)
	{
		confuseActive = 1;
		confuse._coolDown = SECOND_CONFUSE_COOL_DOWN;
		confuse_count_down = CONFUSE_COUNT_DOWN;
	}
	
	void zombieEffect(void)
	{
		uint8_t i;
		
		zombieActive = 1;
		bases_in_completed_levels = 1;
		zombie._coolDown = SECOND_ZOMBIE_COOL_DOWN; 
		zombie_count_down = ZOMBIE_COUNT_DOWN;
		for(i=0;i<GHOSTS_NUMBER;++i)
		{
			if(!(ghosts[i]._status))
			{
				ghosts[i]._imagePtr = &DEAD_GHOST_IMAGE;
			}
		}
	}
	
	void chaseEffect(void)
	{
		unsigned firstAliveIndex;
		
		chasingBullet._status = 1;
		chasingBullet._x = chase._character._x;
		chasingBullet._y = chase._character._y;
		
		chase._coolDown = ((uint16_t)(CHASE_COOL_DOWN)*2);
		
		firstAliveIndex = firstAlive();
		if(firstAliveIndex == GHOSTS_NUMBER)
		{
			chasedEnemyPtr = &skull;
		}
		else
		{
			chasedEnemyPtr = &ghosts[firstAliveIndex];
		}
	}
#endif


/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
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
 
#include "character.h"
#include "item.h"
#include "item.h"
#include "settings.h"
#include "ghost.h"
#include "level.h"


#include "cross_lib.h"


extern uint8_t level;

#if defined(FULL_GAME)
    extern uint8_t isBossLevel;
    extern uint8_t isOneMissileLevel;
    extern uint8_t isMissileLevel;
    extern uint8_t isRocketLevel;
    extern uint8_t isHorizontalWallsLevel;
#endif

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image BOMB_IMAGE;
extern Image DEAD_GHOST_IMAGE;

#if !defined(TINY_GAME)
    extern Image SKULL_IMAGE;
    extern Image BULLET_IMAGE;
    extern Image POWERUP_IMAGE;
    extern Image GUN_IMAGE;
    extern Image EXTRA_POINTS_IMAGE;
#endif

extern Character player; 

#if !defined(TINY_GAME)
    extern Character skull;

    extern Item powerUp;
    extern Item gun;
    extern Item extraPoints;

    extern Character bullet;

#endif

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];


#if defined(FULL_GAME) 
    extern Item powerUp2;

    extern Item chase;
    extern Character chasingBullet;

    extern uint8_t verticalWallY;
    extern uint8_t verticalWallLength;

    extern Image ROCKET_IMAGE;

    extern Image FREEZE_IMAGE;
    extern Image LEFT_HORIZONTAL_MISSILE_IMAGE;
    extern Image RIGHT_HORIZONTAL_MISSILE_IMAGE;

    extern Image EXTRA_LIFE_IMAGE;
    extern Image INVINCIBILITY_IMAGE;
    extern Image SUPER_IMAGE;
    extern Image CONFUSE_IMAGE;
    extern Image ZOMBIE_IMAGE;

    extern Character leftHorizontalMissile;
    extern Character rightHorizontalMissile;

    extern Item freeze;
    extern Item extraLife;
    extern Item invincibility;
    extern Item super;
    extern Item confuse;
    extern Item zombie;
    
    extern Character rockets[ROCKETS_NUMBER];

    extern uint8_t rockets_x[ROCKETS_NUMBER];
    
    extern uint8_t all_skulls_killed_in_completed_levels;
#endif


#if !defined(NO_WALL)

    extern Image HORIZONTAL_BRICK_IMAGE;
    extern Image VERTICAL_BRICK_IMAGE;
    
    void DRAW_HORIZONTAL_LINE(uint8_t x,uint8_t y, uint8_t length) 
    {
        uint8_t i;
        for(i=0;i<length;++i)
        {
            _draw(x+i,y,&HORIZONTAL_BRICK_IMAGE);
        }
    }

    void DRAW_VERTICAL_LINE(uint8_t x,uint8_t y, uint8_t length) 
    {
        uint8_t i;
        for(i=0;i<length;++i)
        {
            _draw(x,y+i,&VERTICAL_BRICK_IMAGE);
        }        
    }
#endif

#if defined(FULL_GAME)
    void updateInnerWallVerticalData(void)
    {    
        uint8_t lvmod = level%5;

        if((lvmod==1)||(lvmod==0))
        {
            verticalWallLength = 0;
        }
        else
        {
            #if defined(WIDE)
                verticalWallLength = YSize-10+(lvmod-2)*2;
            #elif YSize<=14
                verticalWallLength = 4;
            #else
                verticalWallLength = YSize-14+(lvmod-2)*2;        
            #endif
        }

        verticalWallY = (YSize>>1)-(verticalWallLength>>1);
    }

    uint8_t oneMissileLevel(void)
    {
        #if defined(BUGGY_MOD5)
            return (level==3) || (level==8) || (level==11) || (level==16) || (level==5);
        #else
            return ((level%5)==3) || (level==5);  
        #endif
    }

    uint8_t rocketLevel(void)
    {
        return (level >= FIRST_ROCKETS_LEVEL) && ((level%5)==2 || (level%5)==3);
    }

    uint8_t missileLevel(void)
    {
        #if defined(BUGGY_MOD5)
            return (level==4) || (level==9) || (level==14) || (level==19);
        #else
            return (level%5)==4;
        #endif
    }    

    uint8_t bossLevel(void)
    {
        
        #if defined(BUGGY_MOD5)
            if(level==5 || level==10 || level==15 || level==20)
            {
                return 1;
            }
            return 0;
        #else
            return !(level%5);
        #endif
    }
    
    uint8_t horizWallsLevel(void)
    {
        return ((level >= FIRST_HORIZONTAL_WALLS_LEVEL) && ((level%5==1) || (level%5==4)));
    }    

    void initializeAwayFromWall(Character * characterPtr, uint8_t x, uint8_t y, uint8_t status, Image *imagePtr)
    {
        do{
            initializeCharacter(characterPtr, x, y, status, imagePtr);
            relocateCharacter(characterPtr);
        } while(innerWallReached(characterPtr));
    }
#endif 


#if defined(BETWEEN_LEVEL)

#if XSize<YSize
    #define MIN_SIZE XSize
#else
    #define MIN_SIZE YSize
#endif

#if MIN_SIZE>12
    #define SPIRAL_LOOPS (2*MIN_SIZE-21)
#else
    #define SPIRAL_LOOPS (2*MIN_SIZE-18)
#endif

void spiral(register Character *characterPtr)
{
    uint8_t i;
    uint8_t j;
    
    characterPtr->_x = XSize/2;
    characterPtr->_y = YSize/2;
    for(i=0;i<SPIRAL_LOOPS;++i)
    {
        for(j=0;j<i/2;++j)
            {
                displayCharacter(characterPtr);        
                if(i&2)
                {
                    ++(*((uint8_t *) characterPtr + (i&1)));
                }
                else
                {
                    --(*((uint8_t *) characterPtr + (i&1)));                
                }
                #if _XL_SLOW_DOWN_FACTOR>0
                    _XL_SLOW_DOWN(55);
                #endif
            }
    }
}

#endif


void fillLevelWithCharacters(void)
{
    uint8_t i;
    uint8_t j;
    uint8_t count = 0;
    
    #if defined(FULL_GAME) && !defined(_XL_NO_COLOR)
    if(isBossLevel)
    {
            HORIZONTAL_BRICK_IMAGE._color = _XL_RED;
            VERTICAL_BRICK_IMAGE._color = _XL_RED;
    }
    else if(level&1)
    {
        HORIZONTAL_BRICK_IMAGE._color = _XL_YELLOW;
        VERTICAL_BRICK_IMAGE._color = _XL_YELLOW;
    }
    else
    {
        HORIZONTAL_BRICK_IMAGE._color = _XL_CYAN;
        VERTICAL_BRICK_IMAGE._color = _XL_CYAN;
    }
    #endif
    
    #if !defined(TINY_GAME) && !defined(NO_BORDERS)
        DRAW_BORDERS();
    #endif    
    
    #if defined(FULL_GAME)

        if(isBossLevel && level) // level 0 is the final screen and ghostCount should remain at GHOSTS_NUMBER
        {
            ghostCount = 2+(level>>2);
            #if !defined(_XL_NO_COLOR)
                SKULL_IMAGE._color = _XL_CYAN;
                BOMB_IMAGE._color = _XL_YELLOW;
            #endif
        }
        #if !defined(_XL_NO_COLOR)
        else
        {
            SKULL_IMAGE._color = _XL_YELLOW;
            BOMB_IMAGE._color = _XL_RED;
        }
        #endif
    
        if(isRocketLevel || isBossLevel)
        {
            for(i=0;i<ROCKETS_NUMBER;i++)
            {
                rockets_x[i] = (uint8_t) (i+1)*(XSize/(ROCKETS_NUMBER+1));
                initializeCharacter(&rockets[i],(uint8_t) rockets_x[i],(uint8_t)(YSize-1),1,&ROCKET_IMAGE);
                displayRocket(&rockets[i]);
            }
        }
    #endif
            
    #if GHOSTS_NUMBER >= 9
        ROUND_NINE_GHOSTS();
    #elif GHOSTS_NUMBER==8 
        #if (!defined(TINY_GAME) || defined(ROUND_ENEMIES)) && !defined(FLAT_ENEMIES)
            ROUND_EIGHT_GHOSTS();    
        #else
            FLAT_EIGHT_GHOSTS();
        #endif
    #elif GHOSTS_NUMBER==7
        ROUND_SEVEN_GHOSTS();
    #else
        FLAT_SIX_GHOSTS();
    #endif
  
    #if BOMBS_NUMBER==4
        FOUR_BOMBS();
    #elif BOMBS_NUMBER==3    
        THREE_BOMBS();
    #elif BOMBS_NUMBER==2
        TWO_BOMBS();
    #elif BOMBS_NUMBER==1
        ONE_BOMB();
    #endif
            
    #if defined(FULL_GAME)
        initializeAwayFromWall(&(powerUp._character),(XSize>>1),(YSize>>1),1,&POWERUP_IMAGE);
        initializeAwayFromWall(&(powerUp2._character),(XSize>>1),(YSize>>1),0,&POWERUP_IMAGE);
        initializeAwayFromWall(&(freeze._character),(XSize>>1),(YSize>>1),0,&FREEZE_IMAGE);
        initializeAwayFromWall(&(extraPoints._character), (XSize>>1), (YSize>>1), 0, &EXTRA_POINTS_IMAGE);
        initializeAwayFromWall(&(super._character), (XSize>>1), (YSize>>1), 0, &SUPER_IMAGE);
        initializeAwayFromWall(&(confuse._character), (XSize>>1), (YSize>>1), 0, &CONFUSE_IMAGE);        
        initializeAwayFromWall(&(zombie._character), (XSize>>1), (YSize>>1), 0, &ZOMBIE_IMAGE);    
        
        initializeAwayFromWall(&(gun._character),(XSize>>1), (YSize>>1), (isBossLevel ? 1 : 0), &GUN_IMAGE);

        initializeAwayFromWall(&player,(uint8_t) ((XSize>>1)+(_XL_RAND()&1)),(uint8_t) ((YSize>>1)+(_XL_RAND()&1)),1,&PLAYER_IMAGE);
                
        initializeAwayFromWall(&(extraLife._character), (XSize>>1), (YSize>>1), 0, &EXTRA_LIFE_IMAGE);

        initializeAwayFromWall(&(invincibility._character), (XSize>>1), (YSize>>1), 0, &INVINCIBILITY_IMAGE);

        if(isOneMissileLevel)
        {
            initializeCharacter(&rightHorizontalMissile,         XSize-1,                      (YSize>>1), 1,&RIGHT_HORIZONTAL_MISSILE_IMAGE);            
        }
        else if(isMissileLevel || isBossLevel)
        {    
            initializeCharacter(&rightHorizontalMissile,         XSize-1,         HORIZONTAL_MISSILE_OFFSET, 1,&RIGHT_HORIZONTAL_MISSILE_IMAGE);
            initializeCharacter(&leftHorizontalMissile,                0, YSize-1-HORIZONTAL_MISSILE_OFFSET, 1,&LEFT_HORIZONTAL_MISSILE_IMAGE);        
        }        
        
        initializeAwayFromWall(&(chase._character), (XSize>>1), (YSize>>1),0,&BULLET_IMAGE);
        initializeCharacter(&chasingBullet, 0,0, 0, &BULLET_IMAGE);
    #else
        #if !defined(TINY_GAME)
            initializeCharacter(&(powerUp._character),(XSize>>1),(YSize>>1),1,&POWERUP_IMAGE);
            initializeCharacter(&(gun._character),(XSize>>1), (YSize>>1), 0, &GUN_IMAGE);    
            initializeCharacter(&(extraPoints._character), (XSize>>1), (YSize>>1), 0, &EXTRA_POINTS_IMAGE);    
        #endif
        
        #if defined(NO_RANDOM_LEVEL) || defined(TINY_GAME) || defined(SIMPLE_RANDOM_LEVEL)
            initializeCharacter(&player,(uint8_t) ((XSize>>1)),(uint8_t) ((YSize>>1)),1,&PLAYER_IMAGE);            
        #else
            initializeCharacter(&player,(uint8_t) ((XSize>>1)+(uint8_t) (_XL_RAND()&1)),
                                        (uint8_t) ((YSize>>1)+(uint8_t) (_XL_RAND()&1)),1,&PLAYER_IMAGE);    
        #endif
    #endif
    #if !defined(TINY_GAME)
        displayPlayer(&player);
            
        initializeCharacter(&bullet, 0, 0,0,&BULLET_IMAGE);

        #if defined(FULL_GAME)
            initializeCharacter(&skull,XSize-2,YSize-2, NON_BOSS_SKULL_HITS + (isBossLevel<<3), &SKULL_IMAGE);        
        #else
            initializeCharacter(&skull,XSize-2,YSize-2, NON_BOSS_SKULL_HITS, &SKULL_IMAGE);                    
        #endif
        
    #endif
    
}



#include "settings.h"
#include "move_player.h"
#include "character.h"
#include "skull.h"
#include "definitions.h"


extern Character player;

#if !defined(TINY_GAME)
    extern uint8_t skullXCountDown;
    extern uint8_t skullYCountDown;
    extern uint8_t playerFire;
    extern Character bullet;
    extern uint8_t guns;
    extern uint8_t playerDirection;
#endif

#if defined(FULL_GAME)
    extern uint8_t player_invincibility;
    // extern uint8_t playerBlink;    
#endif

#if !defined(TINY_GAME) 
    #define _DO_MOVE_UP \
        deletePlayer(&player); \
        --player._y; \
        SHOW_UP(); \
        skullYCountDown = SKULL_COUNT_DOWN; \
        playerDirection = UP;
    #define _DO_MOVE_DOWN \
        deletePlayer(&player); \
        ++player._y; \
        SHOW_DOWN(); \
        skullYCountDown = SKULL_COUNT_DOWN; \
        playerDirection = DOWN;
    #define _DO_MOVE_LEFT \
        deletePlayer(&player); \
        --player._x; \
        SHOW_LEFT(); \
        skullXCountDown = SKULL_COUNT_DOWN; \
        playerDirection = LEFT;
    #define _DO_MOVE_RIGHT \
        deletePlayer(&player); \
        ++player._x; \
        SHOW_RIGHT(); \
        skullXCountDown = SKULL_COUNT_DOWN; \
        playerDirection = RIGHT;
#else
    #define _DO_MOVE_UP \
        deletePlayer(&player); \
        --player._y; \
        SHOW_UP();
        
    #define _DO_MOVE_DOWN \
        deletePlayer(&player); \
        ++player._y; \
        SHOW_DOWN();
        
    #define _DO_MOVE_LEFT \
        deletePlayer(&player); \
        --player._x; \
        SHOW_LEFT();
        
    #define _DO_MOVE_RIGHT \
        deletePlayer(&player); \
        ++player._x; \
        SHOW_RIGHT();
#endif

#if defined(NO_INPUT)
    void MOVE_PLAYER(void) 
    {
    }
#else
    void MOVE_PLAYER(void)
    {
        uint8_t input = _XL_INPUT();
        
        if(_XL_UP(input))
        {
            _DO_MOVE_UP
        }
        else if(_XL_DOWN(input))
        {
            _DO_MOVE_DOWN
        }
        else if(_XL_LEFT(input))
        {
            _DO_MOVE_LEFT
        }
        else if(_XL_RIGHT(input))
        {
            _DO_MOVE_RIGHT
        }
        #if !defined(TINY_GAME)
        else if(_XL_FIRE(input) && guns>0 && !bullet._status)
        {
            playerFire = 1;
        }
        #endif
    }
#endif



#include "settings.h"


#if defined(FULL_GAME)

#include "character.h"
#include "level.h"


#include "cross_lib.h"


extern Character rockets[ROCKETS_NUMBER];

extern uint8_t ghostCount;
extern uint8_t isRocketLevel;
extern uint8_t isBossLevel;

	void handle_rockets(void)
	{	
		if((ghostCount<=MAX_GHOST_COUNT_FOR_ROCKETS && isRocketLevel) || isBossLevel)
		{ 
			uint8_t i;

			for(i=0;i<ROCKETS_NUMBER;++i)
			{
				if(rockets[i]._status)
				{
					if(playerKilledBy(&rockets[i]))
					{
						playerDies();
					}
					
					#if defined(TURNED_BASED)
					if(1)
					#else
					if(_XL_RAND()&1)
					#endif
					{
						deleteRocket(&rockets[i]);
						--(rockets[i]._y);
					}

					displayRocket(&rockets[i]);			
					if(rockets[i]._y<=1)
					{	
						deleteRocket(&rockets[i]);
						rockets[i]._x = (1+i)*(XSize/(ROCKETS_NUMBER+1));
						rockets[i]._y = YSize-2;							
					}
				}
			}
		}
	}


#endif // defined(FULL_GAME)


/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
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
#if !defined(TINY_GAME)

#include "settings.h"

#include "skull.h"
#include "level.h"
#include "strategy.h"

#include "cross_lib.h"

extern uint8_t level;
#if !defined(TINY_GAME) || defined(_XL_TURN_BASED)
extern uint16_t loop;
#endif
extern uint16_t skullSlowDown;
extern uint8_t skullXCountDown;
extern uint8_t skullYCountDown ;

extern uint16_t ghostLevel;

extern Character skull;
extern Character player;

extern uint8_t skullActive;

extern uint8_t isBossLevel;

#if defined(FULL_GAME)
	extern uint8_t confuseActive;
#endif


#if defined(FULL_GAME)

extern uint16_t levelSlowDown;


uint16_t computeSkullSlowDown(void)
{
	if(loop<MAX_SKULL_LOOP)
	{
		#if defined(_XL_TURN_BASED)
			return levelSlowDown - ghostLevel*8;				
		#else
			return levelSlowDown - ghostLevel*16;		
		#endif
	}
	return SKULL_MIN_SLOWDOWN; // You must die!
}
#endif


// Redability macros
#define INACTIVITY_COUNT_DOWN_REACHED ((skullXCountDown==0) || (skullYCountDown==0))
#define GHOST_COUNT_TRIGGER_REACHED (ghostCount<=SKULL_TRIGGER)
#define BOSS_LOOP_TRIGGER_REACHED (loop>=SKULL_LOOP_TRIGGER)

#define NON_BOSS_TRIGGER_REACHED (INACTIVITY_COUNT_DOWN_REACHED || GHOST_COUNT_TRIGGER_REACHED)

#define SKULL_RAND_CONDITION (_XL_RAND()>skullSlowDown)

void handle_skull(void)
{
	if(!skullActive)
	{		
		#if defined(FULL_GAME)
		if(((!isBossLevel && NON_BOSS_TRIGGER_REACHED) || 
			 (isBossLevel && BOSS_LOOP_TRIGGER_REACHED)))
		#else
		if(NON_BOSS_TRIGGER_REACHED)
		#endif
		{
			skullActive = 1;
		}
		else
		{
			--skullXCountDown;
			--skullYCountDown;
		}
	}
	else
	{ 	
		#if defined(FULL_GAME)
			skullSlowDown = computeSkullSlowDown();
		#else
			--skullSlowDown;
		#endif

		if(SKULL_RAND_CONDITION)
		{
			// _XL_TOCK_SOUND();
			#if defined(FULL_GAME)
				if(!confuseActive || !(loop&3))
				{
					#if !defined(SIMPLE_STRATEGY)
						moveTowardCharacter(&player, &skull, 4);
					#else
						moveTowardCharacter(&player, &skull);						
					#endif
				}
			#else
				moveTowardCharacter(&skull);
			#endif
		}
        #if defined(FULL_GAME)
        else
        {
            displayCharacter(&skull);
        }
        #endif
		#if defined(FULL_GAME)
		if (playerKilledBy(&skull))
		#else
		if(areCharctersAtSamePosition(&skull, &player))
		#endif
		{
			playerDies();
		}
	}
}


#endif


/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
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

#include "settings.h"
#include "character.h"
#include "strategy.h"

#include "cross_lib.h"

extern uint8_t level;
#if !defined(TINY_GAME) || defined(_XL_TURN_BASED)
extern uint16_t loop;
#endif
extern uint8_t ghostCount;

extern Character player; 
extern Character ghosts[GHOSTS_NUMBER];

extern uint8_t strategyArray[GHOSTS_NUMBER];

extern uint16_t ghostSlowDown;

#if defined(FULL_GAME)
    extern uint8_t zombieActive;
#endif


uint8_t moveCharacter(register uint8_t *hunterOffsetPtr, register uint8_t *preyOffsetPtr)
{
    if((uint8_t) *((uint8_t *)hunterOffsetPtr) < (uint8_t) *((uint8_t *)preyOffsetPtr))
    {
        ++(*((uint8_t *) hunterOffsetPtr));
    }
    else if((uint8_t) *((uint8_t *) hunterOffsetPtr) > (uint8_t) *((uint8_t *)preyOffsetPtr))
    {
        --(*((uint8_t *) hunterOffsetPtr));
    }    
    else
    {
        return 0;
    }
    return 1;
}


#if defined(FULL_GAME) && !defined(SIMPLE_STRATEGY)
    void chaseXStrategy(Character* hunterPtr, Character* preyPtr)
    {
        deleteGhost(hunterPtr);

        if(moveCharacter((uint8_t *)hunterPtr + X_MOVE, 
                         (uint8_t *)preyPtr + X_MOVE))
        {
            return;
        }
        else
        {
            (void) moveCharacter((uint8_t *)hunterPtr + Y_MOVE, 
                                 (uint8_t *)preyPtr + Y_MOVE);
        }
    }

    void chaseYStrategy(Character* hunterPtr, Character* preyPtr)
    {
        
        deleteGhost(hunterPtr);

        if(moveCharacter((uint8_t *)hunterPtr + Y_MOVE, 
                         (uint8_t *)preyPtr + Y_MOVE))
        {
            return;
        }
        else
        {
            (void) moveCharacter((uint8_t *)hunterPtr + X_MOVE, 
                                 (uint8_t *)preyPtr + X_MOVE);
        }
    }
#endif


// strategy: 
// 4 means do no prefer horizontal to vertical movement
// 0 means always horizontal
// 9 means always vertical
#if defined(FULL_GAME) && !defined(SIMPLE_STRATEGY)
    void moveTowardCharacter(Character* preyPtr, Character *hunterPtr, uint8_t strategy)
    {
        if((_XL_RAND()&7) > strategy) // Select blind chase strategy
            { // 0 - 4
                chaseXStrategy(hunterPtr, preyPtr);    
            }
        else
            { // 5 - 9
                chaseYStrategy(hunterPtr, preyPtr);
            }
        displayGhost(hunterPtr);
    }
#elif defined(FULL_GAME) && defined(SIMPLE_STRATEGY)
    void moveTowardCharacter(Character* preyPtr, Character *hunterPtr)
    {
        uint8_t offset = (uint8_t) _XL_RAND()&1;
        
        deleteGhost(hunterPtr);
        (void) moveCharacter((uint8_t *)hunterPtr + offset, 
                      (uint8_t *)preyPtr + offset);
        displayGhost(hunterPtr);
    }    
#else
    void moveTowardCharacter(Character *hunterPtr)
    {
        uint8_t offset = (uint8_t) _XL_RAND()&1;
        
        deleteGhost(hunterPtr);
        (void) moveCharacter((uint8_t *)hunterPtr + offset,
                      (uint8_t *)(&player) + offset);
                      
        #if !defined(TINY_GAME)
        displayGhost(hunterPtr);
        #endif
    }
#endif


// #if !defined(TINY_GAME) 
#if defined(FULL_GAME) && !defined(SIMPLE_STRATEGY)
void computeStrategy(void)
{
    uint8_t i;
    uint8_t skew = level / 8;
    
    for(i=5; i<GHOSTS_NUMBER; ++i) // 3 (if total=8)
    {
        strategyArray[i] = 4+skew; // 4,5,6 
        strategyArray[9-i] = 2-skew; // 2,1,0
            
    }
    strategyArray[0] = 3;
    strategyArray[1] = 3;                
}
#endif

#define GHOST_RANDOM_CONDITION (_XL_RAND()>ghostSlowDown)


// #if defined(FULL_GAME)
// Ghosts move to new positions if they get their chanche
#if defined(FULL_GAME)
void chaseCharacter(Character *preyPtr)
#else
void chaseCharacter(void)    
#endif
{
    uint8_t i;
    
    _XL_WAIT_VSYNC();
    #if defined(DEBUG_GHOST_DISPLAY)
        SET_DEBUG_BORDER();
    #endif
    for(i=0;i<GHOSTS_NUMBER;++i)
    {
        #if defined(FULL_GAME)
            if((ghosts[i]._status || (zombieActive && loop&1)) && GHOST_RANDOM_CONDITION)
        #else
            if((ghosts[i]._status) && GHOST_RANDOM_CONDITION)    
        #endif
            {
                #if defined(FULL_GAME) && !defined(SIMPLE_STRATEGY)
                    moveTowardCharacter(preyPtr, &ghosts[i], strategyArray[i]);    
                #elif defined(FULL_GAME) && defined(SIMPLE_STRATEGY)
                    moveTowardCharacter(preyPtr, &ghosts[i]);    
                #else
                    moveTowardCharacter(&ghosts[i]);    
                #endif
            }
            #if !defined(TINY_GAME)
            else
            {
                displayGhost(&ghosts[i]);
            }
            #else
            displayGhost(&ghosts[i]);
            #endif
    }
    #if defined(DEBUG_GHOST_DISPLAY)
        UNSET_DEBUG_BORDER();
    #endif
}


#include "settings.h"

#include "character.h"
#include "item.h"
#include "strategy.h"
#include "game_text.h"
#include "level.h"
#include "ghost.h"
#include "move_player.h"
#include "text_strings.h"
#include "init_images.h"
#include "end_screen.h"
#include "variables.h"

#if !defined(TINY_GAME)
    #include "bullet.h"
    #include "skull.h"
#endif
    

#if defined(FULL_GAME)
    #include "horizontal_missile.h"
    #include "rocket.h"
#endif


#if defined(BENCHMARK)
    #include <stdio.h>
    #include <time.h>
#endif

#include "cross_lib.h"

// #define DEBUG_ITEMS

#if defined(DEBUG_ITEMS)
    #if defined(ANIMATE_PLAYER)
        extern Image PLAYER_DOWN_IMAGE;
        extern Image PLAYER_UP_IMAGE;
        extern Image PLAYER_RIGHT_IMAGE;
        extern Image PLAYER_LEFT_IMAGE;
    #else
        extern Image PLAYER_IMAGE;
    #endif
    extern Image GHOST_IMAGE;
    extern Image BOMB_IMAGE;
    extern Image DEAD_GHOST_IMAGE;
    extern Image SKULL_IMAGE;
    extern Image POWERUP_IMAGE;
    extern Image GUN_IMAGE;
    extern Image EXTRA_POINTS_IMAGE;
    extern Image BULLET_IMAGE;
    extern Image VERTICAL_BRICK_IMAGE;
    extern Image HORIZONTAL_BRICK_IMAGE;
    extern Image LEFT_HORIZONTAL_MISSILE_IMAGE;
    extern Image RIGHT_HORIZONTAL_MISSILE_IMAGE;
    extern Image ROCKET_IMAGE;
    extern Image FREEZE_IMAGE;
    extern Image SUPER_IMAGE;
    extern Image EXTRA_LIFE_IMAGE;
    extern Image INVINCIBILITY_IMAGE;
    extern Image CONFUSE_IMAGE;
    extern Image ZOMBIE_IMAGE;
    extern Image BROKEN_BRICK_IMAGE;
#endif

// Level
// The level affects:
// 1. powerUpCoolDown (how long before a new powerUp is spawned)
// 2. ghostSlowDown (how much the power up slows the enemies down)
// 3. skullXCountDown (time needed to activate the skull ghost)
// 4. skullYCountDown
// 5. skullSlowDown (how much the skull ghost is slowed-down)
// 6. skullLoopTrigger (how long before the skull ghost appears)


#if !defined(TINY_GAME)
    void resetItems(void)
    {
        gun._coolDown = GUN_COOL_DOWN;
        powerUp._coolDown = POWER_UP_COOL_DOWN;
        extraPoints._coolDown = EXTRA_POINTS_COOL_DOWN;        
        
        #if defined(FULL_GAME)
            powerUp2._coolDown = POWER_UP2_COOL_DOWN;
            freeze._coolDown = FREEZE_COOL_DOWN;                
            invincibility._coolDown = INVINCIBILITY_COOL_DOWN;

            chase._coolDown = CHASE_COOL_DOWN;
            
            super._coolDown = SUPER_COOL_DOWN;
            extraLife._coolDown = EXTRA_LIFE_COOL_DOWN;
            
            confuse._coolDown = CONFUSE_COOL_DOWN;
            zombie._coolDown = ZOMBIE_COOL_DOWN;                
        #endif
    }

    
    // Constructor for all items
    void constructItems(void)
    {
        powerUp._effect = &powerUpEffect;
        gun._effect = &gunEffect;
        extraPoints._effect = &extraPointsEffect;
        #if defined(FULL_GAME)
            powerUp2._effect = &powerUp2Effect;        
            freeze._effect = &freezeEffect;
            extraLife._effect = &extraLifeEffect;
            invincibility._effect = &invincibilityEffect;
            super._effect = &superEffect;
            confuse._effect = &confuseEffect;
            zombie._effect = &zombieEffect;
            chase._effect = &chaseEffect;
        #endif    
    }    

#endif

#if !defined(NO_INITIAL_SCREEN)            
void initialScreen(void)
{    
    _XL_CLEAR_SCREEN();                    
    printStartMessage();
    
    #if defined(FULL_GAME) && !defined(NO_HINTS) && XSize>=14
        _XL_WAIT_FOR_INPUT();
        _XL_CLEAR_SCREEN();
        printHints();    
    #endif
}
#endif


#if defined(FULL_GAME)
    
    void handle_special_triggers(void)
    {
        // confuse_present_on_level_condition is defined as bases_in_completed_levels
        zombie_present_on_level = bases_in_completed_levels>=MISSILES_FOR_ZOMBIE;
        super_present_on_level = all_skulls_killed_in_completed_levels>=SKULLS_FOR_SUPER;
        // chase_present_on_level_condition is defined as all_skulls_killed_in_completed_levels;
        extraLife_present_on_level = super_present_on_level && zombie_present_on_level;
    }
    
#endif

#if defined(DEBUG_ITEMS)

void DO_DEBUG_ITEMS(void)
{
    
    #if defined(ANIMATE_PLAYER)
        _draw_stat(1, 0,&PLAYER_DOWN_IMAGE);
        _draw_stat(1, 1,&PLAYER_UP_IMAGE);
        _draw_stat(1, 2,&PLAYER_RIGHT_IMAGE);
        _draw_stat(1, 3,&PLAYER_LEFT_IMAGE);
    #else
        _draw_stat(1, 0,&PLAYER_IMAGE);
    #endif
    _draw_stat(1, 4,&GHOST_IMAGE);
    _draw_stat(1, 5,&BOMB_IMAGE);
    #if !defined(NO_DEAD_GHOST)
        _draw_stat(1, 6,&DEAD_GHOST_IMAGE);
    #endif
    #if !defined(TINY_GAME)
        _draw_stat(1, 7,&SKULL_IMAGE);
        _draw_stat(1, 8,&POWERUP_IMAGE);
        _draw_stat(1, 9,&GUN_IMAGE);
        _draw_stat(1,10,&EXTRA_POINTS_IMAGE);
        _draw_stat(1,11,&BULLET_IMAGE);
        _draw_stat(1,12,&VERTICAL_BRICK_IMAGE);
        _draw_stat(1,13,&HORIZONTAL_BRICK_IMAGE);
        #if defined(FULL_GAME)
            _draw_stat(1,14,&LEFT_HORIZONTAL_MISSILE_IMAGE);
            _draw_stat(1,15,&RIGHT_HORIZONTAL_MISSILE_IMAGE);
            #if (YSize+Y_OFFSET)>18
            _draw_stat(1,16,&ROCKET_IMAGE);
            _draw_stat(1,17,&FREEZE_IMAGE);
            _draw_stat(1,18,&SUPER_IMAGE);
            _draw_stat(1,19,&EXTRA_LIFE_IMAGE);
            _draw_stat(1,20,&INVINCIBILITY_IMAGE);
            _draw_stat(1,21,&CONFUSE_IMAGE);
            _draw_stat(1,22,&ZOMBIE_IMAGE);
            _draw_stat(1,23,&BROKEN_BRICK_IMAGE);
            #else
            _draw_stat(3,7,&ROCKET_IMAGE);
            _draw_stat(3,8,&FREEZE_IMAGE);
            _draw_stat(3,9,&SUPER_IMAGE);
            _draw_stat(3,10,&EXTRA_LIFE_IMAGE);
            _draw_stat(3,11,&INVINCIBILITY_IMAGE);
            _draw_stat(3,12,&CONFUSE_IMAGE);
            _draw_stat(3,13,&ZOMBIE_IMAGE);
            _draw_stat(3,14,&BROKEN_BRICK_IMAGE);
            #endif
        #endif
    #endif
    _XL_PRINTD(4,3,5, 1234U);
    _XL_PRINTD(4,5,5,56789U);
    // PRINT(4,7,"abcdefghijklmnopqrstuvwxyz,./|-");
    // PRINT(4,9,"ABCDEFGHIJKLMNOPQRSTUVWXYZ;+{}=");
    while(1){};
}
#endif


int main(void)
{        
    _XL_INIT_GRAPHICS();

    _XL_INIT_INPUT();

    _XL_INIT_SOUND();

    highScore = 0;
    
    while(1)
    {
        INIT_IMAGES();
        
        #if !defined(NO_INITIAL_SCREEN)
            initialScreen();
            REFRESH();
        #else
            printPressKeyToStart();                
        #endif
        _XL_WAIT_FOR_INPUT();
        _XL_CLEAR_SCREEN();

        #if !defined(LESS_TEXT)
            highScoreScreen();
            REFRESH();
            _XL_WAIT_FOR_INPUT();
            _XL_CLEAR_SCREEN();
        #endif

        
        #if !defined(TINY_GAME)
            extraLifeThroughPointsCounter = 1;
        #endif
        points = 0;
        level = INITIAL_LEVEL;     
        lives = LIVES_NUMBER;
        ghostCount = GHOSTS_NUMBER;
        #if defined(FULL_GAME)
            #if defined(DEBUG_ITEMS_IN_GAME)
                bases_in_completed_levels = 99;
                all_skulls_killed_in_completed_levels = 99;
            #else
                bases_in_completed_levels = 0;
                all_skulls_killed_in_completed_levels = 0;            
            #endif            
        #endif
        
        
        do // Level (Re-)Start
        {
            
            #if defined(FULL_GAME)
                isBossLevel = bossLevel();
                isRocketLevel = rocketLevel();
                isOneMissileLevel = oneMissileLevel();
                isMissileLevel = missileLevel();
                ishorizWallsLevel = horizWallsLevel();
            #endif
            #if !defined(TINY_GAME) || defined(_XL_TURN_BASED)
                loop = 0;
            #endif
            
            #if !defined(TINY_GAME)
        
                playerFire = 0;
            #endif
        
            #if defined(FULL_GAME)
                ghostLevel = 0;
                
                bases = 0;
                
                invincibilityActive = 1;                
                invincibility_count_down = INITIAL_INVINCIBILITY_COUNT_DOWN;
                
                #if !defined(INITIAL_GHOST_FREEZE)
                    freezeActive = 0;
                    freeze_count_down = 0;
                #endif
                confuseActive = 0;
                zombieActive = 0; 
                
                handle_special_triggers();
                
                #if !defined(SIMPLE_STRATEGY)
                    computeStrategy();            
                #endif
                                
            #endif            
            
            #if !defined(TINY_GAME)
                skullActive = 0;
                
                guns = 0;
                
                resetItems();
                
                #if defined(INITIAL_GHOST_FREEZE)
                    freezeActive = 1;
                    freeze_count_down = INITIAL_FROZEN_COUNT_DOWN;
                #endif
                
                skullXCountDown = SKULL_COUNT_DOWN;
                skullYCountDown = SKULL_COUNT_DOWN;        
                #if !defined(FULL_GAME)
                    skullSlowDown = INITIAL_GHOST_SLOWDOWN;
                #endif
            #endif

            #if !defined(FULL_GAME)
                ghostSlowDown = INITIAL_GHOST_SLOWDOWN-(uint16_t)level*256;
            #else
                levelSlowDown = INITIAL_GHOST_SLOWDOWN-(uint16_t)level*256;
                ghostSlowDown = computeGhostSlowDown();
            #endif
            
            _XL_CLEAR_SCREEN();
            #if !defined(LESS_TEXT)
                // Clear the screen, put cursor in upper left corner
                printLevel();
                _XL_SLEEP(1);
                // _XL_CLEAR_SCREEN();
            #endif
                
            #if defined(FULL_GAME)
                        
                arrowRange = computeArrowRange();
            
                if(isBossLevel)
                {
                    printKillTheSkull();
                    _XL_SLEEP(1);
                    _XL_WAIT_FOR_INPUT();
                }
                _XL_CLEAR_SCREEN();
                
                updateInnerWallVerticalData();
            #endif
            
            printPressKeyToStart();
            REFRESH();
            _XL_WAIT_FOR_INPUT();
            _XL_CLEAR_SCREEN();
            
            fillLevelWithCharacters();
            #if !defined(TINY_GAME)
                constructItems();    
                
                #if !defined(NO_STATS)
                displayStatsTitles();
                #endif
            #endif
            
            displayScore();        
            #if !defined(NO_STATS)    
                printLevelStats();
                printLivesStats();
            #endif
            //
            #if !defined(TINY_GAME)
                #if !defined(NO_STATS)
                printGunsStats();
                #endif
                printGhostCountStats();

            #endif        
            
            #if defined(DEBUG_ITEMS)
                DO_DEBUG_ITEMS();
            #else
            
            #if defined(FULL_GAME)
                #if !defined(BENCHMARK)
                    while(player._status && ((ghostCount>0 && !isBossLevel) || (skull._status && isBossLevel))) // while alive && there are still ghosts
                #else
                    Ticks = clock();

                    while(benchmark_count<BENCHMARK_MAX && player._status && ((ghostCount>0 && isBossLevel) || (skull._status && isBossLevel))) // while alive && there are still ghosts
                #endif
            #else
                while(player._status && (ghostCount>0) )
            #endif
            {
                
                #if defined(BENCHMARK)
                    ++benchmark_count;
                #endif
                #if defined(DEBUG_END)
                    //gameCompleted();
                #endif
                    
                #if !defined(_XL_TURN_BASED)
                    MOVE_PLAYER();                
                    _DRAW_PLAYER();    
                #endif
                        
                #if defined(FULL_GAME)
                    handle_rockets();
                    handle_missiles();
                #endif
                

                #if !defined(TINY_GAME)
                if(points>(extraLifeThroughPointsCounter*EXTRA_LIFE_THROUGH_POINTS))
                {
                    ++extraLifeThroughPointsCounter;
                    _XL_PING_SOUND();
                    ++lives;
                    printLivesStats();
                }
                #endif
                
                #if !defined(FULL_GAME)
                    if(ghostSlowDown) 
                    {
                        --ghostSlowDown;
                    }
                #endif
                #if !defined(TINY_GAME)
                    handle_bullet();
                #endif
                #if !defined(TINY_GAME)                        

                    if(!freezeActive)
                    {
                        #if defined(FULL_GAME)
                            if(confuseActive && skullActive && skull._status)
                            {
                                chasedByGhosts=&skull;
                            }
                            else
                            {
                                chasedByGhosts=&player;
                            }
                            
                            chaseCharacter(chasedByGhosts);

                            #if BOMBS_NUMBER==4
                                if(isBossLevel && ghostCount<=2)
                                {
                                    #if defined(SIMPLE_STRATEGY)
                                        moveTowardCharacter(chasedByGhosts, &bombs[loop&3]);
                                    #else
                                        moveTowardCharacter(chasedByGhosts, &bombs[loop&3], (uint8_t) (1+(uint8_t)(loop&3))<<1);
                                    #endif
                                }
                            #endif                            
                            ++ghostLevel;
                        #else
                            chaseCharacter();
                        #endif
                        
                    }
                    else // Frozen ghosts must be redisplayed anyway
                    {
                        // Display ghosts
						for(ind=0;ind<GHOSTS_NUMBER;++ind)
						{
							displayGhost(&ghosts[ind]);
						}
                    }
                    
                    if(skull._status)
                    {
                        handle_skull();
                    }

                    // This detects collisions of ghosts that have just moved
                    if(bullet._status)
                    {
                        checkBullet(&bullet);
                    }
                    
                #else
                    #if !defined(NO_CHASE)
                        chaseCharacter();
                    #endif
                #endif
                
                
                // Check collisions bombs vs ghosts
                for(ind=0;ind<GHOSTS_NUMBER;++ind)
                    {
                        checkBombsVsGhost(&ghosts[ind]);
                    }
                
                #if !defined(TINY_GAME)
                    handle_extraPoints_item();
                    handle_gun_item();
                    handle_powerup_item();
                    handle_freeze_count_down();                    
                #endif
                
                #if defined(FULL_GAME)
                    handle_powerup2_item();                    
                    handle_freeze_item();    
                    handle_invincibility_item();
                    handle_invincibility_count_down();                    

                    if(chase_present_on_level_condition)
                    {
                        handle_chase_item();
                        handle_chasing_bullet();                        
                        if(super_present_on_level)
                        {
                            handle_super_item();
                            if(extraLife_present_on_level)
                            {
                                handle_extraLife_item();
                            }
                        }
                    }
                                        
                    if(confuse_present_on_level_condition)
                    {
                        handle_confuse_item();
                        handle_confuse_count_down();
                        if(zombie_present_on_level)
                        {
                            handle_zombie_item();
                            handle_zombie_count_down();    
                            if(zombieActive && !(loop&15))
                            {
                                points+=ZOMBIE_BONUS;
                                displayScore();
                                reducePowerUpsCoolDowns();
                            }
                        }
                    }
                    
                #endif
                
                #if defined(FULL_GAME)
                    if(wallReached(&player) || 
                       (!invincibilityActive && (playerReachedGhosts() || characterReachedBombs(&player) || innerWallReached(&player) || (ishorizWallsLevel && horizWallsReached())))
                      )
                #else
                    if(wallReached(&player) || playerReachedGhosts() || characterReachedBombs(&player))
                #endif
                    {
                        playerDies();
                    }

                #if defined(FULL_GAME)
                    if(invincibilityActive && (loop&1))
                        {
                            DELETE_CHARACTER(player._x, player._y);
                        }    
                #endif
                
                #if defined(FULL_GAME)
                    SKIP_BOMB_DRAW
                    {
                        displayBombs();    
                    }
                    SKIP_WALL_DRAW
                    {                        
                    
                        #if defined(FULL_GAME)
                            ghostSlowDown = computeGhostSlowDown();
                        #endif
                        
                        #if defined(BUGGY_VERTICAL_DRAW)
                        DRAW_VERTICAL_LINE(XSize/2, 1+YSize/2-(verticalWallLength/2), verticalWallLength);            
                        #else
                        DRAW_VERTICAL_LINE(XSize/2, YSize/2-(verticalWallLength/2), verticalWallLength);            
                        #endif
                        if(ishorizWallsLevel)
                        {                
                            horizWallsLength = HORIZONTAL_WALLS_INITIAL_LENGTH + (level>>4) + (uint8_t) (loop/HORIZONTAL_WALLS_INCREASE_LOOP);        
                            DRAW_HORIZONTAL_WALLS(horizWallsLength);    
                        }                        
                    }
                                        
                #else
                    SKIP_BOMB_DRAW
                    {
                        displayBombs();
                    }
                #endif
                

                #if defined(_XL_TURN_BASED) 
                    #if !defined(EVEN_LOOP_MOVE)
                    if((loop<TURN_BASED_MAX_LOOP) || loop&1)
                    #else
                    if(loop<TURN_BASED_MAX_LOOP)   
                    #endif
                    {
                        MOVE_PLAYER();
                    }                    
                    _DRAW_PLAYER();    
                #endif    
                #if !defined(TINY_GAME) || defined(_XL_TURN_BASED)
                    ++loop;
                #endif

                #if defined(_XL_SLOW_DOWN_FACTOR) && _XL_SLOW_DOWN_FACTOR>0
                    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                #endif
                REFRESH();
            }; // end inner while [while (player._alive && ghostCount>0), i.e., exit on death or end of level]
    
            #if defined(BENCHMARK)
                TicksDelta = clock() - Ticks;
                Sec = (uint16_t) (TicksDelta / CLOCKS_PER_SEC);
                Milli = ((TicksDelta % CLOCKS_PER_SEC) * 1000) / CLOCKS_PER_SEC;
                printf ("\nTime used: %u.%03u secs = %u ticks\n", Sec, Milli, (uint16_t) TicksDelta);    
                getchar();
            #endif

            if(player._status) // if level finished
            {
                
                #if defined(FULL_GAME)
                    _DRAW_PLAYER();
                    _XL_SLEEP(1);
                #endif
                
                #if defined(BETWEEN_LEVEL)
                    chasedEnemyPtr = &player;
                    SHOW_DOWN();
                #endif
                
                #if defined(FULL_GAME)
                    #if !defined(LESS_TEXT)
                        printVictoryMessage();
                    // #else
                        // _XL_SLEEP(2);
                        // _XL_CLEAR_SCREEN();
                    #endif
                    points+= LEVEL_BONUS;
                    displayScore();
                    _XL_SLEEP(1);
                    // _XL_WAIT_FOR_INPUT();

                #endif            

                ghostCount = GHOSTS_NUMBER;

                #if defined(FULL_GAME)            
                    if(isBossLevel)
                    {    
                        all_skulls_killed_in_completed_levels = 1;
                        bases_in_completed_levels/=2;
                    }
                    else
                    {
                        if(!skull._status)
                        {
                            ++all_skulls_killed_in_completed_levels;
                        }
                        bases_in_completed_levels+=bases;
                    }
                #endif
                ++level;
            }
            else // if dead
            {        
                #if defined(FULL_GAME) && (defined(END_SCREEN) || defined(DANCE))
                    for(bulletDirection=0;bulletDirection<30;++bulletDirection)
                    {
                        for(ind=0;ind<GHOSTS_NUMBER;++ind)
                        {
                            if(ghosts[ind]._status)
                                dance(&ghosts[ind]);
                        }
                    }
                #endif
                
                #if defined(BETWEEN_LEVEL)
                    chasedEnemyPtr = &skull;    
                #endif
                
                if(--lives>0)
                {
                    player._status = 1;
                }
            }
            #if defined(BETWEEN_LEVEL)
                spiral(chasedEnemyPtr);
                _XL_SLEEP(2);
            #endif                
            
            #endif
        } while (player._status && (level<(FINAL_LEVEL+1))); // lives left and not completed game game 
            
        // _XL_SLEEP(2);
        if(level==FINAL_LEVEL+1) // if completed game
        {
            // _XL_WAIT_FOR_INPUT();
            #if !defined(NO_GAME_COMPLETED)
				gameCompleted();
			#endif
            #if !defined(NO_SLEEP)
                _XL_SLEEP(2);
            #else
                _XL_WAIT_FOR_INPUT();
            #endif
        }

        // GAME OVER    
        _XL_CLEAR_SCREEN();
        displayScore();
        printGameOver();
        
        _XL_WAIT_FOR_INPUT();
        
        if(points>highScore)
        {
            highScore = points;
        }
        
    } // while(1) -> restart from the beginning

    return EXIT_SUCCESS;
}
