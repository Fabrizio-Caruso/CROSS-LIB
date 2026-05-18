#ifndef _TERMINAL_GRAPHICS_H
#define _TERMINAL_GRAPHICS_H

void _terminal_draw(uint8_t x, uint8_t y, uint8_t tile, uint8_t color);
void _terminal_delete(uint8_t x, uint8_t y);

    #define _XL_DRAW(x,y,tile,color) \
        _terminal_draw(x,y,tile,color)
		


	#define _XL_DELETE(x,y) \
        _terminal_delete(x,y)



#endif // _TERMINAL_GRAPHICS_H



