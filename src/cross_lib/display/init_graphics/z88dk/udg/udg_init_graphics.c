
#include <graphics.h>

#include "display_macros.h"

#include <sys/ioctl.h>

#if defined(__HECTORHR__)
        #include <arch/hector.h>
#endif 

extern uint8_t udgs[];


void _XL_INIT_GRAPHICS(void)
{
	void *param = &udgs;
	console_ioctl(IOCTL_GENCON_SET_FONT32, &param);
	

    // Necessary to have .vz file correctly started on several emulators that do not load .vz as they should
    #if defined(__VZ__)
        *((unsigned char *)0x7839) |= 1;
    #endif

	#if !defined(__SCREEN_MODE)
		#define __SCREEN_MODE 2
	#endif
	
	#if defined(__MC1000__) && defined(__BACKGROUND_COLOR) && __BACKGROUND_COLOR==1
		int mode = __SCREEN_MODE + 32;
	#else
		int mode = __SCREEN_MODE;
	#endif
		
	console_ioctl(IOCTL_GENCON_SET_MODE, &mode); 
    
    // TODO: remove the Spectrum special case
	#if defined(__SPECTRUM__)
        printf("\x01\x20");

        textbackground(_XL_BACKGROUND_COLOR);
        zx_border(_XL_BACKGROUND_COLOR);
    #elif defined(__HECTORHR__)
        // #include <arch/hector.h>
        #if defined(__USE_GREEN)
            hector_set_palette(HECTOR_BLACK, HECTOR_WHITE, HECTOR_GREEN, HECTOR_RED);
        #else
            hector_set_palette(HECTOR_BLACK, HECTOR_WHITE, HECTOR_CYAN, HECTOR_RED);
        #endif
	#else
        _setScreenColors();
    #endif
}


