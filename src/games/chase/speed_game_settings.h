#ifndef _SPEED_TARGET_SETTINGS
#define _SPEED_TARGET_SETTINGS


#if defined(TINY_GAME) || defined(TURN_BASED) || !defined(GHOST_DRAW_SKIP)
    #define SKIP_GHOST_DRAW 
#else
    #define SKIP_GHOST_DRAW if(!(loop&GHOST_DRAW_SKIP))
#endif

#if defined(TINY_GAME) || defined(TURN_BASED) || !defined(BOMB_DRAW_SKIP)
    #define SKIP_BOMB_DRAW 
#else
    #define SKIP_BOMB_DRAW if(!(loop&BOMB_DRAW_SKIP))
#endif

#if defined(TURN_BASED) || !defined(WALL_DRAW_SKIP)
    #define SKIP_WALL_DRAW 
#else
    #define SKIP_WALL_DRAW if(!(loop&WALL_DRAW_SKIP))
#endif


	
#if defined(WIDE) && !defined(SLOW_MISSILE)
	#define ADVANCED_LEFT_MISSILE()  ++leftHorizontalMissile._x
	#define ADVANCED_RIGHT_MISSILE() --rightHorizontalMissile._x	
#else
	#define ADVANCED_LEFT_MISSILE()  do{if(loop&1)++leftHorizontalMissile._x;}while(0)
	#define ADVANCED_RIGHT_MISSILE() do{if(loop&1)--rightHorizontalMissile._x;}while(0)
#endif
	


#endif // _SPEED_TARGET_SETTINGS

