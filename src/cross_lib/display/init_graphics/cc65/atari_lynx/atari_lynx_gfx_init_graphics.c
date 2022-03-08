#include "display_macros.h"
#include "graphics_settings.h"
#include "input_macros.h"

#include <tgi.h>
#include <6502.h>
#include <lynx.h>
#include <time.h>


// #define CLI() asm("\tcli")



const unsigned char __tgi_tile[] = {
    0x02,0x54,
    0x02,0x38,
    0x02,0xEE,
    0x02,0x38,
    0x02,0x54,
    0x00,
};


const unsigned char grid[] = {
    0x02,170,
    0x02,255,
    0x02,170,
    0x02,255,
    0x02,170,
    0x02,255,
    0x02,170,
    0x02,255,
    0x00,
};



const unsigned char empty[] = {
    0x02,255,
    0x02,255,
    0x02,255,
    0x02,255,
    0x02,255,
    0x02,255,
    0x00,
};

// const unsigned char freeze[] = {
    // 0x02,0x10,
    // 0x02,0x54,
    // 0x02,0x38,
    // 0x02,0xEE,
    // 0x02,0x38,
    // 0x02,0x54,
    // 0x02,0x10,
    // 0x00,
// };

// unsigned char palette[] = {
    // 0x00,0x0C,0x08,0x0E,0x02,0x0C,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,
    // 0x00,0x2C,0x22,0xEE,0x2E,0xE4,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
// };


// static SCB_REHV_PAL Stile3 = {
	// BPP_3 | TYPE_BACKGROUND,
	// LITERAL | REHV,
	// NO_COLLIDE,
	// 0,
	// freeze,
	// 60,30,
	// 0x100, 0x100,
	// {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef}
// };


// typedef struct SCB_REHV_PAL {             // SCB without str/tilt, w/ penpal
  // unsigned char sprctl0;
  // unsigned char sprctl1;
  // unsigned char sprcoll;
  // char *next;
  // unsigned char *data;
  // signed int hpos;
  // signed int vpos;
  // unsigned int hsize;
  // unsigned int vsize;
  // unsigned char penpal[8];
// } SCB_REHV_PAL;



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
        _tgi_tile[i].data = (unsigned char*) __tgi_tile;
        _tgi_tile[i].hsize = (unsigned int) 0x100;
        _tgi_tile[i].vsize = (unsigned int) 0x100;
        _tgi_tile[i].penpal[0] = (unsigned char) 0x14;
    }
}
    

// SCB_REHV_PAL Sfreeze= {
        // BPP_1 | TYPE_NORMAL,
        // LITERAL | REHV,
        // NO_COLLIDE,
        // 0,
        // (unsigned char *) freeze,
        // 10, 10,
        // 0x100, 0x100,
        // {0x13}
// };


// SCB_REHV_PAL Sgrid= {
        // BPP_1 | TYPE_NORMAL,
        // LITERAL | REHV,
        // NO_COLLIDE,
        // 0,
        // (unsigned char *) grid,
        // 10, 10,
        // 0x100, 0x100,
        // {0x13}
// };


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

// #include<stdio.h>

SCB_REHV_PAL *empty_spr;   

void _XL_INIT_GRAPHICS(void)
{
    uint8_t i;
    uint16_t j;
    
    // SCB_REHV_PAL *freeze_spr;
    // SCB_REHV_PAL *grid_spr;

    // freeze_spr = &Sfreeze;
    // grid_spr = &Sgrid;
    empty_spr = &Sempty;
    
    tgi_install (tgi_static_stddrv);

	tgi_init ();		
	CLI();

    init_tgi_tile();

	while (tgi_busy())  {  };

    tgi_setcolor(TGI_COLOR_BLACK);
    // tgi_setcolor(TGI_COLOR_WHITE);

    tgi_bar(0,0,159,101);
    
    
    // freeze_spr->hpos = 40;
    // freeze_spr->vpos = 40;
    // freeze_spr->penpal[0] = 0x04;
    // tgi_sprite(freeze_spr);
    
    // empty_spr->hpos = 40;
    // empty_spr->vpos = 40;
    // empty_spr->penpal[0] = 1;
    // tgi_sprite(empty_spr);    
    
    // for(i=1;i<15;++i)
    // {
        // _tgi_tile[i].hpos = 0+10*i;
        // _tgi_tile[i].vpos = 0+i;
        // _tgi_tile[i].penpal[0] = 0x04;
        // tgi_sprite(&_tgi_tile[i]);       
        // for(j=1;j<10000;++j){};
    // }
    
    // for(i=1;i<16;++i)
    // {
        // empty_spr->hpos = 0+10*i;
        // empty_spr->vpos = 0+i;
            // empty_spr->penpal[0] = TGI_COLOR_BLACK;
            // tgi_sprite(empty_spr);
    // }
    
    
    // tgi_updatedisplay();
    
    // while(1){};
}
