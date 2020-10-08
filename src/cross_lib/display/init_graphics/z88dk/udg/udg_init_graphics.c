
#include <graphics.h>

#include "display_macros.h"

#include <sys/ioctl.h>

extern uint8_t udgs[];

void INIT_GRAPHICS(void)
{
	void *param = &udgs;
	console_ioctl(IOCTL_GENCON_SET_FONT32, &param);
	
	#if defined(__MC1000__) || defined(__PC6001__) || defined(__CPC__) || defined(__MULTI8__) || defined(__SPC1000__) || defined(__GB__) || defined(__SMS__)
	{
		#if defined(FORCE_SCREEN_MODE)
			#define SCREEN_MODE FORCE_SCREEN_MODE
		#elif defined(__MC1000__) || defined(__GB__) 
			#define SCREEN_MODE 1
		#elif defined(__SPC1000__) || defined(__SMS__)
			#define SCREEN_MODE 2
		#else
			#define SCREEN_MODE 2
		#endif
		int mode = SCREEN_MODE;
		console_ioctl(IOCTL_GENCON_SET_MODE, &mode);
        
    // #if defined(__GAME_GEAR__)
    // {
        // #include <sms.h>
        // static unsigned char pal[] = { 0x00, 0x00, 0x08, 0x0c, 0x10, 0x30, 0x01, 0x3c, 0x02, 0x03, 0x05, 0x0f, 0x04, 0x33, 0x15, 0x3f };

        // load_palette(&pal,0,16);
    // }
    // #endif
	}
	#elif defined(__SPECTRUM__)
	printf("\x01\x20");
    zx_border(0);
    textbackground(COLOR_BLACK);
	#endif
}


