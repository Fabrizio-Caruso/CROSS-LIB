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

#ifndef _SOUNDS_MACROS
#define _SOUNDS_MACROS
    #if !defined(_XL_NO_SOUND)
        #  if defined(__BIT_BANG_SOUND)
            #include "z88dk_bit_bang_sounds.h"
        #elif defined(__PSG_SOUND)
            #include "psg_sounds.h"
        #elif defined(ASCII_SOUNDS)
            #define _XL_INIT_SOUND()
            #include "ascii_sounds.h"
        #elif defined(__TI99__) 
            #include "ti99_sounds.h"
        #elif defined(__ACE__) 
            #include "z88dk_bit_bang_sounds.h"
        #elif (defined(__ATARI__) )
            #include "pokey_sounds.h"
        #elif defined(__ATARI5200__) 
            #include "pokey_sounds.h"
        #elif defined(__ATMOS__) 
            #include "atmos_sounds.h"
        #elif defined(__AQUARIUS__) 
            #include "z88dk_bit_bang_sounds.h"
        #elif defined(__BEE__) 
            #include "z88dk_bit_bang_sounds.h"
        #elif defined(__C16__) 
            #include "c264_sounds.h"
        #elif defined(__C64__) 
            #include "sid_sounds.h"
        #elif defined(__C128__) 
            #include "sid_sounds.h"
        #elif defined(__CBM510__) 
            #include "sid_sounds.h"
        // #elif defined(__COLECO__) 
            // #include "psg_sounds.h"
        // #elif defined(__COLECO_ADAM__) 
            // #include "psg_sounds.h"
        #elif (defined(__COMX__) || defined(__PECOM__) || defined(__TMC600__) || defined(__CIDELSA__)) 
            #include "comx_sounds.h"
        #elif defined(__CPC__) 
            #include "psg_sounds.h"
        #elif defined(__MC1000__) 
            #include "z88dk_bit_bang_sounds.h"
        #elif defined(__MSX__) 
            #include "psg_sounds.h"
        #elif defined(__RX78__) 
            #include "psg_sounds.h"
        #elif defined(__SPECTRUM__) 
            #include "z88dk_bit_bang_sounds.h"
        #elif defined(__SV8000__) 
            #include "psg_sounds.h"
        #elif defined(__SVI__) 
            #include "psg_sounds.h"
        #elif defined(__VG5K__) 
            #include "z88dk_bit_bang_sounds.h"
        #elif defined(__VIC20__) && !defined(__VIC20_UNEXPANDED) 
            #include "vic20_sounds.h"
        #elif defined(__VIC20__) && defined(__VIC20_UNEXPANDED) 
            #include "vic20_explosion_sound.h"
        #elif defined(__VZ__) 
            #include "z88dk_bit_bang_sounds.h"
        #elif defined(__WINCMOC__) 
            #include "wincmoc_sounds.h"
        #elif (defined(__COCO__) || defined(__DRAGON__))  
            #include "dac_sounds.h"
        #elif defined(__COCO3__)  
            #include "dac_sounds.h"
        #elif (defined(__MO5__) || defined(__TO7__)) 
            #include "dac_sounds.h"
        #elif defined(__GAMATE__) 
            #include "gamate_sounds.h"
        #elif defined(__CREATIVISION__) 
            #include "creativision_sounds.h"
        #elif defined(__GB__) 
            #include "gb_sounds.h"
        #elif (defined(__APPLE2__) || defined(__APPLE2ENH__)) 
            #include "bit_bang_sounds.h"
        #elif defined(__PET__) 
            #include "pet_sounds.h"
        #elif defined(__ATARI_LYNX__)
            #include "atari_lynx_sounds.h"
        #elif defined(__MC10__) 
            #include "bit_bang_sounds.h"
        // #elif defined(__SUPERVISION__) 
            // #include "supervision_sounds.h"
        #else
            #define _XL_NO_SOUND
        #endif
    #endif
    
    
    #if defined(_XL_NO_SOUND)
		#define _XL_EXPLOSION_SOUND() 
		#define _XL_PING_SOUND() 
		#define _XL_SHOOT_SOUND() 
		#define _XL_TICK_SOUND() 
		#define _XL_TOCK_SOUND() 
		#define _XL_ZAP_SOUND() 
	#endif
    
    #if !defined(_XL_NO_SOUND) && (defined(__GB__) \
        || defined(__MO5__) || defined(__TO7__) || defined(__COCO3__) || defined(__COCO__) || defined(__DRAGON__) \
        || defined(__MSX__))
     // _XL_INIT_SOUND() needs to be defined
	 void _XL_INIT_SOUND();
    #else
        #define _XL_INIT_SOUND()
    #endif
#endif // _SOUNDS_MACROS



