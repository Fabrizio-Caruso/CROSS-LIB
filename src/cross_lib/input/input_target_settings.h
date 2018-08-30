#ifndef _INPUT_TARGET_SETTINGS
#define _INPUT_TARGET_SETTINGS

#  if defined(__CPC_JOYSTICK__) // Special Joystick 
	#define _MOVE_UP 0x0B 
	#define _MOVE_DOWN 0x0A 
	#define _MOVE_LEFT 0x08
	#define _MOVE_RIGHT 0x09
	#define _FIRE 0x58
#elif defined(__MC1000_JOYSTICK__) // Special Joystick 
	#define _MOVE_UP 'I' 
	#define _MOVE_DOWN 'Q' 
	#define _MOVE_LEFT 'Y'
	#define _MOVE_RIGHT '1'
	#define _FIRE '9'
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
#elif defined(__NASCOM__) || defined(__MC1000__) || defined(__OSIC1P__) || defined(__WINCMOC__) || defined(__CMOC__) \
	||  defined(__GAL__)  || defined(__Z9001__) || defined(__ABC80__) || defined(__APPLE2__) || defined(__APPLE2ENH__) \
	|| defined(__VZ__) || defined(__ATMOS__) || defined(__MZ__) || defined(__MTX__) || defined(__SC3000__) \
	|| defined(__Z1013__) || defined(__KC__) || defined(__C128_Z80__) \
	|| defined(__EINSTEIN__) || defined(__MULTI8__) || defined(__G800__) || defined(__FP1100__)
	#define _MOVE_UP 'I'
	#define _MOVE_DOWN 'K'
	#define _MOVE_LEFT 'J'
	#define _MOVE_RIGHT 'L'
	#define _FIRE ' '	
#else
	#define _MOVE_UP 'i'
	#define _MOVE_DOWN 'k'
	#define _MOVE_LEFT 'j'
	#define _MOVE_RIGHT 'l'
	#define _FIRE ' '		
#endif


#if defined(__VIC20__) || defined(__CREATIVISION__) \
		|| defined(__SUPERVISION__) || defined(__ATARI_LYNX__) || defined(__GAMATE__) \
		|| defined(__C64__) || defined(__C16__) || defined(__PLUS4__) \
		|| defined(__CBM510__) || defined(__PET__) \
		|| (defined(__C128__) && !defined(__C128_Z80__)) \
		|| defined(__ATARI__) || defined(__ATARIXL__) || defined(__ATARI5200__) \
		|| defined(__NES__) || defined(__PCE__) \
		|| defined(Z88DK_JOYSTICK)
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


#endif // _INPUT_TARGET_SETTINGS
