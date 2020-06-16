#include <apple2enh.h>

void INIT_GRAPHICS(void)
{
	#if defined(__APPLE2ENH__) && defined(APPLE2ENH_80COL_VIDEO_MODE)
		videomode(80);
	#endif
}

