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

#if !defined(_TARGET_SETTINGS)
#define _TARGET_SETTINGS

#if !defined(__SMS__) && !defined(__CMOC__) && !defined(__SUPERVISION__) \
	&& !defined(__ATARI_LYNX__) && !defined(__MSX__) \
    && !(defined(__SVI__) && defined(MSX_MODE0)) && !(defined(__SPECTRUM__) \
	&& !defined(CLIB_ANSI)) && !defined(__ENTERPRISE__) \
	&& !defined(__PX8__) && !defined(__KC__) \
	&& !defined(__EG2K__) && !defined(__TRS80__) \
	&& !defined(__ABC800__) \
	&& !(defined(__KC__) && defined(Z88DK_SPRITES)) \
	&& !(defined(__NC100__) && defined(Z88DK_SPRITES)) \
	&& !defined(__CPM_80X24__) \
	&& !defined(__EINSTEIN__) \
	&& !defined(__SRR__) \
	&& !defined(__NCURSES__)
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


#if defined(TURN_BASED) || defined(TINY_GAME)
	#if !defined(NO_BLINKING)
		#define NO_BLINKING
	#endif
#endif

#if defined(TURN_BASED)
	#define SKIP_DRAW 
	#define SKIP_MORE_DRAW
#elif defined(__ZX81__) || defined(__LAMBDA__)
	#define SKIP_DRAW \
		if((loop&1)==1)
	
	#define SKIP_MORE_DRAW \
		if((loop&15)==1)	
#elif defined(__C128_Z80__)
	#define SKIP_DRAW \
		if(loop&1)
	
	#define SKIP_MORE_DRAW \
		if((loop&15)==1)	
						
#elif defined(__SVI__) && !defined(MSX_MODE0)
	#define SKIP_DRAW \
		if((loop&1)==1) 
	
	#define SKIP_MORE_DRAW \
		if((loop&7)==1)		
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
	|| defined(__Z1013__) || defined(__TRS80__) || defined(__EG2K__) || defined(__KC__) || defined(__C128_Z80__) \
	|| defined(__EINSTEIN__) || defined(__PPS__)
	#define _MOVE_UP 'I'
	#define _MOVE_DOWN 'K'
	#define _MOVE_LEFT 'J'
	#define _MOVE_RIGHT 'L'
	#define _FIRE ' '
#elif defined(__CPC_JOYSTICK__) // Special Joystick 
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
#elif defined(__TI82__) || defined(__TI83__) || defined(__TI8X__) || defined(__TI85__) || defined(__TI86__)
	#define _MOVE_UP '8'
	#define _MOVE_DOWN '5'
	#define _MOVE_LEFT '4'
	#define _MOVE_RIGHT '6'
	#define _FIRE '1'			
#else
	#define _MOVE_UP 'i'
	#define _MOVE_DOWN 'k'
	#define _MOVE_LEFT 'j'
	#define _MOVE_RIGHT 'l'
	#define _FIRE ' '		
#endif

#if defined(TURN_BASED)
	#if defined(__ZX80__) || defined(__C128_Z80__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 0
	#else
		#define GAME_SLOW_DOWN 0
	#endif
#else	
	#  if defined(__NCURSES__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 100
	#elif defined(__SUPERVISION__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 3300			
	#elif defined(__MTX__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 3300			
	#elif defined(__MC1000__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 900			
	#elif defined(__VIC20__) && defined(ALT_PRINT)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 500	
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
		#define GAME_SLOW_DOWN 250
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
		#define GAME_SLOW_DOWN 3600	
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
		#define GAME_SLOW_DOWN 1500
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
	#elif defined(__M5__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 200		
	#else
		#define GAME_SLOW_DOWN 0
	#endif
#endif

#if defined(__VIC20__) || defined(__CREATIVISION__) \
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


#if defined(__GAL__) || defined(__GAMATE__) \
    || defined(__PC6001__) || defined(__ATARI5200__) || defined(__CREATIVISION__) \
	|| defined(__WINCMOC__) || defined(__CMOC__) || defined(__OSIC1P__) || defined(__MC1000__) \
	|| defined(__LAMBDA__) || defined(__MSX__) \
	|| (defined(__SVI__) && defined(MSX_MODE0) ) || defined(__VZ__) || defined(__VG5k__) \
	|| ((defined(__ATARI__) || defined(__ATARIXL__))) && !defined(ATARI_MODE1) \
	|| defined(__PET__) || defined(__CBM610__) || defined(__APPLE2__) || defined(__APPLE2ENH__) \
	|| defined(__ZX81__) || defined(__ZX80__) || defined(__ACE__) \
	|| defined(__TRS80__) || defined(__CPM_80X24__) || defined(__PX8__) || defined(__PX4__) \
	|| defined(__ABC800__) || defined(__SRR__) || defined(__PV1000__) || defined(__PV2000__)
	#define NO_COLOR 
#else
	#define COLOR
#endif
	
#if defined(__ATARI__) || defined(__ATARIXL__) \
	|| defined(ALT_PRINT)
	#define NO_TEXT_COLOR
#endif


#define BACKGROUND_COLOR COLOR_BLACK

#define BORDER_COLOR COLOR_BLACK

#if defined(__CPC__) || defined(__GAMATE__)
	#define TEXT_COLOR 3
#else
	#define TEXT_COLOR COLOR_WHITE
#endif

// Directions
#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3


#endif // _TARGET_SETTINGS
