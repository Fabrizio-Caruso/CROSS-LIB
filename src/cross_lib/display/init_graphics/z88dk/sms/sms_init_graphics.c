#include <stdio.h>
#include <stropts.h>
#include <arch/sms.h>
#include <rect.h>
#include <input.h>

#include "display_macros.h"


const uint8_t author[] = "z88dk";
const uint8_t name[] = "Hello World";
const uint8_t description[] = "Test program for the output terminal.";

extern uint8_t font_8x8_bbc_system[];

const uint8_t palette[] = {0x00,0x01,0x05,0x15,0x24,0x25,0x29,0x39,0x16,0x1a,0x1b,0x2a,0x3a,0x3e,0x2b,0x3f};

const struct r_Rect8 rect = {0, 32, 0, 24};

void _XL_INIT_GRAPHICS(void)
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

    _setScreenColors();
}

