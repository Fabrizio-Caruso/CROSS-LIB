
#  if defined(__C64__)
	#include "c64_memory_mapped_settings.h"
#elif defined(__CREATIVISION__) && defined(REDEFINED_CHARS)
    #include "creativision_settings.h"
#elif defined(__VIC20__) && defined(VIC20_EXP_3K) && defined(TINY_GAME) && !defined(REDEFINED_CHARS)
	#include "vic20_exp_3k_settings.h"
#elif defined(__VIC20__) && defined(VIC20_EXP_3K) && defined(REDEFINED_CHARS)
	#include "vic20_exp_3k_light_settings.h"
#elif defined(__VIC20__) && (defined(VIC20_EXP_8K) || (defined(VIC20_EXP_3K) && !defined(TINY_GAME))) && defined(REDEFINED_CHARS)
	#include "vic20_exp_8k_settings.h"
#elif defined(__VIC20__) && defined(VIC20_UNEXPANDED) && defined(REDEFINED_CHARS)
	#include "vic20_unexpanded_gfx.h"
#elif defined(__VIC20__) && defined(CONIO)
	#include "vic20_exp_16k_settings.h"
#elif defined(__C16__) && defined(REDEFINED_CHARS) && !defined(C16_UNEXPANDED)
	#include "c264_redefined_chars_settings.h"
#elif defined(__C16__) && defined(REDEFINED_CHARS) && defined(C16_UNEXPANDED)
	#include "c264_unexpanded_redefined_chars_settings.h"
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
#elif defined(__COMX__) || defined(__PECOM__) || (defined(__CIDELSA__) && defined(REDEFINED_CHARS))
	#include "comx_settings.h"
#elif defined(UDG_GRAPHICS)
	#include "udg_settings.h"
#else
	#include "default_image_settings.h"
#endif 


