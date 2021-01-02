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
	#  if defined(FORCE_BIT_BANG)
		#include "bit_bang_sounds.h"
	#elif defined(FORCE_PSG)
		#include "psg_sounds.h"
	#elif defined(__MO5__)
		#include "mo5_sounds.h"
	#elif defined(ASCII_SOUNDS)
		#include "ascii_sounds.h"
	#elif defined(__ACE__) && defined(SOUNDS)
		#include "bit_bang_sounds.h"
	#elif (defined(__ATARI__) && defined(SOUNDS))
		#include "pokey_sounds.h"
	#elif defined(__ATARI5200__) && defined(SOUNDS)
		#include "pokey_sounds.h"
	#elif defined(__ATMOS__) && defined(SOUNDS)
		#include "atmos_sounds.h"
	#elif defined(__AQUARIUS__) && defined(SOUNDS)
		#include "bit_bang_sounds.h"
	#elif defined(__BEE__) && defined(SOUNDS)
		#include "bit_bang_sounds.h"
	#elif defined(__C16__) && defined(SOUNDS)
		#include "c264_sounds.h"
	#elif defined(__C64__) && defined(SOUNDS)
		#include "sid_sounds.h"
	#elif defined(__C128__) && defined(SOUNDS)
		#include "sid_sounds.h"
	#elif defined(__CBM510__) && defined(SOUNDS)
		#include "sid_sounds.h"
	#elif defined(__COLECO__) && defined(SOUNDS)
		#include "psg_sounds.h"
	#elif defined(__COLECO_ADAM__) && defined(SOUNDS)
		#include "psg_sounds.h"
	#elif (defined(__COMX__) || defined(__PECOM__) || defined(__TMC600__) || defined(__CIDELSA__)) && defined(SOUNDS)
		#include "comx_sounds.h"
	#elif defined(__CPC__) && defined(SOUNDS)
		#include "psg_sounds.h"
	#elif defined(__MC1000__) && defined(SOUNDS)
		#include "bit_bang_sounds.h"
	#elif defined(__MSX__) && defined(SOUNDS)
		#include "psg_sounds.h"
	#elif defined(__RX78__) && defined(SOUNDS)
		#include "psg_sounds.h"
	#elif defined(__SPECTRUM__) && defined(SOUNDS)
		#include "bit_bang_sounds.h"
	#elif defined(__SV8000__) && defined(SOUNDS)
		#include "psg_sounds.h"
	#elif defined(__SVI__) && defined(SOUNDS)
		#include "psg_sounds.h"
	#elif defined(__VG5K__) && defined(SOUNDS)
		#include "bit_bang_sounds.h"
	#elif defined(__VIC20__) && !defined(VIC20_UNEXPANDED) && defined(SOUNDS)
		#include "vic20_sounds.h"
	#elif defined(__VIC20__) && defined(VIC20_UNEXPANDED) && defined(SOUNDS)
		#include "vic20_explosion_sound.h"
	#elif defined(__VZ__) && defined(SOUNDS)
		#include "bit_bang_sounds.h"
	#elif defined(__WINCMOC__) && defined(SOUNDS)
		#include "wincmoc_sounds.h"
	#else
		#define EXPLOSION_SOUND() 
		#define PING_SOUND() 
		#define SHOOT_SOUND() 
		#define TICK_SOUND() 
		#define TOCK_SOUND() 
		#define ZAP_SOUND() 
	#endif
#endif // _SOUNDS_MACROS

