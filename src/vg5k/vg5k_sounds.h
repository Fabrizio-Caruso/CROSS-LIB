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

#ifndef _VG5K_SOUNDS
#define _VG5K_SOUNDS

	void _ping_sound(void);
	#define PING_SOUND() _ping_sound();	
	#define TOCK_SOUND() _ping_sound();
	#define TICK_SOUND() _ping_sound();
	
	void _explosion_sound(void);
	#define SHOOT_SOUND() _explosion_sound();
	#define EXPLOSION_SOUND() _explosion_sound();
	
	void ZAP_SOUND(void);	

#endif // _VG5K_SOUNDS