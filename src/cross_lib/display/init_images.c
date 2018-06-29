#include "display_macros.h"

#include "graphics.h"

#if !defined(REDEFINED_CHARS)
	extern Image PLAYER_IMAGE;
#else
	extern Image PLAYER_DOWN;
	extern Image PLAYER_UP;
	extern Image PLAYER_RIGHT;
	extern Image PLAYER_LEFT;	
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
	
	extern Image BROKEN_WALL_IMAGE;
#endif


void INIT_IMAGES(void)
{		
	#if !defined(NO_COLOR)
		
		#if defined(REDEFINED_CHARS)
			PLAYER_DOWN._color = COLOR_CYAN;
			PLAYER_UP._color = COLOR_CYAN;
			PLAYER_RIGHT._color = COLOR_CYAN;
			PLAYER_LEFT._color = COLOR_CYAN;
		#else
			PLAYER_IMAGE._color = COLOR_CYAN;
		#endif	
	
		BOMB_IMAGE._color = COLOR_RED;
		GHOST_IMAGE._color = COLOR_WHITE;		
		
		#if !defined(NO_DEAD_GHOSTS)
			DEAD_GHOST_IMAGE._color = COLOR_RED;
		#endif
		
		#if !defined(TINY_GAME)
			SKULL_IMAGE._color = COLOR_YELLOW;
			POWERUP_IMAGE._color = COLOR_GREEN;
			GUN_IMAGE._color = COLOR_BLUE;
			EXTRA_POINTS_IMAGE._color = COLOR_YELLOW;
			BULLET_IMAGE._color = COLOR_BLUE;
		#endif
		
		#if defined(FULL_GAME)
			RIGHT_HORIZONTAL_MISSILE_IMAGE._color = COLOR_WHITE;		
			LEFT_HORIZONTAL_MISSILE_IMAGE._color = COLOR_WHITE;
			
			ROCKET_IMAGE._color = COLOR_WHITE;

			FREEZE_IMAGE._color = COLOR_CYAN;
			EXTRA_LIFE_IMAGE._color = COLOR_YELLOW;
			
			INVINCIBILITY_IMAGE._color = COLOR_YELLOW;
			SUPER_IMAGE._color = COLOR_RED;
			CONFUSE_IMAGE._color = COLOR_RED;
			ZOMBIE_IMAGE._color = COLOR_YELLOW;
		#endif	
	#endif

	#if defined(REDEFINED_CHARS)
		PLAYER_DOWN._imageData = _PLAYER_DOWN;
		PLAYER_UP._imageData = _PLAYER_UP;	
		PLAYER_RIGHT._imageData = _PLAYER_RIGHT;
		PLAYER_LEFT._imageData = _PLAYER_LEFT;	
	#else
		PLAYER_IMAGE._imageData = _PLAYER;			
	#endif
	
	GHOST_IMAGE._imageData = _GHOST;
	BOMB_IMAGE._imageData = _BOMB;
	
	#if !defined(NO_DEAD_GHOSTS)
		DEAD_GHOST_IMAGE._imageData = _DEAD_GHOST;	
	#endif
	
	#if !defined(TINY_GAME)
		SKULL_IMAGE._imageData = _SKULL;

		POWERUP_IMAGE._imageData = _POWERUP;
		GUN_IMAGE._imageData = _GUN;
		EXTRA_POINTS_IMAGE._imageData = _EXTRA_POINTS;
		
		BULLET_IMAGE._imageData = _BULLET;
	#endif
	
	#if defined(FULL_GAME)
		LEFT_HORIZONTAL_MISSILE_IMAGE._imageData = _LEFT_HORIZONTAL_MISSILE;
		RIGHT_HORIZONTAL_MISSILE_IMAGE._imageData = _RIGHT_HORIZONTAL_MISSILE;
		
		ROCKET_IMAGE._imageData = _ROCKET;

		#if !defined(NO_COLOR)
			FREEZE_IMAGE._imageData = _POWERUP;		
			SUPER_IMAGE._imageData = _POWERUP;		
		#else
			FREEZE_IMAGE._imageData = _FREEZE;
			SUPER_IMAGE._imageData = _SUPER;				
		#endif
		
		EXTRA_LIFE_IMAGE._imageData = _EXTRA_LIFE;
		INVINCIBILITY_IMAGE._imageData = _INVINCIBILITY;	
		SUPER_IMAGE._imageData = _POWERUP;
		CONFUSE_IMAGE._imageData = _CONFUSE;
		ZOMBIE_IMAGE._imageData = _GHOST;
	#endif
}

	
/*	
	void INIT_IMAGES(void)
	{		
		#if !defined(NO_COLOR)
			#if !defined(REDEFINED_CHARS)
				PLAYER_IMAGE._color = COLOR_CYAN;
			#endif
			BOMB_IMAGE._color = COLOR_RED;
			
			#if !defined(DNO_DEAD_GHOSTS)			
				DEAD_GHOST_IMAGE._color = COLOR_RED;
			#endif
			
			#if !defined(TINY_GAME)
				SKULL_IMAGE._color = COLOR_YELLOW;
				POWERUP_IMAGE._color = COLOR_GREEN;
				GUN_IMAGE._color = COLOR_BLUE;
				EXTRA_POINTS_IMAGE._color = COLOR_YELLOW;			
			#endif

			#if defined(FULL_GAME)
				FREEZE_IMAGE._color = COLOR_CYAN;
				EXTRA_LIFE_IMAGE._color = COLOR_BLUE;
				INVINCIBILITY_IMAGE._color = COLOR_YELLOW;	
				SUPER_IMAGE._color = COLOR_RED;
				CONFUSE_IMAGE._color = COLOR_RED;
				ZOMBIE_IMAGE._color = COLOR_RED;
			#endif		
		#endif
		
		#if defined(__WINCMOC__) || defined(__CMOC__)
			BOMB_IMAGE._imageData = 'x';	
			GHOST_IMAGE._imageData = 'O';				
		#else
			BOMB_IMAGE._imageData = 'X';
			GHOST_IMAGE._imageData = 'o';			
		#endif
		
		#if !defined(REDEFINED_CHARS)
			#if !defined(__CREATIVISION__)
				PLAYER_IMAGE._imageData = '*';			
			#else
				PLAYER_IMAGE._imageData = 'I';
			#endif
		#endif
		
		#if defined(REDEFINED_CHARS)
			#if defined(__APPLE2ENH__)
				PLAYER_DOWN._imageData = 77;
				PLAYER_UP._imageData = 75;
				PLAYER_LEFT._imageData = 76;
				PLAYER_RIGHT._imageData = 78;
			#else
			#endif
			#if !defined(NO_COLOR)
				PLAYER_DOWN._color = COLOR_CYAN;
				PLAYER_UP._color = COLOR_CYAN;
				PLAYER_LEFT._color = COLOR_CYAN;
				PLAYER_RIGHT._color = COLOR_CYAN;			
			#endif
		#endif
		
		#if !defined(TINY_GAME)
			SKULL_IMAGE._imageData = '+';
			EXTRA_POINTS_IMAGE._imageData = '$';			
			#if defined(__WINCMOC__) || defined(__CMOC__)
				POWERUP_IMAGE._imageData = 's';	
				GUN_IMAGE._imageData = '!';				
			#else
				POWERUP_IMAGE._imageData = 'S';
				GUN_IMAGE._imageData = '!';			
			#endif

			BULLET_IMAGE._imageData = '.';
		#endif
		
		#if !defined(DNO_DEAD_GHOSTS)
			#  if defined(__CREATIVISION__) || defined(__ZX81__) || defined(__LAMBDA__) || defined(__ZX80__) || defined(__ACE__) 
				DEAD_GHOST_IMAGE._imageData = 'x';		 
			#else
				DEAD_GHOST_IMAGE._imageData = '#';
			#endif
		#endif

		#if !defined(NO_COLOR)
			#if !defined(__GAMATE__)
				GHOST_IMAGE._color = COLOR_WHITE;
				#if !defined(TINY_GAME)
					BULLET_IMAGE._color = COLOR_WHITE;
				#endif
			#else
				GHOST_IMAGE._color = COLOR_YELLOW;
				#if !defined(TINY_GAME)
					BULLET_IMAGE._color = COLOR_YELLOW;
				#endif
			#endif
		#endif
		
		#if defined(FULL_GAME)
			#if !defined(NO_COLOR)
				LEFT_HORIZONTAL_MISSILE_IMAGE._color = COLOR_WHITE;
				RIGHT_HORIZONTAL_MISSILE_IMAGE._color = COLOR_WHITE;
				ROCKET_IMAGE._color = COLOR_WHITE;	
				BROKEN_WALL_IMAGE._color = COLOR_RED;		
			#endif
			LEFT_HORIZONTAL_MISSILE_IMAGE._imageData = '>';
			RIGHT_HORIZONTAL_MISSILE_IMAGE._imageData = '<';
		
			ROCKET_IMAGE._imageData = '^';
			#if defined(__WINCMOC__) || defined(__CMOC__)	
				FREEZE_IMAGE._imageData = 'f';		
				#if !defined(REDEFINED_CHARS)
					EXTRA_LIFE_IMAGE._imageData = PLAYER_IMAGE._imageData;
				#else
					EXTRA_LIFE_IMAGE._imageData = PLAYER_DOWN._imageData;				
				#endif
				INVINCIBILITY_IMAGE._imageData = 'v';	
				SUPER_IMAGE._imageData = 'h';
				CONFUSE_IMAGE._imageData = 'c';
				ZOMBIE_IMAGE._imageData = 'z';
			#else
				FREEZE_IMAGE._imageData = 'F';
				
				#if !defined(REDEFINED_CHARS)
					EXTRA_LIFE_IMAGE._imageData = PLAYER_IMAGE._imageData;
				#else
					EXTRA_LIFE_IMAGE._imageData = PLAYER_DOWN._imageData;				
				#endif
				
				INVINCIBILITY_IMAGE._imageData = 'V';	
				SUPER_IMAGE._imageData = 'H';	
				CONFUSE_IMAGE._imageData = 'C';
				ZOMBIE_IMAGE._imageData = 'Z';
			#endif
			BROKEN_WALL_IMAGE._imageData = 'X';
		#endif
	}
*/
	
	