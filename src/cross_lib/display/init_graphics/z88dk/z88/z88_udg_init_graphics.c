#include <dor.h>

// #define HELP1   "A pointless demo application made with z88dk"
// #define HELP2   "Simply loops, points out menu selections and dies"
// #define HELP3   "..eventually..see source for details"
// #define HELP4   ""
// #define HELP5   "v0.1 - 9.4.99 (djm)"
// #define HELP6   ""

#pragma -no-expandz88

#if !defined(BAD_PAGES)
	#define BAD_PAGES 2
#else
	#define BAD_PAGES 1
#endif 
#pragma -reqpag=BAD_PAGES

#pragma printf = ""

// /* Redirect OZ required callbacks to our functions */
// #pragma redirect handlecmds=_handlecmds
// #pragma redirect redrawscreen=_redrawscreen

#define APP_INFO "by Fabrizio Caruso"
#define APP_KEY  'X'
#define APP_NAME "XChase"

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
		
	console_ioctl(IOCTL_GENCON_SET_MODE, &mode); 

	console_ioctl(IOCTL_GENCON_SET_UDGS, &param);

    // static struct window win; /* Window structure */ 	

    // win.graph=1; 
    // win.width=255; 
    // win.number='4'; 
    // window(win); 

    _setScreenColors();
}

