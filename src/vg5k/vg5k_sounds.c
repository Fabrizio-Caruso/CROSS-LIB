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

#include "vg5k_sounds.h"
	
	
	void _explosion_sound(void)
	{
	#asm
	jp _explosion_code
	
	_explosion_buffer:
		defb 0x54, 0x36, 0x44
		
	_explosion_code:
		ld bc,_explosion_buffer
		ld e,3
		call _explosion_play
		ret
	
	_explosion_play:
		push hl
		jp 0x008f
		ret
	#endasm
	}

	
	void _ping_sound(void)
	{ 
	#asm
	jp _ping_code
	
	_ping_buffer:
		defb 0x54, 0x36, 0x44
		
	_ping_code:
		ld bc,_ping_buffer
		ld e,3
		call _ping_play
		ret
	
	_ping_play:
		push hl
		jp 0x008f
		ret
	#endasm
	}			
	

	void _ZAP_SOUND(void) 
	{ 
	#asm
	jp _zap_code
	
	_zap_buffer:
		defb 0x54, 0x36, 0x44
		
	_zap_code:
		ld bc,_zap_buffer
		ld e,3
		call _zap_play
		ret
	
	_zap_play:
		push hl
		jp 0x008f
		ret
	#endasm
	}
	
	void ZAP_SOUND(void) 
	{
		unsigned char i;
		for(i=0;i<250;++i)
		{
			_ZAP_SOUND();
		}
	}
	