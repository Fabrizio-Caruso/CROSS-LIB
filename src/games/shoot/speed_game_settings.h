#ifndef _SPEED_TARGET_SETTINGS
#define _SPEED_TARGET_SETTINGS

#if defined(_XL_TURN_BASED) || !defined(BOMB_DRAW_SKIP)
    #define SKIP_BOMB_DRAW 
#else
    #define SKIP_BOMB_DRAW if((loop&(BOMB_DRAW_SKIP))==1)
#endif

#if defined(_XL_TURN_BASED) || !defined(WALL_DRAW_SKIP)
    #define SKIP_WALL_DRAW 
#else
    #define SKIP_WALL_DRAW if((loop&(WALL_DRAW_SKIP))==1)
#endif

	
#define ADVANCED_LEFT_MISSILE()  ++leftHorizontalMissile._x
#define ADVANCED_RIGHT_MISSILE() --rightHorizontalMissile._x	


#endif // _SPEED_TARGET_SETTINGS

