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

#ifndef _PET_SOUNDS
#define _PET_SOUNDS

#include <peekpoke.h>

// Auxiliary control register (Free run)
#define AUX_CTRL_REG 59467U

// Timbre and octave
#define SHIFT_REG 59466U

// Period: Small -> high pitch
#define TIMER_2_LOW 59464U
#define TIMER_2_HIGH 59465U 

void beep(uint8_t freq);
void noise(uint8_t duration);
#define _XL_EXPLOSION_SOUND() noise(250)

#define _XL_PING_SOUND() beep(90)
  
#define _XL_SHOOT_SOUND() noise(180)

#define _XL_TICK_SOUND() beep(60)

#define _XL_TOCK_SOUND() beep(220)

void _XL_ZAP_SOUND(void);

#endif // _GAMATE_SOUNDS



