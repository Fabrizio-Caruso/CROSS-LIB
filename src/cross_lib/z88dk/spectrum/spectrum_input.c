#include <input.h>



unsigned char GET_CHAR(void)
{
	#if defined(CLIB_ANSI)
		return in_Inkey();
	#else
		return in_inkey();
	#endif
}

