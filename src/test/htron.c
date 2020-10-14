/**************************************************************
*    htron.c                                   	              *
*    H-Tron - a Tron motorcycle game                          *
*    Originally written in Borland Turbo C 2.01 for           *
*    XT-class DOS PCs equipped with a Hercules graphics card  *
*    Author: RobertK (RobertK@psc.at), December 2016          *
*                                                             *
*    Ported to the varous Z80-based computers                 *
*    (based on DOS Version 2016-12-26a)                       *
*    by RobertK in 2018 using the z88dk C compiler            *
*                                                             *
*    Portable z88dk Version 2020-05-04                        *
*                                                             *
*    Compile with z88dk nightly build                         *
*    (last full compilation with version 2018-07-09,          *
*    ColecoVision target with 2018-07-04 due to a             *
*    problem with that target in the 2018-07-09 nightly)      * 
*    Do not use the stable release 1.99B because              *
*    the z88dk team have since then massively improved        *
*    the graphics functions for various targets.              *
*                                                             *
*    This is the classic overhead view "Tron" game from       *
*    the movie of the same title (motorcycles that leave      *
*    "wall trails" behind them). If you crash into a wall,    *
*    your opponent scores a point. The player who first       *
*    reaches a score of 5 wins.                               *
*                                                             *
*    Controls (on QWERTY keyboards, AZERTY differs)           *
*    Turn left/right:                                         *
*      Left Player: A/S                                       *
*      Right Player: N/M or 5/8 (Cursor left/right on ZX81)   *
*      (in one-player mode, human is right player)            *
*    Start round: Space or 0                                  *
*    Pause game: P                                            *
*    End game: X                                              *
**************************************************************/

#include <stdio.h>
// #include <zx81plot.h>	// custom-made ASM plot() and point() functions for the ZX81 target (by A. Rea)
#include <graphics.h>		// contains standard plot() and point() functions

#if defined(__PC6001__)	|| defined(__MC1000__) || defined(__EINSTEIN80COLUMNS__) || defined(__TIKI100__) || defined(__GALPLUSHIRES__) || defined(__SMC777__) || defined(__EXCALIBUR64__) || defined(__FP1100__) // NEC PC-6001, CCE MC-1000, Tatung Einstein 80 column mode, Tiki-100, Galaksija Plus HiRes-Mode, Sony SMC-777, Casio FP-1100
	#include <sys/ioctl.h>		// required for switching the screen mode
#endif

#if defined(__TIKI100__)
	#include <tiki100.h>
#endif

#if defined(__MSX__)
	#pragma output nofileio		// we don't need any file I/O in this game, so we can save a few bytes here
#endif

// Some systems have no keyboard
// These are usually consoles with two joysticks
#if defined(__PV1000__) || (defined(__COLECO__) && !defined(__BIT90__)) || defined(__PACMAN__) || defined(__Z80TVGAME__) || defined(__GAMEBOY__) || defined(__MYVISION__) // Casio PV-1000, ColecoVision, PacMan-Hardware, etc.
	#define JOYSTICK_ONLY 1
	#include <games.h>
#endif
// But some have only one joystick
#if defined(__Z80TVGAME__) || defined(__GAMEBOY__)
	#define TWO_PLAYER_MODE_NOT_AVAILABLE 1
#endif
// For joystick-only systems we have to define the joystick numbers, and
// if there is a "Pause" button and a "Menu" button
// -1 means that such a button is not available
#if defined(__PV1000__) // Casio PV-1000
	#define JOYSTICK_1 0
	#define JOYSTICK_2 1
	#define JOYSTICK_BUTTON_MENU MOVE_FIRE3	// Select
	#define JOYSTICK_BUTTON_PAUSE MOVE_FIRE4 // Start	
#elif defined(__COLECO__) && !defined(__BIT90__) // ColecoVision
	#define JOYSTICK_1 1
	#define JOYSTICK_2 2
	#define JOYSTICK_BUTTON_MENU MOVE_FIRE2	// Select
	#define JOYSTICK_BUTTON_PAUSE -1 // only two-Joystick buttons available	
#elif defined(__PACMAN__)	// PacMan-Hardware
	// PROVISIONAL VALUES
	#define JOYSTICK_1 1
	#define JOYSTICK_2 2
	#define JOYSTICK_BUTTON_MENU -1  // only 1-player start and 2-player-start button
	#define JOYSTICK_BUTTON_PAUSE -1 // only 1-player start and 2-player-start button
#elif defined(__Z80TVGAME__)	// Z80 TV-Game (homebrew)
	#define JOYSTICK_1 1
	#define JOYSTICK_2 1	// The z88dk wiki says that this is a single joystick system, but the Takeda emulator supports two??
	#define JOYSTICK_BUTTON_MENU MOVE_FIRE2 // Button 2
	#define JOYSTICK_BUTTON_PAUSE -1
#elif defined(__GAMEBOY__)	// Nintendo Gameboy
	#define JOYSTICK_1 0
	#define JOYSTICK_2 1
	#define JOYSTICK_BUTTON_MENU MOVE_FIRE3	// Select Button
	#define JOYSTICK_BUTTON_PAUSE MOVE_FIRE4 // Start Button
#elif defined(__MYVISION__)
	#define JOYSTICK_1 1
	#define JOYSTICK_2 2
	#define JOYSTICK_BUTTON_MENU MOVE_FIRE3	// Select Button
	#define JOYSTICK_BUTTON_PAUSE MOVE_FIRE4 // Start Button
#endif

// VT100 Console I/O ANSI driver for the targets that support it
// we use this for gotoxy() and keyboard input
#if defined(__ABC80__) || defined(__ACE__) || defined(__AQUARIUS__) || defined(__NASCOM__) || defined(__P2000__) || defined(__VG5000__) || defined(__SHARPMZ__) || defined(__VZ200__) || defined(__Z1013__) || defined(__Z9001__) || defined(__PC6001__) || defined(__MSX__) || defined(__CPC__) || defined(__X1__)
	#include <conio.h>
	#define USE_ANSI 1
#elif ((defined(__TRS80__) && !defined(__EG2000__)) || defined(__PV1000__)) || defined(__SORCERER__) || defined(__COLECO__) || defined(__ALPHATRO__) || defined(__SPC1000__) || defined(__MULTI8__) || defined(__RX78__) || defined(__PV2000__) || defined(__SPECTRUM__) || defined(__KC__) || defined(__SVI__) || defined(__MTX__) || defined(__M5__) || defined(__SC3000__) || defined(__LASER500__) || defined(__EINSTEIN__) || (defined(__LYNX__) && !defined(__LYNXSTANDARDCONSOLE__)) || defined(__MC1000__) || defined(__BEE__) || defined(__TIKI100__) || defined(__SUPER80__) || defined(__GAL__) || defined(__SMC777__) || defined(__Z80TVGAME__) || defined(__EXCALIBUR64__) || defined(__GAMEBOY__) || defined(__RC700__) || defined(__PC88__) || defined(__FP1100__) || defined(__MYVISION__) || defined(__PENCIL2__) || defined(__HGMC__) || defined(__PMD85__) || defined(__VECTOR06C__) || defined(__HOMELAB__) || defined(__HOMELAB2__) || defined(__PRIMO__) || defined(__MIKRO80__) // TRS-80, Casio PV-1000, Exidy Sorcerer, ColecoVision, ...
	#define USE_GENERIC_CONSOLE 1
#elif defined(__OSBORNE1__) || defined(__PACMAN__) || defined(__KAYPRO84__) || defined(__ATTACHE__) // Osborne 1, PacMan-Hardware, Kaypro, Otrona Attache
	#define USE_NATIVE_CONSOLE 1 
#endif

// #if defined(__TRS80__) || defined(__SORCERER__) 
// Triumph-Adler Alphatronic PC, Samsung SPC-1000, Mitsubishi Multi8
// Note: TRS-80 / EG2000 and Exidy Sorcerer use inkey_pressed() input during gameplay, but native keyboard input is used in menus
#if defined(__ALPHATRO__) || defined(__SPC1000__) || defined(__MULTI8__) || defined(__TRS80__) || defined(__SORCERER__) || defined(__RX78__) || defined(__PV2000__) || defined(__SPECTRUM__) || defined(__SVI__)|| defined(__MTX__) || defined(__M5__) || defined(__SC3000__) || defined(__LASER500__) || defined(__EINSTEIN__) || defined(__OSBORNE1__) || defined(__LYNX__) || defined(__MC1000__) || defined(__BEE__) || defined(__TIKI100__) || defined(__SUPER80__) || defined(__GAL__) || defined(__SMC777__) || defined(__ATTACHE__) || defined(__EXCALIBUR64__) || defined(__RC700__) || defined(__PC88__) || defined(__PENCIL2__) || defined(__PMD85__) || defined(__BIT90__) || defined(__VECTOR06C__) || defined(__HOMELAB2__) || defined(__MIKRO80__) || defined(__PRIMO__)
	#define USE_NATIVE_INPUT 1		// kbhit() and getch() without conio.h
#endif

#if defined(__ZX81__) || defined(__ZX80__) || defined(__ACE__) || defined(__VZ200__) || defined(__SORCERER__) || defined(__VG5000__) || defined(__TRS80__) || defined(__AQUARIUS__) || defined(__SPECTRUM__) || defined(__PMD85__) || defined(__PRIMO__) || defined(__MIKRO80__)
	#define USE_INKEYPRESSED 1		// allows simultaneously checking the status of more than just one key
#endif

// These systems can only display upper-case characters, but in the C source you need to
// use lower-case characters while the upper-case ones have a different meaning (for the 
// ZX80 and ZX81 they produce inverse character)
#if defined(__ZX81__) || defined(__ZX80__) || defined(__PACMAN__) || (defined(__GAL__) && !defined(__GALPLUSHIRES__)) || defined(__HOMELAB2__) || defined(__MIKRO80__)
	#define SMALL_LETTERS_ONLY 1
#endif

// Here we define the position of the separator line, etc.
// Most systems use the values in the ELSE section, but some need special values.
#if (defined(__VZ200__) && !defined(VZ200_LGFX)) || defined(__P2000__) || (defined(__TRS80__) && defined(__EG2000__)) || defined(__PC88__)
  // Some systems cannot combine text and graphics, so the z88dk team kindly added
  // 4x6 character support to their graphic modes. This requires a different method
  // of text positioning by specifying pixel coordinates instead of line and column 
  // coordinates. For these systems we define FOUR_BY_SIX_CHARACTERS.
  #define FOUR_BY_SIX_CHARACTERS 1
  #define SCORE_LINE 0
  #define SCORE_COL 0
  #define BORDER_TOP 7
  #define SEPARATOR_LINE -1	// not drawn
#elif defined(__TRS80__) && !defined(__EG2000__)	// TRS-80
  #define SCORE_LINE 1
  #define SCORE_COL 2
  #define BORDER_TOP 0
  #define SEPARATOR_LINE 6
#elif defined(__SORCERER__) || defined(__MULTI8__) || defined(__SPECTRUM__) || defined(__OSBORNE1__) || defined(__BEE__) || defined(__MIKRO80__) // Exidy Sorcerer, Mitsubishi Multi8, ZX Spectrum, Osborne 1, Microbee, Mikro-80
  #define SCORE_LINE 1
  #define SCORE_COL 2
  #define BORDER_TOP 0
  #define SEPARATOR_LINE 5
#elif defined(__ALPHATRO__)  // Triumph-Adler Alphatronic PC
  #define SCORE_LINE 1
  #define SCORE_COL 1
  #define BORDER_TOP 0
  #define SEPARATOR_LINE 7
#elif defined(__NASCOM__)	// Nascom
  #define SCORE_LINE 1
  #define SCORE_COL 1
  #define BORDER_TOP 0
  #define SEPARATOR_LINE 7
#elif defined(__PC6001__)	// NEC PC-6001
  #define SCORE_LINE 1
  #define SCORE_COL 1
  #define BORDER_TOP 0
  #define SEPARATOR_LINE 7
#elif defined(__SPC1000__) // Samsung SPC-1000
  #define SCORE_LINE 1
  #define SCORE_COL 1
  #define BORDER_TOP 0
  #define SEPARATOR_LINE 7
#elif defined(__VG5000__)  // Philips VG5000
  #define SCORE_LINE 1
  #define SCORE_COL 1
  #define BORDER_TOP 0
  #define SEPARATOR_LINE 7
#elif defined(__EINSTEIN80COLUMNS__) || defined(__RC700__)  // Tatung Einstein 80 column mode, RC-700
  #define SCORE_LINE 0
  #define SCORE_COL 3
  #define BORDER_TOP 1
  #define SEPARATOR_LINE -1 // not drawn
#elif defined(__SUPER80__) && !defined(__SUPER80_VDUEM__) // Super-80 e or m model
  #define SCORE_LINE 1
  #define SCORE_COL 1
  #define BORDER_TOP 0
  #define SEPARATOR_LINE 7
#elif defined(__SUPER80__) && defined(__SUPER80_VDUEM__) // Super-80 r or v model
  #define SCORE_LINE 1
  #define SCORE_COL 2
  #define BORDER_TOP 0
  #define SEPARATOR_LINE 5  
#elif defined(__PACMAN__)	// PacMan-Hardware
  // PROVISIONAL VALUES
  #define SCORE_LINE 2
  #define SCORE_COL 1
  #define BORDER_TOP 0
  #define SEPARATOR_LINE 5
#elif (defined(__GAL__) && !defined(__GALPLUSHIRES__))
  #define SCORE_LINE 0
  #define SCORE_COL 1
  #define BORDER_TOP 4
  #define SEPARATOR_LINE -1 // not drawn
#elif defined(__ATTACHE__)	// Otrona Attache
  #define SCORE_LINE 1
  #define SCORE_COL 2
  #define BORDER_TOP 0
  #define SEPARATOR_LINE 6
#elif defined(__EXCALIBUR64__)
  #define SCORE_LINE 1
  #define SCORE_COL 1
  #define BORDER_TOP 0
  #define SEPARATOR_LINE 7
#else
  #define SCORE_LINE 1
  #define SCORE_COL 1
  #define BORDER_TOP 0
  #define SEPARATOR_LINE 5
#endif

// Some 4x6 character systems offer a higher resolution, there we 
// do not need to shorten the text strings
#if (defined(__VZ200__) && !defined(VZ200_LGFX)) || (defined(__TRS80__) && defined(__EG2000__)) || defined(__PC88__)
  #define FOUR_BY_SIX_CHARACTERS_WIDE_SCREEN 1
#endif
// On some 4x6 systems we don't need to show the menu texts in two parts
#if defined(__TRS80__) && defined(__EG2000__) || defined(__PC88__)
  #define FOUR_BY_SIX_CHARACTERS_TALL_SCREEN 1
#endif
// Some systems support uppercase and lowercase characters, but have a narrow screen
#if defined(__PV1000__)
  // identical to the FOUR_BY_SIX_CHARACTERS_WIDE_SCREEN variant, but with both uppercase and lowercase letters
  #define STANDARD_CHARACTERS_NARROW_SCREEN 1
#elif defined(__RX78__)
  // RX78: This system is special: 24x23 makes an almost quadratic layout 
  #define STANDARD_CHARACTERS_NARROW_BUT_TALL_SCREEN 1
#elif defined(__Z80TVGAME__) || defined(__GAMEBOY__)
  #define STANDARD_CHARACTERS_VERYNARROW_BUT_TALL_SCREEN 1
#endif

// Some systems have a screen resolution that requires us to double the 
// pixels on one axis to get closer to a 4:3 aspect ratio. Apart from looking 
// odd, the movement on one axis would be twice as fast as on the other axis
// if we wouldn't make this correction.
#if (defined(__TRS80__) && !defined(__EG2000__)) || defined(__SORCERER__) || defined(__MULTI8__) || defined(__OSBORNE1__) || defined(__CPC__) || defined(__BEE__) || defined(__EINSTEIN80COLUMNS__) || defined(__SUPER80_VDUEM__) || defined(__MIKRO80__) // || defined(__HOMELAB__) // || defined(__RC700__)
// TRS-80, Exidy Sorcerer, Mitsubishi Multi8, Osborne 1, CPC, Tatung Einstein 80 Columns, Microbee, Super-80 r or v model
  #define DOUBLE_PIXELS_HORIZONTALLY 1
#endif 

// On some systems with high resolution graphics we use c_plot() and c_point() to 
// achieve a pseudo lo-res graphics resolution. c_plot() creates a 4x4 pixel rectangle,
// which turns e.g. a 256x192 resolution into 64x48.
#if defined(__COLECO__) || defined(__MULTI8__) || defined(__RX78__) || defined(__PV2000__) || defined(__SPECTRUM__) || defined(__SC3000__) || defined(__KC__) || defined(__MSX__) || defined(__M5__) || defined(__SVI__) || defined(__MTX__) || (defined(__EINSTEIN__) && !defined(__EINSTEIN80COLUMNS__)) || defined(__CPC__) || defined(__MC1000__) || defined(__TIKI100__) || (__GALPLUSHIRES__) || defined(__Z80TVGAME__) || defined(__ATTACHE__) || defined(__X1__) || defined(__MYVISION__) || defined(__PENCIL2__) || defined(__HGMC__) || defined(__PMD85__) || defined(__VECTOR06C__) || defined(__PRIMO__)
  #define USE_C_PLOT 1
#endif 

// The Sharp MZ seems to run out of stack space when the program crashes at the 
// "char textP1[25];" declaration within the InitPlayfield() function. Solution according to dom: 
// "compiling with the atexit size removed - by default on the MZ that's taking up 64 bytes of stack"
// "For your case, I don't think there's any benefit in protecting that #pragma as just for the MZ - 
// I'm guessing you're not using atexit() so it might as well be 0 for all targets."
// #if defined(__SHARPMZ__)	// Sharp MZ
	#pragma output CLIB_EXIT_STACK_SIZE = 0
// #endif

#define POINTS_TO_WIN 5

// On systems with very low resolution we reduce the AI player's sighting distance to get him closer to the walls
#if (defined(__VZ200__) && defined(VZ200_LGFX)) // || defined(__SPC1000__)
	#define AI_MAX_DISTANCE_TO_CHECK 8
	#define AI_SHORTSIGHTED_FACTOR 5
#else
	#define AI_MAX_DISTANCE_TO_CHECK 12 // (DOS version: 15)
	#define AI_SHORTSIGHTED_FACTOR 8
#endif

// ZX81 and ZX80 Key Codes
#define KEY_CODE_0 48
#define KEY_CODE_1 49
#define KEY_CODE_2 50
#define KEY_CODE_SPACE 32

// Keypress codes - only for those systems where in_KeyPressed() is implemented
// You can find out these codes like this:
// printf("in_lookupkey() for a is %d\n",in_LookupKey('a'));
#if defined(__ZX81__) || defined(__ZX80__) || defined(__SPECTRUM__)
	#define KEYPRESS_CODE_5 4343
	#define KEYPRESS_CODE_8 1263
	#define KEYPRESS_CODE_A 509
	#define KEYPRESS_CODE_M 1151
	#define KEYPRESS_CODE_N 2175
	#define KEYPRESS_CODE_P 479
	#define KEYPRESS_CODE_S 765
	#define KEYPRESS_CODE_X 1278	
#elif defined(__ACE__)
	#define KEYPRESS_CODE_5 4343
	#define KEYPRESS_CODE_8 1263
	#define KEYPRESS_CODE_A 509
	#define KEYPRESS_CODE_M 639
	#define KEYPRESS_CODE_N 1151
	#define KEYPRESS_CODE_P 479
	#define KEYPRESS_CODE_S 765
	#define KEYPRESS_CODE_X 2302
#elif defined(__VZ200__)
	#define KEYPRESS_CODE_5 -2303
	#define KEYPRESS_CODE_8 -8440
	#define KEYPRESS_CODE_A -752
	#define KEYPRESS_CODE_M -4320
	#define KEYPRESS_CODE_N -4351
	#define KEYPRESS_CODE_P -16624
	#define KEYPRESS_CODE_S -766
	#define KEYPRESS_CODE_X -1278
#elif defined(__SORCERER__)
	#define KEYPRESS_CODE_5 4101
	#define KEYPRESS_CODE_8 4104
	#define KEYPRESS_CODE_A 1026
	#define KEYPRESS_CODE_M 262
	#define KEYPRESS_CODE_N 518
	#define KEYPRESS_CODE_P 2057
	#define KEYPRESS_CODE_S 1027
	#define KEYPRESS_CODE_X 258
#elif defined(__VG5000__)	// this machine from France has an AZERTY keyboard
	#define KEYPRESS_CODE_5 4099
	#define KEYPRESS_CODE_8 16389
	#define KEYPRESS_CODE_A 513		// Q instead of A
	#define KEYPRESS_CODE_M 4101	// , instead of M
	#define KEYPRESS_CODE_N 1031
	#define KEYPRESS_CODE_P -32764
	#define KEYPRESS_CODE_S 259
	#define KEYPRESS_CODE_X 514
#elif defined(__TRS80__) 	// TRS-80 or EG2000, both use the same key codes
	#define KEYPRESS_CODE_5 8196
	#define KEYPRESS_CODE_8 261
	#define KEYPRESS_CODE_A 512
	#define KEYPRESS_CODE_M 8193
	#define KEYPRESS_CODE_N 16385
	#define KEYPRESS_CODE_P 258
	#define KEYPRESS_CODE_S 2050
	#define KEYPRESS_CODE_X 259
#elif defined(__AQUARIUS__)	// Mattel Aquarius
	#define KEYPRESS_CODE_5 261
	#define KEYPRESS_CODE_8 259
	#define KEYPRESS_CODE_A 8198
	#define KEYPRESS_CODE_M 2050
	#define KEYPRESS_CODE_N 4098
	#define KEYPRESS_CODE_P 2049
	#define KEYPRESS_CODE_S 1030
	#define KEYPRESS_CODE_X 8197
#elif defined(__PMD85__)
	#define KEYPRESS_CODE_5 516
	#define KEYPRESS_CODE_8 519
	#define KEYPRESS_CODE_A 2048
	#define KEYPRESS_CODE_M 4103
	#define KEYPRESS_CODE_N 4102
	#define KEYPRESS_CODE_P 1033
	#define KEYPRESS_CODE_S 2049
	#define KEYPRESS_CODE_X 4098
#elif defined(__PRIMO__)
	#define KEYPRESS_CODE_5 29
	#define KEYPRESS_CODE_8 33
	#define KEYPRESS_CODE_A 14
	#define KEYPRESS_CODE_M 44
	#define KEYPRESS_CODE_N 32
	#define KEYPRESS_CODE_P 50
	#define KEYPRESS_CODE_S 2
	#define KEYPRESS_CODE_X 10
#elif defined(__MIKRO80__)
	#define KEYPRESS_CODE_5 8192
	#define KEYPRESS_CODE_8 513
	#define KEYPRESS_CODE_A 2050
	#define KEYPRESS_CODE_M 516
	#define KEYPRESS_CODE_N 1028
	#define KEYPRESS_CODE_P 4100
	#define KEYPRESS_CODE_S 261
	#define KEYPRESS_CODE_X 8197
#endif

// The key codes on the ZX81 and ZX80 are the same, but for the menu screens we use in_Inkey() 
// on the ZX81 and fgetc_cons() on the ZX80, and these two function return different codes.
#if defined(__ZX80__) || defined(__SPECTRUM__)
	#define KEY_CODE_D 100
	#define KEY_CODE_M 109
	#define KEY_CODE_N 110
	#define KEY_CODE_P 112
	#define KEY_CODE_X 120
	#define KEY_CODE_Y 121
#else	// we use these ZX81 constants for the ANSI variants as well (the actual code values are irrelevant for this purpose)
	#define KEY_CODE_D 68
	#define KEY_CODE_M 77
	#define KEY_CODE_N 78
	#define KEY_CODE_P 80
	#define KEY_CODE_X 88
	#define KEY_CODE_Y 89
#endif


// ZX81/ZX80/Ace playfield dimensions:  64 x 48  (DOS version:  90 x 58 )

// Define the screen dimensions and startup positions for the current system
// Note: the screen dimensions can be determined on runtime by calling getmaxx() and getmaxy()
#if defined(__ZX81__) || defined(__ZX80__) || defined(__ACE__) || (defined(__GAL__) && !defined(__GALPLUSHIRES__)) || defined(__COLECO__) || defined(__PV2000__) || defined(__SC3000__) || defined(__PC6001__) || defined(__MSX__) || defined(__M5__) || defined(__SVI__) || defined(__MTX__) || defined(__SPC1000__) || (defined(__EINSTEIN__) && !defined(__EINSTEIN80COLUMNS__)) || defined(__MC1000__) || (defined(__SUPER80__) && !defined(__SUPER80_VDUEM__)) || defined(__MYVISION__) || defined(__PENCIL2__) || defined(__PRIMO__)
// ZX81, ZX80, Jupiter Ace, Galaksija, ColecoVision, Casio PV-2000 (using c_plot), etc.
	#define X_RES 64
	#define Y_RES 48
	#define X_COLS 32
	#define STARTUP_P1_X 10
	#define STARTUP_P2_X 53
	#define STARTUP_Y 26
#elif defined(__SPECTRUM__)	// ZX Spectrum
	#define X_RES 64
	#define Y_RES 48
	#define X_COLS 64
	#define STARTUP_P1_X 10
	#define STARTUP_P2_X 53
	#define STARTUP_Y 26
#elif defined(__GAL__) && defined(__GALPLUSHIRES__)	// Galaksija Plus Hi-Res Mode 256x208
	#define X_RES 64
	#define Y_RES 52
	#define X_COLS 32
	#define STARTUP_P1_X 10
	#define STARTUP_P2_X 53
	#define STARTUP_Y 28
#elif defined(__VG5000__) || defined(__EXCALIBUR64__)	// Philips VG5000 or Excalibur 64
	#define X_RES 80
	#define Y_RES 72
	#define X_COLS 40
	#define STARTUP_P1_X 12
	#define STARTUP_P2_X 67
	#define STARTUP_Y 39
#elif defined(__AQUARIUS__) || defined(__Z9001__) || defined(__LASER500__) || defined(__FP1100__)	// Mattel Aquarius, Robotron Z9001, Laser 500/700, Casio FP-1100
	#define X_RES 80
	#define Y_RES 48
	#define X_COLS 40
	#define STARTUP_P1_X 12
	#define STARTUP_P2_X 67
	#define STARTUP_Y 26
#elif defined(__VZ200__)	// VZ200 / Laser 310/210/110
	#if !defined(VZ200_LGFX)
		#define X_RES 128
		#define Y_RES 64
		#define X_COLS 32
		#define STARTUP_P1_X 20
		#define STARTUP_P2_X 107
		#define STARTUP_Y 35
	#else
		#define X_RES 64
		#define Y_RES 32
		#define X_COLS 32
		#define STARTUP_P1_X 10
		#define STARTUP_P2_X 53
		#define STARTUP_Y 17		
	#endif
#elif defined(__TRS80__) && !defined(__EG2000__)	// TRS-80
	#define X_RES 64	// normally 128, but on this system we double the pixels horizontally to get a better aspect ratio
	#define Y_RES 48
	#define X_COLS 64	// no VT100 Console I/O ANSI for this system
	#define STARTUP_P1_X 10 // normally 20, see above
	#define STARTUP_P2_X 53 // normally 107, see above
	#define STARTUP_Y 26
#elif defined(__TRS80__) && defined(__EG2000__)		// EG2000  (constant for this sub-type available since nightly 2018-03-31)
	#define X_RES 160
	#define Y_RES 96	// 160x102 on the newer models, but only 160x96 on the older ones - we use the latter so it will work on both models
	#define X_COLS 40	// no VT100 Console I/O ANSI for this system
	#define STARTUP_P1_X 25
	#define STARTUP_P2_X 134
	#define STARTUP_Y 54
#elif defined(__Z1013__) || defined(__HGMC__) || defined(__LYNX__) || defined(__VECTOR06C__)	// Robotron Z1013, Hübler-Grafik-MC, Camputers Lynx, Vector 06c
	#define X_RES 64
	#define Y_RES 64
	#define X_COLS 32
	#define STARTUP_P1_X 10
	#define STARTUP_P2_X 53
	#define STARTUP_Y 35
#elif defined(__SHARPMZ__) || defined(__SMC777__) || defined(__X1__) || defined(__HOMELAB2__)	// Sharp MZ, Sony SMC-777 or Sharp X1
	#define X_RES 80
	#define Y_RES 50
	#define X_COLS 40
	#define STARTUP_P1_X 12
	#define STARTUP_P2_X 67
	#define STARTUP_Y 27
#elif defined(__ABC80__)	// Luxor ABC 80
	#define X_RES 78
	#define Y_RES 72	
	#define X_COLS 40
	#define STARTUP_P1_X 12
	#define STARTUP_P2_X 65
	#define STARTUP_Y 39
#elif defined(__NASCOM__)	// Nascom
	#define X_RES 96
	#define Y_RES 48
	#define X_COLS 48
	#define STARTUP_P1_X 15
	#define STARTUP_P2_X 80
	#define STARTUP_Y 26
#elif defined(__P2000__)	// Philips P2000
	#define X_RES 78
	#define Y_RES 72
	#define X_COLS 19	// normally 40, but with 4x6 characters LOWER(X_RES / 4)
	#define STARTUP_P1_X 12
	#define STARTUP_P2_X 65
	#define STARTUP_Y 39
#elif defined(__PV1000__)	// Casio PV-1000
	#define X_RES 56
	#define Y_RES 48
	#define X_COLS 28
	#define STARTUP_P1_X 8
	#define STARTUP_P2_X 47
	#define STARTUP_Y 26
#elif defined(__SORCERER__)
	#define X_RES 64	// normally 128, but on this system we double the pixels horizontally to get a better aspect ratio
	#define Y_RES 60
	#define X_COLS 64
	#define STARTUP_P1_X 10 // normally 20, see above
	#define STARTUP_P2_X 53 // normally 107, see above
	#define STARTUP_Y 31
#elif defined(__ALPHATRO__)	// Triumph-Adler Alphatronic PC
	#define X_RES 80
	#define Y_RES 72
	#define X_COLS 40
	#define STARTUP_P1_X 12
	#define STARTUP_P2_X 67
	#define STARTUP_Y 39
/*
// SPC-1000 resolution is now 64x48, see above
#elif defined(__SPC1000__)	// Samsung SPC-1000
	#define X_RES 64
	#define Y_RES 32
	#define X_COLS 32
	#define STARTUP_P1_X 10
	#define STARTUP_P2_X 53
	#define STARTUP_Y 17
	/*
	#define X_RES 32	// normally 64, but on this system we double the pixels horizontally to get a better aspect ratio
	#define Y_RES 32
	#define X_COLS 32
	#define STARTUP_P1_X 5	// normally 10, see above
	#define STARTUP_P2_X 26 // normally 53, see above
	#define STARTUP_Y 17
	*/
*/
#elif defined(__MULTI8__)	// Mitsubishi Multi8 (640x200)
	#define X_RES 80		// normally 160, but on this system we double the pixels horizontally to get a better aspect ratio
	#define Y_RES 50
	#define X_COLS 80
	#define STARTUP_P1_X 12	// normally 25, see above
	#define STARTUP_P2_X 67	// normally 134, see above
	#define STARTUP_Y 27
#elif defined(__CPC__)		// Amstrad/Schneider CPC (640x200)
	#define X_RES 80		// normally 160, but on this system we double the pixels horizontally to get a better aspect ratio
	#define Y_RES 50
	#define X_COLS 40
	#define STARTUP_P1_X 12	// normally 25, see above
	#define STARTUP_P2_X 67	// normally 134, see above
	#define STARTUP_Y 27
#elif defined(__RX78__)	// Bandai RX-78
	#define X_RES 48
	#define Y_RES 46
	#define X_COLS 24
	#define STARTUP_P1_X 7
	#define STARTUP_P2_X 40
	#define STARTUP_Y 25
#elif defined(__GAMEBOY__)	// Nintendo Gameboy
	#define X_RES 40
	#define Y_RES 36
	#define X_COLS 20
	#define STARTUP_P1_X 6
	#define STARTUP_P2_X 33
	#define STARTUP_Y 19
#elif defined(__KC__) // Robotron HC-900, KC85/2..KC85/5
	#define X_RES 80
	#define Y_RES 64	
	#define X_COLS 40
	#define STARTUP_P1_X 12
	#define STARTUP_P2_X 67
	#define STARTUP_Y 35
#elif defined(__OSBORNE1__)
	#define X_RES 52 // normally 104, but on this system we double the pixels horizontally to get a better aspect ratio
	#define Y_RES 48	
	#define X_COLS 52
	#define STARTUP_P1_X 8  // normally 16, see above
	#define STARTUP_P2_X 43 // normally 87, see above
	#define STARTUP_Y 26
#elif defined(__EINSTEIN80COLUMNS__) // || defined(__RC700__) // Tatung Einstein 80 column mode
	#define X_RES 40	// normally 80, but on this system we double the pixels horizontally to get a better aspect ratio
	#define Y_RES 25
	#define X_COLS 80
	#define STARTUP_P1_X 6	// normally 12, see above
	#define STARTUP_P2_X 33	// normally 67, see above
	#define STARTUP_Y 13
#elif defined(__RC700__) // RC-700
	#define X_RES 80
	#define Y_RES 25
	#define X_COLS 80
	#define STARTUP_P1_X 12
	#define STARTUP_P2_X 67
	#define STARTUP_Y 13
#elif defined(__BEE__) // Microbee
	#define X_RES 80	// normally 160, but on this system we double the pixels horizontally to get a better aspect ratio
	#define Y_RES 48
	#define X_COLS 80
	#define STARTUP_P1_X 12	// normally 25, see above
	#define STARTUP_P2_X 67	// normally 134, see above
	#define STARTUP_Y 26
#elif defined(__TIKI100__) || defined(__HOMELAB__) //  || defined(__MIKRO80__)    // Tiki-100 (also known as "Kontiki-100"), Homelab 3/4, Mikro-80
	#define X_RES 128
	#define Y_RES 64
	#define X_COLS 64
	#define STARTUP_P1_X 20
	#define STARTUP_P2_X 107
	#define STARTUP_Y 35
#elif defined(__MIKRO80__)    // Mikro-80
	#define X_RES 64	// normally 128, but on this system we double the pixels horizontally to get a better aspect ratio
	#define Y_RES 64
	#define X_COLS 64
	#define STARTUP_P1_X 10		// normally 20, see above
	#define STARTUP_P2_X 53		// normally 107, see above
	#define STARTUP_Y 35	
/*
#elif defined(__HOMELAB__)	// Homelab 3/4
	#define X_RES 64	// normally 128, but on this system we double the pixels horizontally to get a better aspect ratio
	#define Y_RES 64
	#define X_COLS 64
	#define STARTUP_P1_X 10		// normally 20, see above
	#define STARTUP_P2_X 53		// normally 107, see above
	#define STARTUP_Y 35
*/
#elif defined(__SUPER80__) && defined(__SUPER80_VDUEM__)		// Super-80 r or v model
	#define X_RES 80 // normally 160, but on this system we double the pixels horizontally to get a better aspect ratio
	#define Y_RES 50
	#define X_COLS 80
	#define STARTUP_P1_X 12	// normally 25, see above
	#define STARTUP_P2_X 67	// normally 134, see above
	#define STARTUP_Y 27	
#elif defined(__PACMAN__)	// PacMan-Hardware
	#define X_RES 56
	#define Y_RES 72
	#define X_COLS 28
	#define STARTUP_P1_X 8
	#define STARTUP_P2_X 47
	#define STARTUP_Y 39
#elif defined(__Z80TVGAME__)	// Z80 TV-Game (homebrew)
	#define X_RES 42
	#define Y_RES 52
	#define X_COLS 21
	#define STARTUP_P1_X 6
	#define STARTUP_P2_X 35
	#define STARTUP_Y 28
#elif defined(__ATTACHE__)	// Otrona Attache
	#define X_RES 80
	#define Y_RES 60
	#define X_COLS 80
	#define STARTUP_P1_X 12	// normally 25, see above
	#define STARTUP_P2_X 67	// normally 134, see above
	#define STARTUP_Y 31
#elif defined(__PC88__)		// NEC PC-800x
	#define X_RES 160
	#define Y_RES 100
	#define X_COLS 40
	#define STARTUP_P1_X 25
	#define STARTUP_P2_X 134
	#define STARTUP_Y 54	
#elif defined(__PMD85__)
	#define X_RES 72
	#define Y_RES 64
	#define X_COLS 48
	#define STARTUP_P1_X 11
	#define STARTUP_P2_X 60
	#define STARTUP_Y 35	
#else
	#define X_RES 0
	#define Y_RES 0	
	#define X_COLS 0
	#define STARTUP_P1_X 0
	#define STARTUP_P2_X 0
	#define STARTUP_Y 0
#endif 

// Delay Values
// These values are milliseconds
#define DELAY_DEMO_BEFOREROUNDSTART 1000
#define DELAY_AFTER_CRASH 250

// We adjust the speed for each system so that in 1-player mode it takes the player 
// about 5,5 seconds to horizontally cross the entire screen.
// On the higher-resolution systems (VZ200, EG2000), we set the 
// values so that this takes about 7,5 seconds.
#if defined(__ZX81__) || defined(__ZX80__) 	// ZX81, ZX80
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 0
	#define DELAY_GAME_ONEPLAYER 5
	#define DELAY_GAME_TWOPLAYERS 18
#elif defined(__ACE__)		// Jupiter Ace
	#define DELAY_BEFORE_NEWKEY 0
	#define DELAY_GAME_DEMO 40
	#define DELAY_GAME_ONEPLAYER 50
	#define DELAY_GAME_TWOPLAYERS 60
#elif defined(__AQUARIUS__)	// Mattel Aquarius
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 20
	#define DELAY_GAME_ONEPLAYER 30
	#define DELAY_GAME_TWOPLAYERS 40
#elif defined(__Z1013__)	// Robotron Z1013
	// Check if "Z1013_2MHZ" has been defined in the compile parameters
	#if !defined(Z1013_2MHZ) // 1 MHz values
		#define DELAY_BEFORE_NEWKEY 50
		#define DELAY_GAME_DEMO 0
		#define DELAY_GAME_ONEPLAYER 6
		#define DELAY_GAME_TWOPLAYERS 60
	#else // 2 MHz values
		#define DELAY_BEFORE_NEWKEY 50
		#define DELAY_GAME_DEMO 20
		#define DELAY_GAME_ONEPLAYER 75
		#define DELAY_GAME_TWOPLAYERS 130
	#endif	
#elif defined(__Z9001__)	// Robotron Z9001
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 15
	#define DELAY_GAME_ONEPLAYER 32
	#define DELAY_GAME_TWOPLAYERS 55
#elif defined(__P2000__)	// Philips P2000
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 12
	#define DELAY_GAME_ONEPLAYER 35
	#define DELAY_GAME_TWOPLAYERS 60
#elif defined(__VG5000__)	// Philips VG5000
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 30
	#define DELAY_GAME_ONEPLAYER 40
	#define DELAY_GAME_TWOPLAYERS 50
#elif defined(__TRS80__) && !defined(__EG2000__)  // TRS-80
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 0
	#define DELAY_GAME_ONEPLAYER 4
	#define DELAY_GAME_TWOPLAYERS 60
#elif defined(__TRS80__) && defined(__EG2000__)  // EG2000
	// here it takes about 7,5 seconds in one-player mode to horizontally move across the entire screen
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 0
	#define DELAY_GAME_ONEPLAYER 0
	#define DELAY_GAME_TWOPLAYERS 27
#elif defined(__SHARPMZ__)	// Sharp MZ
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 20
	#define DELAY_GAME_ONEPLAYER 30
	#define DELAY_GAME_TWOPLAYERS 40
#elif defined(__VZ200__)  // VZ200 / Laser 310/210/110
	#if !defined(VZ200_LGFX)
		// 128x64 values
		// here it takes about 7,5 seconds in one-player mode to horizontally move across the entire screen
		#define DELAY_BEFORE_NEWKEY 50
		#define DELAY_GAME_DEMO 15
		#define DELAY_GAME_ONEPLAYER 25
		#define DELAY_GAME_TWOPLAYERS 40
	#else
		// 64x32 values
		#define DELAY_BEFORE_NEWKEY 50
		#define DELAY_GAME_DEMO 40
		#define DELAY_GAME_ONEPLAYER 50
		#define DELAY_GAME_TWOPLAYERS 65
	#endif
#elif defined(__SORCERER__) // Exidy Sorcerer
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 3
	#define DELAY_GAME_ONEPLAYER 27
	#define DELAY_GAME_TWOPLAYERS 57
#elif defined(__PV1000__)	// Casio PV-1000
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 0
	#define DELAY_GAME_ONEPLAYER 8
	#define DELAY_GAME_TWOPLAYERS 20
#elif defined(__ALPHATRO__)  // Triumph-Adler Alphatronic PC
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 10
	#define DELAY_GAME_ONEPLAYER 26
	#define DELAY_GAME_TWOPLAYERS 50
#elif defined(__SPC1000__)	// Samsung SPC-1000
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 60
	#define DELAY_GAME_ONEPLAYER 65
	#define DELAY_GAME_TWOPLAYERS 75
#elif defined(__MULTI8__)  // Mitsubishi Multi8
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 10
	#define DELAY_GAME_ONEPLAYER 20
	#define DELAY_GAME_TWOPLAYERS 40
#elif defined(__COLECO__)	// ColecoVision or Bit-90
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 35
	#define DELAY_GAME_ONEPLAYER 45
	#define DELAY_GAME_TWOPLAYERS 58
#elif defined(__RX78__)	// Bandai RX-78	- due to the smaller screen, we let it take 5 seconds
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 50
	#define DELAY_GAME_ONEPLAYER 60
	#define DELAY_GAME_TWOPLAYERS 70
#elif defined(__PV2000__)	// Casio PV-2000
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 35
	#define DELAY_GAME_ONEPLAYER 45
	#define DELAY_GAME_TWOPLAYERS 58
#elif defined(__NASCOM__)	// Nascom
	// Note: Unfortunately, this system is not powerful enough for this game.
    // Only the 2-player mode is playable (on a 4 MHz Nascom at about 90% of the desired speed).
	// EDIT: the accurate speed of the original hardware is still unknown, because the emulators 
	// run at different speeds.	
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 0
	#define DELAY_GAME_ONEPLAYER 0
	#define DELAY_GAME_TWOPLAYERS 0
#elif defined(__SPECTRUM__)	// ZX Spectrum
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 35
	#define DELAY_GAME_ONEPLAYER 45
	#define DELAY_GAME_TWOPLAYERS 62
#elif defined(__SC3000__)	// Sega SC-3000
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 20
	#define DELAY_GAME_ONEPLAYER 35
	#define DELAY_GAME_TWOPLAYERS 55
#elif defined(__PC6001__)	// NEC PC-6001 - these are the values for the Mk2 model, the original machine was slower
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 0
	#define DELAY_GAME_ONEPLAYER 12
	#define DELAY_GAME_TWOPLAYERS 27
#elif defined(__MSX__)	// MSX
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 20
	#define DELAY_GAME_ONEPLAYER 30
	#define DELAY_GAME_TWOPLAYERS 50
#elif defined(__M5__)	// Sord M5
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 30
	#define DELAY_GAME_ONEPLAYER 39
	#define DELAY_GAME_TWOPLAYERS 60
#elif defined(__SVI__)	// Spectravideo SVI
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 20
	#define DELAY_GAME_ONEPLAYER 35
	#define DELAY_GAME_TWOPLAYERS 55
#elif defined(__KC__)	// Robotron KC85/2..KC85/5 and HC900
	// Note: Unfortunately, this system is not powerful enough for this game.
	// One-Player mode is a little slower than it should be
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 0
	#define DELAY_GAME_ONEPLAYER 0
	#define DELAY_GAME_TWOPLAYERS 30
#elif defined(__LASER500__) // VTech Laser 350/500/700
// MAME 0.201 delay values - in MAME the game runs faster than on the real hardware
/*
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 30
	#define DELAY_GAME_ONEPLAYER 40
	#define DELAY_GAME_TWOPLAYERS 55
*/
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 15
	#define DELAY_GAME_ONEPLAYER 21
	#define DELAY_GAME_TWOPLAYERS 35
#elif defined(__EINSTEIN__)  // Tatung Einstein
	#if !defined(__EINSTEIN80COLUMNS__)	// 40 column mode
		#define DELAY_BEFORE_NEWKEY 50
		#define DELAY_GAME_DEMO 30
		#define DELAY_GAME_ONEPLAYER 40
		#define DELAY_GAME_TWOPLAYERS 55
	#else	// 80 column mode (TK02)
		#define DELAY_BEFORE_NEWKEY 50
		#define DELAY_GAME_DEMO 80
		#define DELAY_GAME_ONEPLAYER 90
		#define DELAY_GAME_TWOPLAYERS 100
	#endif
#elif defined(__OSBORNE1__)	// Osborne 1
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 60
	#define DELAY_GAME_ONEPLAYER 70
	#define DELAY_GAME_TWOPLAYERS 82
#elif defined(__LYNX__)	// Camputers Lynx
	#if defined(__LYNXSTANDARDCONSOLE__)
		// Standard console (ROM printer)
		#define DELAY_BEFORE_NEWKEY 50
		#define DELAY_GAME_DEMO 4
		#define DELAY_GAME_ONEPLAYER 20
		#define DELAY_GAME_TWOPLAYERS 50
	#else
		// Generic console
		#define DELAY_BEFORE_NEWKEY 50
		#define DELAY_GAME_DEMO 2
		#define DELAY_GAME_ONEPLAYER 19
		#define DELAY_GAME_TWOPLAYERS 50
	#endif
#elif defined(__MTX__)	// Memotech MTX
	#define DELAY_BEFORE_NEWKEY 30
	#define DELAY_GAME_DEMO 30
	#define DELAY_GAME_ONEPLAYER 40
	#define DELAY_GAME_TWOPLAYERS 55
#elif defined(__CPC__)  // Amstrad/Schneider CPC
	#define DELAY_BEFORE_NEWKEY 10
	#define DELAY_GAME_DEMO 5
	#define DELAY_GAME_ONEPLAYER 10
	#define DELAY_GAME_TWOPLAYERS 25
#elif defined(__MC1000__)	// CCE MC-1000
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 20
	#define DELAY_GAME_ONEPLAYER 30
	#define DELAY_GAME_TWOPLAYERS 50
#elif defined (__BEE__) // Microbee
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 15
	#define DELAY_GAME_ONEPLAYER 30
	#define DELAY_GAME_TWOPLAYERS 40
#elif defined(__TIKI100__)  // Tiki-100 (also known as "Kontiki-100")
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 5
	#define DELAY_GAME_ONEPLAYER 15
	#define DELAY_GAME_TWOPLAYERS 33
#elif defined(__SUPER80__) && !defined(__SUPER80_VDUEM__) // Super-80 e or m model
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 0
	#define DELAY_GAME_ONEPLAYER 3
	#define DELAY_GAME_TWOPLAYERS 30
#elif defined(__SUPER80__) && defined(__SUPER80_VDUEM__) // Super-80 r or v model
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 5
	#define DELAY_GAME_ONEPLAYER 17
	#define DELAY_GAME_TWOPLAYERS 45
#elif defined(__PACMAN__)	// PacMan-Hardware
	// PROVISIONAL VALUES
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 35
	#define DELAY_GAME_ONEPLAYER 60
	#define DELAY_GAME_TWOPLAYERS 90
#elif (defined(__GAL__) && !defined(__GALPLUSHIRES__))	// Galaksija
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 0
	#define DELAY_GAME_ONEPLAYER 1	// still too slow, but 1 instead of 0 to avoid massive speed-ups when the computer player is within closely spaced walls
	#define DELAY_GAME_TWOPLAYERS 20
#elif (defined(__GAL__) && defined(__GALPLUSHIRES__))	// Galaksija Plus Hi-Res
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 0
	#define DELAY_GAME_ONEPLAYER 1	// still too slow, but 1 instead of 0 to avoid massive speed-ups when the computer player is within closely spaced walls
	#define DELAY_GAME_TWOPLAYERS 16
#elif defined(__SMC777__)
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 25
	#define DELAY_GAME_ONEPLAYER 35
	#define DELAY_GAME_TWOPLAYERS 50
#elif defined(__Z80TVGAME__)	// Z80 TV-Game (homebrew)
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 50
	#define DELAY_GAME_ONEPLAYER 60
	#define DELAY_GAME_TWOPLAYERS 70
#elif defined(__ATTACHE__)	// Otrona Attache
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 18
	#define DELAY_GAME_ONEPLAYER 30
	#define DELAY_GAME_TWOPLAYERS 47
#elif defined(__EXCALIBUR64__)
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 30
	#define DELAY_GAME_ONEPLAYER 40
	#define DELAY_GAME_TWOPLAYERS 55
#elif defined(__GAMEBOY__)
	#define DELAY_BEFORE_NEWKEY 10
	#define DELAY_GAME_DEMO 80
	#define DELAY_GAME_ONEPLAYER 95
	#define DELAY_GAME_TWOPLAYERS 110
#elif defined(__RC700__)
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 80
	#define DELAY_GAME_ONEPLAYER 90
	#define DELAY_GAME_TWOPLAYERS 100
#elif defined(__PC88__)
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 45
	#define DELAY_GAME_ONEPLAYER 55
	#define DELAY_GAME_TWOPLAYERS 65
#elif defined(__X1__)
	#define DELAY_BEFORE_NEWKEY 100
	#define DELAY_GAME_DEMO 13
	#define DELAY_GAME_ONEPLAYER 24
	#define DELAY_GAME_TWOPLAYERS 45
#elif defined(__FP1100__)
	#define DELAY_BEFORE_NEWKEY 20
	#define DELAY_GAME_DEMO 25
	#define DELAY_GAME_ONEPLAYER 35
	#define DELAY_GAME_TWOPLAYERS 47
#elif defined(__MYVISION__)
	#define DELAY_BEFORE_NEWKEY 10
	#define DELAY_GAME_DEMO 12
	#define DELAY_GAME_ONEPLAYER 27
	#define DELAY_GAME_TWOPLAYERS 45
#elif defined(__PENCIL2__)
	#define DELAY_BEFORE_NEWKEY 70
	#define DELAY_GAME_DEMO 35
	#define DELAY_GAME_ONEPLAYER 45
	#define DELAY_GAME_TWOPLAYERS 63
#elif defined(__HGMC__)		// Hübler-Grafik-MC
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 0
	#define DELAY_GAME_ONEPLAYER 22
	#define DELAY_GAME_TWOPLAYERS 55
#elif defined(__PMD85__)
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 0
	#define DELAY_GAME_ONEPLAYER 0
	#define DELAY_GAME_TWOPLAYERS 38
#elif defined(__VECTOR06C__)
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 0
	#define DELAY_GAME_ONEPLAYER 15
	#define DELAY_GAME_TWOPLAYERS 43
#elif defined(__HOMELAB__)
	#define DELAY_BEFORE_NEWKEY 100
	#define DELAY_GAME_DEMO 15
	#define DELAY_GAME_ONEPLAYER 27
	#define DELAY_GAME_TWOPLAYERS 45
#elif defined(__HOMELAB2__)
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 45
	#define DELAY_GAME_ONEPLAYER 60
	#define DELAY_GAME_TWOPLAYERS 75
#elif defined(__PRIMO__)
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 25
	#define DELAY_GAME_ONEPLAYER 40
	#define DELAY_GAME_TWOPLAYERS 60
#elif defined(__MIKRO80__)
	// #define DELAY_BEFORE_NEWKEY 50
	// #define DELAY_GAME_DEMO 2
	// #define DELAY_GAME_ONEPLAYER 20
	// #define DELAY_GAME_TWOPLAYERS 45
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 24
	#define DELAY_GAME_ONEPLAYER 53
	#define DELAY_GAME_TWOPLAYERS 81
#else		// default values from Mattel Aquarius, need to be adjusted for each new supported target
	#define DELAY_BEFORE_NEWKEY 50
	#define DELAY_GAME_DEMO 20
	#define DELAY_GAME_ONEPLAYER 30
	#define DELAY_GAME_TWOPLAYERS 40
#endif 

// Text constants
#if defined(SMALL_LETTERS_ONLY) || defined(FOUR_BY_SIX_CHARACTERS_WIDE_SCREEN)
	#define SCORE_COMP1 "computer 1: %d"
	#define SCORE_COMP2 "computer 2: %d"
	#define SCORE_COMP "computer: %d"
	#define SCORE_HUMAN "human: %d"
	#define SCORE_P1 "player 1: %d"
	#define SCORE_P2 "player 2: %d"

	#define MENU_LINE1 "*** h-tron ***\n"
	#define MENU_LINE2 "by robertk (2016, 2018)\n"
	
	#if defined(JOYSTICK_ONLY)
		#define MENU_LINE3 "\nuse joystick left/right\n"
		#define MENU_LINE4 "to select game type\n\n"
	#else
		#define MENU_LINE3 "controls:\n\n"
		#define MENU_LINE4 "turn left/right:\n"
		#define MENU_LINE5 " left player: a/s\n"
		#define MENU_LINE6 " right player: n/m or 5/8\n"
		#define MENU_LINE8 " (in one-player mode,\n"
		#define MENU_LINE9 "  human is right player)\n\n"
		
		// On the Galaksija, we need to save one line to make everything fit on the screen 
		#if (defined(__GAL__) && !defined(__GALPLUSHIRES__))
			#define MENU_LINE10 "pause game: p, end game: x\n"
		#else
			#define MENU_LINE10 "pause game: p, end game: x\n\n"			
		#endif
		
		#define MENU_LINE11 "press space or 0\n"
		#define MENU_LINE12 "to start a round\n\n"
		#define MENU_LINE13 "one or two players (1/2)?\n"
		#define MENU_LINE14 "(or press d for demo mode)"
	#endif
		
	#define END_FINALSCORE "final score"

	#define END_COMP1_WINS "computer player one wins\n"
	#define END_P1_WINS "player one wins\n"
	#define END_COMP_WINS "computer wins\n"
	#define END_COMP2_WINS "computer player two wins\n"
	#define END_P2_WINS "player two wins\n"
	#define END_YOU_WIN "you win\n"
	#define END_A_DRAW "a draw\n"
	
	#if !defined(JOYSTICK_ONLY)
		#define END_PLAY_AGAIN "\nplay again (y/n)?\n\n"
		#define END_M_FOR_MENU "(press m to return to the menu)\n"
	#else
		#define END_PLAY_AGAIN "\npress fire to play again\n\n"
		#define END_M_FOR_MENU "(move joystick up for menu)\n"
	#endif 	

#elif defined(FOUR_BY_SIX_CHARACTERS)
	#define SCORE_COMP1 "comp 1: %d"
	#define SCORE_COMP2 "comp 2: %d"
	#define SCORE_COMP "comp: %d"
	#define SCORE_HUMAN "human: %d"
	#define SCORE_P1 "plr 1: %d"
	#define SCORE_P2 "plr 2: %d"

	#define MENU_LINE1 "*** h-tron ***\n"
	#define MENU_LINE2 "by robertk\n(2016, 2018)\n"
	#define MENU_LINE3 "\ncontrols:\n\n"
	#define MENU_LINE4 "turn left/right:\n"
	#if defined(__VG5000__) // this machine from France has an AZERTY keyboard
		#define MENU_LINE5 " left player: q/s\n"
		#define MENU_LINE6 " right player: n/,\n or 5/8\n"
	#else
		#define MENU_LINE5 " left player: a/s\n"
		#define MENU_LINE6 " right player: n/m\n or 5/8\n"
	#endif
	#define MENU_LINE8 "(in 1-player mode,\n"
	#define MENU_LINE9 " human is right pl.)\n"
	#define MENU_LINE10 "pause game: p\nend game: x\n\n"
	#define MENU_LINE11 "press space or 0\n"
	#define MENU_LINE12 "to start a round\n\n"
	#define MENU_LINE13 "(1) or (2) players?\n"
	#define MENU_LINE14 "(or d for demo mode)"

	#define END_FINALSCORE "final score"

	#define END_COMP1_WINS "computer player one wins\n"
	#define END_P1_WINS "player one wins\n"
	#define END_COMP_WINS "computer wins\n"
	#define END_COMP2_WINS "computer player two wins\n"
	#define END_P2_WINS "player two wins\n"
	#define END_YOU_WIN "you win\n"
	#define END_A_DRAW "a draw\n"
	#define END_PLAY_AGAIN "\nplay again (y/n)?\n\n"
	#define END_M_FOR_MENU "(press m to return\nto the menu)\n"
	
	
#elif defined(STANDARD_CHARACTERS_NARROW_SCREEN)
	#define SCORE_COMP1 "Comp 1: %d"
	#define SCORE_COMP2 "Comp 2: %d"
	#define SCORE_COMP "Comp: %d"
	#define SCORE_HUMAN "Human: %d"
	#define SCORE_P1 "Plr 1: %d"
	#define SCORE_P2 "Plr 2: %d"
	
	#define MENU_LINE1 "*** H-Tron ***\n"
	#define MENU_LINE2 "by RobertK (2016, 2018)\n"
	
	#if defined(JOYSTICK_ONLY)
		#define MENU_LINE3 "\nUse Joystick left/right\n"
		#define MENU_LINE4 "to select game type\n\n"
	#else	
		#define MENU_LINE3 "Controls:\n\n"
		#define MENU_LINE4 "Turn left/right:\n"
		#define MENU_LINE5 " Left player: A/S\n"
		#define MENU_LINE6 " Right player: N/M or 5/8\n"
		#define MENU_LINE8 " (in one-player mode,\n"
		#define MENU_LINE9 "  human is right player)\n\n"
		#define MENU_LINE10 "Pause game: P, End Game: X\n\n"
		#define MENU_LINE11 "Press Space or 0\n"
		#define MENU_LINE12 "to start a round\n\n"
		#define MENU_LINE13 "One or two players (1/2)?\n"
		#define MENU_LINE14 "(or press D for demo mode)"
	#endif 
	
	#define END_FINALSCORE "Final score"

	#define END_COMP1_WINS "Computer player one wins\n"
	#define END_P1_WINS "Player one wins\n"
	#define END_COMP_WINS "Computer wins\n"
	#define END_COMP2_WINS "Computer player two wins\n"
	#define END_P2_WINS "Player two wins\n"
	#define END_YOU_WIN "You win\n"
	#define END_A_DRAW "A draw\n"
	
	#if !defined(JOYSTICK_ONLY)
		#define END_PLAY_AGAIN "\nPlay again (y/n)?\n\n"
		#define END_M_FOR_MENU "(press M to return to the menu)\n"
	#else
		#define END_PLAY_AGAIN "\nPress fire to play again\n\n"
		#define END_M_FOR_MENU "(move joystick up for menu)\n"
	#endif 

#elif defined(STANDARD_CHARACTERS_NARROW_BUT_TALL_SCREEN)
	#define SCORE_COMP1 "Comp 1: %d"
	#define SCORE_COMP2 "Comp 2: %d"
	#define SCORE_COMP "Comp: %d"
	#define SCORE_HUMAN "Human: %d"
	#define SCORE_P1 "Plr 1: %d"
	#define SCORE_P2 "Plr 2: %d"
	
	#define MENU_LINE1 "*** H-Tron ***\n"
	#define MENU_LINE2 "by RobertK (2016, 2018)\n"
	
	#if defined(JOYSTICK_ONLY)
		#define MENU_LINE3 "\nUse Joystick left/right\n"
		#define MENU_LINE4 "to select game type\n\n"
	#else	
		#define MENU_LINE3 "Controls:\n\n"
		#define MENU_LINE4 "Turn left/right:\n"
		#define MENU_LINE5 " Left player: A/S\n"
		#define MENU_LINE6 " Right plr: N/M or 5/8\n"
		#define MENU_LINE8 " (in one-player mode,\n"
		#define MENU_LINE9 "  human is right player)\n\n"
		#define MENU_LINE10 "Pause: P, End Game: X\n\n"
		#define MENU_LINE11 "Press Space or 0\n"
		#define MENU_LINE12 "to start a round\n\n"
		#define MENU_LINE13 "One or two players?\n"
		#define MENU_LINE14 "Press 1 or 2, or \nD for demo mode..."
	#endif 
	
	#define END_FINALSCORE "Final score"

	#define END_COMP1_WINS "Computer player one wins\n"
	#define END_P1_WINS "Player one wins\n"
	#define END_COMP_WINS "Computer wins\n"
	#define END_COMP2_WINS "Computer player two wins\n"
	#define END_P2_WINS "Player two wins\n"
	#define END_YOU_WIN "You win\n"
	#define END_A_DRAW "A draw\n"
	
	#if !defined(JOYSTICK_ONLY)
		#define END_PLAY_AGAIN "\nPlay again (y/n)?\n\n"
		#define END_M_FOR_MENU "(press M to return \nto the menu)\n"
	#else
		#define END_PLAY_AGAIN "\nPress fire to play again\n\n"
		#define END_M_FOR_MENU "(move joystick up for menu)\n"
	#endif 
	
	
#elif defined(STANDARD_CHARACTERS_VERYNARROW_BUT_TALL_SCREEN)
	
	#if defined(__GAMEBOY__)
		#define SCORE_COMP1 "Cmp 1: %d"
		#define SCORE_COMP2 "Cmp 2: %d"
	#else
		#define SCORE_COMP1 "Comp 1: %d"
		#define SCORE_COMP2 "Comp 2: %d"
	#endif
	
	#define SCORE_COMP "Comp: %d"
	#define SCORE_HUMAN "Human: %d"
	#define SCORE_P1 "Plr 1: %d"
	#define SCORE_P2 "Plr 2: %d"
	
	#define MENU_LINE1 "*** H-Tron ***\n"
	#define MENU_LINE2 "RobertK (2016, 2018)\n"
	
	#if defined(JOYSTICK_ONLY)
		#define MENU_LINE3 "\nUse Joy left/right\n"
		#define MENU_LINE4 "to select game type\n\n"
	#else
		// these texts are untested
		#define MENU_LINE3 "Controls:\n\n"
		#define MENU_LINE4 "Turn left/right:\n"
		#define MENU_LINE5 " Left player: A/S\n"
		#define MENU_LINE6 " Right plr: N/M or 5/8\n"
		#define MENU_LINE8 " (in one-player mode,\n"
		#define MENU_LINE9 "  human is right player)\n\n"
		#define MENU_LINE10 "Pause: P, End Game: X\n\n"
		#define MENU_LINE11 "Press Space or 0\n"
		#define MENU_LINE12 "to start a round\n\n"
		#define MENU_LINE13 "One or two players?\n"
		#define MENU_LINE14 "Press 1 or 2, or \nD for demo mode..."
	#endif 
	
	#define END_FINALSCORE "Final score"

	#define END_COMP1_WINS "Computer player one wins\n"
	#define END_P1_WINS "Player one wins\n"
	#define END_COMP_WINS "Computer wins\n"
	#define END_COMP2_WINS "Computer player two wins\n"
	#define END_P2_WINS "Player two wins\n"
	#define END_YOU_WIN "You win\n"
	#define END_A_DRAW "A draw\n"
	
	#if !defined(JOYSTICK_ONLY)
		#define END_PLAY_AGAIN "\nPlay again (y/n)?\n\n"
		#define END_M_FOR_MENU "(press M to return \nto the menu)\n"
	#else
		// these texts are untested
		#define END_PLAY_AGAIN "\nFire to play again\n\n"
		#define END_M_FOR_MENU "Joystick up for menu\n"
	#endif 

	
	
#else
	#define SCORE_COMP1 "Computer 1: %d"
	#define SCORE_COMP2 "Computer 2: %d"
	#define SCORE_COMP "Computer: %d"
	#define SCORE_HUMAN "Human: %d"
	#define SCORE_P1 "Player 1: %d"
	#define SCORE_P2 "Player 2: %d"

	#define MENU_LINE1 "*** H-Tron ***\n"
	#define MENU_LINE2 "by RobertK (2016, 2018)\n"
	
	#if defined(JOYSTICK_ONLY)
		#define MENU_LINE3 "\nUse Joystick left/right\n"
		#define MENU_LINE4 "to select game type\n\n"
	#else
		#define MENU_LINE3 "Controls:\n\n"
		#define MENU_LINE4 "Turn left/right:\n"
		#define MENU_LINE5 " Left player: A/S\n"
		#define MENU_LINE6 " Right player: N/M or 5/8\n"
		#define MENU_LINE8 " (in one-player mode,\n"
		#define MENU_LINE9 "  human is right player)\n\n"
		
		
		#if defined(__PC6001__)	|| (defined(__VZ200__) && defined(VZ200_LGFX)) || (defined(__SUPER80__) && !defined(__SUPER80_VDUEM__))
			// NEC PC-6001, VZ200 lo-res, Super-80 e or m model
			// On these systems, we need to save one line to make everything fit on the screen 
			#define MENU_LINE10 "Pause game: P, End Game: X\n"
		#else
			#define MENU_LINE10 "Pause game: P, End Game: X\n\n"
		#endif			

		#define MENU_LINE11 "Press Space or 0\n"
		#define MENU_LINE12 "to start a round\n\n"
		#define MENU_LINE13 "One or two players (1/2)?\n"
		#define MENU_LINE14 "(or press D for demo mode)"
	#endif 
	
	#define END_FINALSCORE "Final score"

	#define END_COMP1_WINS "Computer player one wins\n"
	#define END_P1_WINS "Player one wins\n"
	#define END_COMP_WINS "Computer wins\n"
	#define END_COMP2_WINS "Computer player two wins\n"
	#define END_P2_WINS "Player two wins\n"
	#define END_YOU_WIN "You win\n"
	#define END_A_DRAW "A draw\n"
	#if !defined(JOYSTICK_ONLY)
		#define END_PLAY_AGAIN "\nPlay again (y/n)?\n\n"
		#define END_M_FOR_MENU "(press M to return to the menu)\n"
	#else
		#define END_PLAY_AGAIN "\nPress fire to play again\n\n"
		#define END_M_FOR_MENU "(move joystick up for menu)\n"
	#endif 	
#endif 

#if (X_COLS>36) && defined(SMALL_LETTERS_ONLY)
	#define MENU_LINE11_12 "press space or 0 to start a round\n\n"
#elif (X_COLS>36) && !defined(SMALL_LETTERS_ONLY) 
	#define MENU_LINE11_12 "Press Space or 0 to start a round\n\n"
#endif 

/* indicates whether the player has hit an obstacle */
int p1Crashed;
int p2Crashed;

/* player coordinate variables */
int p1x,p1y,p2x,p2y;

/* player direction variables in radian degrees */
/* 0 (or 360) = right, 90 = up, 180 = left, 270 = down */
int p1dir,p2dir;

/* player score variables */
int p1Score,p2Score;

/* One or two players */
int numberOfPlayers;


void myTextOut(int x, int y, const char* text)
{
	// writes a text to the specified x,y (column and row) screen position
	// this is the place for the platform-dependent 
	// implementation of this function
    #if defined(__ZX81__) || defined(__ZX80__) // || defined(__SPECTRUM__)
		zx_setcursorpos(y,x);
		printf(text);
	#elif defined(__LYNXSTANDARDCONSOLE__)	// for the Camputers Lynx "ROM printer" (when compiling without the generic console parameter)
		// See page 62 of thy Lynx user manual
		// TODO
		/*
		printf("%c",23); // homes cursor
		for (int i = 1; i <= y; i++)
			printf("%c",10); // line feed
		for (int i = 1; i <= x; i++)
			printf("%c",12); // moves cursor one character block to the right
		*/
		printf(text);
	#elif defined(__PACMAN__) // PacMan-Hardware
		// TODO: Screen positioning on PacMan-HW?
		printf(text);
	#elif defined(FOUR_BY_SIX_CHARACTERS)
		x_4x6 = x*4;
		y_4x6 = y*6;
		printf(text);
	#elif defined(USE_NATIVE_CONSOLE)
		printf("\033=%c%c",y+32,x+32);
		printf(text);
	#elif defined(USE_ANSI) || defined(USE_GENERIC_CONSOLE)
		// ANSI VT100 (conio.h) or native console (including conio.h not required, e.g. TRS-80)
		gotoxy(x,y);
		cprintf(text);	
    #endif
}


void myTextOutXY(int x, int y, const char* text)
{
	// writes a text to the specified x,y (graphics coordinate) screen position
	// only for targets that support this (currently those with 4x6 character mode)
	#if defined(FOUR_BY_SIX_CHARACTERS)
		x_4x6 = x;
		y_4x6 = y;
		printf(text);
    #endif
}


void myPlot(short x, short y, short pmode)
{
	// sets or clears a pixel
	// pmode 1 plots, pmode 0 unplots
	// this is the place for the platform-dependent 
	// implementation of this function
	
	#if defined(__EINSTEIN80COLUMNS__)  // Tatung Einstein 80 column mode
		gotoxy(x*2,y);
		if (pmode==1)
		{
			printf("%c",160);
		}
		else
		{
			cprintf(" ");		
		}
		gotoxy(x*2+1,y);
		if (pmode==1)
		{
			printf("%c",160);			
		}
		else
		{
			cprintf(" ");		
		}
	#elif defined(DOUBLE_PIXELS_HORIZONTALLY)
		// graphics.h
		if (pmode==1)
		{
			#if defined(USE_C_PLOT)
				c_plot(x*2,y);
				c_plot(x*2 + 1,y);			
			#else
				plot(x*2,y);
				plot(x*2 + 1,y);
			#endif
		}
		else
		{ 
			#if defined(USE_C_PLOT)
				c_unplot(x*2,y);
				c_unplot(x*2 + 1,y);
			#else
				unplot(x*2,y);
				unplot(x*2 + 1,y);
			#endif
		}
		// zx81plot.h
		// zx_plot(x*2,y,pmode);	
		// zx_plot(x*2 + 1,y,pmode);			
	#else	
		#if defined(USE_C_PLOT)
			if (pmode==1) c_plot(x,y); else c_unplot(x,y);	// graphics.h
		#else
			if (pmode==1) plot(x,y); else unplot(x,y);	// graphics.h
		#endif
		// zx_plot(x,y,pmode);				// zx81plot.h	
    #endif
}


void myCls()	// clear the screen 
{
#if defined(__PACMAN__)
	printf("%c",12);
	printf("\x0c");  // the "\x0c" (0x0c character) resets the cursor position to the top left corner
#elif defined(USE_NATIVE_CONSOLE)
	printf("\032");
#elif defined(__LYNXSTANDARDCONSOLE__)	// for the Camputers Lynx "rom printer" (when compiling without the generic console parameter)
	printf("%c",4);		// See page 62 of thy Lynx user manual
	clg();
#else
	printf("%c",12);
	printf("\x0c");  // the "\x0c" (0x0c character) resets the cursor position to the top left corner
#endif
}

int myPoint(short x, short y)
{
	// checks the status of a pixel - returns 0 if unset or 1 if set
	// this is the place for the platform-dependent 
	// implementation of this function

	#if defined(__EINSTEIN80COLUMNS__)  // Tatung Einstein 80 column mode
		if (cvpeekr(x*2,y)==160) return 1; else return 0;
	#elif defined(DOUBLE_PIXELS_HORIZONTALLY)
		#if defined(USE_C_PLOT)
			return c_point(x*2,y);						// graphics.h
		#else
			return point(x*2,y);						// graphics.h
		#endif
		// return zx_point(x*2,y);					// zx81plot.h
	#else
		#if defined(USE_C_PLOT)
			return c_point(x,y);						// graphics.h
		#else
			return point(x,y);						// graphics.h
		#endif
		// return zx_point(x,y);					// zx81plot.h
    #endif
	
}


void myWait(uint msec)
{
	// waits a specified number of milliseconds
	// this is the place for the platform-dependent 
	// implementation of this function
	if (msec==0) return;
	msleep(msec);
	
	// The msleep() function should now be available on all platforms
	// (it runs the same code as the previously Sinclair-only in_Wait() function)

	// If there should be a future platform that does not support msleep(), 
	// we can wait by doing a busy loop:
	/*
    #if defined(__....__)
		// Taken from CROSS CHASE by Fabrizio Caruso (sleep_macros.c)
		// value is not in milliseconds, has to be adjusted for each platform		
		unsigned int ii; 
		for(ii=0;ii<msec;++ii){}; 
	#else
		msleep(msec);
	#endif
	*/	
}

uint myKeyPressed(uint scancode)
{
	#if defined(USE_INKEYPRESSED)
		return in_KeyPressed(scancode);
	#else
		// Currently such a key status function is implemented in z88dk 
		// only for a few computers. Provisionally we use ANSI key input 
		// for all other systems, although this is problematic in two-player 
		// mode as it does not allow two keys to be pressed simultaneously.
		return 0;
	#endif
}

void InitPlayfield()
{
  // sets up the initial playfield before a round starts
  int x,y;
  char textP1[25];
  char textP2[25];
		
  myCls();

  // Workaround for systems with a background colour problem: on these 
  // we simply fill the screen with black pixels to get a black background
  #if defined(__SPC1000__) || (defined(__VZ200__) && defined(VZ200_LGFX)) // || defined(__PC6001__)
	for (x = 0; x <= X_RES-1; ++x)
		for (y = 0; y <= Y_RES-1; ++y)
			myPlot(x,y,0);
  #endif
	
  /* Draw border */
  for (x = 0; x <= X_RES-1; ++x)
  {
	myPlot(x,Y_RES-1,1);
	myPlot(x,BORDER_TOP,1);
  }
  for (y = BORDER_TOP+1; y <= Y_RES-1; ++y)
  {
	myPlot(0,y,1);
	myPlot(X_RES-1,y,1);
  }
  // score area separator
  // we draw this only if the border was drawn from the top of the screen
  if (BORDER_TOP==0)
  {
	  for (x = 0; x <= X_RES-1; ++x)
		myPlot(x,SEPARATOR_LINE,1);
  }
  /* display P1 and P2 score */
  // we first use sprintf() for string formatting,
  // and then we put the formatted string on the 
  // screen using myTextOut()
 
  if (numberOfPlayers==0)
  {
   sprintf(textP1,SCORE_COMP1,p1Score);
   sprintf(textP2,SCORE_COMP2,p2Score);   
  }
  else if (numberOfPlayers==1)
  {
   sprintf(textP1,SCORE_COMP,p1Score);
   sprintf(textP2,SCORE_HUMAN,p2Score);
  }
  else
  {
   sprintf(textP1,SCORE_P1,p1Score);
   sprintf(textP2,SCORE_P2,p2Score);
  }

  myTextOut(SCORE_COL, SCORE_LINE, textP1);
  // Workaround for the Camputers Lynx standard console until screen positioning works
  #if defined(__LYNXSTANDARDCONSOLE__)
	  if (numberOfPlayers==0)      printf("              ");
	  else if (numberOfPlayers==1) printf("                     ");
	  else                         printf("                  ");
  #endif
  
  myTextOut(X_COLS-strlen(textP2)-SCORE_COL, SCORE_LINE, textP2);
  
  #if defined(__OSBORNE1__)	// Osborne 1
	// Workaround to hide the "_" cursor
	// We position it where there is already a plotted block
	// As the cursor is not XORed, it becomes invisible
	myTextOut(1, 2, "");
  #endif
  
  /* Initiate player positions, directions and crash indicators */
  p1x=STARTUP_P1_X; // ZX81: 10;	// DOS version: 15
  p1y=STARTUP_Y; 	// ZX81: 26;	// DOS version: 29
  p2x=STARTUP_P2_X; // ZX81: 53;	// DOS version: 74
  p2y=STARTUP_Y; 	// ZX81: 26;	// DOS version: 29
  p1dir=0;
  p2dir=180;
  p1Crashed=0;
  p2Crashed=0;

  /* Draw players at their start positions */
  myPlot(p1x,p1y,1);
  myPlot(p2x,p2y,1);
}


void MovePlayers()
{
  // Moves both players one step forward
  switch(p1dir)
  {
    case 0:   p1x=p1x+1; break;
    case 90:  p1y=p1y-1; break;
    case 180: p1x=p1x-1; break;
    case 270: p1y=p1y+1; break;
  }
  switch(p2dir)
  {
    case 0:   p2x=p2x+1; break;
    case 90:  p2y=p2y-1; break;
    case 180: p2x=p2x-1; break;
    case 270: p2y=p2y+1; break;
  }

  /* Collision detection */
  if (myPoint(p1x,p1y)==1)
  { 
    p1Crashed=1;
    p2Score++;
  }
  if (myPoint(p2x,p2y)==1)
  {
    p2Crashed=1;
    p1Score++;
  }
  /* no points when both players crashed */
  if (p1Crashed==1 && p2Crashed==1)
  {
   p1Score--;
   p2Score--;
  }
  
  /* Draw players at their new positions */
  /* (unless a collision has been detected) */
  if (p1Crashed==0) myPlot(p1x,p1y,1); 

  // ZX80: make the screen visible (as the ZX80 only runs in FAST mode, 
  // the screen is normally only visible during fgetc_cons() calls)
  #if defined(__ZX80__)
	gen_tv_field();
  #endif

  if (p2Crashed==0) myPlot(p2x,p2y,1);

  // ZX80: make the screen visible (as the ZX80 only runs in FAST mode, 
  // the screen is normally only visible during fgetc_cons() calls)
  #if defined(__ZX80__)
	gen_tv_field();
  #endif

}


void AIComputerPlayer(int *x, int *y, int *dir)
{
  /* Let the computer player decide whether to turn left or right,
     or continue straight on.
     The coordinate and direction variables are passed by reference,
     so we can use this function for either computer player one or two */

  /* number of empty grid blocks before an obstacle
     from the AI player's point of view */
  int distanceFront,distanceLeft,distanceRight;

// kkkk
// int debugOutputCounter = 0;
  
  int maxDistanceToChk;
  maxDistanceToChk=AI_MAX_DISTANCE_TO_CHECK;
  
  /* To make the AI player's movements less predictable,
  we occasionally make him a little short-sighted */
  maxDistanceToChk=maxDistanceToChk-(rand() % AI_SHORTSIGHTED_FACTOR);

// kkkk
// debugOutputCounter++;
// myPlot(2 + debugOutputCounter*2,1,1);				

  /* Check the distance from the AI player's position
     to the next obstacle in the front, left and right
     direction from the AI player's point of view. According
     to the players direction, we have to use the respective
     grid direction */
  switch(*dir)
  {
    case 0: /* right */
// myPlot(2 + debugOutputCounter*2,2,1);				
       distanceFront=AICheckDistanceEast(*x,*y,maxDistanceToChk);
       distanceLeft=AICheckDistanceNorth(*x,*y,maxDistanceToChk);
       distanceRight=AICheckDistanceSouth(*x,*y,maxDistanceToChk);
       break;
    case 90: /* up */
// myPlot(2 + debugOutputCounter*2,3,1);				
       distanceFront=AICheckDistanceNorth(*x,*y,maxDistanceToChk);
       distanceLeft=AICheckDistanceWest(*x,*y,maxDistanceToChk);
       distanceRight=AICheckDistanceEast(*x,*y,maxDistanceToChk);
       break;
    case 180: /* left */
// myPlot(2 + debugOutputCounter*2,4,1);				
       distanceFront=AICheckDistanceWest(*x,*y,maxDistanceToChk);
       distanceLeft=AICheckDistanceSouth(*x,*y,maxDistanceToChk);
       distanceRight=AICheckDistanceNorth(*x,*y,maxDistanceToChk);
       break;
    case 270: /* down */
// myPlot(2 + debugOutputCounter*2,5,1);				
       distanceFront=AICheckDistanceSouth(*x,*y,maxDistanceToChk);
       distanceLeft=AICheckDistanceEast(*x,*y,maxDistanceToChk);
       distanceRight=AICheckDistanceWest(*x,*y,maxDistanceToChk);
       break;
  }

// kkkk
// debugOutputCounter++;
// myPlot(2 + debugOutputCounter*2,1,1);				

  /* Now we decide what the AI player shall do */
  if ((distanceFront>=distanceLeft) && (distanceFront>=distanceRight))
  {
    /* clear sailing ahead, nothing to do */
  }
  else if ((distanceFront<distanceLeft) || (distanceFront<distanceRight))
  {
    /* now we know that it would be safer to turn either left or right */
    /* let us check which direction would be better */
    if (distanceLeft>distanceRight)
    {
    	/* turn left */
    	*dir = *dir+90;
    	if (*dir>=360) *dir=0;
    }
    else if (distanceLeft<distanceRight)
    {
        /* turn right */
		*dir = *dir-90;
		if (*dir<0) *dir=270;
    }
    else /* distanceLeft == distanceRight */
    {
		/* randomly turn either left or right */
		if ((rand() % 2)==0)
		{
		  /* turn left */
		  *dir = *dir+90;
		  if (*dir>=360) *dir=0;
		}
		else
		{
		  /* turn right */
		  *dir = *dir-90;
		  if (*dir<0) *dir=270;
		}
    }
  }
}


/* The following four functions calculate the distance from the
   given position to the next obstacle in grid map direction */
int AICheckDistanceEast(int x,int y,int maxDistanceToCheck)
{
  int distance;
  distance=1;
  while(myPoint(x+distance,y)==0 && distance<=maxDistanceToCheck)
  {
    distance++;
  }
  return distance;
}

int AICheckDistanceWest(int x,int y,int maxDistanceToCheck)
{
  int distance;
  distance=1;
  while(myPoint(x-distance,y)==0 && distance<=maxDistanceToCheck)
  {
    distance++;
  }
  return distance;
}

int AICheckDistanceNorth(int x,int y,int maxDistanceToCheck)
{
  int distance;
  distance=1;
  while(myPoint(x,y-distance)==0 && distance<=maxDistanceToCheck)
  {
    distance++;
  }
  return distance;
}

int AICheckDistanceSouth(int x,int y,int maxDistanceToCheck)
{
  int distance;
  distance=1;
  while(myPoint(x,y+distance)==0 && distance<=maxDistanceToCheck)
  {
    distance++;
  }
  return distance;
}

#if defined(JOYSTICK_ONLY)
void DisplayGameType(int gameType)
{
	#if defined(STANDARD_CHARACTERS_VERYNARROW_BUT_TALL_SCREEN)
		switch(gameType)
		{
			case 1: printf("1-Plr"); break;
			case 2: printf("2-Plr"); break;
			case 3: printf("Demo"); break;
		}
		printf(" Mode selected\n");
		printf("Fire to continue");
	#else
		switch(gameType)
		{
			case 1: printf("One-Player"); break;
			case 2: printf("Two-Player"); break;
			case 3: printf("Demo"); break;
		}
		printf(" Mode selected\n");
		printf("Press Fire to continue");
	#endif
}
#endif

void DoWhenProgramTerminates()
{
  #if defined(__MC1000__) || defined(__GALPLUSHIRES__) || defined(__SMC777__) || defined(__EXCALIBUR64__) || defined(__FP1100__)	// CCE MC-1000, Galaksija Plus Hi-Res, Sony SMC-777, Casio FP-1100
	int mode=0;
	console_ioctl(IOCTL_GENCON_SET_MODE, &mode);	// return to text mode (green/yellow):
	myCls();
  #elif defined(__TIKI100__)  // Tiki-100 (also known as "Kontiki-100")
	int mode=3; // set to standard display mode 256x256
	console_ioctl(IOCTL_GENCON_SET_MODE, &mode);
  #endif
}

void main()
{
  int exitgame;
  int key;
  #if defined(JOYSTICK_ONLY)
	int gameType=1;	// for Joystick menu selection; 1 = 1 player, 2 players, 3 = demo
	int joystickNumber1 = JOYSTICK_1;
	int joystickNumber2 = JOYSTICK_2;
	int joyButtonMenu=JOYSTICK_BUTTON_MENU;
	int joyButtonPause=JOYSTICK_BUTTON_PAUSE;
  #endif
  int notPlayAgain, restartAgain;
  int randomSeed=0;
  char c; // for ANSI keyboard input
  
  // Key press indicators
  // Note: internally P1 is always the "right" player, i.e. human in 
  // 1-player-mode and player 2 in two player mode. 
  int p1PressedLeft;
  int p1PressedRight;
  int p2PressedLeft;
  int p2PressedRight;
  int lastState_p1PressedLeft;
  int lastState_p1PressedRight;
  int lastState_p2PressedLeft;
  int lastState_p2PressedRight;

  int mode; // for IOCTL_GENCON_SET_MODE. Note that this requires a pointer parameter, so you cannot pass the value directly
	
  // Workaround for solving the "Visible Cursor Problem" occasionally occurring on the VZ 
  #if defined(__VZ200__)
  #asm
  di
  #endasm
  #endif
   
  restartAgain=0;
  // randomize();		// no longer available in nightly builds, we have to use srand(value) instead

  // Set the background and foreground colour of the block-graphics characters where necessary
  #if defined(__PC6001__) // NEC PC-6001
	mode=1
	console_ioctl(IOCTL_GENCON_SET_MODE, &mode);
	textcolor(WHITE);
	textbackground(BLACK);
  #elif (defined(__VZ200__) && defined(VZ200_LGFX)) // VZ200 / Laser 310/210/110
	// Note: in MAME, the block-graphics characters have a black background colour (it should be dark green)
	// Strangely, in VZEM the background colour is dark green as it should be.
	// This seems to have no effect:
	// textcolor(LIGHTGREEN);
	// textbackground(GREEN);
  #elif defined(__CPC__)
	textcolor(YELLOW);  // this gives only the text output a yellow colour, the plotted points are still red
  #endif

  // On some systems we first need to activate the graphics mode
  #if (defined(__VZ200__) && !defined(VZ200_LGFX)) || defined(__ABC80__) || defined(__P2000__) || defined(__Z9001__) || defined(__MULTI8__) || defined(__MSX__) || defined(__SVI__) || defined(__PACMAN__) || defined(__ATTACHE__)
	clg();  // activate graphics mode
  #elif defined(__MC1000__) // CCE MC-1000
	mode=1;
	console_ioctl(IOCTL_GENCON_SET_MODE, &mode);	// switch to hires mode
	clg();
  #elif defined(__GALPLUSHIRES__) || defined(__SMC777__) || defined(__EXCALIBUR64__) // Galaksija Plus Hi-Res, Sony SMC-777 or Excalibur 64
	mode=1;
	console_ioctl(IOCTL_GENCON_SET_MODE, &mode);	// switch to hires mode
  #elif defined(__FP1100__)	// Casio FP-1100
	mode=2;
	console_ioctl(IOCTL_GENCON_SET_MODE, &mode);	// switch to 80x48 lores mode	
  #elif defined(__EINSTEIN80COLUMNS__)  // Tatung Einstein 80 column mode
	mode=10;
	console_ioctl(IOCTL_GENCON_SET_MODE, &mode);	// switch the generic console to the 80 columns screen
  #elif defined(__TIKI100__)  // Tiki-100 (also known as "Kontiki-100")
	clg();		  // initiate graphicsets
	mode=2; // set to 512x256
	console_ioctl(IOCTL_GENCON_SET_MODE, &mode);
	// set palette to avoid the rather dark default cyan colour
	// the first value is the background colour
	// the last is used as the default foreground colour
	char palette[]={0,0xD2,0x7F,0xFF};
	gr_setpalette(4,palette);
	// textcolor(1);
  #endif
  
  do
  {
  
	  notPlayAgain=0;
	  myCls();
	  printf(MENU_LINE1);
	  printf(MENU_LINE2);
	  printf(MENU_LINE3);
	  printf(MENU_LINE4);	  
	  #if !defined(JOYSTICK_ONLY)
		  printf(MENU_LINE5);
		  printf(MENU_LINE6);

		  // when using the 4x6 character set, we show the rest on the second 
		  // screen, because of the very tall characters 
		  // in the graphics mode of these systems
		  #if defined(FOUR_BY_SIX_CHARACTERS) && !defined(FOUR_BY_SIX_CHARACTERS_TALL_SCREEN)
			getch();
			myCls();
		  #endif	  
		  printf(MENU_LINE8);
		  printf(MENU_LINE9);
		  printf(MENU_LINE10);
		  
		  #if defined(__SPC1000__)
			getch();
			myCls();
		  #endif
		  
		 #if defined(MENU_LINE11_12) // on some systems, we show the text from lines 11 and 12 in only one line
			printf(MENU_LINE11_12);
		 #else
			printf(MENU_LINE11);
			printf(MENU_LINE12);
		 #endif		 

		  printf(MENU_LINE13);
		  printf(MENU_LINE14);
		  // printf("\nxcols=%d\n",X_COLS);
	 #endif		 
	  
	  
	  #if defined(__ZX80__)
		gen_tv_field_init(0);	// has to be called once for screen-refreshing on the ZX80
	  #endif
	  
	  /*
	  myCls();
	  printf("\n");
	  printf("in_lookupkey() for 5 is %d\n",in_LookupKey('5'));	  
	  printf("in_lookupkey() for 8 is %d\n",in_LookupKey('8'));	  
	  printf("in_lookupkey() for a is %d\n",in_LookupKey('a'));	  
	  printf("in_lookupkey() for m is %d\n",in_LookupKey('m'));	  
	  printf("in_lookupkey() for n is %d\n",in_LookupKey('n'));
	  printf("in_lookupkey() for p is %d\n",in_LookupKey('p'));	  
	  printf("in_lookupkey() for s is %d\n",in_LookupKey('s'));	  
	  printf("in_lookupkey() for x is %d\n",in_LookupKey('x'));	  
	  */
	  
	  // VG5000
	  /*
	  myCls();
	  printf("in_lookupkey() for 5 is %d\n",in_LookupKey('5'));	  
	  printf("in_lookupkey() for 8 is %d\n",in_LookupKey('8'));	  
	  printf("in_lookupkey() for a is %d\n",in_LookupKey('q'));	  // Q instead of A
	  printf("in_lookupkey() for m is %d\n",in_LookupKey(','));	  // , instead of M
      printf("in_lookupkey() for n is %d\n",in_LookupKey('n'));
	  printf("in_lookupkey() for p is %d\n",in_LookupKey('p'));	  
	  printf("in_lookupkey() for s is %d\n",in_LookupKey('s'));	  
	  printf("in_lookupkey() for x is %d\n",in_LookupKey('x'));	
	  */
	  do
	  {		
		// The ZX80 has FAST-mode only, therefore using in_Inkey() would leave the screen black (calling 
		// gen_tv_field() within the loop would help a little but this would still make the screen flicker heavily)
		// So we use fgetc_cons() instead, which halts program execution and makes the ZX80 screen visible.
		// Unfortunately, with this we cannot provide a different random seed counter every time on the ZX80.
		#if defined(__ZX80__)
			key=fgetc_cons();
		#elif defined(__ZX81__) || defined(__SPECTRUM__)
			key=in_Inkey();	// from input.h, available on the following platforms: zx spectrum, zx81, sam), returns the currently pressed key and program execution continues 
		#elif defined(JOYSTICK_ONLY)
			DisplayGameType(gameType);
			while (!(joystick(joystickNumber1) & MOVE_FIRE1) && !(joystick(joystickNumber2) & MOVE_FIRE1))
			{
				if (((joystick(joystickNumber1) & MOVE_RIGHT) || (joystick(joystickNumber2) & MOVE_RIGHT)) && gameType<3)
				{
					gameType++;
					#if defined(TWO_PLAYER_MODE_NOT_AVAILABLE)
					  if (gameType==2) gameType=3;
					#endif
					myCls(); // clear the screen
					DisplayGameType(gameType);
					myWait(DELAY_BEFORE_NEWKEY);
				}
				if (((joystick(joystickNumber1) & MOVE_LEFT) || (joystick(joystickNumber2) & MOVE_LEFT)) && gameType>1)
				{
					gameType--;
					#if defined(TWO_PLAYER_MODE_NOT_AVAILABLE)
					  if (gameType==2) gameType=1;
					#endif
					myCls(); // clear the screen
					DisplayGameType(gameType);
					myWait(DELAY_BEFORE_NEWKEY);
				}
			}
			switch(gameType)
			{
				case 1: key=KEY_CODE_1; break;
				case 2: key=KEY_CODE_2; break;
				case 3: key=KEY_CODE_D; break;
			}			
		#elif defined(USE_ANSI) || defined(USE_NATIVE_INPUT)
			if(kbhit())
			{
				c = getch();
				if (c=='d' || c=='D') key=KEY_CODE_D;
				else if (c=='1') key=KEY_CODE_1;
				else if (c=='2') key=KEY_CODE_2;
				else if (c=='x' || c=='X') key=KEY_CODE_X;
				}
			else key=0;
		#else
			key=fgetc_cons();	
		#endif

		// if (key>0) printf("\nkeycode: %d",key);				
		// printf("key a (65) pressed: %d\n",myKeyPressed(in_LookupKey('A')));
		
		if (key==KEY_CODE_D) numberOfPlayers=0;
		if (key==KEY_CODE_1) numberOfPlayers=1;
		if (key==KEY_CODE_2) numberOfPlayers=2;
		if (key==KEY_CODE_X)
		{
		  myCls();
		  DoWhenProgramTerminates();
		  return;
		}
		// we use this counter to every time get a different random seed value,
		// depending on how long it took until the user pressed a button
		randomSeed++;
		if (randomSeed>32766) randomSeed=0;	// reset counter to avoid overflow
	  }
	  while(key!=KEY_CODE_D && key!=KEY_CODE_1 && key!=KEY_CODE_2);

	  srand(randomSeed);
	  /*
	  myCls();
	  printf("\n\nrandom seed is %d\n",randomSeed);
	  printf("first rand value is %d\n",rand());
	  printf("press space to continue\n",rand());
	  do
	  {	  
		  #if defined(USE_ANSI) || defined(USE_NATIVE_INPUT)
			  if(kbhit())
			  {
				c = getch();
				if (c==' ') key=KEY_CODE_SPACE; else key=0;
			  }
			  else key=0;
		  #else
			  key=fgetc_cons();	
		  #endif
	  }
	  while(key!=KEY_CODE_SPACE);
	  */
	  
	  /* game restart loop */
	  while(notPlayAgain<1)
	  {
		p1Score=0;
		p2Score=0;
		exitgame=0;
		
		/* round restart loop */
		while(p1Score<POINTS_TO_WIN && p2Score<POINTS_TO_WIN && exitgame<1)
		{
			InitPlayfield();
			key=0;
	
			if (numberOfPlayers>0)
			{
			  do
			  {
				#if defined(JOYSTICK_ONLY)
					// Fire
					if ((joystick(joystickNumber1) & MOVE_FIRE1) || (numberOfPlayers>1 && (joystick(joystickNumber2) & MOVE_FIRE1))) key=KEY_CODE_SPACE;
					// Return to Menu
					if (joyButtonMenu>0)
					{
						if ((joystick(joystickNumber1) & joyButtonMenu) || (joystick(joystickNumber2) & joyButtonMenu)) key=KEY_CODE_X;
					}
				#elif defined(USE_ANSI) || defined(USE_NATIVE_INPUT)
					if(kbhit())
					{
						c = getch();
						if (c==' ') key=KEY_CODE_SPACE;
						else if (c=='0') key=KEY_CODE_0;
						else if (c=='x' || c=='X') key=KEY_CODE_X;
					}
					else key=0;
				#elif defined(__ZX81__) || defined(__SPECTRUM__)
					key=in_Inkey();	// from input.h, available on the following platforms: zx spectrum, zx81, sam), returns the currently pressed key and program execution continues 
				#else
					key=fgetc_cons();  // for ZX80 to keep the screen visible
				#endif
				if (key==KEY_CODE_X) exitgame=1;
			  } while (key!=KEY_CODE_SPACE && key!= KEY_CODE_0 && key!=KEY_CODE_X);
			}
			else myWait(DELAY_DEMO_BEFOREROUNDSTART); /* in demo mode, the round starts automatically */

			lastState_p1PressedLeft=0;
			lastState_p1PressedRight=0;
			lastState_p2PressedLeft=0;
			lastState_p2PressedRight=0;
			
			/* main game loop */
			while(exitgame<1 && p1Crashed<1 && p2Crashed<1)
			{		
				// Check keyboard state for player's control
				
				// ZX81 note: here we could use in_Inkey() and then switch-case to set the 
				// actions for the possible key combinations. But this would not allow 
				// both players to simultaneously press their control key - one would lock 
				// the other one's steering.
				// So we have to use in_KeyPressed() which requires more code lines and 
				// is probably a little less efficient, but it does the job that we need.				
				// The lookup keys can be determined using the in_LookupKey() function.

				p1PressedLeft=0;
				p1PressedRight=0;
				p2PressedLeft=0;
				p2PressedRight=0;
				
				#if defined(USE_INKEYPRESSED)
					if (myKeyPressed(KEYPRESS_CODE_5)>0 || myKeyPressed(KEYPRESS_CODE_N)>0) p1PressedLeft=1;
					if (myKeyPressed(KEYPRESS_CODE_8)>0 || myKeyPressed(KEYPRESS_CODE_M)>0) p1PressedRight=1;
					if (myKeyPressed(KEYPRESS_CODE_A)>0) p2PressedLeft=1;
					if (myKeyPressed(KEYPRESS_CODE_S)>0) p2PressedRight=1;
				#elif defined(JOYSTICK_ONLY)
					if (joystick(joystickNumber1) & MOVE_LEFT) p1PressedLeft=1;
					if (joystick(joystickNumber1) & MOVE_RIGHT) p1PressedRight=1;
					if (joystick(joystickNumber2) & MOVE_LEFT) p2PressedLeft=1;
					if (joystick(joystickNumber2) & MOVE_RIGHT) p2PressedRight=1;
				#else
				// #if defined(USE_ANSI) || defined(USE_NATIVE_INPUT)
					// myPlot(p1x+5,p1y+5,1);	// FOR TESTING PURPOSES

					if(kbhit())
					{
						c = getch();
						if (c=='5' || c=='n' || c=='N') p1PressedLeft=1;
					#if defined(__VG5000__) // this machine from France has an AZERTY keyboard
						else if (c=='8' || c==',') p1PressedRight=1;	
						else if (c=='q' || c=='Q') p2PressedLeft=1;
					#else
						else if (c=='8' || c=='m' || c=='M') p1PressedRight=1;	
						else if (c=='a' || c=='A') p2PressedLeft=1;
					#endif											
						else if (c=='s' || c=='S') p2PressedRight=1;
						else if (c=='p' || c=='P')
						{
							 // in ANSI mode we do not make a pause here because here key 
							 // input is not as responsive as with in_keypressed() on the Sinclair computers
							 // myWait(DELAY_BEFORE_NEWKEY);  	// make pause before waiting for new key
							 // Some systems are however special, so we still need to make a short pause...
							 #if defined(__X1__) || defined(__HOMELAB__) || defined(__HOMELAB2__)
								myWait(DELAY_BEFORE_NEWKEY);  	// make pause before waiting for new key
							 #endif
							 fgetc_cons();
						}
						else if (c=='x' || c=='X') exitgame=1;
					}					
				#endif
				
				// We react to the key press only if there was a change since the 
				// last time we came along here, hence the lastState...-Check. 
				// By doing this we avoid unintended multiple turn commands, 
				// the player must always first release the button and then press it again.
				
				if (p1PressedLeft>0 && p1PressedLeft!=lastState_p1PressedLeft)
				{
				  if (numberOfPlayers>0)
				  {
					p2dir = p2dir+90;
					if (p2dir>=360) p2dir=0;
				  }
				}
				
				if (p1PressedRight>0 && p1PressedRight!=lastState_p1PressedRight)
				{
				  if (numberOfPlayers>0)
				  {
					p2dir = p2dir-90;
					if (p2dir<0) p2dir=270;
				  }
				}

				if (p2PressedLeft>0 && p2PressedLeft!=lastState_p2PressedLeft)
				{
				  if (numberOfPlayers==2)
				  {
					p1dir = p1dir+90;
					if (p1dir>=360) p1dir=0;
				  }
				}

				if (p2PressedRight>0 && p2PressedRight!=lastState_p2PressedRight)
				{
				  if (numberOfPlayers==2)
				  {
					p1dir = p1dir-90;
					if (p1dir<0) p1dir=270;
				  }
				 }

				
				// For the remaining keys we do not need to remember the status
				// For ANSI we have already handled this above				
				#if defined(USE_INKEYPRESSED)
					if (myKeyPressed(KEYPRESS_CODE_P)>0)	// Pause
					{
						 myWait(DELAY_BEFORE_NEWKEY);  	// make pause before waiting for new key
						 fgetc_cons();
					}
					else if (myKeyPressed(KEYPRESS_CODE_X)>0)	// Exit
					{
						exitgame=1;
					}
				#elif defined(JOYSTICK_ONLY)
					if (joyButtonPause>0)	// Pause
					{
						if ((joystick(joystickNumber1) & joyButtonPause) || (joystick(joystickNumber2) & joyButtonPause))
						{
							 myWait(DELAY_BEFORE_NEWKEY);  	// make pause before waiting for new key
							 while (!(joystick(joystickNumber1) & MOVE_FIRE1) && !(joystick(joystickNumber2) & MOVE_FIRE1));
						 }
					}
					if (joyButtonMenu>0)	// Exit to Menu
					{
						if ((joystick(joystickNumber1) & joyButtonMenu) || (joystick(joystickNumber2) & joyButtonMenu)) exitgame=1;
					}
				#endif

				/* let the computer player decide what to do */
				/* in one-player mode, Player 1 is controlled by the computer */
				if (numberOfPlayers<2) AIComputerPlayer(&p1x,&p1y,&p1dir);

				/* in demo mode, Player 2 is also controlled by the computer */
				if (numberOfPlayers==0) AIComputerPlayer(&p2x,&p2y,&p2dir);
				
				MovePlayers();

				// speed throttling
				// depending on the type of game we use different delay values
				switch(numberOfPlayers)
				{
					case 0: if (DELAY_GAME_DEMO>0) myWait(DELAY_GAME_DEMO); break;	// usually no delay required because AI for two computer players is most demanding for the CPU
					case 1: myWait(DELAY_GAME_ONEPLAYER); break;
					case 2: myWait(DELAY_GAME_TWOPLAYERS); break;
				}		  
								
				// remember the last state of the control keys			  
				lastState_p1PressedLeft=p1PressedLeft;
				lastState_p1PressedRight=p1PressedRight;
				lastState_p2PressedLeft=p2PressedLeft;
				lastState_p2PressedRight=p2PressedRight;			  
			
			}

			/* wait a quarter of a second after a crash has happened before clearing the screen */
			if (p1Crashed || p2Crashed) myWait(DELAY_AFTER_CRASH);
		
		}

		myCls();
		printf(END_FINALSCORE);
		#if !defined(FOUR_BY_SIX_CHARACTERS)
			printf("    %d : %d\n\n", p1Score, p2Score);
		#else
			printf("  %d : %d\n\n", p1Score, p2Score);
		#endif
				
		if (p1Score>p2Score)
			if (numberOfPlayers==0)
			  printf(END_COMP1_WINS);
			else if (numberOfPlayers==2)
			  printf(END_P1_WINS);
			else
			  printf(END_COMP_WINS);
		if (p1Score<p2Score)
			if (numberOfPlayers==0)
			  printf(END_COMP2_WINS);
			else if (numberOfPlayers==2)
			  printf(END_P2_WINS);
			else
			  printf(END_YOU_WIN);
		if (p1Score==p2Score)
			printf(END_A_DRAW);
			
		printf(END_PLAY_AGAIN);
		printf(END_M_FOR_MENU);
		key=0;
		do
		{
			#if defined(__ZX80__)
				key=fgetc_cons();
			#elif defined(__ZX81__) || defined(__SPECTRUM__)
				key=in_Inkey();	// from input.h, available on the following platforms: zx spectrum, zx81, sam), returns the currently pressed key and program execution continues 		
			#elif defined(JOYSTICK_ONLY)
				if ((joystick(joystickNumber1) & MOVE_FIRE1) || (joystick(joystickNumber2) & MOVE_FIRE1)) key=KEY_CODE_Y;
				if ((joystick(joystickNumber1) & MOVE_UP) || (joystick(joystickNumber2) & MOVE_UP)) key=KEY_CODE_M;
				// N for No (= end program) makes no sense on consoles
			#elif defined(USE_ANSI) || defined(USE_NATIVE_INPUT)
				if(kbhit())
				{
					c = getch();
					if (c=='y' || c=='Y') key=KEY_CODE_Y;
					else if (c=='0') key=KEY_CODE_0;
					else if (c=='m' || c=='M') key=KEY_CODE_M;
					else if (c=='n' || c=='N') key=KEY_CODE_N;			
				}
				else key=0;
			#else
				key=fgetc_cons();	
			#endif
		
			// if (key>0) printf("\nkeycode: %d",key);					
			if (key==KEY_CODE_N)
			{
				notPlayAgain=1;
				restartAgain=0;	
			}
			if (key==KEY_CODE_M)
			{
				notPlayAgain=1;
				restartAgain=1;	
			}

		} while(key!=KEY_CODE_Y && key!=KEY_CODE_N && key!=KEY_CODE_0 && key!=KEY_CODE_M);  // key 0 is for the ZX81 "cursor joystick" fire button

	  }
  } while (restartAgain>0);

  myCls();
  
  DoWhenProgramTerminates();
  
  return;
}
