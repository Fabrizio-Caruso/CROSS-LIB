#ifndef _GRAPHICS_SETTING_H
#define _GRAPHICS_SETTING_H


#  if defined(__MEMORY_MAPPED_GRAPHICS) && !defined(__CIDELSA__)
	#include "memory_mapped_graphics.h"
#elif defined(QUAD_MEMORY_MAPPED)
	#include "quad_memory_mapped_graphics.h"
#elif defined(DUAL_MEMORY_MAPPED)
	#include "dual_memory_mapped_graphics.h" 
#elif defined(APPLE2_HGR)
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
    #include "atari_mode_1_color_graphics.h"
#elif defined(VECTORIAL)
	#include "vectorial_graphics.h"
#elif defined(__BUFFERED_GRAPHICS)
	#include "buffered_graphics.h"
#elif defined(__Z88DK_SPRITES_GRAPHICS)
	#include "z88dk_sprites_graphics.h"
#elif defined(__LYNX_TGI_GRAPHICS)
    #include "tgi_gfx_graphics.h"
// #elif defined(TGI)
	// #include "tgi_graphics.h"
#elif defined(CPCRSLIB)
	#include "cpc_cpcrslib_graphics.h"
#elif defined(__BIT_MAPPED_4_GRAPHICS)
	#include "bit_mapped_4_graphics.h"
#elif defined(__BIT_MAPPED_GRAPHICS)
	#include "bit_mapped_graphics.h"
#elif defined(REX)
	#include "rex_graphics.h"
#elif defined(__LCC1802_GRAPHICS)
	#include "lcc1802_graphics.h"
#elif defined(NO_GRAPHICS)
	#include "no_graphics.h"
#endif

#endif // _GRAPHICS_SETTING_H

