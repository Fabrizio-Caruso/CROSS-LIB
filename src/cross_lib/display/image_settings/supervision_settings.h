

#define _POWERUP (char) (0x7A-0x20)

#define _FREEZE _POWERUP
#define _SUPER _POWERUP

#define _GHOST (char) (0x76-0x20)
#define _BOMB (char) (0x78-0x20)
#define _GUN (char) (0x7B-0x20)

#define _PLAYER_DOWN '\''
#define _PLAYER_UP ((char) 0x76 - 6 - 0x20)
#define _PLAYER_RIGHT ((char) 0x76 - 2 - 0x20)
#define _PLAYER_LEFT ((char) 0x76 - 4 -0x20)
	

#define _BULLET (char) (0x7C-0x20)
#define _ROCKET ('^'-0x40)
#define _EXTRA_POINTS '*'
#define _LEFT_HORIZONTAL_MISSILE '>'
#define _RIGHT_HORIZONTAL_MISSILE '<'
#define _INVINCIBILITY (0x73 - 0x20)

#define _SKULL (char) (0x77 - 0x20)

#define _DEAD_GHOST _GHOST
#define _EXTRA_LIFE _PLAYER_DOWN
#define _CONFUSE _SKULL


#define _SPACE ' '
#define _VERTICAL_BRICK 66
#define _HORIZONTAL_BRICK (0xC3-128)
#define _BROKEN_BRICK _BOMB


