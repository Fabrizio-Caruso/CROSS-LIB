// Alex Thissen has helped a lot to fix a bug

#include "display_macros.h"
#include "graphics_settings.h"
#include "input_macros.h"

#include <tgi.h>
#include <6502.h>
#include <lynx.h>
#include <time.h>


uint8_t _atari_lynx_text_color;


extern uint8_t _compressed_tile_data[_XL_NUMBER_OF_TILES][6];

uint8_t _compressed_font_data[FONT_SIZE][6] = 
{
    // A
    {60, 98, 126, 98, 98, 0},

    // B
    {124, 98, 124, 98, 124,0},

    // C
    {60, 98, 96, 98, 60, 0}, // 

    // D
    {124, 98, 98, 98, 124,0},

    // E
    {126, 96, 120, 96, 126,0},

    // F
    {126, 96, 120, 96, 96, 0},

    // G
    {60, 96, 102, 98, 60, 0}, //

    // H
    {98, 98, 126, 98, 98, 0},

    // I
    {60, 24, 24, 24, 60, 0},

    // J
    {6, 6, 6, 70, 60,0},

    // K
    {100, 104, 112, 104, 100, 0}, //

    // L
    {96, 96, 96, 96, 124, 0},

    // M
    {98, 118, 106, 98, 98, 0},

    // N
    {98, 114, 106, 102, 98, 0},

    // O
    {60, 98, 98, 98, 60, 0},

    // P
    {124, 98, 124, 96, 96, 0},

    // Q
    {60, 98, 98, 106, 60, 0},

    // R
    {124, 98, 124, 104, 100, 0}, //

    // S
    {62, 96, 60, 6, 124, 0},

    // T
    {126, 24, 24, 24, 24, 0},

    // U
    {98, 98, 98, 98, 60, 0},

    // V
    {98, 98, 52, 52, 24, 0},

    // W
    {193, 193, 106, 106, 52, 0},
    
    // X
    {99, 54, 28, 54, 99, 0},
    
    // Y
    {98, 52, 24, 24, 24, 0},
    
    // Z
    {126, 6, 24, 96, 126,0}, 
   
    // DIGITS
    
    // 0
    // {14, 17, 17, 17, 14, 0},
    {28, 34, 34, 34, 28, 0},

    // 1
    // {6, 10, 2, 2, 15, 0},
    {12, 20, 4, 4, 30, 0},

    // 2
    // {30, 1, 14, 16, 31, 0}, 
    {60, 2, 28, 32, 62, 0}, 

    // 3
    // {30, 1, 6, 1, 30, 0},
    {60, 2, 12, 2, 60, 0},

    // 4
    // {6, 10, 18, 31, 2, 0},
    {12, 20, 36, 62, 4, 0},

    // 5
    // {31, 16, 14, 1, 30, 0},
    {62, 32, 28, 2, 60, 0},


    // 6
    // {15, 16, 30, 17, 14, 0},
    {30, 32, 60, 34, 28, 0},

    // 7
    // {31, 1, 2, 4, 8, 0},
    {62, 2, 4, 8, 16, 0},

    // 8
    // {14, 17, 14, 17, 14, 0},
    {28, 34, 28, 34, 28, 0},

    // 9
    // {14, 17, 15, 1, 30, 0},
    {28, 34, 30, 2, 60, 0},

    // SPACE CHARACTER
    {0, 0, 0, 0, 0, 0}

};



uint8_t _tile_data[_XL_NUMBER_OF_TILES+FONT_SIZE][3*6+1];


// Alex Thissen has helped a lot to fix a bug in this code
void init_font_data(void)
{
    uint8_t i;
    uint8_t j;
    
    for(i=0;i<FONT_SIZE;++i)
    {
		uint8_t line; // for line

		for(line=0;line<6;line++)
		{
		  j = line*3;
		  _tile_data[_XL_NUMBER_OF_TILES+i][j] = 0x03;
		  _tile_data[_XL_NUMBER_OF_TILES+i][j+1] = _compressed_font_data[i][line];
		  _tile_data[_XL_NUMBER_OF_TILES+i][j+2] = 0x00;
		}
		_tile_data[_XL_NUMBER_OF_TILES+i][6*3] = 0x00;
    }
}

// Alex Thissen has helped a lot to fix a bug in this code
void init_tile_data(void)
{
    uint8_t i;
    uint8_t j;
    
    for(i=0;i<_XL_NUMBER_OF_TILES;++i)
    {
		uint8_t line; // for line

		for(line=0;line<6;line++)
		{
		  j = line*3;
		  _tile_data[i][j] = 0x03;
		  _tile_data[i][j+1] = _compressed_tile_data[i][line];
		  _tile_data[i][j+2] = 0x00;
		}
		_tile_data[i][6*3] = 0x00;
    }
}




const unsigned char empty[] = {
    0x03,255,0,
    0x03,255,0,
    0x03,255,0,
    0x03,255,0,
    0x03,255,0,
    0x03,255,0,
    0x00,
};


// const unsigned char _clean_bug[] = {
    // 0x03,1,0,
    // 0x03,1,0,
    // 0x03,1,0,
    // 0x03,1,0,
    // 0x03,1,0,
    // 0x03,1,0,
    // 0x00,
// };

SCB_REHV_PAL _tgi_tile[_XL_NUMBER_OF_TILES+FONT_SIZE];

void init_tgi_tile(void)
{
    uint8_t i;
    
    for(i=0;i<_XL_NUMBER_OF_TILES+FONT_SIZE;++i)
    {
		_tgi_tile[i].sprctl0 = (unsigned char) (BPP_1 | TYPE_NORMAL | 0x07);
		_tgi_tile[i].sprctl1 = (unsigned char) (LITERAL | REHV);
        _tgi_tile[i].sprcoll = (unsigned char) NO_COLLIDE;
        _tgi_tile[i].next = (char *) NULL;
        _tgi_tile[i].data = (unsigned char*) _tile_data[i];
        _tgi_tile[i].hpos = (unsigned int) 0x00;
        _tgi_tile[i].vpos = (unsigned int) 0x00;
        _tgi_tile[i].hsize = (unsigned int) 0x100;
        _tgi_tile[i].vsize = (unsigned int) 0x100;
		// _tgi_tile[i].penpal = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
		
        _tgi_tile[i].penpal[0] = (unsigned char) 0x01;
		// _tgi_tile[i].penpal[1] = (unsigned char) 0x23;
		// _tgi_tile[i].penpal[2] = (unsigned char) 0x45;
		// _tgi_tile[i].penpal[3] = (unsigned char) 0x67;
		// _tgi_tile[i].penpal[4] = (unsigned char) 0x89;
		// _tgi_tile[i].penpal[5] = (unsigned char) 0xab;
		// _tgi_tile[i].penpal[6] = (unsigned char) 0xcd;
		// _tgi_tile[i].penpal[7] = (unsigned char) 0xef;
// {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
    }
}



SCB_REHV_PAL Sempty= {
        BPP_1 | TYPE_NORMAL,
        LITERAL | REHV,
        NO_COLLIDE,
        0, // next
        (unsigned char *) empty, // data
        10, 10, // ??
        0x100, 0x100, // hsize/vsize
        {0,255,0,0,0,0,0,0}
};

// SCB_REHV_PAL Sclean_bug= {
        // BPP_1 | TYPE_NORMAL,
        // LITERAL | REHV,
        // NO_COLLIDE,
        // 0,
        // (unsigned char *) _clean_bug,
        // 10, 10,
        // 0x100, 0x100,
        // {0x01}
// };

SCB_REHV_PAL *empty_spr;   
// SCB_REHV_PAL *clean_bug_spr;



void _tgi_gfx_delete(uint8_t x, uint8_t y)
{
    empty_spr->hpos = (x)*8;
    empty_spr->vpos = (y)*6;
    empty_spr->penpal[0] = 1;
    tgi_sprite(empty_spr);
}


void _tgi_gfx_draw(uint8_t x, uint8_t y, uint8_t tile, uint8_t color)
{
    empty_spr->hpos = (x)*8;
    empty_spr->vpos = (y)*6;
    empty_spr->penpal[0] = 1;
    _tgi_tile[tile].hpos = (x)*8;
    _tgi_tile[tile].vpos = (y)*6;
    _tgi_tile[tile].penpal[0]=color;
	tgi_sprite(empty_spr);
    tgi_sprite(&(_tgi_tile[tile]));
}


void _XL_INIT_GRAPHICS(void)
{
    empty_spr = &Sempty;
    // clean_bug_spr = &Sclean_bug;
    
    tgi_install (tgi_static_stddrv);

	tgi_init ();		
	CLI();

    init_tile_data();
    
    init_font_data();

    init_tgi_tile();

	while (tgi_busy())  {  };

    #if defined(_BACKGROUND_COLOR) && _BACKGROUND_COLOR==_XL_WHITE
        tgi_setcolor(TGI_COLOR_WHITE);
	#else
        tgi_setcolor(TGI_COLOR_BLACK);
    #endif

    tgi_bar(0,0,159,101);
	
	// _tgi_gfx_draw(0,0,0,_XL_RED);
	
	// _tgi_gfx_draw(3,0,8,_XL_BLUE);	
	
	// while(1){};

}
