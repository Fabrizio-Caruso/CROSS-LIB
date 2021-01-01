#ifndef _CPC_CPCRSLIB_GRAPHICS
#define _CPC_CPCRSLIB_GRAPHICS

#include "standard_libs.h"

extern uint8_t char_list[];

extern uint8_t space_str[];

#define CPC_WHITE 1
#define CPC_YELLOW 2 
#define CPC_RED 3
#define CPC_BLACK 4

#include <cpc.h>

#define _DRAW_AUX(color,str,x,y) cpc_PrintGphStrStdXY(color,str,(x*2),(y*8))

#define _XLIB_DRAW_TILE(x,y,tile,color) _DRAW_AUX(color, char_list+(tile), (x), (y))

#define __DRAW(x,y,image) _DRAW_AUX(image->_color, char_list+image->_imageData, (x), (y))

#define __DELETE(x,y) _DRAW_AUX(CPC_BLACK,space_str,(x),(y))


#endif // _CPC_CPCRSLIB_GRAPHICS

