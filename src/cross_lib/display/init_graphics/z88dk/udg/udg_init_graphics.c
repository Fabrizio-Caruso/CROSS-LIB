
#include <graphics.h>

#include "../../../display_macros.h"

#include <sys/ioctl.h>

extern unsigned char udgs[];

void INIT_GRAPHICS(void)
{
	void *param = &udgs;
	console_ioctl(IOCTL_GENCON_SET_FONT32, &param);
	
	#if defined(__MC1000__) || defined(__PC6001__) || defined(__CPC__) || defined(__MULTI8__) || defined(__SPC1000__)
	{
		#if defined(FORCE_SCREEN_MODE)
			#define SCREEN_MODE FORCE_SCREEN_MODE
		#elif defined(__MC1000__)
			#define SCREEN_MODE 1
		#else
			#define SCREEN_MODE 2
		#endif
		int mode = SCREEN_MODE;
		console_ioctl(IOCTL_GENCON_SET_MODE, &mode);
	}
	#endif
}


