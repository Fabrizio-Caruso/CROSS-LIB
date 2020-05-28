
#  if defined(__C64__)
	#include "c64_memory_mapped_settings.h"
#elif defined(__CREATIVISION__) && defined(ANIMATE_PLAYER)
    #include "creativision_settings.h"
#elif defined(__VIC20__) && defined(VIC20_EXP_3K) && defined(TINY_GAME)
	#include "vic20_exp_3k_settings.h"	
#elif defined(__VIC20__) && (defined(VIC20_EXP_8K) || (defined(VIC20_EXP_3K) && !defined(TINY_GAME))) && defined(REDEFINED_CHARS)
	#include "vic20_exp_8k_settings.h"
#elif defined(__VIC20__) && defined(VIC20_UNEXPANDED) && defined(REDEFINED_CHARS)
	#include "vic20_unexpanded_gfx.h"
#elif defined(__VIC20__) && defined(CONIO)
	#include "vic20_exp_16k_settings.h"
#elif defined(__C16__) && defined(REDEFINED_CHARS) && !defined(C16_UNEXPANDED)
	#include "c264_redefined_chars_settings.h"
#elif defined(__C16__) && defined(REDEFINED_CHARS) && defined(C16_UNEXPANDED)
	#include "c264_redefined_chars_settings_2.h"	
#elif defined(__CBM__) && defined(MEMORY_MAPPED)
	#include "petscii_memory_mapped_settings.h"
#elif defined(__MO5__)
	#include "udg_settings.h"
#elif defined(__COCO__) || defined(__DRAGON__)
	#include "cmoc_memory_mapped_settings.h"
#elif (defined(__ATARI__) && defined(ATARI_MODE1))
	#include "atari_mode1_redefined_chars_settings.h"
#elif defined(__ATARI5200__) && defined(MEMORY_MAPPED)
	#include "atari_mode1_redefined_chars_settings.h"
#elif defined(__ATMOS__)
	#include "atmos_redefined_characters_settings.h"
#elif defined(Z88DK_SPRITES) 
	#include "udg_settings.h"
#elif defined(__MSX__)
	#include "msx_redefined_chars_settings.h"
#elif defined(CPCRSLIB)
	#include "cpc_cpcrslib_settings.h"
#elif defined(__REX__)
	#include "cpc_cpcrslib_settings.h"
#elif defined(__SUPERVISION__)
	#include "supervision_settings.h"
#elif defined(__NES__) && defined(ANIMATE_PLAYER)
	#include "nes_settings.h"
#elif defined(__COMX__) || defined(__PECOM__)
	#include "comx_settings.h"
#elif defined(UDG_GRAPHICS)
	#include "udg_settings.h"
#else
	#include "default_image_settings.h"
#endif 



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
