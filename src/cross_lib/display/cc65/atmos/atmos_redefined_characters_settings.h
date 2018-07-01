
#define _INVERTED 128u

#if !defined(REDEFINED_CHARS)
	#define _PLAYER_OFFSET 0x5c
	#define _PLAYER  0x5c
#else		
	#define _PLAYER_LEFT_OFFSET '*'
	#define _PLAYER_RIGHT_OFFSET ','
	#define _PLAYER_UP_OFFSET '.'
	#define _PLAYER_DOWN_OFFSET '/'	
	
	#define _PLAYER_LEFT '*'
	#define _PLAYER_RIGHT ','
	#define _PLAYER_UP '.'
	#define _PLAYER_DOWN '/'		
#endif
	
#define _GHOST_OFFSET 0x60
#define _SKULL_OFFSET 0x7b
#define _BOMB_OFFSET 0x5b
#define _POWERUP_OFFSET 0x5d
#define _GUN_OFFSET 0x5e
#define _BULLET_OFFSET 0x5f	
	
#define _GHOST (0x60 + _INVERTED)
#define _SKULL (0x7b + _INVERTED)
#define _BOMB (0x5b + _INVERTED)
#define _POWERUP ( 0x5d + _INVERTED)
#define _GUN (0x5e + _INVERTED)
#define _EXTRA_POINTS '$'
	
#define _BULLET 0x5f

#if !defined(NO_DEAD_GHOST)
	#define _DEAD_GHOST_OFFSET 0x5a
	#define _DEAD_GHOST (0x5a + _INVERTED)
#endif

#if defined(FULL_GAME)
	#define _FREEZE_OFFSET 'F'
	#define _EXTRA_LIFE_OFFSET _PLAYER_DOWN
	#define _INVINCIBILITY_OFFSET 0x3B  
	#define _CONFUSE_OFFSET 'C'
	#define _SUPER_OFFSET 'H'		
	#define _ZOMBIE_OFFSET 'Z' 

	#define _FREEZE ('F'+_INVERTED)
	#define _EXTRA_LIFE (_PLAYER_DOWN + _INVERTED)
	#define _INVINCIBILITY (0x3B + _INVERTED) 
	#define _CONFUSE ('C' + _INVERTED)
	#define _SUPER ('H'+ _INVERTED)		
	#define _ZOMBIE ('Z' + _INVERTED)

	#define _CHASE_OFFSET _BULLET
	#define _LEFT_HORIZONTAL_MISSILE_OFFSET '>'
	#define _RIGHT_HORIZONTAL_MISSILE_OFFSET '<'
	#define _ROCKET_OFFSET 0x23	
	
	#define _VERTICAL_BRICK_OFFSET '|'
	#define _HORIZONTAL_BRICK_OFFSET '-'

	#define _VERTICAL_BRICK '|'
	#define _HORIZONTAL_BRICK '-'	
	
	#define _SPACE ' '
	#define _BROKEN_BRICK _BOMB
	
	#define _CHASE _BULLET
	#define _LEFT_HORIZONTAL_MISSILE '>'
	#define _RIGHT_HORIZONTAL_MISSILE '<'
	#define _ROCKET 0x23		
#endif

