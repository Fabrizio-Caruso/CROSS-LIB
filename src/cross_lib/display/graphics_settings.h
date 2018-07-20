

#  if defined(__C64__)
	#include "image_settings/c64_memory_mapped_settings.h"
#elif defined(__VIC20__) && defined(VIC20_EXP_3K)
	#include "image_settings/vic20_exp_3k_settings.h"	
#elif defined(__VIC20__) && defined(CONIO)
	#include "image_settings/vic20_exp_16k_settings.h"
#elif defined(__C16__) && defined(CONIO)
	#include "image_settings/c264_redefined_chars_settings.h"
#elif defined(__CBM__) && defined(MEMORY_MAPPED)
	#include "image_settings/petscii_memory_mapped_settings.h"
#elif defined(__CMOC__) && !defined(__WINCMOC__)
	#include "image_settings/cmoc_memory_mapped_settings.h"
#elif (defined(__ATARI__) && defined(ATARI_MODE1))
	#include "image_settings/atari_mode1_redefined_chars_settings.h"
#elif defined(__ATMOS__)
	#include "image_settings/atmos_redefined_characters_settings.h"
#elif defined(__SPECTRUM__) && defined(CLIB_ANSI)
	#include "image_settings/spectrum_redefined_chars_settings.h"
#elif defined(Z88DK_SPRITES) 
	#include "image_settings/z88dk_sprites_settings.h"
#elif defined(__MSX__)
	#include "image_settings/msx_redefined_chars_settings.h"
#elif defined(CPCRSLIB)
	#include "image_settings/cpc_cpcrslib_settings.h"
#elif defined(__SUPERVISION__)
	#include "image_settings/supervision_settings.h"
#else
	#include "image_settings/default_graphics_settings.h"
#endif 


#  if defined(MEMORY_MAPPED)
	#include "graphics_mode/memory_mapped/memory_mapped_graphics.h"
#elif defined(CONIO)
	#include "graphics_mode/conio/conio_graphics.h"
#elif defined(Z88DK_SPRITES)
	#include "graphics_mode/z88dk_sprites/z88dk_sprites_graphics.h"
#elif defined(TGI)
	#include "graphics_mode/tgi/tgi_graphics.h"
#elif defined(CPCRSLIB)
	#include "graphics_mode/cpcrslib/cpc_cpcrslib_graphics.h"
#elif defined(BIT_MAPPED)
	#include "graphics_mode/bit_mapped/bit_mapped_graphics.h"
#endif
