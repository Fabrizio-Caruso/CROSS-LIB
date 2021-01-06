#ifndef _GRAPHICS_SETTING_H
#define _GRAPHICS_SETTING_H

#  if defined(MEMORY_MAPPED)
	#include "memory_mapped_graphics.h"
#elif defined(NES_CONIO)
	#include "nes_conio_graphics.h"
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

