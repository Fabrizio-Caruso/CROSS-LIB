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
    && !(defined(__SVI__) && defined(MSX_MODE0)) \
	&& !(defined(__SPECTRUM__) && !defined(CLIB_ANSI)) \
	&& !defined(__ENTERPRISE__) \
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
	// #define ANIMATE_PLAYER
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


#  if (defined(__CBM__) && !defined(__VIC20__)) || defined(__APPLE2__) || defined(__APPLE2ENH__) || defined(__ATMOS__) \
    || ((defined(__ATARI__) || defined(__ATARIXL__)) && !defined(ATARI_MODE1)) \
	|| defined(__VIC20__) || defined(__GAMATE__) || ((defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1)) \
    || defined(__ATARI5200__) || defined(__OSIC1P__)
	#define CC65
#elif defined(__WINCMOC__) || defined(__CMOC__)
	#define CMOC
#else
	#define Z88DK
#endif


// #if (defined(__CBM__) && !defined(__VIC20__)) || defined(__APPLE2__) || defined(__APPLE2ENH__) || defined(__ATMOS__) \
    // || ((defined(__ATARI__) || defined(__ATARIXL__)) && !defined(ATARI_MODE1)) 
	// #define CC65
	// #define WIDE
// #elif defined(__VIC20__) || defined(__GAMATE__) || ((defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1)) \
      // || defined(__ATARI5200__) || defined(__OSIC1P__)
	// #define CC65
	// #define NARROW
// #elif defined(__WINCMOC__) || defined(__CMOC__)
	// #define CMOC
	// #define NARROW
// #elif defined(__SPECTRUM__) || defined(__CPC__) || defined(__VG5K__) || defined(__ZX81__) || defined(__ZX80__) \
      // || defined(__AQUARIUS__) || defined(__SVI__) || defined(__MZ__)
	// #define Z88DK
	// #define WIDE
// #else 
	// #define Z88DK
	// #define NARROW
// #endif

#if defined(FORCE_NARROW) && defined(WIDE)
	#undef WIDE
	#define NARROW
#endif


// TEXT SETTINGS
#if (defined(__VIC20__) && defined(ANIMATE_PLAYER)) || (defined(__VIC20__) && defined(ALT_PRINT)) \
	|| defined(__WINCMOC__) || defined(__CMOC__) || defined(__GAMATE__) \
    || defined(__APPLE2__) || (defined(__C64__) && defined(ANIMATE_PLAYER)) || defined(__ATARI__) || defined(__ATARIXL__) \
	|| defined(__ZX80__) || (defined(__C16__) && defined(ALT_PRINT))
	#define NO_CASE_LETTERS
#else
	#define CASE_LETTERS
#endif

#if defined(__CPC__) || defined(__GAMATE__)
	#define TEXT_COLOR 3
#else
	#define TEXT_COLOR COLOR_WHITE
#endif

#include "input/input_target_settings.h"
#include "input/input_macros.h"

#include "display/display_target_settings.h"
#include "display/display_macros.h"

#include "sound/sound_macros.h"

#include "sleep/sleep_macros.h"

#include "text/text_macros.h"

#endif // _TARGET_SETTINGS


