#ifndef _8X8_CHARS
#define _8X8_CHARS

	#define _PLAYER_DOWN_UDG              { 24, 36, 24,102,153, 24, 36,102}
	#define _PLAYER_UP_UDG                { 24, 60, 24,102,153, 24, 36,102}
	#define _PLAYER_LEFT_UDG              { 24, 44,152,110, 25, 24, 40, 40}
	#define _PLAYER_RIGHT_UDG             { 24, 52, 25,118,152, 24, 20, 20}

	#define _GHOST_UDG                    {129,126,165,129,129,189,129,126}

	#define _SKULL_UDG                    { 60, 66,165,129, 90, 36, 36, 60}

	#define _GUN_UDG                      {  0,128,126,200,248,192,128,  0}

	#define _POWERUP_UDG                  {  0, 60, 54,223,231,122, 36, 24}

	#define _BULLET_UDG                   {  0,  0,  8, 56, 28, 16,  0,  0}

	#define _BOMB_UDG                     { 60, 66,165,153,153,165, 66, 60}

	#define _ROCKET_UDG                   { 24, 60, 60, 60,126, 90, 66, 66}

	#define _RIGHT_HORIZONTAL_MISSILE_UDG {  0,  0, 15,252,252, 15,  0,  0}

	#define _LEFT_HORIZONTAL_MISSILE_UDG  {  0,  0,240, 63, 63,240,  0,  0}

	#define _INVINCIBILITY_UDG            { 24, 36, 24,  0,153,  0, 36,102}

	#define _DEAD_GHOST_UDG               {129,126,165,129,129,189,129,126}

	#define _VERTICAL_BRICK_UDG           { 24, 24, 24, 48, 24, 12, 24, 24}

	#define _HORIZONTAL_BRICK_UDG         {  0,  0,  0,255,  0,  0,  0,  0}

	#define _EXTRA_POINTS_UDG             { 16, 62, 32, 60,  4,124,  8,  0}
	
	#define _FREEZE_UDG                   _POWERUP_UDG
	
	#define _SUPER_UDG                    _POWERUP_UDG
	
	#define _EXTRA_LIFE_UDG               _PLAYER_DOWN_UDG
	
	#define _
#endif // _8x8_CHARS


