
#include <rex/rex.h>
#include "display_macros.h"


#define UDG_N 20

char char_list[UDG_N*2] = 
{ 
'*', '\0', // PLAYER
'O', '\0', // GHOST
'X', '\0', // BOMB
'+', '\0', // SKULL
'.', '\0', // BULLET
'P', '\0', // POWERUP
'!', '\0', // GUN
'$', '\0', // EXTRA_POINTS_IMAGE
'V', '\0', // INVINCIBILITY
'*', '\0', // EXTRA LIFE
'>', '\0', // LEFT_HORIZONTAL_MISSILE
'<', '\0', // RIGHT_HORIZONTAL_MISSILE
'^', '\0', // ROCKET
'*', '\0', // PLAYER_DOWN
'*', '\0', // PLAYER_UP
'*', '\0', // PLAYER_RIGHT
'*', '\0', // PLAYER_LEFT    
'|', '\0', // VERTICAL_BRICK
'-', '\0' // HORIZONTAL_BRICK 
};

char space_str[2] = {' ', '\0'};

void _XL_INIT_GRAPHICS(void)
{
	DsEventClear();
	DsClearScreen();
    
    _setScreenColors();
}

