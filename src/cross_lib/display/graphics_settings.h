#ifndef _GRAPHICS_SETTING_H
#define _GRAPHICS_SETTING_H


#  if defined(MEMORY_MAPPED) && !defined(__CIDELSA__)
	#include "memory_mapped_graphics.h"
#elif defined(QUAD_MEMORY_MAPPED)
	#include "quad_memory_mapped_graphics.h"
#elif defined(DUAL_MEMORY_MAPPED)
	#include "dual_memory_mapped_graphics.h" 
#elif defined(APPLE2_HGR)
    #include "apple2_hgr_graphics.h"
#elif defined(CREATIVISION_COLOR) || defined(TI99_COLOR)
	#include "vdp_mode1_graphics.h"
#elif defined(NES_CONIO)
	#include "nes_conio_graphics.h"
#elif defined(COMX_COLOR)
	#include "lcc1802_color_graphics.h"
#elif defined(ORIC_COLOR)
	#include "oric_color_graphics.h"
#elif defined(CONIO)
	#include "conio_graphics.h"
#elif defined(ATARI_MODE_1_COLOR)
    #include "atari_mode_1_color_graphics.h"
#elif defined(VECTORIAL)
	#include "vectorial_graphics.h"
#elif defined(BUFFERED)
	#include "buffered_graphics.h"
#elif defined(Z88DK_SPRITES)
	#include "z88dk_sprites_graphics.h"
#elif defined(TGI_GFX)
    #include "tgi_gfx_graphics.h"
#elif defined(TGI)
	#include "tgi_graphics.h"
#elif defined(CPCRSLIB)
	#include "cpc_cpcrslib_graphics.h"
#elif defined(BIT_MAPPED_4)
	#include "bit_mapped_4_graphics.h"
#elif defined(BIT_MAPPED)
	#include "bit_mapped_graphics.h"
#elif defined(REX)
	#include "rex_graphics.h"
#elif defined(LCC1802_GRAPHICS)
	#include "lcc1802_graphics.h"
#elif defined(NO_GRAPHICS)
	#include "no_graphics.h"
#endif

#endif // _GRAPHICS_SETTING_H

