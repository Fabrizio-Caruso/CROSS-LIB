#include <dor.h>
#pragma -no-expandz88
#if !defined(BAD_PAGES)
	#define BAD_PAGES 2
#else
	#define BAD_PAGES 1
#endif 
#pragma -reqpag=BAD_PAGES
#define APP_INFO "by Fabrizio Caruso"
#define APP_KEY  'X'
#define APP_NAME "XL"
#include <application.h>
#include <graphics.h> 
#include "display_macros.h"

extern uint8_t udgs[];

void _XL_INIT_GRAPHICS(void) 
{ 
	void *param = &udgs;
	#if !defined(__SCREEN_MODE)
		#define __SCREEN_MODE 2
	#endif
    int mode = __SCREEN_MODE;
	console_ioctl(IOCTL_GENCON_SET_FONT32, &param);
        
	console_ioctl(IOCTL_GENCON_SET_MODE, &mode); 

    z88_disable_cursor();
    _setScreenColors();
}

