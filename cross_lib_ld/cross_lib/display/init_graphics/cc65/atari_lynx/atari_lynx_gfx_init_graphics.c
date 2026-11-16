// Alex Thissen has helped a lot to fix a bug

#include "display_macros.h"
#include "graphics_settings.h"
#include "input_macros.h"

#include <tgi.h>
#include <6502.h>
#include <lynx.h>
#include <time.h>

// Default to black
uint8_t _atari_lynx_background_color = TGI_COLOR_BLACK;
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

#define BYTES_PER_LINE 4

uint8_t _tile_data[_XL_NUMBER_OF_TILES+FONT_SIZE][BYTES_PER_LINE*6+1];

// Alex Thissen has helped a lot to fix a bug in this code
void init_tile_data(void)
{
    uint8_t i;
    uint8_t j;
    uint8_t data, bit, byte1, byte2;

    for(i = 0; i < FONT_SIZE+_XL_NUMBER_OF_TILES; ++i)
    {
		uint8_t line; // for line

		for(line = 0; line < 6; line++)
		{
		  j = line*BYTES_PER_LINE;

          // Indicate literal sprite data of 3 bytes (plus 1)
		  _tile_data[i][j] = BYTES_PER_LINE;

          byte1 = 0;
          byte2 = 0;

          for (bit = 0; bit < 8; bit++)
          {
              data = i < _XL_NUMBER_OF_TILES ?
                _compressed_tile_data[i][line] :
                _compressed_font_data[i-_XL_NUMBER_OF_TILES][line];

              if (data & (1 << bit)) {
                  if (bit * 2 < 8) {
                      byte2 |= 1 << (bit * 2);
                  } else {
                      byte1 |= 1 << ((bit * 2) - 8);
                  }
              }
          }
		  _tile_data[i][j+1] = byte1;
          _tile_data[i][j+2] = byte2;

          _tile_data[i][j+3] = 0xAA;

		}
		_tile_data[i][6*BYTES_PER_LINE] = 0x00;
    }
}

scb_hv_pal2 _tgi_tile[_XL_NUMBER_OF_TILES+FONT_SIZE];

void init_tgi_tile(void)
{
    uint8_t i;
    scb_hv_pal2 *tile;

    for(i = 0; i < _XL_NUMBER_OF_TILES+FONT_SIZE; ++i)
    {
        tile = &_tgi_tile[i];
		tile->sprctl0 = BPP_2 | TYPE_NORMAL;
		tile->sprctl1 = LITERAL | REHV;
        tile->sprcoll = NO_COLLIDE;
        tile->next = (char *)NULL;
        tile->data = _tile_data[i];
        tile->hpos = 0;
        tile->vpos = 0;
        tile->hsize = 0x100;
        tile->vsize = 0x100;

        // Palette will use 4 pens for background and color and transparent (for hardware bug)
        tile->penpal[0] = (_atari_lynx_background_color << 4); // Dynamically changed during draw
        tile->penpal[1] = 0x00; // Two (same) transparent pens
    }
}

// No significant bit at end, so no extra byte is needed
const unsigned char empty[] = {
    4, 0, 0, 0x55,
    4, 0, 0, 0x55,
    4, 0, 0, 0x55,
    4, 0, 0, 0x55,
    4, 0, 0, 0x55,
    4, 0, 0, 0x55,
    0x00,
};

scb_hv_pal2 Sempty= {
        BPP_2 | TYPE_NORMAL,
        LITERAL | REHV,
        NO_COLLIDE,
        0, // next
        (unsigned char *)empty, // data
        0, 0,
        0x100, 0x100, // hsize/vsize
        {0x00, 0xef}  // Pen palette 0 will be set in _XL_INIT_GRAPHICS  
};

scb_hv_pal2 *empty_spr;

void _tgi_gfx_delete(uint8_t x, uint8_t y)
{
    // Use 8x6 pixel empty sprite to erase tile
    empty_spr->hpos = (x)*8;
    empty_spr->vpos = (y)*6;
    tgi_sprite(empty_spr);
}

void _tgi_gfx_draw(uint8_t x, uint8_t y, uint8_t tile, uint8_t color)
{
    // Reposition sprite
    _tgi_tile[tile].hpos = (x)*8;
    _tgi_tile[tile].vpos = (y)*6;

    // Solid black for first pen 0 and selected color for pen 1
    _tgi_tile[tile].penpal[0] = (_atari_lynx_background_color << 4) + color;
    tgi_sprite(&(_tgi_tile[tile]));
}

void _XL_INIT_GRAPHICS(void)
{
   #if defined(_XL_BACKGROUND_COLOR)
    _atari_lynx_background_color = _XL_BACKGROUND_COLOR;
    #endif

  Sempty.penpal[0] = _atari_lynx_background_color << 4;
  empty_spr = &Sempty;

  tgi_install(tgi_static_stddrv);

	tgi_init();
	CLI();

  init_tile_data();
  init_tgi_tile();

  tgi_setcolor(_atari_lynx_background_color);
  tgi_bar(0,0,159,101);
  
  while (tgi_busy())  {  };
}
