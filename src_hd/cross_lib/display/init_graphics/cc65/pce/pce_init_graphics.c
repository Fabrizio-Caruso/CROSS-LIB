#include <peekpoke.h>
#include <pce.h>
#include <_vdc.h>

#include "display_macros.h"


#define VDC_CTRL     0x0000
#define VDC_DATA_LO  0x0002
#define VDC_DATA_HI  0x0003

/*
VDC_CTRL        = $0000
VDC_DATA_LO     = $0002
VDC_DATA_HI = $0003
*/


void _XL_INIT_GRAPHICS(void)
{
	uint8_t i;
	// uint8_t j;
	
	// for(j=0;j<8;++j)
		for(i=0;i<254;++i)
		{
			POKE(VDC_CTRL,0x20);
			POKE(VDC_CTRL,0x00);
			
			POKE(VDC_DATA_HI,i);
			POKE(VDC_DATA_LO,i);
			// __vdc.data = i;
			// POKE(VDC_MAWR,0x20+j);
			// POKE(VDC_MAWR,0x00+i);
			// POKE(VDC_VWR,i);
		}

    _setScreenColors();
}


