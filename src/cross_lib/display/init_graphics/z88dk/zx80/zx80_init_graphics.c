
#include "display_macros.h"


void _XL_INIT_GRAPHICS(void)
{
	#if defined(ZX80_GEN_TV_FIELD)
		gen_tv_field_init();
	#endif
    _setScreenColors();
}


