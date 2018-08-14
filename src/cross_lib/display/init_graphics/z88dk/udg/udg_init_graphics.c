
#include <graphics.h>

#include "../../../display_macros.h"

#include <sys/ioctl.h>

extern unsigned char udgs[];

void INIT_GRAPHICS(void)
{
	void *param = &udgs;
	console_ioctl(IOCTL_GENCON_SET_FONT32, &param);
	
	#if defined(__MC1000__)
	{
		int mode = 1;
		console_ioctl(IOCTL_GENCON_SET_MODE, &mode);
	}
	#endif
}


