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

#include "c264_sounds.h"

#include <stdint.h>

void _set_sound()
{
	POKE(SELECT,MAX_VOLUME+VOICE_1); 
	POKE(HI_FREQ_1,PEEK(HI_FREQ_1) & (255-3)); 	
}
	
void _pause(uint8_t length)
{
	uint8_t i;
	for(i=0;i<length;++i)
	{
	}
}
	
void _noise_sound(uint8_t type)
	{ 
		uint8_t volume = 0; 

		for(;volume<=MAX_VOLUME;++volume) 
		{ 
			_pause(type);
			POKE(SELECT,volume+NOISE); 
		} 
		POKE(SELECT,128); 
	};	


void _short_sound(uint8_t type) 
{ 
	_set_sound();
	POKE(LO_FREQ_1,type); 
	_pause(type);
	POKE(SELECT,128); 
};



void _XL_ZAP_SOUND(void) 
{ 
	uint8_t freq; 

	_set_sound();
	for(freq=100;freq<255;++freq) 
	{ 
		_pause(25);
		POKE(LO_FREQ_1,freq); 
	} 
	POKE(SELECT,128); 
};	



