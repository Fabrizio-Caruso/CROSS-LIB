#include "display_macros.h"
#include "graphics_settings.h"
#include "input_macros.h"

#include <tgi.h>
#include <6502.h>
#include <lynx.h>
#include <time.h>

extern uint8_t _compressed_tile_data[_XL_NUMBER_OF_TILES][6];

uint8_t _tile_data[_XL_NUMBER_OF_TILES][2*6+1];


void init_tile_data(void)
{
    uint8_t i;
    uint8_t j;
    
    for(i=0;i<_XL_NUMBER_OF_TILES;++i)
    {
        for(j=0;j<2*6;j+=2)
        {
            _tile_data[i][j] = 0x02;
            _tile_data[i][j+1] = _compressed_tile_data[i][j>>1];
        }
        _tile_data[i][2*6]=0x00;
    }
}


const unsigned char empty[] = {
    0x02,255,
    0x02,255,
    0x02,255,
    0x02,255,
    0x02,255,
    0x02,255,
    0x00,
};


const unsigned char _clean_bug[] = {
    0x02,1,
    0x02,1,
    0x02,1,
    0x02,1,
    0x02,1,
    0x02,1,
    0x00,
};

SCB_REHV_PAL _tgi_tile[_XL_NUMBER_OF_TILES];

void init_tgi_tile(void)
{
    uint8_t i;
    
    for(i=0;i<_XL_NUMBER_OF_TILES;++i)
    {
        _tgi_tile[i].sprctl0 = (unsigned char) (BPP_1 | TYPE_NORMAL);
        _tgi_tile[i].sprctl1 = (unsigned char) (LITERAL | REHV);
        _tgi_tile[i].sprcoll = (unsigned char) NO_COLLIDE;
        _tgi_tile[i].next = (char *) NULL;
        _tgi_tile[i].data = (unsigned char*) _tile_data[i];
        _tgi_tile[i].hsize = (unsigned int) 0x100;
        _tgi_tile[i].vsize = (unsigned int) 0x100;
        _tgi_tile[i].penpal[0] = (unsigned char) 0x14;
    }
}
    

SCB_REHV_PAL Sempty= {
        BPP_1 | TYPE_NORMAL,
        LITERAL | REHV,
        NO_COLLIDE,
        0,
        (unsigned char *) empty,
        10, 10,
        0x100, 0x100,
        {0x11}
};

SCB_REHV_PAL Sclean_bug= {
        BPP_1 | TYPE_NORMAL,
        LITERAL | REHV,
        NO_COLLIDE,
        0,
        (unsigned char *) _clean_bug,
        10, 10,
        0x100, 0x100,
        {0x01}
};

SCB_REHV_PAL *empty_spr;   
SCB_REHV_PAL *clean_bug_spr;

void _XL_INIT_GRAPHICS(void)
{
    empty_spr = &Sempty;
    clean_bug_spr = &Sclean_bug;
    
    tgi_install (tgi_static_stddrv);

	tgi_init ();		
	CLI();

    init_tile_data();

    init_tgi_tile();

	while (tgi_busy())  {  };

    tgi_setcolor(TGI_COLOR_BLACK);
    // tgi_setcolor(TGI_COLOR_WHITE);

    tgi_bar(0,0,159,101);

}
