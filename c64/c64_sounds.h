#ifndef _C64_SOUNDS
#define _C64_SOUNDS
	#if defined(__C64__)
    #include <c64.h>
	#else
	#include <c128.h>
	#endif

	#define EXPLOSION_SOUND() \
	{ \
		unsigned int i; \
		SID.v3.freq  = 0x2000; \
	    SID.v3.ad    = 0x00; \
	    SID.v3.sr    = 0xA8; \
	    SID.flt_freq = 0xA000; \
	    SID.flt_ctrl = 0x44; \
	    SID.amp      = 0x1F; \
	    SID.v3.ctrl  = 0x81; \
		\
		for(i=0;i<1500;++i) {} \
		SID.amp      = 0x00; \
	    SID.v3.ctrl  = 0x08; \
	};
	
	
	#define PING_SOUND() \
	{ \
		unsigned int i; \
		SID.v3.freq  = 0x3000; \
	    SID.v3.ad    = 0x00; \
	    SID.v3.sr    = 0xC8; \
	    SID.flt_freq = 0xA000; \
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
	    SID.flt_freq = 0xA000; \
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
	    SID.flt_freq = 0xB000; \
	    SID.flt_ctrl = 0x44; \
	    SID.amp      = 0x1F; \
	    SID.v3.ctrl  = 0x21; \
		\
		for(i=0;i<200;++i) {} \
		SID.amp      = 0x00; \
	    SID.v3.ctrl  = 0x08; \
	};
	
	#define TOCK_SOUND() \
	{ \
		unsigned int i; \
		SID.v3.freq  = 0x3000; \
	    SID.v3.ad    = 0x00; \
	    SID.v3.sr    = 0xC8; \
	    SID.flt_freq = 0x5000; \
	    SID.flt_ctrl = 0x44; \
	    SID.amp      = 0x1F; \
	    SID.v3.ctrl  = 0x21; \
		\
		for(i=0;i<200;++i) {} \
		SID.amp      = 0x00; \
	    SID.v3.ctrl  = 0x08; \
	};
	
	#define ZAP_SOUND() \
	{ \
		unsigned int i; \
		SID.v3.freq  = 0x3000; \
	    SID.v3.ad    = 0x00; \
	    SID.v3.sr    = 0xC8; \
	    SID.flt_freq = 0xA000; \
	    SID.flt_ctrl = 0x44; \
	    SID.amp      = 0x1F; \
	    SID.v3.ctrl  = 0x21; \
		\
		for(i=0;i<1200;++i) {} \
		SID.amp      = 0x00; \
	    SID.v3.ctrl  = 0x08; \
	};

#endif