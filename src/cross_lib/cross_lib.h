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

#if !defined(NO_GRAPHICS)
#include "standard_libs.h"
#endif

#if !defined(_TARGET_SETTINGS)
#define _TARGET_SETTINGS

#if !defined(__SMS__) && !defined(__COCO__) && !defined(__DRAGON__) && !defined(__SUPERVISION__) \
	&& !defined(__ATARI_LYNX__) && !defined(__MSX__) \
    && !defined(__SVI__) \
	&& !defined(__ENTERPRISE__) \
	&& !defined(__ABC800__) \
	&& !(defined(__KC__) && defined(Z88DK_SPRITES)) \
	&& !(defined(__OSBORNE1__) && defined(Z88DK_SPRITES)) \
	&& !defined(__CPM_80X24__) \
	&& !defined(__EINSTEIN__) \
	&& !defined(__SRR__) \
	&& !defined(__NCURSES__) \
	&& !defined(__MZ2500__) \
	&& !defined(__VECTREX__) \
	&& !defined(ACK) \
	&& !defined(FORCE_NO_CONIO_LIB) \
	&& !defined(__GCC_BUFFERED__)
	#define CONIO_LIB
#endif
#if defined(__WINCMOC__) || defined(FORCE_CONIO_LIB)
	#define CONIO_LIB
#endif


#if defined(__WINCMOC__)
	#define ASM_KEY_DETECT
#endif


#if defined(TURN_BASED) || defined(TINY_GAME)
	#if !defined(NO_BLINKING)
		#define NO_BLINKING
	#endif
#endif


#  if (defined(__CBM__) && !defined(__VIC20__)) || defined(__APPLE2__) || defined(__APPLE2ENH__) || defined(__ATMOS__) \
    || defined(__ATARI__) \
	|| defined(__VIC20__) || defined(__GAMATE__) \
    || defined(__ATARI5200__) || defined(__OSIC1P__) || defined(__SUPERVISION__) \
	|| defined(__TELESTRAT__) || defined(__PCE__) || defined(__NES__) || defined(__CREATIVISION__) || defined(__ATARI_LYNX__) \
    || defined(__CX16__)
	#define CC65
#elif defined(__WINCMOC__) || defined(__CMOC__)
	#define CMOC
#elif defined(ACK) || defined(__NCURSES__) || defined(__GCC_BUFFERED__) || defined(__LCC1802__)
	//
#else
	#define Z88DK
#endif


// TEXT SETTINGS
#if defined(__VIC20__) || (defined(__VIC20__) && defined(ALT_PRINT)) \
	|| defined(__WINCMOC__) || defined(__COCO__) || defined(__DRAGON__) || defined(__MO5__) || defined(__TO7__) || defined(__GAMATE__) \
    || defined(__APPLE2__) || defined(__C64__) || defined(__ATARI__)  \
	|| defined(__ZX80__) || (defined(__C16__) && defined(ALT_PRINT))
	#define ONLY_SMALL_LETTERS
#elif defined(__PET__) || defined(__LCC1802__) || defined(FEWER_SPRITES)
	#define ONLY_CAPITAL_LETTERS
#else
	#define SMALL_AND_CAPITAL_LETTERS
#endif

#if defined(__GAMATE__)
	#define TEXT_COLOR 3
#elif defined(__PC6001__) && FORCE_SCREEN_MODE==2
	#define TEXT_COLOR COLOR_YELLOW
#elif defined(__SPC1000__) 
	#define TEXT_COLOR COLOR_BLUE
#else
	#define TEXT_COLOR COLOR_WHITE
#endif


#include "input_target_settings.h"
#include "input_macros.h"

#include "display_target_color.h"

#include "sound_macros.h"

#include "sleep_macros.h"

#include "text_macros.h"

#include "graphics_settings.h"

#include "display_macros.h"


#if !defined(__LCC1802__)
    #include "rand.h"
#endif

#endif // _TARGET_SETTINGS


