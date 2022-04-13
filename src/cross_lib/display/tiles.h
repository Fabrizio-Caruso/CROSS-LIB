#ifndef _TILES_H
#define _TILES_H

#  if defined(__C64__)
	#include "c64_memory_mapped_settings.h"
#elif defined(__CREATIVISION__) && defined(CREATIVISION_COLOR) && defined(REDEFINED_CHARS)
    #include "vdp_mode1_settings.h"
#elif defined(__TI99__) && defined(REDEFINED_CHARS)
    #include "vdp_mode1_settings.h"
#elif defined(__CREATIVISION__) && defined(REDEFINED_CHARS)
    #include "creativision_settings.h"
#elif defined(__VIC20__) && defined(VIC20_EXP_3K) && !defined(REDEFINED_CHARS)
	#include "vic20_exp_3k_settings.h"
#elif defined(__VIC20__) && defined(VIC20_EXP_3K) && defined(REDEFINED_CHARS)
	#include "vic20_exp_3k_light_settings.h"
#elif defined(__VIC20__) && defined(MEMORY_MAPPED) && (defined(VIC20_EXP_8K) || defined(VIC20_EXP_3K)) && defined(REDEFINED_CHARS)
	#include "vic20_rom_chars_and_26_tiles_settings.h"
#elif defined(__VIC20__) && defined(VIC20_UNEXPANDED) && defined(REDEFINED_CHARS)
	#include "vic20_rom_chars_and_6_tiles_settings.h"
#elif defined(__VIC20__) && defined(CONIO)
    #if defined(_API_VERSION) && _API_VERSION>=2
        #include "vic20_exp_16k_v2_settings.h"
    #else
        #include "vic20_exp_16k_settings.h"
    #endif
#elif defined(__C16__) && defined(REDEFINED_CHARS) && !defined(C16_UNEXPANDED)
	#include "c264_redefined_chars_settings.h"
#elif defined(__C16__) && defined(REDEFINED_CHARS) && defined(C16_UNEXPANDED)
	#include "c264_link_time_redefined_chars_settings.h"
#elif defined(__CBM__) && defined(MEMORY_MAPPED)
	#include "petscii_memory_mapped_settings.h"
#elif defined(__MO5__) || defined(__TO7__) || ( (defined(__COCO__) || defined(__DRAGON__)) && (defined(BIT_MAPPED) || defined(BIT_MAPPED_4)))
	#include "udg_settings.h"
#elif (defined(__COCO__) || defined(__DRAGON__)) && defined(MEMORY_MAPPED)
	#include "cmoc_memory_mapped_settings.h"
#elif (defined(__ATARI__) && defined(ATARI_MODE1))
	#include "atari_mode1_redefined_chars_settings.h"
#elif defined(__ATARI5200__)
	#include "atari_mode1_redefined_chars_settings.h"
#elif defined(__ATMOS__) && !defined(NO_UDG)
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
#elif defined(__NES__) && defined(CONIO)
	#include "nes_settings.h"
#elif defined(__NES__) && defined(NES_CONIO)
	#include "nes_conio_settings.h"
#elif defined(__CIDELSA__) && !defined(REDEFINED_CHARS)
    #include "cidelsa_no_gfx.h"
#elif defined(COMX_COLOR) && !defined(__MICRO__)
	#include "comx_color_settings.h"
#elif defined(__COMX__) || defined(__PECOM__) || ((defined(__CIDELSA__) && defined(REDEFINED_CHARS)))
    #include "comx_settings.h"
#elif defined(__MICRO__)
	// #include "micro_settings.h"
#elif defined(__ATARI_LYNX__) && defined(TGI_GFX)
    #include "tgi_gfx_settings.h"
#elif defined(UDG_GRAPHICS)
	#include "udg_settings.h"
#elif defined(__PCE__) || ( defined(__GAMATE__) && defined(REDEFINED_CHARS))
	#include "pce_settings.h"
#elif (defined(__APPLE2__)||defined(__APPLE2ENH__))&&defined(APPLE2_HGR)
    #include "apple2_hgr_settings.h"
#else
	#include "default_image_settings.h"
#endif 


#endif // _TILES_H

