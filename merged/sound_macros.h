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
	#if defined(__ATMOS__) && defined(SOUNDS)
		#include "atmos/atmos_sounds.h"
	#elif (defined(__ATARI__) || defined(__ATARIXL__)) && defined(SOUNDS)
		#include "atari/atari_sounds.h"
	#elif defined(__SPECTRUM__) && !defined(SPECTRUM_NATIVE_DIRECTIVES)
		#include "spectrum/spectrum_sounds.h"	
	#elif defined(__VIC20__) && defined(SOUNDS)
		#include "vic20/vic20_sounds.h"
	#elif defined(__C16__) && defined(SOUNDS)
		#include "c264/c264_sounds.h"
	#elif defined(__C64__) && defined(SOUNDS)
		#include "c64/c64_sounds.h"
	#elif defined(__C128__) && defined(SOUNDS)
		#include "c64/c64_sounds.h"		
	#elif defined(__MSX__) && defined(SOUNDS)
		#include "msx/msx_sounds.h"
	#else
		#define EXPLOSION_SOUND() {};
		#define PING_SOUND() {};
		#define SHOOT_SOUND() {};
		#define TICK_SOUND() {};
		#define TOCK_SOUND() {};
		#define ZAP_SOUND() {};
	#endif
#endif // _SOUNDS_MACROS