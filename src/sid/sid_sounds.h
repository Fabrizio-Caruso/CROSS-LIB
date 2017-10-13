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
		#include <cbm510.h>
	#endif

	#define EXPLOSION_SOUND() \
	{ \
		unsigned int i; \
		unsigned int j; \
		SID.v3.freq  = 0x1200; \
	    SID.v3.ad    = 0x00; \
	    SID.v3.sr    = 0xA8; \
	    SID.flt_freq = 0x6000; \
	    SID.flt_ctrl = 0x44; \
	    SID.amp      = 0x1F; \
	    SID.v3.ctrl  = 0x81; \
		\
		for(i=0;i<300;++i) \
			{ \
			} \
		for(j=0;j<15;++j) \
		{ \
			SID.amp      = 0x1F - j; \
			for(i=0;i<100;++i) \
			{ \
			} \
		} \
		SID.amp      = 0x00; \
	    SID.v3.ctrl  = 0x08; \
	};
	
	
	#define PING_SOUND() \
	{ \
		unsigned int i; \
		SID.v3.freq  = 0x3000; \
	    SID.v3.ad    = 0x00; \
	    SID.v3.sr    = 0xC8; \
	    SID.flt_freq = 0x8000; \
	    SID.flt_ctrl = 0x44; \
	    SID.amp      = 0x1F; \
	    SID.v3.ctrl  = 0x21; \
		\
		for(i=0;i<400;++i) {} \
		SID.amp      = 0x00; \
	    SID.v3.ctrl  = 0x08; \
	};

	#define SHOOT_SOUND() \
	{ \
		unsigned int i; \
		SID.v3.freq  = 0x3000; \
	    SID.v3.ad    = 0x00; \
	    SID.v3.sr    = 0xA8; \
	    SID.flt_freq = 0x9000; \
	    SID.flt_ctrl = 0x44; \
	    SID.amp      = 0x1F; \
	    SID.v3.ctrl  = 0x81; \
		\
		for(i=0;i<800;++i) {} \
		SID.amp      = 0x00; \
	    SID.v3.ctrl  = 0x08; \
	};	
	
	#define TICK_SOUND() \
	{ \
		unsigned int i; \
		SID.v3.freq  = 0x3000; \
	    SID.v3.ad    = 0x00; \
	    SID.v3.sr    = 0xC8; \
	    SID.flt_freq = 0x8000; \
	    SID.flt_ctrl = 0x44; \
	    SID.amp      = 0x1F; \
	    SID.v3.ctrl  = 0x21; \
		\
		for(i=0;i<600;++i) {} \
		SID.amp      = 0x00; \
	    SID.v3.ctrl  = 0x08; \
	};
	
	#define TOCK_SOUND() \
	{ \
		unsigned int i; \
		SID.v3.freq  = 0x3000; \
	    SID.v3.ad    = 0x00; \
	    SID.v3.sr    = 0xC8; \
	    SID.flt_freq = 0x7000; \
	    SID.flt_ctrl = 0x44; \
	    SID.amp      = 0x1F; \
	    SID.v3.ctrl  = 0x21; \
		\
		for(i=0;i<600;++i) {} \
		SID.amp      = 0x00; \
	    SID.v3.ctrl  = 0x08; \
	};
	
	#define ZAP_SOUND() \
	{ \
		unsigned int i; \
		SID.v3.freq  = 0x2000; \
	    SID.v3.ad    = 0x00; \
	    SID.v3.sr    = 0xC8; \
	    SID.flt_freq = 0x5000; \
	    SID.flt_ctrl = 0x44; \
	    SID.amp      = 0x1F; \
	    SID.v3.ctrl  = 0x21; \
		\
		for(i=0;i<1200;++i) {} \
		SID.amp      = 0x00; \
	    SID.v3.ctrl  = 0x08; \
	};

#endif // SID_SOUNDS

