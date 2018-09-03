#ifndef _DISPLAY_TARGET_SETTINGS
#define _DISPLAY_TARGET_SETTINGS

#include "../cross_lib.h"

#include "display_target_geometry.h"


#if defined(__GAL__) || defined(__GAMATE__) \
    || (defined(__PC6001__) && !(FORCE_SCREEN_MODE==2)) || defined(__ATARI5200__) || defined(__CREATIVISION__) \
	|| defined(__WINCMOC__) || defined(__COCO__) || defined(__OSIC1P__) || defined(__MC1000__) \
	|| defined(__LAMBDA__) || defined(__MSX__) \
	|| defined(__VZ__) || defined(__VG5k__) \
	|| (defined(__ATARI__) || defined(__ATARIXL__)) \
	|| defined(__PET__) || defined(__CBM610__) || defined(__APPLE2__) || defined(__APPLE2ENH__) \
	|| defined(__ZX81__) || defined(__ZX80__) || defined(__ACE__) \
	|| defined(__TRS80__) || defined(__CPM_80X24__) || defined(__PX8__) || defined(__PX4__) \
	|| defined(__ABC800__) || defined(__SRR__) \
	|| defined(__KC__) || defined(__ATMOS__) || defined(__SUPERVISION__) \
	|| defined(__VECTREX__)
	#define NO_COLOR 
#else
	#define COLOR
#endif


#if !defined(__ATMOS__)
	#if defined(__ATARI__) || defined(__ATARIXL__) \
		|| (defined(ALT_PRINT) && !defined(__NCURSES__)) \
		|| defined(NO_COLOR)
		#define NO_TEXT_COLOR
	#endif
#endif

#define BACKGROUND_COLOR COLOR_BLACK

#define BORDER_COLOR COLOR_BLACK


#endif // _DISPLAY_TARGET_SETTINGS
