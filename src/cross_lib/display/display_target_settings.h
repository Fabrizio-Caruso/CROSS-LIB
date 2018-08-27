#ifndef _DISPLAY_TARGET_SETTINGS
#define _DISPLAY_TARGET_SETTINGS

#include "../cross_lib.h"


#if defined(Z88DK_SPRITES)
	#if defined(__NC100__)
		#define SPRITE_X_SIZE 8
		#define SPRITE_X_STEP 6
		#define SPRITE_Y_SIZE 7
		#define SPRITE_Y_STEP 5
	#else
		#define SPRITE_X_SIZE 8
		#define SPRITE_X_STEP 6	
		#define SPRITE_Y_SIZE 8
		#define SPRITE_Y_STEP 6		
	#endif
	
	#ifndef SPRITE_X_STEP
		#define SPRITE_X_STEP SPRITE_X_SIZE
	#endif
	
	#ifndef SPRITE_Y_STEP
		#define SPRITE_Y_STEP SPRITE_Y_SIZE
	#endif
#endif


#if !defined(FORCE_YSIZE)
	#  if defined(__NCURSES__)
		#define YSize (24-Y_OFFSET)
	#elif defined(__PPS__)
		#define YSize (32-Y_OFFSET)
	#elif defined(__Z1013__)
		  #define YSize (32-Y_OFFSET)
	#elif defined(__SRR__)
		  #define YSize (30-Y_OFFSET)
	#elif defined(__KC__) && defined(Z88DK_SPRITES)
		  #define YSize ((256/SPRITE_Y_STEP)-Y_OFFSET)	
	#elif defined(__KC__) && defined(UDG_GRAPHICS)
		  #define YSize (32-Y_OFFSET)
	#elif defined(__ATMOS__) || defined(__NES__)  
		#define YSize (28-Y_OFFSET) 		  
	#elif defined(__CBM610__) || defined(__PET__) || defined(__C128__) \
		  || defined(__C16__) || defined(__PLUS4__) || defined(__C64__) \
		  || defined(__PET__) || defined(__CBM510__) \
		  || defined(__MZ__) || defined(__BEE__) || defined(__X1__) \
		  || defined(__PCE__) || defined(__CPC__) || defined(__VG5K__) || defined(__ABC80__) \
		  || defined(__ENTERPRISE__) 
		#define YSize (25-Y_OFFSET)
	#elif defined(__CREATIVISION__) || defined(__MSX__) || defined(__SVI__) \
		|| defined(__ZX81__) || defined(__ZX80__) || defined(__LAMBDA__) \
		|| defined(__SPECTRUM__) || defined(__SC3000__) || defined(__MTX__) \
		|| defined(__APPLE2ENH__) || defined(__APPLE2__) \
		|| defined(__Z9001__) || defined(__P2000__) \
		|| (defined(__ATARI__) || defined(__ATARIXL__)) \
		|| defined(__ATARI5200__) \
		|| defined(__ACE__) \
		|| defined(__CPM_80X24__) \
		|| defined(__EINSTEIN__) \
		|| defined(__OSIC1P__) \
		|| defined(__MC1000__) \
		|| defined(__M5__) \
		|| defined(__AQUARIUS__) \
		|| (defined(__EG2K__) && defined(UDG_GRAPHICS)) \
		|| defined(__COLECO__) \
		|| defined(__PV2000__) \
		|| defined(__MULTI8__)
		#define YSize (24-Y_OFFSET)
	#elif defined(__VIC20__)
		#define YSize 23
	#elif defined(__SAM__)
		#define YSize (24-2-Y_OFFSET)
	#elif (defined(__NC200__) && defined(Z88DK_SPRITES))
		#define YSize ((128/SPRITE_Y_STEP)+1)
	#elif defined(__SUPERVISION__) 
		#define YSize 20
	#elif defined(__GAMATE__)
		#define YSize 19	
	#elif defined(__ATARI_LYNX__)
		#define YSize 13
	#elif (defined(__PX4__) && defined(Z88DK_PUTC4X6))
		#define YSize (10+1)
	#elif defined(__Z88__) || (defined(__PX4__) && !defined(Z88DK_PUTC4X6))|| defined(__PX8__) \
		|| defined(__TI82__) || defined(__TI83__) || defined(__TI8X__) || defined(__TI85__) || defined(__TI86__)
		#define YSize 8
	#elif (defined(__NC100__) && defined(Z88DK_SPRITES)) 
		#define YSize ((64/SPRITE_Y_STEP)+1)
	#elif defined(__PC6001__) && FORCE_SCREEN_MODE==2
		#define YSize 24
	#else
		#define YSize 16
	#endif
#else
	#define YSize FORCE_YSIZE
#endif


#if !defined(FORCE_XSIZE)
	#  if defined(__NCURSES__)
		#define XSize 80
	#elif defined(__CBM610__) || defined(__PET__) || (defined(__C128__) && defined(C128_80COL_VIDEO_MODE)) \
		  || defined(__BEE__) || defined(__PET__) || defined(__CBM610__) \
		  || defined(__PX8__) || defined(__CPM_80X24__) \
		  || (defined(__APPLE2ENH__) && defined(APPLE2ENH_80COL_VIDEO_MODE)) \
		  || defined(__PPS__) || (defined(__MULTI8__) && defined(UDG_GRAPHICS))
		#define XSize 80
	#elif (defined(__TRS80__) && !defined(__EG2K__)) \
		  || (defined(__EG2K__) && !defined(UDG_GRAPHICS)) \
		  || defined(__SRR__)
		#define XSize 64
	#elif defined(__PCE__) || (defined(__PX4__) && defined(Z88DK_PUTC4X6))
		 #define XSize 60
	#elif ((defined(__NC100__) || defined(__NC200__)) && defined(Z88DK_SPRITES))
		#define XSize (480/SPRITE_X_STEP)
	#elif defined(__NASCOM__)
		#define XSize 48
	#elif defined(__VG5K__) || (defined(__APPLE2ENH__) && !defined(APPLE2ENH_80COL_VIDEO_MODE))|| defined(__APPLE2__) \
		  || defined(__Z9001__) || defined(__P2000__) || defined(__ABC80__) \
		  || defined(__MZ__) || defined(__X1__) || defined(__ATMOS__) \
		  || defined(__CPC__) || defined(__C16__) || defined(__PLUS4__) || defined(__C64__) \
		  || (defined(__C128__) && !defined(C128_80COL_VIDEO_MODE)) \
		  || defined(__AQUARIUS__) || (defined(__SVI__) && defined(MSX_MODE0)) \
		  || defined(__ENTERPRISE__) \
		  || (defined(__PX4__) && !defined(Z88DK_PUTC4X6)) \
		  || (defined(__ATARI__) && !defined(ATARI_MODE1)) \
		  || defined(__CBM510__) \
		  || (defined(__EG2K__) && defined(UDG_GRAPHICS))
		#define XSize (40-X_OFFSET)
	#elif defined(__KC__) && defined(Z88DK_SPRITES)
		#define XSize (320/SPRITE_X_STEP)
	#elif defined(__KC__) && defined(UDG_GRAPHICS)
		#define XSize 40
	#elif defined(__VZ__) || defined(__NES__) || defined(__CREATIVISION__) || defined(__MSX__) \
		  || (defined(__SVI__) && !defined(MSX_MODE0) ) || defined(__ZX81__) || defined(__ZX80__) \
		  || defined(__LAMBDA__) || defined(__SPECTRUM__) || (defined(__PC6001__) && !defined(FORCE_SCREEN_MODE))\
		  || defined(__SC3000__) || defined(__MC1000__) || defined(__MTX__) || defined(__SAM__) \
		  || defined(__GAL__) || defined(__CMOC__) || defined(__WINCMOC__) \
		  || defined(__Z1013__) || defined(__Z88__) || defined(__ACE__) \
		  || defined(__EINSTEIN__) \
		  || defined(__TI82__) || defined(__TI83__) || defined(__TI8X__) || defined(__TI85__) || defined(__TI86__) \
		  || defined(__M5__) || defined(__COLECO__) || defined(__PV2000__)
		#define XSize 32	
	#elif defined(__OSIC1P__)
		#define XSize 24
	#elif defined(__VIC20__)
		#define XSize 22
	#elif (defined(__ATARI__) && defined(ATARI_MODE1)) \
		  || defined(__ATARI_LYNX__) || defined(__SUPERVISION__) \
		  || defined(__ATARI5200__) || defined(__GAMATE__)
		#define XSize 20
	#elif defined(__PC6001__) && FORCE_SCREEN_MODE==2
		#define XSize 16
	#else
		#define XSize 16
	#endif
#else
	#define XSize FORCE_XSIZE
#endif

#if XSize < YSize
	#define MIN_SIZE XSize
#else
	#define MIN_SIZE YSize
#endif

#  if XSize>=32 && YSize>=20
	#define WIDE
#else 
	#define NARROW
#endif


#if defined(FORCE_NARROW) && defined(WIDE)
	#undef WIDE
	#define NARROW
#endif


#if defined(NARROW) || defined(TINY_GAME)
	#define Y_OFFSET 0
#else
	#define Y_OFFSET 2		
#endif

#if defined(__ATMOS__)
	#define X_OFFSET 2
#else
	#define X_OFFSET 0
#endif



#if defined(__GAL__) || defined(__GAMATE__) \
    || (defined(__PC6001__) && !(FORCE_SCREEN_MODE==2)) || defined(__ATARI5200__) || defined(__CREATIVISION__) \
	|| defined(__WINCMOC__) || defined(__CMOC__) || defined(__OSIC1P__) || defined(__MC1000__) \
	|| defined(__LAMBDA__) || defined(__MSX__) \
	|| defined(__VZ__) || defined(__VG5k__) \
	|| (defined(__ATARI__) || defined(__ATARIXL__)) \
	|| defined(__PET__) || defined(__CBM610__) || defined(__APPLE2__) || defined(__APPLE2ENH__) \
	|| defined(__ZX81__) || defined(__ZX80__) || defined(__ACE__) \
	|| defined(__TRS80__) || defined(__CPM_80X24__) || defined(__PX8__) || defined(__PX4__) \
	|| defined(__ABC800__) || defined(__SRR__) \
	|| defined(__KC__) || defined(__ATMOS__) || defined(__SUPERVISION__)
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
