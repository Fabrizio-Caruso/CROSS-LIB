
#include <spectrum.h>
#include <graphics.h>

#include "../../../image_settings/spectrum_redefined_chars_settings.h"
#include "../../../display_macros.h"

#include "../../../graphics_data/8x8_chars.h"

extern char udg[];

#include "../../../redefine_characters/udg_map.h"

void INIT_GRAPHICS(void)
{
	unsigned char i;
	for (i = 0; i < sizeof(redefine_map) / sizeof(*redefine_map); ++i)
	{
	   memcpy(udg + (redefine_map[i].ascii - 32)*8, redefine_map[i].bitmap, 8);
	}

	clg();
	zx_border(0);
	zx_colour(PAPER_BLACK|INK_WHITE);
	textbackground(COLOR_BLACK);
}


