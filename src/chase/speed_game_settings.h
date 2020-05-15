#ifndef _SPEED_TARGET_SETTINGS
#define _SPEED_TARGET_SETTINGS


#if defined(TURN_BASED) || !defined(GHOST_DRAW_SKIP)
    #define SKIP_GHOST_DRAW 
#else
    #define SKIP_GHOST_DRAW if((loop&(GHOST_DRAW_SKIP))==1)
#endif

#if defined(TURN_BASED) || !defined(BOMB_DRAW_SKIP)
    #define SKIP_BOMB_DRAW 
#else
    #define SKIP_BOMB_DRAW if((loop&(BOMB_DRAW_SKIP))==1)
#endif

#if defined(TURN_BASED) || !defined(WALL_DRAW_SKIP)
    #define SKIP_WALL_DRAW 
#else
    #define SKIP_WALL_DRAW if((loop&(WALL_DRAW_SKIP))==1)
#endif


	
#if defined(WIDE)
	#define ADVANCED_LEFT_MISSILE()  ++leftHorizontalMissile._x
	#define ADVANCED_RIGHT_MISSILE() --rightHorizontalMissile._x	
#else
	#define ADVANCED_LEFT_MISSILE()  do{if(loop&1)++leftHorizontalMissile._x;}while(0)
	#define ADVANCED_RIGHT_MISSILE() do{if(loop&1)--rightHorizontalMissile._x;}while(0)
#endif
	
#if defined(TURN_BASED)
		#define GAME_SLOW_DOWN 0
#else	
	#  if defined(__NCURSES__)
		#define GAME_SLOW_DOWN 100
    #elif defined(__OSIC1P__)
        #define GAME_SLOW_DOWN 500U
	#elif defined(__OSCA__)
		#define GAME_SLOW_DOWN 8167U		
	#elif defined(__SC3000__)
		#define GAME_SLOW_DOWN 2500
	#elif defined(__NC100__) || defined(__NC200__)
		#define GAME_SLOW_DOWN 1500
	#elif defined(__MTX__)
		#define GAME_SLOW_DOWN 3300U
	#elif defined(__MSX__)
		#define GAME_SLOW_DOWN 2000U		
	#elif defined(__MC1000__)
		#define GAME_SLOW_DOWN 1200			
	#elif defined(__VIC20__) && defined(ALT_PRINT)
		#define GAME_SLOW_DOWN 500	
	#elif defined(__VIC20__) && !defined(TINY_GAME) 
		#define GAME_SLOW_DOWN 250
	#elif defined(__SAM__)
		#define GAME_SLOW_DOWN 500
	#elif defined(__VIC20__) && defined(TINY_GAME) && !defined(VIC20_UNEXPANDED)
		#define GAME_SLOW_DOWN 400
	#elif defined(__VIC20__) && defined(TINY_GAME) && defined(VIC20_UNEXPANDED)
		#define GAME_SLOW_DOWN 0	
	#elif defined(__WINCMOC__) || defined(__CMOC__) 
		#define GAME_SLOW_DOWN 600
	#elif defined(__PC6001__)
		#define GAME_SLOW_DOWN 500	
	#elif defined(__NASCOM__)
		#define GAME_SLOW_DOWN 8000U
	#elif defined(__ABC80__)
		#define GAME_SLOW_DOWN 8000U		
	#elif defined(__GAMATE__)
		#define GAME_SLOW_DOWN 1200
	#elif defined(__CREATIVISION__) || defined(__ATARI5200__) || (defined(__ATARI__) && defined(ATARI_MODE1)) 
		#define GAME_SLOW_DOWN 1000
	#elif defined(__Z9001__)
		#define GAME_SLOW_DOWN 800	
	#elif defined(__X1__)
		#define GAME_SLOW_DOWN 1500			
	#elif defined(__VZ__)
		#define GAME_SLOW_DOWN 3000U
	#elif defined(__TI82__) || defined(__TI83__) || defined(__TI85__) || defined(__TI8X__) || defined(__TI86__)
		#define GAME_SLOW_DOWN 6000U		
	#elif defined(__NES__)
		#define GAME_SLOW_DOWN 700
	#elif defined(__MZ__)
		#define GAME_SLOW_DOWN 500		
	#elif defined(__APPLE2__) || defined(__APPLE2ENH__)
		#define GAME_SLOW_DOWN 200
	#elif (defined(__C16__) || defined(__PLUS4__)) && defined(FULL_GAME)
		#define GAME_SLOW_DOWN 250
	#elif (defined(__C16__) || defined(__PLUS4__)) && !defined(FULL_GAME)
		#define GAME_SLOW_DOWN 250			
	#elif defined(__ATMOS__)
		#define GAME_SLOW_DOWN 50		
	#elif defined(__CBM610__)
		#define GAME_SLOW_DOWN 700
	#elif defined(__VG5K__) && !defined(ALT_PRINT)
		#define GAME_SLOW_DOWN 2000U
	#elif defined(__VG5K__) && defined(ALT_PRINT)
		#define GAME_SLOW_DOWN 5000U		
	#elif defined(__AQUARIUS__) && defined(ALT_PRINT)
		#define GAME_SLOW_DOWN 1500		
	#elif defined(__AQUARIUS__) && !defined(ALT_PRINT)
		#define GAME_SLOW_DOWN 800	
	#elif defined(__ACE__) || defined(__BEE__)
		#define GAME_SLOW_DOWN 900	
	#elif defined(__MSX__)
		#define GAME_SLOW_DOWN 550		
	#elif defined(__PCE__)
		#define GAME_SLOW_DOWN 4200U	
	#elif defined(__SPECTRUM__) && !defined(__TS2068__)
		#define GAME_SLOW_DOWN 1400
	#elif defined(__TS2068__)
		#define GAME_SLOW_DOWN 3000
	#elif defined(__CPC__) && defined(CPCRSLIB)
		#define GAME_SLOW_DOWN 500	
	#elif defined(__SVI__) && defined(MSX_MODE0)
		#define GAME_SLOW_DOWN 1200
	#elif defined(__ATARI_LYNX__)
		#define GAME_SLOW_DOWN 6000U
	#elif defined(__SVI__) && !defined(MSX_MODE0)
		#define GAME_SLOW_DOWN 1200	
	#elif (defined(__TRS80__) || defined(__EG2K__)) &&  !defined(Z88DK_SPRITES)
		#define GAME_SLOW_DOWN 100	
	#elif defined(__EINSTEIN__)
		#define GAME_SLOW_DOWN 5000	
	#elif defined(__PX4__)
		#define GAME_SLOW_DOWN 2500
	#elif defined(__PX8__)
		#define GAME_SLOW_DOWN 30000U
	#elif defined(__M5__)
		#define GAME_SLOW_DOWN 900		
	#elif defined(__LASER500__)
		#define GAME_SLOW_DOWN 500			
	#else
		#define GAME_SLOW_DOWN 0
	#endif
#endif

#endif // _SPEED_TARGET_SETTINGS

