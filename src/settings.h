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

// Final level 
#define FINAL_LEVEL 20

#if !defined(__SMS__) && !defined(__CMOC__) && !defined(__SUPERVISION__) \
	&& !defined(__ATARI_LYNX__) && !defined(__MSX__) \
    && !(defined(__SVI__) && defined(MSX_MODE0)) && !(defined(__SPECTRUM__) \
	&& !defined(CLIB_ANSI)) && !defined(__GCC__) && !defined(__ENTERPRISE__) \
	&& !defined(__PX8__) && !defined(__KC__) \
	&& !defined(__M5__) \
	&& !defined(__EG2K__) && !defined(__TRS80__) \
	&& !defined(__ABC800__) \
	&& !(defined(__KC__) && defined(Z88DK_SPRITES)) \
	&& !(defined(__NC100__) && defined(Z88DK_SPRITES)) \
	&& !defined(__CPM_ADM3A__) && !defined(__CPM_VT52__) && !defined(__CPM_VT100__) \
	&& !defined(__EINSTEIN__)
	#define CONIO_LIB	
#endif
#if defined(__WINCMOC__) || defined(FORCE_CONIO)
	#define CONIO_LIB
#endif

#if defined(__WINCMOC__)
	//#define FULL_GAME
	#define TINY_GAME	
	#define ASM_KEY_DETECT
	// #define REDEFINED_CHARS
	// #define SOUNDS
	// #define LESS_TEXT
	// #define NO_SLEEP
	// #define NO_RANDOM_LEVEL
	// #define NO_SET_SCREEN_COLORS
	// #define NO_INITIAL_SCREEN
#endif

#if defined(TURN_BASED)
	#define NO_BLINKING
#endif

#if defined(TURN_BASED)
	#define SKIP_DRAW 
	#define SKIP_MORE_DRAW
#elif defined(__ZX81__) || defined(__LAMBDA__)
	#define SKIP_DRAW \
		if((loop&1)==1)
	
	#define SKIP_MORE_DRAW \
		if((loop&15)==1)	
#elif defined(__SVI__) && !defined(MSX_MODE0)
	#define SKIP_DRAW \
		if((loop&1)==1) 
	
	#define SKIP_MORE_DRAW \
		if((loop&7)==1)
// #elif defined(__MC1000__)
	// #define SKIP_DRAW
	
	// #define SKIP_MORE_DRAW \
		// if((loop&3)==1)			
#elif !defined(__WINCMOC__) && defined(__CMOC__)
	#define SKIP_DRAW
	
	#define SKIP_MORE_DRAW 	\
		if((loop%16)==1)				
#elif defined(__WINCMOC__) && defined(__CMOC__)
	#define SKIP_DRAW
	
	#define SKIP_MORE_DRAW \
		if((loop%8)==1)					
#else
	#define SKIP_DRAW 

	#define SKIP_MORE_DRAW 

#endif
	

#if defined(__NASCOM__) || defined(__MC1000__) || defined(__OSIC1P__) || defined(__WINCMOC__) || defined(__CMOC__) \
	||  defined(__GAL__)  || defined(__Z9001__) || defined(__ABC80__) || defined(__APPLE2__) || defined(__APPLE2ENH__) \
	|| defined(__VZ__) || defined(__ATMOS__) || defined(__MZ__) || defined(__MTX__) || defined(__SC3000__) \
	|| defined(__Z1013__) || defined(__TRS80__) || defined(__EG2K__) || defined(__KC__) || defined(__C128_Z80__)
	#define _MOVE_UP 'I'
	#define _MOVE_DOWN 'K'
	#define _MOVE_LEFT 'J'
	#define _MOVE_RIGHT 'L'
	#define _FIRE ' '
#elif defined(__CPC_PLUS__)
	#define _MOVE_UP 0x0B 
	#define _MOVE_DOWN 0x0A 
	#define _MOVE_LEFT 0x08
	#define _MOVE_RIGHT 0x09
	#define _FIRE 0x58
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

#if defined(TURN_BASED)
	#define GAME_SLOW_DOWN 0
#else	
	#  if defined(__MC1000__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 900			
	#elif defined(__VIC20__) && defined(ALT_PRINT)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 400	
	#elif defined(__VIC20__) && !defined(TINY_GAME) 
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 250
	#elif defined(__SAM__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 500
	#elif defined(__VIC20__) && defined(TINY_GAME) && !defined(VIC20_UNEXPANDED)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 400
	#elif defined(__VIC20__) && defined(TINY_GAME) && defined(VIC20_UNEXPANDED)
		#define GAME_SLOW_DOWN 0	
	#elif defined(__WINCMOC__) || defined(__CMOC__) 
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 600
	#elif defined(__PC6001__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 500	
	#elif defined(__NASCOM__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 9000
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
		#define GAME_SLOW_DOWN 200
	#elif (defined(__C16__) || defined(__PLUS4__)) && !defined(FULL_GAME)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 250		
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
	#elif defined(__AQUARIUS__) && defined(ALT_PRINT)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 1500		
	#elif defined(__AQUARIUS__) && !defined(ALT_PRINT)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 800	
	#elif defined(__ACE__) || defined(__BEE__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 500	
	#elif defined(__MSX__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 550		
	#elif defined(__PCE__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 3000		
	#elif defined(__VZ__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 800	
	#elif defined(__SPECTRUM__) && defined(TINY_GAME)
		#define GAME_SLOW_DOWN 0	
	#elif defined(__SPECTRUM__) && !defined(TINY_GAME)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 1700
	#elif defined(__CPC__) && defined(CPCRSLIB)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 400	
	#elif defined(__SVI__) && defined(MSX_MODE0)
		#define SLOW_DOWN 
		#define GAME_SLOW_DOWN 1200
	#elif defined(__ATARI_LYNX__)
		#define SLOW_DOWN 
		#define GAME_SLOW_DOWN 2500	
	#elif defined(__SVI__) && !defined(MSX_MODE0)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 1200	
	#elif defined(__TRS80__) || defined(__EG2K__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 100	
	#else
		#define GAME_SLOW_DOWN 0
	#endif
#endif

#if (defined(__VIC20__) && !defined(VIC20_UNEXPANDED)) \
		|| defined(__CREATIVISION__) \
		|| defined(__SUPERVISION__) || defined(__ATARI_LYNX__) || defined(__GAMATE__) \
		|| defined(__C64__) || defined(__C16__) || defined(__PLUS4__) \
		|| defined(__CBM510__) || defined(__PET__) \
		|| (defined(__C128__) && !defined(__C128_Z80__)) \
		|| defined(__ATARI__) || defined(__ATARIXL__) || defined(__ATARI5200__) || defined(__LYNX__) \
		|| defined(__NES__) || defined(__PCE__)
	#define JOYSTICK_CONTROL
#else
	#define KEYBOARD_CONTROL
#endif

#if defined FORCE_KEYBOARD
	#undef JOYSTICK_CONTROL
	#define KEYBOARD_CONTROL
#endif

#if defined(KEYBOARD_CONTROL)
	#define WAIT_FOR_KEY
#else
	#define WAIT_FOR_JOY
#endif

#if (defined(__CBM__) && !defined(__VIC20__)) || defined(__APPLE2__) || defined(__APPLE2ENH__) || defined(__ATMOS__) \
    || ((defined(__ATARI__) || defined(__ATARIXL__)) && !defined(ATARI_MODE1)) 
	#define CC65
	#define WIDE
#elif defined(__VIC20__) || defined(__GAMATE__) || ((defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1)) \
      || defined(__ATARI5200__) || defined(__OSIC1P__)
	#define CC65
	#define NARROW
#elif defined(__WINCMOC__) || defined(__CMOC__)
	#define CMOC
	#define NARROW
#elif defined(__SPECTRUM__) || defined(__CPC__) || defined(__VG5K__) || defined(__ZX81__) || defined(__ZX80__) \
      || defined(__AQUARIUS__) || (defined(__SVI__) && !defined(MSX_MODE0)) || defined(__MZ__)
	#define Z88DK
	#define WIDE
#else 
	#define Z88DK
	#define NARROW
#endif

#if defined(FORCE_NARROW) && defined(WIDE)
	#undef WIDE
	#define NARROW
#endif

#if defined(WIDE)
	#define ADVANCED_LEFT_MISSILE()  ++leftEnemyMissile._x
	#define ADVANCED_RIGHT_MISSILE() --rightEnemyMissile._x	
#else
	#define ADVANCED_LEFT_MISSILE()  do{if(loop&1)++leftEnemyMissile._x;}while(0)
	#define ADVANCED_RIGHT_MISSILE() do{if(loop&1)--rightEnemyMissile._x;}while(0)
#endif

#if (defined(__VIC20__) && defined(REDEFINED_CHARS)) || (defined(__VIC20__) && defined(ALT_PRINT)) \
	|| defined(__WINCMOC__) || defined(__CMOC__) || defined(__GAMATE__) \
    || defined(__APPLE2__) || (defined(__C64__) && defined(REDEFINED_CHARS)) || defined(__ATARI__) || defined(__ATARIXL__) \
	|| defined(__ZX80__) || (defined(__C16__) && defined(ALT_PRINT))
	#define NO_CASE_LETTERS
#else
	#define CASE_LETTERS
#endif

#if defined(FORCE_BOMBS_NUMBER)
	#define BOMBS_NUMBER FORCE_BOMBS_NUMBER
#else
	#if defined(__GAMATE__) 
		#define BOMBS_NUMBER 2
	#elif defined(__ZX81__) || defined(__LAMBDA__) 
		#define BOMBS_NUMBER 4
	#elif defined(__SPECTRUM__) && !defined(TINY_GAME)
		#define BOMBS_NUMBER 4
	#elif defined(__PCE__) || (defined(__VIC20__) && defined(TINY_GAME)) || (defined(__SPECTRUM__) && defined(TINY_GAME))
		#define BOMBS_NUMBER 4
	#elif defined(__AQUARIUS__) && defined(TINY_GAME)
		#define BOMBS_NUMBER 4
	#elif defined(__OSIC1P__) && defined(TINY_GAME)
		#define BOMBS_NUMBER 4
	#else
		#define BOMBS_NUMBER 4
	#endif
#endif

// Possible current values are FULL: 8,9, LIGHT: 8, TINY: 8,7,6
// #if !defined(FULL_GAME) && !defined(TINY_GAME)
	// #define GHOSTS_NUMBER 8
	
#if defined(FORCE_GHOSTS_NUMBER)
	#define GHOSTS_NUMBER FORCE_GHOSTS_NUMBER
#elif defined(TURN_BASED)
	#if defined(TINY_GAME)
		#define GHOSTS_NUMBER 8
	#else
		#define GHOSTS_NUMBER 9
	#endif
#else	
	#if defined(__PCE__)
		#define GHOSTS_NUMBER 8
	#elif defined(__SPECTRUM__) && defined(TINY_GAME)
		#define GHOSTS_NUMBER 8	
	#elif defined(__VIC20__) && defined(TINY_GAME) && defined(VIC20_UNEXPANDED)
		#define GHOSTS_NUMBER 6
	#elif defined(__ATARI_LYNX__)
		#define GHOSTS_NUMBER 4
	#elif defined(__VIC20__) && (defined(TINY_GAME) && !defined(REDEFINED_CHARS))
		#define GHOSTS_NUMBER 8
	#elif defined(__VIC20__) && (defined(TINY_GAME) && defined(REDEFINED_CHARS))
		#define GHOSTS_NUMBER 8
	#elif defined(__GAL__) && defined(TINY_GAME) 
		#define GHOSTS_NUMBER 6
	#elif defined(__GAMATE__) 
		#define GHOSTS_NUMBER 4
	#elif defined(__SPECTRUM__) && defined(FULL_GAME)
		#define GHOSTS_NUMBER 9
	#elif defined(__SPECTRUM__) && defined(TINY_GAME)
		#define GHOSTS_NUMBER 8
	#elif defined(__ATMOS__)
		#define GHOSTS_NUMBER 9
	#elif defined(__ZX81__) || defined(__LAMBDA__)
		#define GHOSTS_NUMBER 7
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
#endif
	
// || (defined(__AQUARIUS__) && defined(TINY_GAME)) || (defined(__VIC20__) && defined(VIC20_UNEXPANDED) ) 
#if defined(__GAL__) || defined(__GAMATE__) \
    || defined(__PC6001__) || defined(__ATARI5200__) || defined(__CREATIVISION__) \
	|| defined(__WINCMOC__) || defined(__CMOC__) || defined(__OSIC1P__) || defined(__MC1000__) \
	|| defined(__LAMBDA__) || (defined(__MSX__) && (defined(MSX_MODE1) || defined(MSX_MODE0))) \
	|| (defined(__SVI__) && defined(MSX_MODE0) ) || defined(__VZ__) || defined(__VG5k__) \
	|| ((defined(__ATARI__) || defined(__ATARIXL__))) && !defined(ATARI_MODE1) \
	|| defined(__PET__) || defined(__CBM610__) || defined(__APPLE2__) || defined(__APPLE2ENH__) \
	|| defined(__ZX81__) || defined(__ZX80__) || defined(__ACE__)
	#define NO_COLOR
#else
	#define COLOR
#endif
	
#if defined(__ATARI__) || defined(__ATARIXL__) \
	|| (defined(__VIC20__) && defined(ALT_PRINT))
	#define NO_TEXT_COLOR
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

#define EXTRA_LIFE_THROUGH_POINTS 2000



// #define EXTRA_LIFE_FIRST_LEVEL 10

#if defined(WIDE) || defined(FORCE_NARROW)
	#define MIN_INVINCIBLE_GHOST_HITS 4
#else
	#define MIN_INVINCIBLE_GHOST_HITS 3
#endif	
	
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

#if defined(TURN_BASED)
	#define INITIAL_ENEMY_SLOWDOWN 16000
	#define INITIAL_SKULL_SLOWDOWN 15000
#elif defined(__PC6001__) || defined(__GAMATE__) || defined(__VZ__) || defined(__CBM610__) || defined(__VIC20__) || defined(__ATARI5200__) || ((defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1)) || defined(__APPLE2__) || defined(__APPLE2ENH__)
	#define INITIAL_ENEMY_SLOWDOWN 31500
	#define INITIAL_SKULL_SLOWDOWN 32000	
#elif defined(__VG5K__) || defined(__MSX__) || defined(__WINCMOC__) || defined(__CMOC__)
	#define INITIAL_ENEMY_SLOWDOWN 29000
	#define INITIAL_SKULL_SLOWDOWN 30000	
#else
	#define INITIAL_ENEMY_SLOWDOWN 28000
	#define INITIAL_SKULL_SLOWDOWN 29000
#endif



//#define INVINCIBILITY_FIRST_LEVEL 5

#define FIRST_HORIZONTAL_WALLS_LEVEL 6

#if defined(WIDE)
	#define RELOCATE_RANGE 7
	#define HORIZONTAL_WALLS_INITIAL_LENGTH 2
	#define HORIZONTAL_WALLS_INCREASE_LOOP 250
#else
	#define RELOCATE_RANGE 5
	#define HORIZONTAL_WALLS_INITIAL_LENGTH 1
	#define HORIZONTAL_WALLS_INCREASE_LOOP 350
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

#define TURN_BASED_EFFECT_SCALE 5
#define TURN_BASED_WAIT_SCALE 4
#define ACTION_EFFECT_SCALE 1
#define ACTION_WAIT_SCALE 1

#if defined(TURN_BASED)
	#define EFFECT_SCALE TURN_BASED_EFFECT_SCALE
	#define WAIT_SCALE TURN_BASED_WAIT_SCALE
#else
	#define EFFECT_SCALE ACTION_EFFECT_SCALE
	#define WAIT_SCALE ACTION_WAIT_SCALE
#endif
	
#define POWER_UP_COOL_DOWN (150/WAIT_SCALE)

#define POWER_UP2_COOL_DOWN (400/WAIT_SCALE)

#define SUPER_COOL_DOWN (150/WAIT_SCALE)

#define CONFUSE_COOL_DOWN (250/WAIT_SCALE)

#define ZOMBIE_COOL_DOWN (350/WAIT_SCALE)

#define CHASE_COOL_DOWN (400/WAIT_SCALE)

#define EXTRA_POINTS_COOL_DOWN (1500UL/WAIT_SCALE)

#define INVINCIBILITY_COOL_DOWN (2000UL/WAIT_SCALE)

#define EXTRA_LIFE_COOL_DOWN (2500UL/WAIT_SCALE)

#define FROZEN_COUNT_DOWN (25/EFFECT_SCALE)

#define INITIAL_FROZEN_COUNT_DOWN FROZEN_COUNT_DOWN

#define CONFUSE_COUNT_DOWN (150/EFFECT_SCALE)

#define INVINCIBILITY_COUNT_DOWN (100/EFFECT_SCALE)

#define ZOMBIE_COUNT_DOWN (600/EFFECT_SCALE)


#define ZOMBIE_BONUS 5

#if defined(FULL_GAME)
	#define INVINCIBLE_LOOP_TRIGGER (550-(level<<2))
	#define FREEZE_COOL_DOWN 700
	#define GUN_COOL_DOWN (200/WAIT_SCALE)	
#else
	// #define INVINCIBLE_LOOP_TRIGGER 400
	#define FREEZE_COOL_DOWN 700
	#define GUN_COOL_DOWN (200/WAIT_SCALE)	
#endif	

#define SKULL_COUNT_DOWN 50

#define TURN_BASED_INVINCIBLE_MIN_SLOWDOWN_SCALE 4
#define TURN_BASED_ENEMY_MIN_SLOWDOWN_SCALE 3
#define ACTION_INVINCIBLE_MIN_SLOWDOWN_SCALE 1
#define ACTION_ENEMY_MIN_SLOWDOWN_SCALE 1

#if defined(TURN_BASED)
	#define INVINCIBLE_MIN_SLOWDOWN_SCALE TURN_BASED_INVINCIBLE_MIN_SLOWDOWN_SCALE
	#define ENEMY_MIN_SLOWDOWN_SCALE TURN_BASED_ENEMY_MIN_SLOWDOWN_SCALE
#else
	#define INVINCIBLE_MIN_SLOWDOWN_SCALE ACTION_INVINCIBLE_MIN_SLOWDOWN_SCALE
	#define ENEMY_MIN_SLOWDOWN_SCALE ACTION_ENEMY_MIN_SLOWDOWN_SCALE	
#endif


#define INVINCIBLE_MIN_SLOWDOWN (4000/INVINCIBLE_MIN_SLOWDOWN_SCALE)
#define ENEMY_MIN_SLOWDOWN (1500/ENEMY_MIN_SLOWDOWN_SCALE)	


#define GHOST_LEVEL_DECREASE 50

#define MAX_GHOST_LEVEL 1000

#define MAX_INVINCIBLE_LOOP 1600

#endif // _SETTINGS
