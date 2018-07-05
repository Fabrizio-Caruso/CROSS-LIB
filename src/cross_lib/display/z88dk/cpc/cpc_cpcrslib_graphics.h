
#include "cpcrslib.h"

#define CPC_WHITE 1
#define CPC_YELLOW 2 
#define CPC_RED 3
#define CPC_BLACK 4

#define UDG_N 20

char char_list[UDG_N*2] = 
{ 
33, '\0', // PLAYER
37, '\0', // GHOST
44, '\0', // BOMB
40, '\0', // SKULL
43, '\0', // BULLET
42, '\0', // POWERUP
41, '\0', // GUN
49, '\0', // EXTRA_POINTS_IMAGE
46, '\0', // INVINCIBILITY
33, '\0', // EXTRA LIFE
39, '\0', // LEFT_MISSILE
38, '\0', // RIGHT_MISSILE
45, '\0', // ROCKET
33, '\0', // DOWN
34, '\0', // UP
35, '\0', // RIGHT
36, '\0', // LEFT
47, '\0', // VERTICAL_BRICK
48, '\0' // HORIZONTAL_BRICK 
};

#define _DRAW_AUX(color,str,x,y) cpc_PrintGphStrStdXY(color,str,(x+X_OFFSET)*2,(y+Y_OFFSET)*8)

char space_str[2] = {' ', '\0'};

#define _DRAW(x,y,image) _DRAW_AUX(image->_color, char_list+image->_imageData, x, y)

#define _DELETE(x,y) _DRAW_AUX(CPC_BLACK,space_str,x,y)



