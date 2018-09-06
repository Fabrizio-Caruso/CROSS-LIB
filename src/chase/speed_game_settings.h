#ifndef _SPEED_TARGET_SETTINGS
#define _SPEED_TARGET_SETTINGS

#if defined(TINY_GAME) || defined(TURN_BASED)
	#define SKIP_DRAW 
	#define SKIP_MORE_DRAW
#elif defined(__ZX81__) || defined(__LAMBDA__)
	#define SKIP_DRAW \
		if((loop&1)==1)
	
	#define SKIP_MORE_DRAW \
		if((loop&15)==1)	
#elif defined(__C128_Z80__)
	#define SKIP_DRAW \
		if(loop&1)
	
	#define SKIP_MORE_DRAW \
		if((loop&15)==1)	
						
#elif defined(__SVI__) && !defined(MSX_MODE0)
	#define SKIP_DRAW \
		if((loop&1)==1) 
	
	#define SKIP_MORE_DRAW \
		if((loop&7)==1)		
#elif !defined(__WINCMOC__) && defined(__CMOC__)
	#define SKIP_DRAW
	
	#define SKIP_MORE_DRAW 	\
		if((loop%16)==1)				
#elif defined(__WINCMOC__) && defined(__CMOC__)
	#define SKIP_DRAW
	
	#define SKIP_MORE_DRAW \
		if((loop%8)==1)					
#else
	#define SKIP_DRAW 

	#define SKIP_MORE_DRAW 

#endif
	
#if defined(WIDE)
	#define ADVANCED_LEFT_MISSILE()  ++leftHorizontalMissile._x
	#define ADVANCED_RIGHT_MISSILE() --rightHorizontalMissile._x	
#else
	#define ADVANCED_LEFT_MISSILE()  do{if(loop&1)++leftHorizontalMissile._x;}while(0)
	#define ADVANCED_RIGHT_MISSILE() do{if(loop&1)--rightHorizontalMissile._x;}while(0)
#endif
	
#if defined(TURN_BASED)
	#if defined(__ZX80__) || defined(__C128_Z80__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 0
	#else
		#define GAME_SLOW_DOWN 0
	#endif
#else	
	#  if defined(__NCURSES__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 100
	#elif defined(__SC3000__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 2500
	#elif defined(__SUPERVISION__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 3300U	
	#elif defined(__NC100__) || defined(__NC200__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 1500
	#elif defined(__MTX__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 3300U
	#elif defined(__MSX__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 2000U		
	#elif defined(__MC1000__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 1200			
	#elif defined(__VIC20__) && defined(ALT_PRINT)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 500	
	#elif defined(__VIC20__) && !defined(TINY_GAME) 
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 250
	#elif defined(__SAM__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 500
	#elif defined(__VIC20__) && defined(TINY_GAME) && !defined(VIC20_UNEXPANDED)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 400
	#elif defined(__VIC20__) && defined(TINY_GAME) && defined(VIC20_UNEXPANDED)
		#define GAME_SLOW_DOWN 0	
	#elif defined(__WINCMOC__) || defined(__CMOC__) 
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 600
	#elif defined(__PC6001__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 500	
	#elif defined(__NASCOM__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 8000U
	#elif defined(__ABC80__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 8000U		
	#elif defined(__GAMATE__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 1200
	#elif defined(__CREATIVISION__) || defined(__ATARI5200__) || ((defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1)) 
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 700
	#elif defined(__Z9001__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 800	
	#elif defined(__X1__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 1500			
	#elif defined(__VZ__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 3000U
	#elif defined(__TI82__) || defined(__TI83__) || defined(__TI85__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 6000U		
	#elif defined(__NES__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 700
	#elif defined(__MZ__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 500		
	#elif defined(__APPLE2__) || defined(__APPLE2ENH__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 200
	#elif (defined(__C16__) || defined(__PLUS4__)) && defined(FULL_GAME)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 250
	#elif (defined(__C16__) || defined(__PLUS4__)) && !defined(FULL_GAME)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 250		
	#elif defined(__C64__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 50		
	#elif defined(__ATMOS__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 50		
	#elif defined(__CBM610__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 700
	#elif defined(__VG5K__) && !defined(ALT_PRINT)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 2000U
	#elif defined(__VG5K__) && defined(ALT_PRINT)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 5000U		
	#elif defined(__AQUARIUS__) && defined(ALT_PRINT)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 1500		
	#elif defined(__AQUARIUS__) && !defined(ALT_PRINT)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 800	
	#elif defined(__ACE__) || defined(__BEE__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 900	
	#elif defined(__MSX__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 550		
	#elif defined(__PCE__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 4200U	
	#elif defined(__SPECTRUM__) && defined(TINY_GAME)
		#define GAME_SLOW_DOWN 0	
	#elif defined(__SPECTRUM__) && !defined(TINY_GAME) && !defined(__TS2068__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 1500
	#elif defined(__TS2068__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 3000
	#elif defined(__CPC__) && defined(CPCRSLIB)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 500	
	#elif defined(__SVI__) && defined(MSX_MODE0)
		#define SLOW_DOWN 
		#define GAME_SLOW_DOWN 1200
	#elif defined(__ATARI_LYNX__)
		#define SLOW_DOWN 
		#define GAME_SLOW_DOWN 2500U
	#elif defined(__SVI__) && !defined(MSX_MODE0)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 1200	
	#elif defined(__TRS80__) || defined(__EG2K__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 100	
	#elif defined(__EINSTEIN__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 5000	
	#elif defined(__M5__)
		#define SLOW_DOWN
		#define GAME_SLOW_DOWN 900					
	#else
		#define GAME_SLOW_DOWN 0
	#endif
#endif

#endif // _SPEED_TARGET_SETTINGS

