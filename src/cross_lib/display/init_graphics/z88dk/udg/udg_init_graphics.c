
#include <graphics.h>

#include "display_macros.h"

#include <sys/ioctl.h>

extern uint8_t udgs[];


void _XL_INIT_GRAPHICS(void)
{
	void *param = &udgs;
	console_ioctl(IOCTL_GENCON_SET_FONT32, &param);
	

	#if !defined(__SCREEN_MODE)
		#define __SCREEN_MODE 2
	#endif
	
	#if defined(__MC1000__) && defined(_BACKGROUND_COLOR) && _BACKGROUND_COLOR==_XL_WHITE
		int mode = __SCREEN_MODE + 32;
	#else
		int mode = __SCREEN_MODE;
	#endif
		
	console_ioctl(IOCTL_GENCON_SET_MODE, &mode); 
    
    // TODO: remove the Spectrum special case
	#if defined(__SPECTRUM__)
        printf("\x01\x20");

        #if defined(_BACKGROUND_COLOR) && _BACKGROUND_COLOR==_XL_WHITE
            textbackground(_XL_WHITE);
            zx_border(7);
        #else
            textbackground(_XL_BLACK);
            zx_border(0);
        #endif
	#else
        _setScreenColors();
    #endif
}


