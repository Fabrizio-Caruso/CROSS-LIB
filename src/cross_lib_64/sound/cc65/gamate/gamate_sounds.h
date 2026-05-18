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

#ifndef _GAMATE_SOUNDS
#define _GAMATE_SOUNDS

void play_frequency(uint16_t value);

#define _XL_EXPLOSION_SOUND() play_frequency(350)

#define _XL_PING_SOUND() play_frequency(200)

#define _XL_SHOOT_SOUND() play_frequency(600)

#define _XL_TOCK_SOUND() play_frequency(90)

#define _XL_TICK_SOUND() play_frequency(150)

#define _XL_ZAP_SOUND() play_frequency(1200)

#endif // _GAMATE_SOUNDS



