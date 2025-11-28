

// void _XL_DRAW(uint8_t x, uint8_t y, uint8_t tile, uint8_t color);


// void _XL_DELETE(uint8_t x, uint8_t y);
// TODO: color 
#define _XL_DRAW(x,y,tile,color) \
    _display_tile(x,y,tile,color);

#define _XL_DELETE(x,y) \
    _delete_vga_tile(8*(uint16_t)(x),8*(uint16_t)(y));

// #define _XL_DELETE(x,y) \
    // _display_tile(x,y,0,0);


// #define _XL_DELETE2(x,y) \
    // do \
    // { \
        // _delete_vga_tile(x,y); \
        // _delete_vga_tile(x,(y+1)); \
        // _delete_vga_tile(x,(y+2)); \
        // _delete_vga_tile(x,(y+3)); \
        // _delete_vga_tile(x,(y+4)); \
        // _delete_vga_tile(x,(y+5)); \
        // _delete_vga_tile(x,(y+6)); \
        // _delete_vga_tile(x,(y+7)); \
    // } while(0)