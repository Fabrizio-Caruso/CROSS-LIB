

#  if defined(__C64__)
	#include "cc65/c64/c64_memory_mapped_settings.h"
#elif defined(__VIC20__) && defined(VIC20_EXP_3K)
	#include "cc65/vic20/vic20_exp_3k_settings.h"	
#elif defined(__VIC20__) && defined(CONIO)
	#include "cc65/vic20/vic20_exp_16k_settings.h"
#elif defined(__C16__) && defined(CONIO)
	#include "cc65/c264/c264_redefined_chars_settings.h"
#elif defined(__CBM__) && defined(MEMORY_MAPPED)
	#include "cc65/petscii_memory_mapped_settings.h"
#elif defined(__CMOC__) && !defined(__WINCMOC__)
	#include "cmoc/cmoc_memory_mapped_settings.h"
#elif (defined(__ATARI__) && defined(ATARI_MODE1))
	#include "cc65/atari/atari_mode1_redefined_chars_settings.h"
#elif defined(__ATMOS__)
	#include "cc65/atmos/atmos_redefined_characters_settings.h"
#elif defined(__SPECTRUM__) && defined(CLIB_ANSI)
	#include "z88dk/spectrum/spectrum_redefined_chars_settings.h"
#elif defined(Z88DK_SPRITES) 
	#include "z88dk/z88dk_sprites/z88dk_sprites_settings.h"
#elif defined(__MSX__)
	#include "z88dk/msx/msx_redefined_chars_settings.h"
#elif defined(CPCRSLIB)
	#include "z88dk/cpc/cpc_cpcrslib_settings.h"
#else
	#include "default_graphics_settings.h"
#endif 


#  if defined(MEMORY_MAPPED)
	#include "memory_mapped/memory_mapped_graphics.h"
#elif defined(CONIO)
	#include "conio/conio_graphics.h"
#elif defined(Z88DK_SPRITES)
	#include "z88dk/z88dk_sprites/z88dk_sprites_graphics.h"
#elif defined(TGI)
	#include "tgi/tgi_graphics.h"
#elif defined(CPCRSLIB)
	#include "z88dk/cpc/cpc_cpcrslib_graphics.h"
#endif