/*****************************************************************************/
/*                                                                           */
/*                                		                                     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* (C) 2017      Fabrizio Caruso                                  		     */
/*                					                                         */
/*              				                                             */
/* EMail:        Fabrizio_Caruso@hotmail.com                                 */
/*                                                                           */
/*                                                                           */
/* This software is provided 'as-is', without any expressed or implied       */
/* warranty.  In no event will the authors be held liable for any damages    */
/* arising from the use of this software.                                    */
/*                                                                           */
/* Permission is granted to anyone to use this software for any purpose,     */
/* including commercial applications, and to alter it and redistribute it    */
/* freely, subject to the following restrictions:                            */
/*                                                                           */
/* 1. The origin of this software must not be misrepresented; you must not   */
/*    claim that you wrote the original software. If you use this software   */
/*    in a product, an acknowledgment in the product documentation would be  */
/*    appreciated but is not required.                                       */
/* 2. Altered source versions must be plainly marked as such, and must not   */
/*    be misrepresented as being the original software.                      */
/* 3. This notice may not be removed or altered from any source              */
/*    distribution.                                                          */
/*                                                                           */
/*****************************************************************************/
/*
bit_fx(0)	Short hi-pitch decreasing "gulp/fall" sound
bit_fx(1)	Fast increasing scale
bit_fx(2)	Fast increasing squeak, mid duration, high pitch
bit_fx(3)	Crash sound, mid duration
bit_fx(4)	Fast increasing squeak/jump, short duration
bit_fx(5)	Short two tones clackson sound
bit_fx(6)	Two tones mid/high pitch bright beep
bit_fx(7)	Medium duration moving down, then up

bit_fx2(0)	Sort of electronic synth, mid-low tone, mid duration
bit_fx2(1)	Mid duration low tone, beating waves effect
bit_fx2(2)	Quick low pitch electronic buzz/noise
bit_fx2(3)	Long two tones truck clackson
bit_fx2(4)	Two tones increasing sound, electr. fx, mid duration
bit_fx2(5)	Two tones bounce sound
bit_fx2(6)	Alt. two tones bounce sound
bit_fx2(7)	Long low explosion sound

bit_fx3(0)	Quick noisy high pitch buzz / break noise
bit_fx3(1)	Longer very noisy high pitch buzz
bit_fx3(2)	Very quick low electronic noise / explosion / drum
bit_fx3(3)	Short low explosion sound
bit_fx3(4)	Electronic zap, mid duration
bit_fx3(5)	Noisy high pitch short increasing sound
bit_fx3(6)	Low level noise (AM radio noise)
bit_fx3(7)	Quick high pitch tremolo sound

bit_fx4(0)	Very quick squeaky sound
bit_fx4(1)	Tape rewind effect, long duration
bit_fx4(2)	Fast squeak/bounce/duck sound
bit_fx4(3)	Fall-down, mid duration
bit_fx4(4)	High pitch fall-down sound, long duration
bit_fx4(5)	Short high pitch slightly noisy decreasing sound
bit_fx4(6)	Short jump sound
bit_fx4(7)	Very quick duck squeak
*/
#ifndef __SPECTRUM_SOUNDS
#define __SPECTRUM_SOUNDS
	#include <sound.h>
	
	void bit_fx(void (*bfx)(void));
	#define SHOOT_SOUND() bit_fx(bitfx_1);	
	#define ZAP_SOUND() bit_fx(bitfx_7);		
	#define EXPLOSION_SOUND() bit_fx(bitfx_16)
	
	#define PING_SOUND() bit_fx(bitfx_19);
	#define TICK_SOUND() bit_fx(bitfx_17);
	#define TOCK_SOUND() bit_fx(bitfx_6);
	

#endif // __SPECTRUM_SOUNDS