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

#if !defined(_SETTINGS)
#define _SETTINGS

#define INITIAL_LEVEL 1


#if defined(__WINCMOC__)
	//#define FULL_GAME
	#define TINY_GAME	
	#define ASM_KEY_DETECT
	// #define REDEFINED_CHARS
	// #define SOUNDS
	// #define NO_TEXT
	// #define NO_SLEEP
	// #define NO_RANDOM_LEVEL
	// #define NO_SET_SCREEN_COLORS
	// #define NO_INITIAL_SCREEN
#endif

#if defined(__ZX81__) || defined(__LAMBDA__)
	#define SKIP_DRAW \
		if((loop%2)==1) 
	
	#define SKIP_MORE_DRAW \
		if((loop%16)==1)
#elif defined(__SVI__) && !defined(MSX_MODE0)
	#define SKIP_DRAW \
		if((loop%2)==1) 
	
	#define SKIP_MORE_DRAW \
		if((loop%8)==1)
#elif defined(__MC1000__)
	#define SKIP_DRAW
	
	#define SKIP_MORE_DRAW \
		if((loop%8)==1)			
#elif defined(__WINCMOC__) || defined(__CMOC__)
	#define SKIP_DRAW
	
	#define SKIP_MORE_DRAW \
		if((loop%8)==1)					
#else
	#define SKIP_DRAW 

	#define SKIP_MORE_DRAW 
#endif
	

#if defined(__NASCOM__) || defined(__MC1000__) || defined(__OSIC1P__) || defined(__WINCMOC__) || defined(__CMOC__) ||  defined(__GAL__)  || defined(__Z9001__) || defined(__ABC80__) || defined(__APPLE2__) || defined(__APPLE2ENH__) || defined(__VZ__) || defined(__ATMOS__) || defined(__MZ__) || defined(__MTX__)
	#define _MOVE_UP 'I'
	#define _MOVE_DOWN 'K'
	#define _MOVE_LEFT 'J'
	#define _MOVE_RIGHT 'L'
	#define _FIRE ' '
#elif defined(__MSX__) // Special Joystick 
	#define _MOVE_UP 1
	#define _MOVE_DOWN 5
	#define _MOVE_LEFT 7
	#define _MOVE_RIGHT 3
	#define _FIRE 9		
#else
	#define _MOVE_UP 'i'
	#define _MOVE_DOWN 'k'
	#define _MOVE_LEFT 'j'
	#define _MOVE_RIGHT 'l'
	#define _FIRE ' '		
#endif


#if defined(__VIC20__) && !defined(TINY_GAME)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 400
#elif defined(__VIC20__) && defined(TINY_GAME)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 900
#elif defined(__WINCMOC__) || defined(__CMOC__) 
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 600
#elif defined(__PC6001__)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 500	
#elif defined(__NASCOM__)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 2500
#elif defined(__GAMATE__)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 1200
#elif defined(__CREATIVISION__) || defined(__ATARI5200__) || ((defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1)) 
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 700
#elif defined(__Z9001__)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 800	
#elif defined(__VZ__)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 800
#elif defined(__NES__)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 700
#elif defined(__MZ__)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 500		
#elif defined(__APPLE2__) || defined(__APPLE2ENH__)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 200
#elif (defined(__C16__) || defined(__PLUS4__)) && defined(FULL_GAME)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 80	
#elif (defined(__C16__) || defined(__PLUS4__)) && !defined(FULL_GAME)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 140		
#elif defined(__C64__)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 50		
#elif defined(__ATMOS__)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 50		
#elif defined(__CBM610__)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 700
#elif defined(__VG5K__)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 800	
#elif defined(__AQUARIUS__)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 800	
#elif defined(__ACE__)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 500	
#elif defined(__MSX__)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 550				
#elif defined(__VZ__)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 800	
#elif defined(__SPECTRUM__) && defined(TINY_GAME)
	#define GAME_SLOW_DOWN 0	
#elif defined(__SPECTRUM__) && !defined(TINY_GAME)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 400		
#elif defined(__CPC__) && defined(CPCRSLIB)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 320	
#elif defined(__SVI__) && defined(MSX_MODE0)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 800
#elif defined(__SVI__) && !defined(MSX_MODE0)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 800	
#else
	#define GAME_SLOW_DOWN 0
#endif

#if (defined(__VIC20__) && defined(TINY_GAME)) || defined(__CBM610__) || ( (!defined(__CREATIVISION__) || !defined(TINY_GAME)) && !defined(__SUPERVISION__) && !defined(__ATARI_LYNX__) && !defined(__GAMATE__) && !defined(__CBM__) && !defined(__ATARI__) && !defined(__ATARIXL__) && !defined(__ATARI5200__) && !defined(__LYNX__) && !defined(__NES__) && !defined(__PCE__))
	#define KEYBOARD_CONTROL
#else
	#define JOYSTICK_CONTROL
#endif

#if defined(KEYBOARD_CONTROL)
	#define WAIT_FOR_KEY
#else
	#define WAIT_FOR_JOY
#endif

#if (defined(__CBM__) && !defined(__VIC20__)) || defined(__APPLE2__) || defined(__APPLE2ENH__) || defined(__ATMOS__) || ((defined(__ATARI__) || defined(__ATARIXL__)) && !defined(ATARI_MODE1))
	#define CC65
	#define WIDE
#elif defined(__VIC20__) || defined(__GAMATE__) || ((defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1)) || defined(__ATARI5200__)
	#define CC65
	#define NARROW
#elif defined(__WINCMOC__) || defined(__CMOC__)
	#define CMOC
	#define NARROW
#elif defined(__SPECTRUM__) || defined(__CPC__) || defined(__VG5K__) || defined(__ZX81__) || defined(__ZX80__) || defined(__AQUARIUS__) || (defined(__SVI__) && !defined(MSX_MODE0)) || defined(__MZ__)
	#define Z88DK
	#define WIDE
#else 
	#define Z88DK
	#define NARROW
#endif

#if defined(WIDE)
	#define ADVANCED_LEFT_MISSILE()  ++leftEnemyMissile._x
	#define ADVANCED_RIGHT_MISSILE() --rightEnemyMissile._x	
#else
	#define ADVANCED_LEFT_MISSILE()  do{if(loop%2)++leftEnemyMissile._x;}while(0)
	#define ADVANCED_RIGHT_MISSILE() do{if(loop%2)--rightEnemyMissile._x;}while(0)
#endif

// || defined(__ZX81__)
#if (defined(__VIC20__) && defined(REDEFINED_CHARS)) || defined(__WINCMOC__) || defined(__CMOC__) || defined(__GAMATE__) || defined(__APPLE2__) || (defined(__C64__) && defined(REDEFINED_CHARS)) || defined(__ATARI__) || defined(__ATARIXL__)  || defined(__ZX80__)
	#define NO_CASE_LETTERS
#else
	#define CASE_LETTERS
#endif

#if defined(__GAMATE__) || defined(__ZX81__) || defined(__LAMBDA__) 
	#define BOMBS_NUMBER 3
#elif defined(__SPECTRUM__) && !defined(TINY_GAME)
	#define BOMBS_NUMBER 4
#elif defined(__PCE__) || (defined(__VIC20__) && defined(TINY_GAME)) || (defined(__SPECTRUM__) && defined(TINY_GAME))
	#define BOMBS_NUMBER 4
#elif defined(__AQUARIUS__) && defined(TINY_GAME)
	#define BOMBS_NUMBER 1
#elif defined(__OSIC1P__) && defined(TINY_GAME)
	#define BOMBS_NUMBER 4
#else
	#define BOMBS_NUMBER 4
#endif

// Possible current values are FULL: 8,9, LIGHT: 8, TINY: 8,7,6
#if !defined(FULL_GAME) && !defined(TINT_GAME)
	#define GHOSTS_NUMBER 8
#elif defined(__PCE__)
	#define GHOSTS_NUMBER 8
#elif defined(__SPECTRUM__) && defined(TINY_GAME)
	#define GHOSTS_NUMBER 8	
#elif defined(__VIC20__) && defined(TINY_GAME)
	#define GHOSTS_NUMBER 8
#elif defined(__GAMATE__) 
	#define GHOSTS_NUMBER 8
#elif defined(__SPECTRUM__) && defined(FULL_GAME)
	#define GHOSTS_NUMBER 9
#elif defined(__SPECTRUM__) && defined(TINY_GAME)
	#define GHOSTS_NUMBER 8
#elif defined(__ATMOS__)
	#define GHOSTS_NUMBER 9
#elif defined(__ZX81__) || defined(__LAMBDA__)
	#define GHOSTS_NUMBER 8
#elif defined(FULL_GAME) && (defined(__MZ__) || defined(__VZ__) || defined(__ZX80__) || ((defined(__ATARI__) || defined(__ATARIXL__)) && !defined(ATARI_MODE1)) || defined(__C64__) || defined(__C16__) || defined(__PLUS4__) || (defined(__CPC__) && defined(CPCRSLIB)) || defined(__MSX__) )
	#define GHOSTS_NUMBER 9
#elif !defined(FULL_GAME) && (defined(__MZ__) || defined(__VZ__) || defined(__ZX80__) || ((defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1)) || defined(__C64__) || defined(__C16__) || defined(__PLUS4__) || (defined(__CPC__) && defined(CPCRSLIB)) || defined(__MSX__) )
	#define GHOSTS_NUMBER 8
#elif defined(__VG5K__) && defined(FULL_GAME)
	#define GHOSTS_NUMBER 9
#elif defined(__VG5K__) && !defined(FULL_GAME)
	#define GHOSTS_NUMBER 8
#elif defined(__SVI__) && !defined(FULL_GAME)
	#define GHOSTS_NUMBER 8
#elif defined(__CREATIVISION__)
	#define GHOSTS_NUMBER 8	
#elif (defined(__OSIC1P__) && defined(TINY_GAME)) 
	#define GHOSTS_NUMBER 8
#else
	#define GHOSTS_NUMBER 8
#endif
	
#if defined(__PC6001__) || defined(__ATARI5200__) || (defined(__AQUARIUS__) && defined(TINY_GAME)) || defined(__PCE__) || defined(__CREATIVISION__) || defined(__WINCMOC__) || defined(__CMOC__) || defined(__OSIC1P__) || defined(__MC1000__) || defined(__LAMBDA__) || (defined(__MSX__) && (defined(MSX_MODE1) || defined(MSX_MODE0))) || (defined(__SVI__) && defined(MSX_MODE0) ) || defined(__VZ__) || defined(__VG5k__) || ((defined(__ATARI__) || defined(__ATARIXL__))) && !defined(ATARI_MODE1) || defined(__PET__) || defined(__CBM610__) || defined(__APPLE2__) || defined(__APPLE2ENH__) || defined(__ZX81__) || defined(__ZX80__) || defined(__ACE__)
	#define NO_COLOR
#else
	#define COLOR
#endif
	
#if defined(__ZX81__) || defined(__LAMBDA__)
	#define BUBBLES_NUMBER 2
#else
	#define BUBBLES_NUMBER 4
#endif

#define LIVES_NUMBER 5
#define GUNS_NUMBER 3

#define BACKGROUND_COLOR COLOR_BLACK

#define BORDER_COLOR COLOR_BLACK

#if defined(__CPC__) || defined(__GAMATE__)
	#define TEXT_COLOR 3
#else
	#define TEXT_COLOR COLOR_WHITE
#endif

#define EXTRA_LIFE_THROUGH_POINTS 2000UL

#define EXTRA_POINTS_COOL_DOWN 1500UL

#define INVINCIBILITY_COOL_DOWN 2000UL

#define PLAYER_INVINCIBILITY_COOL_DOWN 150

#define EXTRA_LIFE_COOL_DOWN 2500UL

// #define EXTRA_LIFE_FIRST_LEVEL 10

#define MIN_INVINCIBLE_GHOST_HITS 5
	
#define HORIZONTAL_MISSILE_BONUS 50UL

#define VERTICAL_MISSILE_BONUS 20UL

#define INVINCIBLE_GHOST_POINTS 300UL;

// Points given by extra points bonus: 
// EXTRA_POINTS + level X EXTRA_POINTS_LEVEL_INCREASE
#define EXTRA_POINTS 100UL 
#define EXTRA_POINTS_LEVEL_INCREASE 5UL

// If a ghost bumps into a bomb
#define GHOST_VS_BOMBS_BONUS 25UL

// Points for shooting a ghost
#define GHOST_VS_MISSILE 10UL

// Extra points for the power up
#define POWER_UP_BONUS 20UL

// Extra points for the power up
#define GUN_BONUS 30UL

// Points gained at the end of each level (to be multipled by level)
#define LEVEL_BONUS 50U

#if defined(__PC6001__) || defined(__GAMATE__) || defined(__VZ__) || defined(__CBM610__) || defined(__VIC20__) || defined(__ATARI5200__) || ((defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1)) || defined(__APPLE2__) || defined(__APPLE2ENH__)
	#define INITIAL_ENEMY_SLOWDOWN 31000
	#define INITIAL_SKULL_SLOWDOWN 32000	
#elif defined(__VG5K__) || defined(__MSX__) || defined(__WINCMOC__) || defined(__CMOC__)
	#define INITIAL_ENEMY_SLOWDOWN 29000
	#define INITIAL_SKULL_SLOWDOWN 30000	
#else
	#define INITIAL_ENEMY_SLOWDOWN 28000
	#define INITIAL_SKULL_SLOWDOWN 29000
#endif


// Final level 
#define FINAL_LEVEL 40

//#define INVINCIBILITY_FIRST_LEVEL 5

#define FIRST_HORIZONTAL_WALLS_LEVEL 6

#if defined(WIDE)
	#define HORIZONTAL_WALLS_INITIAL_LENGTH 7
#else
	#define HORIZONTAL_WALLS_INITIAL_LENGTH 2	
#endif

#define FIRST_BUBBLES_LEVEL 7 // 8 because of check on levels after missile level

// Maximum number of ghost that co-exist with rockets (in non-boss levels)
#define MAX_GHOST_COUNT_FOR_BUBBLES 4

// Directions
#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3

#if defined(WIDE)
	#define ENEMY_MISSILE_OFFSET 3
#else
	#define ENEMY_MISSILE_OFFSET 2
#endif

// -----------------------------------------------------------------------------------
#if GHOSTS_NUMBER>=8
	#define INVINCIBLE_GHOST_TRIGGER 4
#else
	#define INVINCIBLE_GHOST_TRIGGER 3
#endif 




#define POWER_UP_INITIAL_COOLDOWN 150
//(130 + level * 2)
#define POWER_UP2_INITIAL_COOLDOWN 400

#define POWER_UP3_INITIAL_COOLDOWN 600

#define SUPER_COOL_DOWN 100

//550

#define FROZEN_COUNT_DOWN 24

#define INVINCIBLE_COUNT_DOWN 50

#if defined(FULL_GAME)
	#define INVINCIBLE_LOOP_TRIGGER 600-level*4
	#define FREEZE_INITIAL_COOLDOWN 1000-level*16
	#define GUN_INITIAL_COOLDOWN 220-level*4
#else
	#define INVINCIBLE_LOOP_TRIGGER 500
	#define FREEZE_INITIAL_COOLDOWN 700
	#define GUN_INITIAL_COOLDOWN 200	
#endif	

#define INVINCIBLE_MIN_SLOWDOWN 4000

#define ENEMY_MIN_SLOWDOWN 1500

#define GHOST_LEVEL_DECREASE 50

#define MAX_GHOST_LEVEL 1000

#define MAX_INVINCIBLE_LOOP 1600

#endif // _SETTINGS
