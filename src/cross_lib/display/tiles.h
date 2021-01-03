
#  if defined(__C64__)
	#include "c64_memory_mapped_settings.h"
#elif defined(__CREATIVISION__) && defined(REDEFINED_CHARS)
    #include "creativision_settings.h"
#elif defined(__VIC20__) && defined(VIC20_EXP_3K) && !defined(REDEFINED_CHARS)
	#include "vic20_exp_3k_settings.h"
#elif defined(__VIC20__) && defined(VIC20_EXP_3K) && defined(REDEFINED_CHARS)
	#include "vic20_exp_3k_light_settings.h"
#elif defined(__VIC20__) && defined(MEMORY_MAPPED) && (defined(VIC20_EXP_8K) || defined(VIC20_EXP_3K)) && defined(REDEFINED_CHARS)
	#include "vic20_rom_chars_and_19_tiles_settings.h"
#elif defined(__VIC20__) && defined(VIC20_UNEXPANDED) && defined(REDEFINED_CHARS)
	#include "vic20_rom_chars_and_6_tiles_settings.h"
#elif defined(__VIC20__) && defined(CONIO)
	#include "vic20_exp_16k_settings.h"
#elif defined(__C16__) && defined(REDEFINED_CHARS) && !defined(C16_UNEXPANDED)
	#include "c264_redefined_chars_settings.h"
#elif defined(__C16__) && defined(REDEFINED_CHARS) && defined(C16_UNEXPANDED)
	#include "c264_link_time_redefined_chars_settings.h"
#elif defined(__CBM__) && defined(MEMORY_MAPPED)
	#include "petscii_memory_mapped_settings.h"
#elif defined(__MO5__) || defined(__TO7__) || ( (defined(__COCO__) || defined(__DRAGON__)) && defined(BIT_MAPPED))
	#include "udg_settings.h"
#elif (defined(__COCO__) || defined(__DRAGON__)) && defined(MEMORY_MAPPED)
	#include "cmoc_memory_mapped_settings.h"
#elif (defined(__ATARI__) && defined(ATARI_MODE1))
	#include "atari_mode1_redefined_chars_settings.h"
#elif defined(__ATARI5200__)
	#include "atari_mode1_redefined_chars_settings.h"
#elif defined(__ATMOS__)
	#include "atmos_redefined_characters_settings.h"
#elif defined(Z88DK_SPRITES) 
	#include "udg_settings.h"
#elif defined(__MSX__) && defined(MEMORY_MAPPED)
	#include "msx_redefined_chars_settings.h"
#elif defined(CPCRSLIB)
	#include "cpc_cpcrslib_settings.h"
#elif defined(__REX__)
	#include "cpc_cpcrslib_settings.h"
#elif defined(__SUPERVISION__)
	#include "supervision_settings.h"
#elif defined(__NES__)
	#include "nes_settings.h"
#elif defined(__COMX__) || defined(__PECOM__) || (defined(__CIDELSA__) && defined(REDEFINED_CHARS))
	#include "comx_settings.h"
#elif defined(__MICRO__)
	#include "micro_settings.h"
#elif defined(UDG_GRAPHICS)
	#include "udg_settings.h"
#elif defined(__PCE__) || ( defined(__GAMATE__) && defined(REDEFINED_CHARS))
	#include "pce_settings.h"
#elif defined(__CIDELSA__) && !defined(REDEFINED_CHARS)
    #include "cidelsa_no_gfx.h"
#else
	#include "default_image_settings.h"
#endif 


