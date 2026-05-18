
#define _XL_DRAW(x,y,tile,color) \
    _display_tile(x,y,tile,color);

#define _XL_DELETE(x,y) \
    _delete_vga_tile(8*(uint16_t)(x),8*(uint16_t)(y));

