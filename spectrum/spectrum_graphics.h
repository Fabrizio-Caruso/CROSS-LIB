#ifndef _SPECTRUM_64_COL_GRAPHICS
#define _SPECTRUM_64_COL_GRAPHICS

	#include <stdio.h>
	
	Image PLAYER_IMAGE;
	Image GHOST_IMAGE;
	Image DEAD_GHOST_IMAGE;
	Image INVINCIBLE_GHOST_IMAGE;
	Image BOMB_IMAGE;
	Image POWERUP_IMAGE;
	Image MISSILE_IMAGE;
	Image GUN_IMAGE;
	
	Image BUBBLE_IMAGE;

	Image LEFT_ENEMY_MISSILE_IMAGE;
	Image RIGHT_ENEMY_MISSILE_IMAGE;
	
	Image EXTRA_POINTS_IMAGE;
	Image EXTRA_LIFE_IMAGE;
	Image INVINCIBILITY_IMAGE;
	
	char powerUp_blink = 1;
	char gun_blink = 1;
	char extra_points_blink = 1;
	char extra_life_blink = 1;
	char invincibility_blink = 1;
	char player_blink = 1;
	
	void INIT_GRAPHICS(void)
	{
	#if defined(SPECTRUM_64COL)
		printf("\x40");
	#elif defined(SPECTRUM_32COL)
		printf("\x20");
	#endif
	}
 
	void INIT_IMAGES(void)
	{		
	#if defined(SPECTRUM_64COL)
		printf("\x40");
	#elif defined(SPECTRUM_32COL)
		printf("\x20");
	#endif
	
		PLAYER_IMAGE._color = COLOR_WHITE;
		INVINCIBLE_GHOST_IMAGE._color = COLOR_WHITE;
		POWERUP_IMAGE._color = COLOR_WHITE;
		GUN_IMAGE._color = COLOR_WHITE;
		BOMB_IMAGE._color = COLOR_RED;
		DEAD_GHOST_IMAGE._color = COLOR_RED;
		EXTRA_POINTS_IMAGE._color = COLOR_WHITE;
		EXTRA_LIFE_IMAGE._color = COLOR_WHITE;
		INVINCIBILITY_IMAGE._color = COLOR_WHITE;			

			

		GHOST_IMAGE._imageData = 'o';
		INVINCIBLE_GHOST_IMAGE._imageData = '+';
		BOMB_IMAGE._imageData = 'X';
		PLAYER_IMAGE._imageData = '*';
		POWERUP_IMAGE._imageData = 'S';
		GUN_IMAGE._imageData = '!';
		MISSILE_IMAGE._imageData = '.';
		DEAD_GHOST_IMAGE._imageData = BOMB_IMAGE._imageData;

		GHOST_IMAGE._color = COLOR_WHITE;
		MISSILE_IMAGE._color = COLOR_WHITE;

		LEFT_ENEMY_MISSILE_IMAGE._imageData = '>';
		LEFT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = '<';
		RIGHT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;	
		
		BUBBLE_IMAGE._imageData = '^';
		BUBBLE_IMAGE._color = COLOR_WHITE;
		
		EXTRA_POINTS_IMAGE._imageData = '$';
		
		EXTRA_LIFE_IMAGE._imageData = PLAYER_IMAGE._imageData;
		INVINCIBILITY_IMAGE._imageData = 'V';
	}


	void _draw_broken_wall(unsigned char x, unsigned char y)
	{
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		SET_TEXT_COLOR(COLOR_WHITE);
		cputc('X');
	}
	
	void _draw(unsigned char x, unsigned char y, Image * image) 
	{
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		SET_TEXT_COLOR(image->_color);
		cputc(image->_imageData); 
	}
	
	void _delete(unsigned char x, unsigned char y)
	{
		gotoxy(x+X_OFFSET,y+Y_OFFSET);
		cputc(' ');
	}
	
	void _blink_powerUp_draw(unsigned char x, unsigned char y, Image * image) 
	{
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		SET_TEXT_COLOR(image->_color);
		if(powerUp_blink) 
		{
			cputc(image->_imageData); 
			powerUp_blink=0;
		} 
		else 
		{
			cputc(' '); 
			powerUp_blink=1;
		}	
	}
	
	void _blink_gun_draw(unsigned char x, unsigned char y, Image * image) 
	{
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		SET_TEXT_COLOR(image->_color);
		if(gun_blink) 
		{
			cputc(image->_imageData); 
			gun_blink=0;
		} 
		else 
		{
			cputc(' '); 
			gun_blink=1;
		}	
	}

	void _blink_extra_points_draw(unsigned char x, unsigned char y, Image * image) 
	{
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		SET_TEXT_COLOR(image->_color);
		if(extra_points_blink) 
		{
			cputc(image->_imageData); 
			extra_points_blink=0;
		} 
		else 
		{
			cputc(' '); 
			extra_points_blink=1;
		}	
	}

	
	void _blink_extra_life_draw(unsigned char x, unsigned char y, Image * image) 
	{
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		SET_TEXT_COLOR(image->_color);
		if(extra_life_blink) 
		{
			cputc(image->_imageData); 
			extra_life_blink=0;
		} 
		else 
		{
			cputc(' '); 
			extra_life_blink=1;
		}	
	}
	
	void _blink_invincibility_draw(unsigned char x, unsigned char y, Image * image) 
	{
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		SET_TEXT_COLOR(image->_color);
		if(invincibility_blink) 
		{
			cputc(image->_imageData); 
			invincibility_blink=0;
		} 
		else 
		{
			cputc(' '); 
			invincibility_blink=1;
		}	
	}

	void _blink_player_draw(unsigned char x, unsigned char y, Image * image) 
	{
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		SET_TEXT_COLOR(image->_color);
		if(player_blink) 
		{
			cputc(image->_imageData); 
			player_blink=0;
		} 
		else 
		{
			cputc(' '); 
			player_blink=1;
		}	
	}
	

	void DRAW_VERTICAL_LINE(unsigned char x, unsigned char y, unsigned char length) 
	{ 
		unsigned char i; 
		for(i=0;i<length;++i) 
		{ 
			gotoxy(x+X_OFFSET,y+Y_OFFSET+i);  printf("%c",'|'); 
		} 
	}

#endif