#ifndef _6X8_CHARS
#define _6X8_CHARS

#if defined(NINTH_CHAR_LINE)
    #define NINTH_EMPTY_LINE 0
    #define NINTH_VERTICAL_BRICK_LINE 12
#else
    #define NINTH_EMPTY_LINE
    #define NINTH_VERTICAL_BRICK_LINE 
#endif

	#define _PLAYER_UDG                   {12,18,12,51,45,12,18,51, NINTH_EMPTY_LINE}
	#define _PLAYER_DOWN_UDG              {12,18,12,51,45,12,18,51, NINTH_EMPTY_LINE}
	#define _PLAYER_UP_UDG                {12,30,12,51,45,12,18,51, NINTH_EMPTY_LINE}
	#define _PLAYER_LEFT_UDG              {12,22,12,62,13,12,20,20, NINTH_EMPTY_LINE}
	#define _PLAYER_RIGHT_UDG             {12,26,12,31,44,12,10,10, NINTH_EMPTY_LINE}

	#define _GHOST_UDG                    {33,30,33,51,33,45,33,30, NINTH_EMPTY_LINE}
	#define _SKULL_UDG                    {30,33,51,33,33,18,18,12, NINTH_EMPTY_LINE}
	#define _GUN_UDG                      { 0,32,31,40,56,32, 0, 0, NINTH_EMPTY_LINE}
	#define _POWERUP_UDG                  { 0,30,51,55,51,26,18,12, NINTH_EMPTY_LINE}
	#define _BULLET_UDG                   { 0, 0, 4,28,14, 8, 0, 0, NINTH_EMPTY_LINE}
	#define _BOMB_UDG                     {30,33,51,45,45,51,33,30, NINTH_EMPTY_LINE}

	#define _DEAD_GHOST_UDG               {18,30,33,51,33,45,51,30, NINTH_EMPTY_LINE}
	
	#define _ROCKET_UDG                   {12,30,30,30,63,45,33,33, NINTH_EMPTY_LINE}
	#define _LEFT_HORIZONTAL_MISSILE_UDG { 0, 0,56,31,31,56, 0, 0, NINTH_EMPTY_LINE}
	#define _RIGHT_HORIZONTAL_MISSILE_UDG  { 0, 0, 7,62,62, 7, 0, 0, NINTH_EMPTY_LINE}
	#define _INVINCIBILITY_UDG            {12,18,12,51, 0,12, 0,51, NINTH_EMPTY_LINE}
	#define _VERTICAL_BRICK_UDG           {12,12,28,12,12,14,12,12, NINTH_VERTICAL_BRICK_LINE}
	#define _HORIZONTAL_BRICK_UDG         { 0, 0, 0,63,63,00,00,00, NINTH_EMPTY_LINE}
	#define _EXTRA_POINTS_UDG             

#endif // _6x8_CHARS

