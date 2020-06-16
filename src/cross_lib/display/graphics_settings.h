#ifndef _GRAPHICS_SETTING_H
#define _GRAPHICS_SETTING_H

#  if defined(MEMORY_MAPPED)
	#include "memory_mapped_graphics.h"
#elif defined(CONIO)
	#include "conio_graphics.h"
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
#elif defined(LASER500_BITMAP)
	#include "bitmapped_graphics_laser500.h"
#elif defined(NO_GRAPHICS)
	#include "no_graphics.h"
#endif

#endif // _GRAPHICS_SETTING_H

