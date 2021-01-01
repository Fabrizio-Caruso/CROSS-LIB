#ifndef _REX_GRAPHICS
#define _REX_GRAPHICS

#include <rex/rex.h>

#include "standard_libs.h"

extern uint8_t char_list[];

extern uint8_t space_str[];

#define _XLIB_DRAW_TILE(x,y,tile,color) DsPrintf((x),(y),16,char_list+tile)

#define __DRAW(x,y,image) DsPrintf((x),(y),16,char_list+image->_imageData)

#define __DELETE(x,y) DsPrintf((x),(y),16,space_str)

#endif // _REX_GRAPHICS

