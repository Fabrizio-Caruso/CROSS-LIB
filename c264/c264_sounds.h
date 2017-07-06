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

#ifndef __C264_SOUNDS
#define __C264_SOUNDS
	#include<peekpoke.h>
	
	#define NOISE 64
	#define VOICE_1 16
	#define MAX_VOLUME 15
	
	#define LO_FREQ_1 0xFF0E 
	#define HI_FREQ_1 0xFF12 	
	#define SELECT 0xFF11 
	
	#define EXPLOSION_SOUND() \
	{ \
		unsigned char volume = 0; unsigned char j; \
		for(;volume<=MAX_VOLUME;++volume) \
		{ \
			for(j=0;j<255;++j){} \
			POKE(SELECT,volume+NOISE); \
		} \
		POKE(SELECT,128); \
	};

	#define SHOOT_SOUND() \
	{ \
		unsigned char volume; unsigned char j; \
		POKE(SELECT,MAX_VOLUME+NOISE); \
		for(volume=0;volume<=MAX_VOLUME;++volume) \
		{ \
			for(j=50;j<255;++j){} \
			POKE(SELECT,volume+NOISE); \
		} \
		for(volume=0;volume<=MAX_VOLUME;++volume) \
		{ \
			for(j=0;j<255;++j){} \
			for(j=0;j<255;++j){} \
			POKE(SELECT,MAX_VOLUME-volume+NOISE); \
		} \
		POKE(SELECT,128); \
	};
	
	#define PING_SOUND() \
	{ \
		unsigned char freq; unsigned char j; \
		POKE(SELECT,MAX_VOLUME+VOICE_1); \
		POKE(HI_FREQ_1,PEEK(HI_FREQ_1) | 1); \
		for(freq=100;freq<120;++freq) \
		{ \
			for(j=0;j<150;++j) {}; \
			POKE(LO_FREQ_1,freq); \
		} \
		POKE(HI_FREQ_1,PEEK(HI_FREQ_1) & (255-3)); \
		POKE(SELECT,128); \
	};

	#define ZAP_SOUND() \
	{ \
		unsigned char freq; unsigned char j; \
		POKE(SELECT,MAX_VOLUME+VOICE_1); \
		POKE(HI_FREQ_1,PEEK(HI_FREQ_1) & (255-3)); \
		for(freq=100;freq<255;++freq) \
		{ \
			for(j=0;j<25;++j) {}; \
			POKE(LO_FREQ_1,freq); \
		} \
		POKE(SELECT,128); \
	};	
	
	#define TOCK_SOUND() \
	{ \
		unsigned char freq; unsigned char j; \
		POKE(SELECT,MAX_VOLUME+VOICE_1); \
		POKE(HI_FREQ_1,PEEK(HI_FREQ_1) & (255-3)); \
		for(freq=12;freq<16;++freq) \
		{ \
			for(j=0;j<150;++j) {}; \
			POKE(LO_FREQ_1,freq); \
		} \
		POKE(SELECT,128); \
	};
	
	
	#define TICK_SOUND() {};
	
#endif // __C264_SOUNDS