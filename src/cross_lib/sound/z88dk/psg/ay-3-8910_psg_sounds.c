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


#include "psg_sounds.h"
#include <psg.h>

#include <stdint.h>

// PSG code for AY-3-8910


/*
R0	Channel A Period	pa7	pa6	pa5	pa4	pa3	pa2	pa1	pa0
R1	-	-	-	-	paB	paA	pa9	pa8
R2	Channel B Period	pb7	pb6	pb5	pb4	pb3	pb2	pb1	pb0
R3	-	-	-	-	pbB	pbA	pb9	pb8
R4	Channel C Period	pc7	pc6	pc5	pc4	pc3	pc2	pc1	pc0
R5	-	-	-	-	pcB	pcA	pc9	pc8
R6	Noise Tone	-	-	-	n4	n3	n2	n1	n0
R7	/Enable	Port B Dir.	Port A Dir.	C Noise	B Noise	A Noise	C Tone	B Tone	A Tone
R8	Channel A Volume	-	-	-	A Mode	va3	va2	va1	va0
R9	Channel B Volume	-	-	-	B Mode	vb3	vb2	vb1	vb0
R10	Channel C Volume	-	-	-	C Mode	vc3	vc2	vc1	vc0
R11	Envelope Period	ep7	ep6	ep5	ep4	ep3	ep2	ep1	ep0
R12	epF	epE	epD	epC	epB	epA	ep9	ep8
R13	Envelope Wave Shape	-	-	-	-	es3	es2	es1	es0
R14	I/O Port A	Cas
Input	Kbd
Mode	Joy
Trg.B	Joy
Trg.A	Joy
Right	Joy
Left	Joy
Back	Joy
Fwd
R15	I/O Port B	Kana
LED	Joy
Sel	Pulse
2	Pulse
1	1	1	1	1

*/


#define A_PERIOD_LOW 0
#define A_PERIOD_HI 1

#define B_PERIOD_LOW 2
#define B_PERIOD_HI 3

#define C_PERIOD_LOW 4
#define C_PERIOD_HI 5
	
#define NOISE 6
#define CONTROL 7

#define A_VOLUME 8
#define B_VOLUME 9
#define C_VOLUME 10
	
#define ENV_PERIOD_LOW 11
#define ENV_PERIOD_HI 12

#define ENV_WAVE 13


#define POKE(addr,val)     (*(uint8_t*) (addr) = (val))

#if defined(__MSX__)
void _XL_INIT_SOUND(void)
{
    POKE(0xF3DB,0);
}
#endif


#if defined(__CPC__)
	#define SAFE_PORT_A_DIRECTION 0x40
#else
	#define SAFE_PORT_A_DIRECTION 0x00
#endif


void _XL_ZAP_SOUND(void) 
{
	uint8_t i;
	uint8_t j;
	
	set_psg(A_VOLUME,15);
	set_psg(A_PERIOD_LOW,255);
	
	set_psg(A_PERIOD_HI, 15);
	
	set_psg(CONTROL, 0xFF  - 0x01 - SAFE_PORT_A_DIRECTION);
	
	for(i=0;i<16;i++)
	{
		set_psg(A_PERIOD_HI,15-i);		
		for(j=0;j<150U;++j)
		{
		}
	}
	
	set_psg(A_VOLUME,0);		
}


void _ping_sound(uint8_t freq)
{ 
	uint8_t i;
	
	set_psg(A_VOLUME,15);
	set_psg(A_PERIOD_LOW,255);
	
	set_psg(A_PERIOD_HI, 15 - (freq>>4));		

	set_psg(CONTROL, 0xFF - 0x01 - SAFE_PORT_A_DIRECTION); // -0x40 is necessary for the CPC
	
	for(i=0;i<250U;++i)
	{
	}
	for(i=0;i<250U;++i)
	{
	}
		
	set_psg(A_VOLUME,0);
}			


void _explosion_sound(uint8_t freq)
{ 
	uint8_t i;
	// uint8_t j;
	
	set_psg(NOISE,15);
	set_psg(A_VOLUME,15);
	
	set_psg(A_PERIOD_LOW,0);
	set_psg(A_PERIOD_HI, 15 - (freq>>4));
	
	set_psg(CONTROL, 0xFF - 0x08 - SAFE_PORT_A_DIRECTION);
	
	for(i=0;i<16;i++)
	{
		set_psg(A_VOLUME,15-i);		
		// for(j=0;j<40;++j)
		// {
		// }		
	}

	// set_psg(A_VOLUME,0);		
}

