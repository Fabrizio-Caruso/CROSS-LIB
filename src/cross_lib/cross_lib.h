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

#if !defined EXIT_SUCCESS
    #define EXIT_SUCCESS 0
#endif

#if !defined(_TARGET_SETTINGS)
#define _TARGET_SETTINGS

#if !defined(__SMS__) && !defined(__COCO__) && !defined(__DRAGON__) && !defined(__SUPERVISION__) \
	&& !defined(__ATARI_LYNX__) && !(defined(__MSX__) && defined(MEMORY_MAPPED)) \
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


// This may be different on each target
#if !defined(_XL_NUMBER_OF_TILES)
    #define _XL_NUMBER_OF_TILES 27
#endif


#  if (defined(__CBM__) && !defined(__VIC20__)) || defined(__APPLE2__) || defined(__APPLE2ENH__) || defined(__ATMOS__) \
    || defined(__ATARI__) \
	|| defined(__VIC20__) || defined(__GAMATE__) \
    || defined(__ATARI5200__) || defined(__ATARI7800__) || defined(__OSIC1P__) || defined(__SUPERVISION__) \
	|| defined(__TELESTRAT__) || defined(__PCE__) || defined(__NES__) || defined(__CREATIVISION__) || defined(__ATARI_LYNX__) \
    || defined(__CX16__)
	#define CC65
#elif defined(__WINCMOC__) || defined(__CMOC__)
	#define CMOC
#elif defined(ACK) || defined(__NCURSES__) || defined(__GCC_BUFFERED__) || defined(__LCC1802__)
	//
#elif defined(__TI99__)
    //
#elif defined(__CC6303__)
    //
#else
	#define Z88DK
#endif


#  if defined(__COCO__) || defined(__DRAGON__) || (defined(__C64__) && defined(MEMORY_MAPPED)) \
	|| (defined(__C16__) && defined(ALT_PRINT))  || (defined(__ATARI__) && !defined(_XL_NO_COLOR)) \
	|| defined(__LCC1802__) || defined(FEWER_SPRITES) || defined(__SUPERVISION__) || (defined(__MSX__)&&defined(MEMORY_MAPPED)) \
	|| defined(__ZX80__) || defined(__ZX81__) \
	|| (defined(__APPLE2__) && defined(APPLE2_HGR)) || defined(__VIC20__) \
	|| (defined(__APPLE2__) && !defined(__APPLE2ENH__) && !defined(APPLE2_HGR)) \
	|| (defined(__ATARI_LYNX__) && defined(TGI_GFX)) || defined(__MC10__) || defined(__TI99__) \
	|| (defined(__ATARI7800__) &&  !defined(_XL_NO_COLOR))
	#define _XL_NO_SMALL_LETTERS

#else
	
	#define SMALL_AND_CAPITAL_LETTERS

#endif  


#if defined(_XL_NO_UDG)
    #define _XL_TILE_X_SIZE 1
#elif defined(__ORIC__) || defined(__ATMOS__) \
    || defined(__COMX__) || defined(__PECOM__) || defined(__MICRO__)
    #define _XL_TILE_X_SIZE 6
#elif defined(__APPLE2__) || defined(__APPLE2__)
    #define _XL_TILE_X_SIZE 7
#else
    #define _XL_TILE_X_SIZE 8
#endif


#if defined(_XL_NO_UDG)
    #define _XL_TILE_Y_SIZE 1
#elif (defined(__COMX__) || defined(__PECOM__) || defined(__MICRO__)) && !defined(NTSC)
    #define _XL_TILE_Y_SIZE 9
#elif (defined(__MC10__) && defined(BIT_MAPPED_4))
    #define _XL_TILE_Y_SIZE 6
#else
    #define _XL_TILE_Y_SIZE 8
#endif


/*
#if defined(__GAMATE__)
	#define TEXT_COLOR 3
#elif defined(__PC6001__) && FORCE_SCREEN_MODE==2
	#define TEXT_COLOR _XL_YELLOW
#elif defined(__SPC1000__) 
	#define TEXT_COLOR _XL_BLUE
#elif defined(Z88DK) && !defined(_BACKGROUND_COLOR) && _BACKGROUND_COLOR==_XL_WHITE
	#define TEXT_COLOR BLACK    
#elif defined(Z88DK)
	#define TEXT_COLOR WHITE
#else
	#define TEXT_COLOR _XL_WHITE
#endif
*/

#include "input_target_settings.h"
#include "input_macros.h"

#include "display_target_color.h"

#include "color_definitions.h"

#include "sound_macros.h"

#include "sleep_macros.h"

#include "letters.h"

#include "graphics_settings.h"

#include "display_macros.h"

#include "tiles.h"

#if !defined(__LCC1802__)
    #include "rand.h"
#else
    #define _XL_RAND() rand()
#endif

#endif // _TARGET_SETTINGS


