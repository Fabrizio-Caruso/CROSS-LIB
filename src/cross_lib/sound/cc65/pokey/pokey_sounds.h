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

#ifndef _ATARI_SOUNDS
#define _ATARI_SOUNDS

	#if defined(__ATARI__)
		#include<atari.h>
	#else
		#include<atari5200.h>
	#endif
		
	void _ping_sound(unsigned char freq);
	#define PING_SOUND() _ping_sound(0xF0);	
	#define TOCK_SOUND() _ping_sound(0x02);
	#define TICK_SOUND() _ping_sound(0x60);
	
	void _explosion_sound(unsigned char freq);
	#define SHOOT_SOUND() _explosion_sound(0xF0);
	#define EXPLOSION_SOUND() _explosion_sound(0x80);
	
	void ZAP_SOUND(void);	

#endif // _ATARI_SOUNDS