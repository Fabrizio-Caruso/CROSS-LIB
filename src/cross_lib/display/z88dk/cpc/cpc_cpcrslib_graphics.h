#ifndef _CPC_CPCRSLIB_GRAPHICS
#define _CPC_CPCRSLIB_GRAPHICS

#include "cpcrslib.h"

#define CPC_WHITE 1
#define CPC_YELLOW 2 
#define CPC_RED 3
#define CPC_BLACK 4

#define UDG_N 20

#define _DRAW_AUX(color,str,x,y) cpc_PrintGphStrStdXY(color,str,(x+X_OFFSET)*2,(y+Y_OFFSET)*8)

#define _DRAW(x,y,image) _DRAW_AUX(image->_color, char_list+image->_imageData, x, y)

#define _DELETE(x,y) _DRAW_AUX(CPC_BLACK,space_str,x,y)


#endif // _CPC_CPCRSLIB_GRAPHICS

