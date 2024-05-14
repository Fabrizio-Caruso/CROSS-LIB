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

#ifndef _CROSS_LIB_H
#define _CROSS_LIB_H

#if !defined(__NO_GRAPHICS)
#include "standard_libs.h"
#endif

#if !defined EXIT_SUCCESS
    #define EXIT_SUCCESS 0
#endif

#if !defined(_TARGET_SETTINGS)
#define _TARGET_SETTINGS

#if !defined(__COCO3__) && !defined(__COCO__) && !defined(__DRAGON__) && !defined(__MO5__) && !defined(__TO7__) \
    && !defined(__SUPERVISION__) && !defined(__MC10__) && !defined(__LCC1802__) && !defined(__ATARI_LYNX__) \
    && !(defined(__MSX__) && defined(__MEMORY_MAPPED_GRAPHICS)) \
    && !defined(__SVI__) \
	&& !defined(__ENTERPRISE__) \
	&& !defined(__ABC800__) \
	&& !(defined(__KC__) && defined(__Z88DK_SPRITES_GRAPHICS)) \
	&& !(defined(__OSBORNE1__) && defined(__Z88DK_SPRITES_GRAPHICS)) \
	&& !defined(__EINSTEIN__) \
	&& !defined(__SRR__) \
	&& !defined(__NCURSES__) \
	&& !defined(__MZ2500__) \
	&& !defined(__VECTREX__) \
	&& !defined(ACK) \
	&& !defined(__STDIO)
	#define __INCLUDE_CONIO_H
#endif


#if !defined(__BACKGROUND_COLOR)
    #define __BACKGROUND_COLOR 0
#endif

// This may be different on each target
#if !defined(_XL_NUMBER_OF_TILES)
    #define _XL_NUMBER_OF_TILES 27
#endif


#if defined(ACK) || defined(__NCURSES__) || defined(__LCC1802__)
	//
#elif defined(__TI99__)
    //
#elif defined(__CC6303__)
    //
#elif defined(__SCCZ80) || defined(__SDCC)
	#define __Z88DK__
#else 
	//
#endif


#if !defined(_XL_NO_SMALL_LETTERS)
    #  if defined(__COCO3__) || defined(__COCO__) || defined(__DRAGON__) || (defined(__C64__) && defined(__MEMORY_MAPPED_GRAPHICS)) \
        || (defined(__C16__) && defined(__ALT_PRINT))  || (defined(__ATARI__) && !defined(_XL_NO_COLOR)) \
        || defined(__LCC1802__) || defined(__FEWER_SPRITES) || defined(__SUPERVISION__) || (defined(__MSX__)&&defined(__MEMORY_MAPPED_GRAPHICS)) \
        || defined(__ZX80__) || defined(__ZX81__) \
        || (defined(__APPLE2__) && defined(__APPLE2_HGR_GRAPHICS)) || defined(__VIC20__) \
        || (defined(__APPLE2__) && !defined(__APPLE2ENH__) && !defined(__APPLE2_HGR_GRAPHICS)) \
        || (defined(__ATARI_LYNX__) && defined(__LYNX_TGI_GRAPHICS)) || defined(__MC10__) || defined(__TI99__) \
        || (defined(__ATARI7800__) &&  !defined(_XL_NO_COLOR))
        #define _XL_NO_SMALL_LETTERS
    #endif  
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
#elif (defined(__MC10__) && defined(__BIT_MAPPED_4_GRAPHICS))
    #define _XL_TILE_Y_SIZE 6
#else
    #define _XL_TILE_Y_SIZE 8
#endif


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

#endif // _CROSS_LIB_H
