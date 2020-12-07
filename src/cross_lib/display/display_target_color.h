#ifndef _DISPLAY_TARGET_COLOR
#define _DISPLAY_TARGET_COLOR

#include "cross_lib.h"

#include "display_target_geometry.h"


#if defined(__GAL__) || defined(__GAMATE__) \
    || (defined(__PC6001__) && !(FORCE_SCREEN_MODE==2)) || (defined(__ATARI5200__) && !defined(ATARI_MODE_1_COLOR)) || defined(__CREATIVISION__) \
	|| defined(__WINCMOC__) || defined(__COCO__) || defined(__DRAGON__) || defined(__OSIC1P__) || defined(__MC1000__) \
	|| defined(__LAMBDA__) || defined(__MSX__) \
	|| defined(__VZ__) || defined(__VG5k__) \
	|| (defined(__ATARI__) && !defined(ATARI_MODE_1_COLOR)) \
	|| defined(__PET__) || defined(__CBM610__) || defined(__APPLE2__) || defined(__APPLE2ENH__) \
	|| defined(__ZX81__) || defined(__ZX80__) || defined(__ACE__) \
	|| defined(__TRS80__) || defined(__CPM_80X24__) || defined(__PX8__) || defined(__PX4__) \
	|| defined(__ABC80__) || defined(__ABC800__) || defined(__SRR__) \
	|| defined(__KC__) || defined(__ATMOS__) || defined(__SUPERVISION__) \
	|| defined(__VECTREX__) \
	|| defined(__NC100__) || defined(__NC200__)
	#if !defined(NO_COLOR)
		#define NO_COLOR 
	#endif
#else
	#define COLOR
#endif


#if !defined(__ATMOS__)
	#if defined(__ATARI__) \
		|| (defined(ALT_PRINT) && !defined(__NCURSES__) && !defined(__LYNX__) && !defined(__C64__) && !defined(C16_UNEXPANDED) && !defined(VIC20_EXP_8K) && !defined(__TMC600__)) \
		|| defined(NO_COLOR)
		#define NO_TEXT_COLOR
    #else
        //
	#endif
#endif

#define BACKGROUND_COLOR COLOR_BLACK

#define BORDER_COLOR COLOR_BLACK


#endif // _DISPLAY_TARGET_COLOR
