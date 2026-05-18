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

#ifndef _SID_SOUNDS
#define _SID_SOUNDS

	#if defined(__C64__)
		#include <c64.h>
	#elif defined(__C128__)
		#include <c128.h>
	#else
		// TODO: USE pokebsys and pokewsys for the CBM510
		#include <cbm510.h>
	#endif
	
	void _XL_EXPLOSION_SOUND(void);
	
	void _XL_PING_SOUND(void);
	
	void _XL_SHOOT_SOUND(void);

	void _XL_TICK_SOUND(void);
	
	void _XL_TOCK_SOUND(void);
	
	void _XL_ZAP_SOUND(void);

#endif // SID_SOUNDS

