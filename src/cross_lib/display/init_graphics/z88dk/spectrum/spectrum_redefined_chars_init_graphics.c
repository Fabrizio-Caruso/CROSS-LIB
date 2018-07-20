
#include <spectrum.h>
#include <graphics.h>

#include "../../../image_settings/spectrum_redefined_chars_settings.h"
#include "../../../display_macros.h"

#if defined(ANIMATE_PLAYER)
	extern char udg[];
#endif

#if defined(ANIMATE_PLAYER)
struct redefine_struct
{
   unsigned char ascii;
   unsigned char bitmap[8];
} ;
#endif


#if defined(ANIMATE_PLAYER)
	struct redefine_struct redefine_map[] =
	{
		{_PLAYER_DOWN, { 24, 36, 24,102,153, 24, 36,102}},
		{_PLAYER_UP, { 24, 60, 24,102,153, 24, 36,102}},
		{_PLAYER_RIGHT, { 24, 52, 25,118,152, 24, 20, 20}},	
		{_PLAYER_LEFT, { 24, 44,152,110, 25, 24, 40, 40}},
		{_GHOST, {129,126,165,129,129,189,129,126}},
		{_BOMB, { 60, 66,165,153,153,165, 66, 60}},
		
		#if defined(FULL_GAME)
		{_RIGHT_HORIZONTAL_MISSILE, {  0,  0, 15,252,252, 15,  0,  0}},
		{_LEFT_HORIZONTAL_MISSILE, {  0,  0,240, 63, 63,240,  0,  0}},
		#endif
		
		#if !defined(TINY_GAME)
		{_SKULL, { 60, 66,165,129, 90, 36, 36, 60}},
		{_GUN, {  0,128,126,200,248,192,128,  0}},
		{_POWERUP, {  0, 60, 54,223,231,122, 36, 24}},
		{_BULLET, {  0,  0,  8, 56, 28, 16,  0,  0}},
		{_ROCKET, { 24, 60, 60, 60,126, 90, 66, 66}},
		{_INVINCIBILITY, { 24, 36, 24,  0,153,  0, 36,102}},
		{_VERTICAL_BRICK, { 24, 24, 24, 48, 24, 12, 24, 24}},
		{_HORIZONTAL_BRICK, {  0,  0,  0,255,  0,  0,  0,  0}}		
		#endif
	};
#endif


void INIT_GRAPHICS(void)
{
	#if defined(ANIMATE_PLAYER)
		unsigned char i;
		for (i = 0; i < sizeof(redefine_map) / sizeof(*redefine_map); ++i)
		{
		   memcpy(udg + (redefine_map[i].ascii - 32)*8, redefine_map[i].bitmap, 8);
		}
	#endif

	clg();
	zx_border(0);
	zx_colour(PAPER_BLACK|INK_WHITE);
	textbackground(COLOR_BLACK);

}

