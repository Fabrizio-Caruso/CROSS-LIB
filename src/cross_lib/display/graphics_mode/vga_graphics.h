

// void _XL_DRAW(uint8_t x, uint8_t y, uint8_t tile, uint8_t color);


// void _XL_DELETE(uint8_t x, uint8_t y);
// TODO: color 
#define _XL_DRAW(x,y,tile,color) \
    _display_tile(x,y,tile,color);

#define _XL_DELETE(x,y) \
    _display_tile(x,y,0,0);