#ifndef _INPUT_TARGET_SETTINGS
#define _INPUT_TARGET_SETTINGS


#  if defined(__NASCOM__) || defined(__MC1000__) || defined(__OSIC1P__) \
	|| defined(__WINCMOC__) || defined(__COCO3__) || defined(__COCO__) || defined(__DRAGON__) || defined(__TO7__) ||defined(__MO5__) \
	||  defined(__GAL__)  || defined(__Z9001__) || defined(__APPLE2__) || defined(__APPLE2ENH__) \
	|| (defined(__VZ__) && !defined(__HARDWARE_KEYBOARD)) || defined(__ATMOS__) || defined(__MZ__) || defined(__MTX__) || defined(__SC3000__) \
	|| defined(__Z1013__) || defined(__KC__) || defined(__C128_Z80__) || defined(__EINSTEIN__) || defined(__MULTI8__) \
	|| defined(__G800__) || defined(__FP1100__) || defined(__TI99__) || defined(__HEMC__) || defined(__HGMC__) \
	|| defined(__HOMELAB__) || defined(__SPECIAL__) || defined(__KRAMERMC__) || defined(__DAI__) \
    || (defined(__BBC__) && defined(TURN_BASED))|| defined(__BBCMASTER__) \
    || defined(__MC10__) || (defined(__ZX81__) && defined(__CONIO_GRAPHICS))
	#define _MOVE_UP 'I'
	#define _MOVE_DOWN 'K'
	#define _MOVE_LEFT 'J'
	#define _MOVE_RIGHT 'L'
#elif defined(__BBC__)
// 'b' <-> SPACE
// '%' <-> 'I'
// 'F' <-> 'K'
// 'E' <-> 'J'
// 'V' <-> 'L'
	#define _MOVE_UP ('%'+1)
	#define _MOVE_DOWN ('F'+1)
	#define _MOVE_LEFT ('E'+1)
	#define _MOVE_RIGHT ('V'+1)
#else
	#define _MOVE_UP 'i'
	#define _MOVE_DOWN 'k'
	#define _MOVE_LEFT 'j'
	#define _MOVE_RIGHT 'l'
#endif
#if defined(__COMX__)
    #define _FIRE 0x5f 
#elif defined(__BBC__)
    #define _FIRE ('b'+1)
#else
    #define _FIRE ' '
#endif




#if defined(_XL_NO_JOYSTICK)
	#define WAIT_FOR_KEY
#else
	#define WAIT_FOR_JOY
#endif


#endif // _INPUT_TARGET_SETTINGS
