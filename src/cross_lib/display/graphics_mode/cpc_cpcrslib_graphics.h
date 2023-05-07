#ifndef _CPC_CPCRSLIB_GRAPHICS_GRAPHICS
#define _CPC_CPCRSLIB_GRAPHICS_GRAPHICS

#include "standard_libs.h"

extern uint8_t char_list[];

extern uint8_t space_str[];

#define CPC_WHITE 1
#define CPC_YELLOW 2 
#define CPC_RED 3
#define CPC_BLACK 4

#include <cpc.h>

#define _DRAW_AUX(color,str,x,y) cpc_PrintGphStrStdXY(color,str,(x*2),(y*8))

#define _XL_DRAW(x,y,tile,color) _DRAW_AUX(color, char_list+(tile), (x), (y))

#define _XL_DELETE(x,y) _DRAW_AUX(CPC_BLACK,space_str,(x),(y))


#endif // _CPC_CPCRSLIB_GRAPHICS_GRAPHICS

