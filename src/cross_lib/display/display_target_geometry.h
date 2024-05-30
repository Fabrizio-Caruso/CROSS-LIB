#ifndef DISPLAY_TARGET_GEOMETRY_H
#define DISPLAY_TARGET_GEOMETRY_H

#if defined(__Z88DK_SPRITES_GRAPHICS)

#if (defined(__TRS80__) && !defined(__EG2K__) && defined(__Z88DK_SPRITES_GRAPHICS)) \
	|| (defined(__OSBORNE1__) && defined(__Z88DK_SPRITES_GRAPHICS))
	#define EXTRA_TINY_Y
	#define EXTRA_TINY_X
#endif
#if defined(__TI82__) && defined(__Z88DK_SPRITES_GRAPHICS)
	#define EXTRA_TINY_X
#endif

#if !defined(__SPRITE_Y_STEP)
	#if !defined(EXTRA_TINY_Y)
		#define __SPRITE_Y_STEP 5	
	#else
		#define __SPRITE_Y_STEP 3
	#endif
#endif

#if !defined(__SPRITE_X_STEP)
	#if !defined(EXTRA_TINY_X)
		#define __SPRITE_X_STEP 7	
	#else
		#define __SPRITE_X_STEP 6
	#endif	
#endif

#if !defined(SPRITE_X_SIZE)
	#define SPRITE_X_SIZE 8
#endif

#if !defined(SPRITE_Y_SIZE)
	#define SPRITE_Y_SIZE 8
#endif

		
	#ifndef __SPRITE_X_STEP
		#define __SPRITE_X_STEP SPRITE_X_SIZE
	#endif
	
	#ifndef __SPRITE_Y_STEP
		#define __SPRITE_Y_STEP SPRITE_Y_SIZE
	#endif
#endif


#if !defined(YSize)
	#  if defined(__NCURSES__)
		#define YSize (24-Y_OFFSET)
	#elif defined(__CIDELSA__) // DRACO/DESTROYER/ALTAIR
		#define YSize (40-Y_OFFSET)
	#elif defined(__PPS__)
		#define YSize (32-Y_OFFSET)
	#elif defined(__Z1013__)
		  #define YSize (32-Y_OFFSET)
	#elif defined(__SRR__)
		  #define YSize (30-Y_OFFSET)
	#elif defined(__KC__) && defined(__Z88DK_SPRITES_GRAPHICS)
		  #define YSize ((256/__SPRITE_Y_STEP)-Y_OFFSET)	
	#elif (defined(__KC__) && !defined(_XL_NO_UDG)) \
		  || defined(__CAMPUTERS_LYNX__)
		  #define YSize (32-Y_OFFSET)		  
	#elif defined(__ATMOS__) || defined(__NES__) 
		#define YSize (28-Y_OFFSET)	  
	#elif defined(__CBM610__) || defined(__PET__) || (defined(__C128__) && !defined(__Z88DK_SPRITES_GRAPHICS)) \
		  || defined(__C16__) || defined(__PLUS4__) || defined(__C64__) \
		  || defined(__PET__) || defined(__CBM510__) \
		  || defined(__MZ__) || defined(__BEE__) || defined(__X1__) \
		  || defined(__PCE__) || defined(__CPC__) || defined(__VG5K__)  \
		  || defined(__ENTERPRISE__) \
		  || defined(__FP1100__) \
		  || defined(__MULTI8__) \
		  || defined(__MO5__) || defined(__TO7__)
		#define YSize (25-Y_OFFSET)
	#elif defined(__CREATIVISION__) || defined(__MSX__) || defined(__SVI__) \
		|| defined(__ZX81__) || defined(__ZX80__) || defined(__LAMBDA__) \
		|| (defined(__SPECTRUM__) && !defined(__TS2068__)) || defined(__SC3000__) || defined(__MTX__) \
		|| defined(__APPLE2ENH__) || defined(__APPLE2__) \
		|| defined(__Z9001__) || defined(__P2000__) \
		|| defined(__ATARI__) \
		|| defined(__ATARI5200__) \
		|| defined(__ACE__) \
		|| defined(__EINSTEIN__) \
		|| defined(__OSIC1P__) \
		|| defined(__MC1000__) \
		|| defined(__M5__) \
		|| (defined(__AQUARIUS__) && !defined(QUAD_MEMORY_MAP)) \
		|| defined(__COLECO__) \
		|| defined(__PV2000__) \
		|| defined(__PV1000__) \
		|| (defined(__EG2K__) && !!defined(_XL_NO_UDG))  \
		|| (defined(__EG2K__) && !defined(_XL_NO_UDG)) \
		|| defined(__ALPHATRO__) || defined(__ABC80__) \
		|| defined(__LASER500__) \
		|| (defined(__OSCA__) && !defined(__Z88DK_SPRITES_GRAPHICS)) \
		|| defined(__COMX__) || defined(__PECOM__) || defined(__TMC600__) || defined(__MICRO__)
		#define YSize (24-Y_OFFSET)
	#elif defined(__VIC20__) \
		|| defined(__RX78__)
		#define YSize 23
	#elif defined(__SPC1000__)
		#define YSize 22
	#elif defined(__SAM__)
		#define YSize (24-2-Y_OFFSET)
	#elif (defined(__TS2068__) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define YSize (192/__SPRITE_Y_STEP -Y_OFFSET)
	#elif (defined(__NC200__) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define YSize ((128/__SPRITE_Y_STEP)-Y_OFFSET)
	#elif defined(__SUPERVISION__)
		#define YSize 20
	#elif defined(__GAMATE__)
		#define YSize 19	
	#elif defined(__ATARI_LYNX__)
		#define YSize 13
	#elif (defined(__PX4__) && defined(Z88DK_PUTC4X6))
		#define YSize (10+1)
	#elif defined(__Z88__) || (defined(__PX8__) && !defined(__Z88DK_SPRITES_GRAPHICS)) \
		|| ((defined(__TI82__) || defined(__TI83__) || defined(__TI8X__) || defined(__TI85__) || defined(__TI86__)) && !defined(__Z88DK_SPRITES_GRAPHICS))
		#define YSize 8
	#elif defined(__G800__)
		#define YSize 7
	#elif ((defined(__TI82__) || defined(__TI83__) || defined(__TI8X__) || defined(__TI85__) || defined(__TI86__)) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define YSize ((64/__SPRITE_Y_STEP)+1)
	#elif (defined(__VZ__) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define YSize ((64/__SPRITE_Y_STEP)+1)
	#elif (defined(__PX8__) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define YSize ((64/__SPRITE_Y_STEP)+1)		
	#elif (defined(__PX4__) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define YSize ((64/__SPRITE_Y_STEP)+1)
	#elif (defined(__OSBORNE1__) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define YSize ((48/__SPRITE_Y_STEP)-1)
	#elif defined(__TIKI100__)
		#define YSize ((256/__SPRITE_Y_STEP)-Y_OFFSET)
	#elif (defined(__NC100__) && defined(__Z88DK_SPRITES_GRAPHICS)) 
		#define YSize ((64/__SPRITE_Y_STEP)+1)
	#elif (defined(__TRS80__) && !defined(__EG2K__) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define YSize ((48/__SPRITE_Y_STEP)-1)
	#elif defined(__KAYPRO83__)
		#define YSize (50/__SPRITE_Y_STEP)
	#elif defined(__KAYPRO__)
		#define YSize (100/__SPRITE_Y_STEP)
	#elif defined(__ATTACHE__) && defined(__Z88DK_SPRITES_GRAPHICS)
		#define YSize (240/__SPRITE_Y_STEP)	
	#elif (defined(__OZ__) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define YSize (80/__SPRITE_Y_STEP)
	#elif (defined(__C128_Z80__) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define YSize ((200/__SPRITE_Y_STEP)-Y_OFFSET)
	#elif (defined(__Z88__) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define YSize ((64/__SPRITE_Y_STEP))
	#elif (defined(__PC6001__) && __SCREEN_MODE==2) || (defined(__COCO__) || defined(__DRAGON__)) && defined(__BIT_MAPPED_GRAPHICS)
		#define YSize 24
	#else
		#define YSize 16
	#endif
#endif


#if !defined(XSize)
	#  if defined(__NCURSES__)
		#define XSize 80
	#elif defined(__CBM610__) || defined(__PET__) || (defined(__C128__) && defined(C128_80COL_VIDEO_MODE)) \
		  || defined(__BEE__) || defined(__PET__) || defined(__CBM610__) \
		  || (defined(__PX8__) && !defined(__Z88DK_SPRITES_GRAPHICS)) \
		  || (defined(__APPLE2ENH__) && defined(APPLE2ENH_80COL_VIDEO_MODE)) \
		  || defined(__PPS__) || (defined(__MULTI8__) && !defined(_XL_NO_UDG)) \
		  || (defined(__TS2068__) && !defined(__Z88DK_SPRITES_GRAPHICS))
		#define XSize 80
	#elif defined(__SRR__) || (defined(__TRS80__) && !defined(__EG2K__) && !defined(__Z88DK_SPRITES_GRAPHICS))
		#define XSize 64
	#elif defined(__PCE__) || (defined(__PX4__) && defined(Z88DK_PUTC4X6))
		 #define XSize 60
	#elif defined(__TIKI100__)
		#define XSize (1024/__SPRITE_X_STEP)
	#elif ((defined(__NC100__) || defined(__NC200__)) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define XSize (480/__SPRITE_X_STEP)
	#elif (defined(__PX8__) && defined(__Z88DK_SPRITES_GRAPHICS))	
		#define XSize (480/__SPRITE_X_STEP)
	#elif (defined(__PX4__) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define XSize (240/__SPRITE_X_STEP)
	#elif (defined(__ATTACHE__) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define XSize (320/__SPRITE_X_STEP)	
	#elif (defined(__OZ__) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define XSize (239/__SPRITE_X_STEP)
	#elif (defined(__OSBORNE1__) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define XSize (104/__SPRITE_X_STEP)
	#elif (defined(__C128_Z80__) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define XSize (640/__SPRITE_X_STEP)
	#elif defined(__NASCOM__)	
		#define XSize 48
	#elif defined(__VG5K__) || (defined(__APPLE2ENH__) && !defined(APPLE2ENH_80COL_VIDEO_MODE))|| defined(__APPLE2__) \
		  || defined(__Z9001__) || defined(__P2000__) || defined(__ABC80__) \
		  || defined(__X1__) || defined(__ATMOS__) \
		  || (defined(__CPC__) && defined(__CPCRSLIB_GRAPHICS)) || defined(__C16__) || defined(__PLUS4__) || defined(__C64__) \
		  || (defined(__C128__) && !defined(__Z88DK_SPRITES_GRAPHICS) && !defined(C128_80COL_VIDEO_MODE)) \
		  || (defined(__AQUARIUS__) && !defined(QUAD_MEMORY_MAP)) || (defined(__SVI__) && defined(MSX_MODE0)) \
		  || defined(__ENTERPRISE__) \
		  || (defined(__ATARI__) && !defined(__ANTIC_MODE6_GRAPHICS)) \
		  || defined(__CBM510__) \
		  || defined(__FP1100__) \
		  || (defined(__EG2K__) && !defined(_XL_NO_UDG)) \
		  || (defined(__EG2K__) && !!defined(_XL_NO_UDG)) \
		  || defined(__ALPHATRO__) \
		  || defined(__LASER500__) \
		  || (defined(__OSCA__) && !defined(__Z88DK_SPRITES_GRAPHICS)) \
		  || defined(__MO5__) || defined(__TO7__) \
		  || defined(__COMX__) || defined (__PECOM__) || defined(__TMC600__) || defined(__MICRO__)
		#define XSize (40-X_OFFSET)
	#elif defined(__KC__) && defined(__Z88DK_SPRITES_GRAPHICS)
		#define XSize (320/__SPRITE_X_STEP)
	#elif (defined(__TS2068__) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define XSize (512/__SPRITE_X_STEP)
	#elif (defined(__VZ__) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define XSize (128/__SPRITE_X_STEP)
 	#elif defined(__KAYPRO83__)
		#define XSize (80/__SPRITE_X_STEP)   
	#elif defined(__KAYPRO__)
		#define XSize (160/__SPRITE_X_STEP)
	#elif ((defined(__TI82__) ) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define XSize ((96/__SPRITE_X_STEP))
	#elif ((defined(__TI83__)) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define XSize ((96/__SPRITE_X_STEP)+1)
	#elif ((defined(__TI8X__)) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define XSize ((96/__SPRITE_X_STEP)+1)	
	#elif ((defined(__TI85__) || defined(__TI86__)) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define XSize (128/__SPRITE_X_STEP)	
	#elif (defined(__TRS80__) && !defined(__EG2K__) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define XSize (128/__SPRITE_X_STEP)
	#elif (defined(__Z88__) && defined(__Z88DK_SPRITES_GRAPHICS))
		#define XSize (256/__SPRITE_X_STEP)
	#elif defined(__KC__) && !defined(_XL_NO_UDG)
		#define XSize 40
    #elif (defined(__COCO__) || defined(__DRAGON__)) && defined(__BIT_MAPPED_4_GRAPHICS)
        #define XSize 16
	#elif (defined(__VZ__) && !defined(__Z88DK_SPRITES_GRAPHICS)) || defined(__NES__) || defined(__CREATIVISION__) || defined(__MSX__) \
		  || (defined(__SVI__) && !defined(MSX_MODE0) ) || defined(__ZX81__) || defined(__ZX80__) \
		  || defined(__LAMBDA__) || (defined(__SPECTRUM__)&&!defined(__TS2068)) || (defined(__PC6001__) && !defined(__SCREEN_MODE))\
		  || defined(__SC3000__) || defined(__MC1000__) || defined(__MTX__) || defined(__SAM__) \
		  || defined(__GAL__) || defined(__COCO__) || defined(__DRAGON__) || defined(__WINCMOC__) \
		  || defined(__Z1013__) || defined(__Z88__) || defined(__ACE__) \
		  || defined(__EINSTEIN__) \
		  || ((defined(__TI82__) || defined(__TI83__) || defined(__TI8X__) || defined(__TI85__) || defined(__TI86__)) && !defined(__Z88DK_SPRITES_GRAPHICS)) \
		  || defined(__M5__) || defined(__COLECO__) \
		  || defined(__PV2000__) || defined(__MZ__) \
		  || defined(__CAMPUTERS_LYNX__)
		#define XSize 32
	#elif defined(__PV1000__)
		#define XSize 28
	#elif defined(__CIDELSA__) && defined(__DRACO__)
		#define XSize 27
	#elif defined(__CIDELSA__) && (defined(__DESTROYER__) || defined(__ALTAIR__))
		#define XSize 25
	#elif defined(__OSIC1P__) || defined(__G800__) || defined(__RX78__) 
		#define XSize 24
	#elif defined(__VIC20__) 
		#define XSize 22
	#elif (defined(__ATARI__) && defined(__ANTIC_MODE6_GRAPHICS)) \
		  || defined(__ATARI_LYNX__) || defined(__SUPERVISION__) \
		  || defined(__ATARI5200__) || defined(__GAMATE__)
		#define XSize 20
	#elif defined(__PC6001__) && __SCREEN_MODE==2
		#define XSize 16
	#else
		#define XSize 16
	#endif
#endif



#if !defined(Y_OFFSET)
    #define Y_OFFSET 0
#endif

#if !defined(X_OFFSET)
    #if defined(__ATMOS__)
        #define X_OFFSET 2
    #else
        #define X_OFFSET 0
    #endif
#endif

#endif // _DISPLAY_TARGET_GEOMETRY_H 
