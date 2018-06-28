
void init_graphics(void)
{
#if defined(__C128__) && defined(C128_80COL_VIDEO_MODE)
	#include <c128.h>
			
	fast();
	videomode(80);
}	