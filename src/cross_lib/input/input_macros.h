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

#ifndef _INPUT_MACROS
#define _INPUT_MACROS

	#include "input_target_settings.h"

	#if defined(KEYBOARD_CONTROL)
		#if defined(__ATMOS__)
			#include <peekpoke.h>
			#define INIT_INPUT() { POKE(0x26A,PEEK(0x26A) | 8); }
		#elif defined(__MSX__)
			#define POKE(addr,val)     (*(uint8_t*) (addr) = (val))		
			#define INIT_INPUT() { POKE(0xF3DB,0); }
        // Enable key-repeat on all VIC 20 targets that use the keyboard
		#elif defined(__VIC20__) || defined(__C64__)
			#include <peekpoke.h>
			#define INIT_INPUT() \
            do \
            { \
                POKE(0x028A ,0xFF) \
                POKE(657,128); \
            } while(0)
		#elif defined(__NCURSES__) && !defined(TURN_BASED)
			#define INIT_INPUT() nodelay(stdscr,TRUE)
		#elif defined(__M5__)
			#define POKE(addr,val)     (*(uint8_t*) (addr) = (val))	
			#define PEEK(addr)         (*(uint8_t*) (addr))		
			#define INIT_INPUT() { POKE(0x701A,PEEK(0x701A)&0x7F);}
		#else
			#define INIT_INPUT()
		#endif
	#elif defined(Z88DK_JOYSTICK)
		extern uint8_t stick;
		
        #if !defined(__ZX81__)
            #define INIT_INPUT() \
            { \
                stick = 1; \
            }
        #else
            #define INIT_INPUT() \
            { \
                stick = 3; \
            }            
        #endif
	#elif defined(__SMS__) || defined(__LCC1802__)
		#define INIT_INPUT()
	#else // All CBM except CBM610 + ATARI + ATARI XL + ATARI 5200
		#include <joystick.h>
		#if defined(__SUPERVISION__)
			#include <supervision.h>
		#endif
        #if defined(__VIC20__) || defined(__C64__)
            #include <peekpoke.h>
            #define INIT_INPUT() \
                do \
                { \
                joy_install(joy_static_stddrv); \
                POKE(657,128); \
                } while(0);
        #else
            #define INIT_INPUT() { joy_install(joy_static_stddrv); };
        #endif
	#endif // defined(Z88DK_JOYSTICK)


#if defined(JOYSTICK_CONTROL)

        #if defined(__C64__) || defined(__C128__)
            #define STANDARD_JOY JOY_2
        #else
            #define STANDARD_JOY JOY_1
        #endif

    #if defined(Z88DK_JOYSTICK)
        #include <games.h>
        
        #define JOY_UP(joyInput) ((joyInput) & MOVE_UP)
        #define JOY_DOWN(joyInput) ((joyInput) & MOVE_DOWN)
        #define JOY_LEFT(joyInput) ((joyInput) & MOVE_LEFT)
        #define JOY_RIGHT(joyInput) ((joyInput) & MOVE_RIGHT)
        #define JOY_FIRE(joyInput) ((joyInput) & MOVE_FIRE)
    #elif defined(__SMS__)
        #include <arch/sms/SMSLib.h>
        
        #define JOY_UP(joyInput) ((joyInput) & PORT_A_KEY_UP)
        #define JOY_DOWN(joyInput) ((joyInput) & PORT_A_KEY_DOWN)
        #define JOY_LEFT(joyInput) ((joyInput) & PORT_A_KEY_LEFT)
        #define JOY_RIGHT(joyInput) ((joyInput) & PORT_A_KEY_RIGHT)
        #define JOY_FIRE(joyInput) ((joyInput) & PORT_A_KEY_1)
    #elif defined(LCC1802_JOYSTICK)
        #include <devkit/input/joystick.h>

        #define JOY_UP(joyInput) ((joyInput) == MOVE_UP)
        #define JOY_DOWN(joyInput) ((joyInput) == MOVE_DOWN)
        #define JOY_LEFT(joyInput) ((joyInput) == MOVE_LEFT)
        #define JOY_RIGHT(joyInput) ((joyInput) == MOVE_RIGHT)
        #define JOY_FIRE(joyInput) ((joyInput) == MOVE_FIRE)
    #else // CC65
        #include <joystick.h>
        #if !defined(JOY_FIRE)
            #define JOY_FIRE(joyKey) JOY_BTN_1(joyKey)
        #endif
    #endif // defined(Z88DK_JOYSTICK)
    
    #if defined(Z88DK_JOYSTICK)
        extern uint8_t stick;
        
        #define JOY_INPUT() joystick(stick)
    #elif defined(__SMS__)
        #include <arch/sms/SMSLib.h>
        
        #define JOY_INPUT() (SMS_getKeysStatus() & 0xFF)
    #elif defined(__LCC1802__)
        #define JOY_INPUT() get_stick(0)
    #else
        #define JOY_INPUT() joy_read(STANDARD_JOY)
    #endif // defined(Z88DK_JOYSTICK)
    
#endif


	#  if defined(__NCURSES__) || defined(STDLIB)
		#define TURN_BASED_INPUT() getchar()
	#elif defined(Z88DK)
		#define TURN_BASED_INPUT() getch()
	#elif defined(ACK)
		#define TURN_BASED_INPUT() getchar()
    #elif defined(NO_INPUT)
        #define TURN_BASED_INPUT()
	#else
		#define TURN_BASED_INPUT() cgetc()
	#endif	// TURN_BASED_INPUT definitions
	

	#if !defined(NO_INPUT)
        #if defined(KEYBOARD_CONTROL)
            #if defined(ACK) || defined(STDLIB)
                #define GET_CHAR() getchar()
            #else
                char GET_CHAR(void);
            #endif
        #endif // defined(KEYBOARD_CONTROL)

        #if !defined(NO_WAIT) || !defined(NO_SLEEP)
            void WAIT_PRESS(void);
        #else // NO_WAIT + NO_SLEEP
            #define WAIT_PRESS()
        #endif // !defined(NO_WAIT) || !defined(NO_SLEEP)
    #else
        #define WAIT_PRESS()
    #endif // !defined(NO_INPUT)

#endif // _INPUT_MACROS

