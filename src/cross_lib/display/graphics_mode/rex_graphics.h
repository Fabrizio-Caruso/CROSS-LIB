#ifndef _REX_GRAPHICS
#define _REX_GRAPHICS

#include <rex/rex.h>

#define UDG_N 20

#define __DRAW(x,y,image) DsPrintf((x),(y),16,char_list+image->_imageData)

#define __DELETE(x,y) DsPrintf((x),(y),16,space_str)

#endif // _REX_GRAPHICS

