#include <stdio.h>
#include <stropts.h>
#include <arch/sms.h>
#include <rect.h>
#include <input.h>
#include <arch/sms/SMSlib.h>
#include <arch/sms/PSGlib.h>


// -----------------------------------------------------------------------

extern const unsigned char	finish_psg[];
#define				finish_psg_size 88

extern const unsigned char	font_psgcompr[];
#define				font_psgcompr_size 398

extern const unsigned char	gamelogo_psgcompr[];
#define				gamelogo_psgcompr_size 1633

extern const unsigned char	gamelogotilemap_bin[];
#define				gamelogotilemap_bin_size 512

extern const unsigned char	gameover_psg[];
#define				gameover_psg_size 336

extern const unsigned char	mikgames_psgcompr[];
#define				mikgames_psgcompr_size 226

extern const unsigned char	mikgamestilemap_bin[];
#define				mikgamestilemap_bin_size 128

extern const unsigned char	ost_psg[];
#define				ost_psg_size 4464

extern const unsigned char	sfx_die_psg[];
#define				sfx_die_psg_size 43

extern const unsigned char	sfx_explosion_psg[];
#define				sfx_explosion_psg_size 25

extern const unsigned char	sfx_get_psg[];
#define				sfx_get_psg_size 32

extern const unsigned char	sfx_jump_psg[];
#define				sfx_jump_psg_size 10

extern const unsigned char	sfx_life_psg[];
#define				sfx_life_psg_size 35

extern const unsigned char	sprites_psgcompr[];
#define				sprites_psgcompr_size 836

extern const unsigned char	stage1tilemap_bin[];
#define				stage1tilemap_bin_size 1536

extern const unsigned char	stage2tilemap_bin[];
#define				stage2tilemap_bin_size 1536

extern const unsigned char	stage3tilemap_bin[];
#define				stage3tilemap_bin_size 1536

extern const unsigned char	stage4tilemap_bin[];
#define				stage4tilemap_bin_size 1536

extern const unsigned char	stage5tilemap_bin[];
#define				stage5tilemap_bin_size 1536

extern const unsigned char	stage6tilemap_bin[];
#define				stage6tilemap_bin_size 1536

extern const unsigned char	tiles_psgcompr[];
#define				tiles_psgcompr_size 561


// -------------------------------------------------------------------------------------






const unsigned char author[] = "Fabrizio Caruso";
const unsigned char name[] = "Cross Chase";
const unsigned char description[] = "A Universal 8-bit Game";

extern unsigned char font_8x8_bbc_system[];

const unsigned char palette[] = {0x00,0x01,0x05,0x15,0x24,0x25,0x29,0x39,0x16,0x1a,0x1b,0x2a,0x3a,0x3e,0x2b,0x3f};

const struct r_Rect8 rect = {0, 32, 0, 24};


void INIT_GRAPHICS(void)
{
	sms_memcpy_mem_to_cram(0, palette, 16);
	sms_memcpy_mem_to_cram(16, palette, 16);

	sms_vdp_set_write_address(0x0000);
	sms_copy_font_8x8_to_vram(font_8x8_bbc_system, 128 - 32, 0, 1);
	
	sms_display_on();
	sms_border(4);
	
	// clear entire screen with # character
	
	sms_cls_wc(&rect,'#' - 32);
	
	// clear terminal only
	
	ioctl(1, IOCTL_OTERM_CLS);
	ioctl(1, IOCTL_OTERM_PAUSE, 0);

	{
		unsigned char i;
		
		for(i=0;i<20;++i)
		{
		SMS_setNextTileatXY(i,i);
		SMS_setTile(65);
		
		}
	}
	
	in_pause(250);
	in_pause(250);
	
	while(1);
	
}

