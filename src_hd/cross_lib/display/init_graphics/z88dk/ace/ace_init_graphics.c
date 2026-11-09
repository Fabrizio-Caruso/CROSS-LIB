
#include <graphics.h>

#include "display_macros.h"

#include <sys/ioctl.h>

extern uint8_t udgs[];
extern uint8_t fonts[];


void _XL_INIT_GRAPHICS(void)
{
    void *udg_param = &udgs;
    void *font_param = &fonts;

	#if !defined(__SCREEN_MODE)
		#define __SCREEN_MODE 2
	#endif
	
	int mode = __SCREEN_MODE;


	console_ioctl(IOCTL_GENCON_SET_FONT32, &font_param);

	console_ioctl(IOCTL_GENCON_SET_UDGS, &udg_param);

	console_ioctl(IOCTL_GENCON_SET_MODE, &mode); 


    _setScreenColors();
}


