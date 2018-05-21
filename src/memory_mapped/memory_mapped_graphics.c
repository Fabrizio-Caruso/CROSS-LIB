
#  if defined(__VIC20__) && defined(ALT_PRINT)
	#include "../vic20/vic20_alt_print_init.h"
#elif defined(__C16__) && defined(ALT_PRINT)
	#include "../c264/c264_alt_print_init.h"
#else
#endif 

#include "../display_macros.h"

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
#if !defined(NO_DEAD_GHOSTS)
extern Image DEAD_GHOST_IMAGE;
#endif
extern Image BOMB_IMAGE;

#if !defined(TINY_GAME)
	extern Image INVINCIBLE_GHOST_IMAGE;
	extern Image POWERUP_IMAGE;
	extern Image GUN_IMAGE;
	extern Image MISSILE_IMAGE;
	extern Image EXTRA_POINTS_IMAGE;
#endif

#if defined(FULL_GAME)
	extern Image LEFT_ENEMY_MISSILE_IMAGE;
	extern Image RIGHT_ENEMY_MISSILE_IMAGE;

	extern Image BUBBLE_IMAGE;

	extern Image FREEZE_IMAGE;
	extern Image EXTRA_LIFE_IMAGE;
	extern Image INVINCIBILITY_IMAGE;
	extern Image SUPER_IMAGE;	
	extern Image CONFUSE_IMAGE;
	extern Image ZOMBIE_IMAGE;
	
	extern Image BROKEN_WALL_IMAGE;
#endif


#if defined(REDEFINED_CHARS)
	Image PLAYER_DOWN;
	Image PLAYER_UP;
	Image PLAYER_RIGHT;
	Image PLAYER_LEFT;
#endif

#if defined(ALT_PRINT)
// unsigned short loc(unsigned char x, unsigned char y)
// {
	// return ((unsigned short) BASE_ADDR)+(x+X_OFFSET)+(y+Y_OFFSET)*((unsigned short)XSize);
// }

unsigned short loc(unsigned char x, char y)
{
	return ((unsigned short) BASE_ADDR)+(x+X_OFFSET)+(unsigned char)(y+Y_OFFSET)*((unsigned short)XSize);
}
#endif

#if !defined(NO_COLOR)
	#define _DRAW(x,y,image) \
	do \
	{ \
		POKE(loc(x,y), image->_imageData); \
		POKE((unsigned short) (COLOR_ADDR+x+(unsigned short)(y+Y_OFFSET)*XSize),image->_color); \
	} \
	while(0)
	#define _DELETE(x,y) POKE(loc(x,y), 32)
#else
	#define _DRAW(x,y,image) do { gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc(image->_imageData); } while(0)
	#define _DELETE(x,y) do { gotoxy(x+X_OFFSET,y+Y_OFFSET); cputc(' '); } while(0)      
#endif
#define _DRAW_VERTICAL_WALL(x,y)  POKE(loc(x,y),'|')
#define _DRAW_HORIZONTAL_WALL(x,y)  POKE(loc(x,y),'-')
#define _DRAW_BROKEN_WALL(x,y) POKE(loc(x,y),_BROKEN_WALL)

void INIT_IMAGES(void)
{		
	#if !defined(NO_COLOR)
		// PLAYER_IMAGE._color = COLOR_CYAN;
		BOMB_IMAGE._color = COLOR_RED;
		#if !defined(NO_DEAD_GHOSTS)
			DEAD_GHOST_IMAGE._color = COLOR_RED;
		#endif
		GHOST_IMAGE._color = COLOR_WHITE;		
		
		#if !defined(TINY_GAME)
			INVINCIBLE_GHOST_IMAGE._color = COLOR_YELLOW;
			POWERUP_IMAGE._color = COLOR_GREEN;
			GUN_IMAGE._color = COLOR_BLUE;
			EXTRA_POINTS_IMAGE._color = COLOR_YELLOW;
			MISSILE_IMAGE._color = COLOR_BLUE;
		#endif
		
		#if defined(REDEFINED_CHARS)
			PLAYER_DOWN._color = COLOR_CYAN;
			PLAYER_UP._color = COLOR_CYAN;
			PLAYER_RIGHT._color = COLOR_CYAN;
			PLAYER_LEFT._color = COLOR_CYAN;
		#else
			PLAYER_IMAGE._color = COLOR_CYAN;
		#endif
		
		#if defined(FULL_GAME)
			RIGHT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;		
			LEFT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;
			
			BUBBLE_IMAGE._color = COLOR_WHITE;

			FREEZE_IMAGE._color = COLOR_CYAN;
			EXTRA_LIFE_IMAGE._color = COLOR_YELLOW;
			
			INVINCIBILITY_IMAGE._color = COLOR_YELLOW;
			SUPER_IMAGE._color = COLOR_RED;
			CONFUSE_IMAGE._color = COLOR_RED;
			ZOMBIE_IMAGE._color = COLOR_YELLOW;
		#endif	
	#endif

	GHOST_IMAGE._imageData = _GHOST;
	BOMB_IMAGE._imageData = _BOMB;
	
	#if !defined(NO_DEAD_GHOSTS)
	DEAD_GHOST_IMAGE._imageData = _DEAD_GHOST;	
	#endif
	
	#if !defined(TINY_GAME)
		INVINCIBLE_GHOST_IMAGE._imageData = _INVINCIBLE_GHOST;

		POWERUP_IMAGE._imageData = _POWERUP;
		GUN_IMAGE._imageData = _GUN;
		EXTRA_POINTS_IMAGE._imageData = _EXTRA_POINTS;
		
		MISSILE_IMAGE._imageData = _MISSILE;
	#endif
	
	#if defined(REDEFINED_CHARS)
		PLAYER_DOWN._imageData = _PLAYER_DOWN;
		PLAYER_UP._imageData = _PLAYER_UP;	
		PLAYER_RIGHT._imageData = _PLAYER_RIGHT;
		PLAYER_LEFT._imageData = _PLAYER_LEFT;	
	#else
		PLAYER_IMAGE._imageData = _PLAYER;			
	#endif
	
	#if defined(FULL_GAME)
		LEFT_ENEMY_MISSILE_IMAGE._imageData = _LEFT_ENEMY_MISSILE;
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = _RIGHT_ENEMY_MISSILE;
		
		BUBBLE_IMAGE._imageData = _BUBBLE;

		FREEZE_IMAGE._imageData = _POWERUP;		
		EXTRA_LIFE_IMAGE._imageData = _EXTRA_LIFE;
		INVINCIBILITY_IMAGE._imageData = _INVINCIBILITY;	
		SUPER_IMAGE._imageData = _POWERUP;
		CONFUSE_IMAGE._imageData = _CONFUSE;
		ZOMBIE_IMAGE._imageData = _GHOST;
	#endif
}

#if defined(FULL_GAME)
	void DRAW_BROKEN_WALL(unsigned char x, unsigned char y)
	{
		_DRAW_BROKEN_WALL(x,y);
	}
#endif
	
void _draw(unsigned char x, unsigned char y, Image * image) 
{
	_DRAW(x,y,image);
}

void _delete(unsigned char x, unsigned char y)
{
	_DELETE(x,y);
}

#if !defined(NO_BLINKING)
void _blink_draw(unsigned char x, unsigned char y, Image * image, unsigned char *blinkCounter) 
{
	if(*blinkCounter) 
	{
		_DRAW(x,y,image);
		*blinkCounter=0;
	} 
	else 
	{
		_DELETE(x,y);
		*blinkCounter=1;
	}	
}
#endif

#if !defined(TINY_GAME)
void DRAW_HORIZONTAL_LINE(unsigned char x,unsigned char y, unsigned char length) 
{
	unsigned char i;
	for(i=0;i<length;++i) 
	{ 
		gotoxy(x+i+X_OFFSET,y+Y_OFFSET);  cputc('-');
	} 	
}


void DRAW_VERTICAL_LINE(unsigned char x,unsigned char y, unsigned char length) 
{
	unsigned char i;
	for(i=0;i<length;++i) 
	{ 
		gotoxy(x+X_OFFSET,y+Y_OFFSET+i);  cputc('|');
	} 	
}
#endif


#if defined(ALT_PRINT)
void PRINT(unsigned char x, unsigned char y, char * str)
{
	unsigned char i = 0;
	while(str[i]!='\0')
	{
		POKE(loc(x,y)+i, str[i]); 
		++i;
	}
}

void print_05u0(unsigned char x, unsigned char y, unsigned short val)
{
	unsigned char i;
	unsigned char digits[6];
	unsigned short tmp;

	tmp = val;
	
	digits[0] = 0;
	for(i=1;i<6;++i)
	{
		digits[i] = (unsigned char) ((tmp)%10);
		tmp-= digits[i];
		tmp/=10;
	}
	
	for(i=0;i<6;++i)
	{
		POKE(loc(x,y)+i, (unsigned char) (digits[5-i])+48);
	}
}	

void print_02u(unsigned char x, unsigned char y, unsigned short val)
{
	POKE((loc(x,y)), ((unsigned char) val)/10+48);
	POKE((1+loc(x,y)), ((unsigned char) val)%10+48);
}	


void print_u(unsigned char x, unsigned char y, unsigned short val)
{
	POKE(loc(x,y), (unsigned char) (val+48));
}


void PRINTF(unsigned char x, unsigned char y, char * str, unsigned short val)
{
	if(strlen(str)==5)
	{	
		print_05u0(x,y,val);
	}
	else if(strlen(str)==4)
	{
		print_02u(x,y,val);		
	}
	else
	{
		print_u(x,y,val);		
	}
}


#endif
