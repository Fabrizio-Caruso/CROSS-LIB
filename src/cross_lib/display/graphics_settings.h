#ifndef _GRAPHICS_SETTING_H
#define _GRAPHICS_SETTING_H


#  if defined(__MEMORY_MAPPED_GRAPHICS)
	#include "memory_mapped_graphics.h"
#elif defined(__PV1000_GRAPHICS)
    #include "pv1000_graphics.h"
#elif defined(__BBC__) && defined(__BBC_GRAPHICS)
    #include "bbc_graphics.h"
#elif defined(__TERMINAL__) 
    #include "terminal_graphics.h"
#elif defined(__QUAD_MEMORY_MAPPED_GRAPHICS)
	#include "quad_memory_mapped_graphics.h"
#elif defined(__DUAL_MEMORY_MAPPED_GRAPHICS)
	#include "dual_memory_mapped_graphics.h" 
#elif defined(__ATARI7800_COLOR_GRAPHICS)
	#include "atari7800_color_graphics.h"
#elif defined(__APPLE2_HGR_GRAPHICS)
    #include "apple2_hgr_graphics.h"
#elif defined(__VDP_MODE1_GRAPHICS)
	#include "vdp_mode1_graphics.h"
#elif defined(__NES_CONIO_GRAPHICS)
	#include "nes_conio_graphics.h"
#elif defined(__COMX_COLOR_GRAPHICS)
	#include "lcc1802_color_graphics.h"
#elif defined(__ORIC_COLOR_GRAPHICS)
	#include "oric_color_graphics.h"
#elif defined(__CONIO_GRAPHICS)
	#include "conio_graphics.h"
#elif defined(__ANTIC_MODE6_GRAPHICS)
    #include "antic_mode6_graphics.h"
#elif defined(__VECTORIAL_GRAPHICS)
	#include "vectorial_graphics.h"
#elif defined(__BUFFERED_GRAPHICS)
	#include "buffered_graphics.h"
#elif defined(__Z88DK_SPRITES_GRAPHICS)
	#include "z88dk_sprites_graphics.h"
#elif defined(__LYNX_TGI_GRAPHICS)
    #include "atari_lynx_tgi_graphics.h"
#elif defined(__CPCRSLIB_GRAPHICS)
	#include "cpc_cpcrslib_graphics.h"
#elif defined(__BIT_MAPPED_16_GRAPHICS)
	#include "bit_mapped_16_graphics.h"
#elif defined(__BIT_MAPPED_4_GRAPHICS)
	#include "bit_mapped_4_graphics.h"
#elif defined(__BIT_MAPPED_GRAPHICS)
	#include "bit_mapped_graphics.h"
#elif defined(__REX_GRAPHICS)
	#include "rex_graphics.h"
#elif defined(__LCC1802_GRAPHICS)
	#include "lcc1802_graphics.h"
#elif defined(__PV1000_GRAPHICS)
    #include "pv1000_graphics.h"
#elif defined(__NO_GRAPHICS)
	#include "no_graphics.h"
#endif

#endif // _GRAPHICS_SETTING_H

