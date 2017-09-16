#ifndef _NES_GRAPHICS
#define _NES_GRAPHICS

	/* Includes */
	#include <nes.h>
	#include <peekpoke.h>
	
	/* For more information on these PPU registers, see:
	 * http://wiki.nesdev.com/w/index.php/PPU_registers */
	#define PPU_CTRL2       0x2001  /* Aka PPUMASK: Turns features of the PPU on or off. */
	#define PPU_VRAM_ADDR1  0x2005  /* Aka PPUSCROLL: Used for X/Y scroll */
	#define PPU_VRAM_ADDR2  0x2006  /* Aka PPUADDR: Nametable 'cursor' */
	#define PPU_VRAM_IO     0x2007  /* Aka PPUDATA: Access data pointed to by last PPUADDR write. */

	/* For more information on these APU registers, see:
	 * http://wiki.nesdev.com/w/index.php/APU_Status */
	#define APU_STATUS      0x4015  /* Used for activating APU "voices". */
	#define APU_PULSE       0x4000  /* 0x4000-0x4003: Registers for pulse 1. 0x4004-0x4007 for pulse 2. */

	/* Write a byte to a given address: */
	#define poke(addr, data)        (*((unsigned char*)addr) = data)

	/* Write a pair of bytes to the PPU VRAM Address 2. */
	#define ppu_2(a, b)             { poke(PPU_VRAM_ADDR2, a); poke(PPU_VRAM_ADDR2, b); }

	/* Set the nametable x/y position. The top-left corner is 0x2000, and each row
	 * is 32 bytes wide. Hence:
	 *  (0,0)   => 0x2000;
	 *  (1,2)   => 0x2000 + 2*32 + 1 => 0x2041;
	 *  (20,16) => 0x2000 + 16*32 + 20 => 0x2214;
	 */
	#define ppu_set_cursor(x, y)    ppu_2(0x20+((y)>>3), ((y)<<5)+(x))

	/* Set the screen scroll offsets: */
	#define ppu_set_scroll(x, y)    { poke(PPU_VRAM_ADDR1, x); poke(PPU_VRAM_ADDR1, y); }

	/* Set "foreground colour"...
	 * i.e. write color 'c' value into VRAM address 0x3F03
	 * ...which is colour no. 3 in "background palette 0".
	 * See: http://wiki.nesdev.com/w/index.php/PPU_palettes */
	#define ppu_set_color_text(c)   { ppu_2(0x3F, 0x03); ppu_io(c); }

	/* Set "background colour"...
	 * i.e. write color 'c' value into VRAM address 0x3F00
	 * ...which is the "Universal background color" palette entry.
	 * See: http://wiki.nesdev.com/w/index.php/PPU_palettes */
	#define ppu_set_color_back(c)   { ppu_2(0x3F, 0x00); ppu_io(c); }

	/* Write to the PPU IO port, e.g. to write a byte at the nametable 'cursor' position: */
	#define ppu_io(c)               poke(PPU_VRAM_IO, (c))

	/* Write to APU_STATUS register: */
	#define apu_status(c)           poke(APU_STATUS, (c))

	/* Write to one of the APU pulse registers.
	 * Parameter 'ch' is the channel (pulse channel 0 or 1),
	 * 'r' is the register (0-4), and 'c' is the data to write. */
	#define apu_pulse(ch, r, c)     poke(APU_PULSE+((ch)<<2)+(r), (c))

	Image PLAYER_IMAGE;
	Image GHOST_IMAGE;
	Image DEAD_GHOST_IMAGE;
	Image INVINCIBLE_GHOST_IMAGE;
	Image BOMB_IMAGE;
	Image POWERUP_IMAGE;
	Image MISSILE_IMAGE;
	Image GUN_IMAGE;

	Image LEFT_ENEMY_MISSILE_IMAGE;
	Image RIGHT_ENEMY_MISSILE_IMAGE;

	extern char YSize; 
	
	void write_string(char *str)
	{
		/* Position the cursor at what APPEARS to be (1,1). */
		/* We only need to do this once. */
		/* We start 2 rows down since the first 8 pixels from the top of the screen is hidden */
		ppu_set_cursor(1, 2);

		/* Write the string */
		while(*str)
		{
			/* Write a letter */
			/* Note that the compiler's lib/nes.lib defines a CHR ROM that
			has graphics matching ASCII characters. */
			ppu_io(*str);
			/* Advance pointer that reads from the string */
			str++;
		}
	}

	
	void INIT_GRAPHICS(void)
	{
		/* This is just used as a frame counter, so we can synchronise
		 * things based on the number of frames rendered. PAL NES is 50FPS,
		 * while NTSC NES is 60FPS. Hence, on an NTSC NES, we know one frame
		 * lasts approx 16.67ms and after 60 frames, 1 second has elapsed. */
		int frame = 0;
		/* We have to wait for VBLANK or we can't even use the PPU */
		waitvblank(); /* This is found in nes.h */

		/* Now set basic colours which we'll use for foreground and background.
		 * This is based on the NES palette:
		 * http://en.wikipedia.org/wiki/List_of_video_game_console_palettes#NES */
		/* Set the background color (0x11 => medium blue): */
		ppu_set_color_back(0x11);
		/* Set the text colour: */
		/* Then, we need to set the text color (0x10 => light grey): */
		ppu_set_color_text(0x10);

		/* We must write our message to the screen */
		write_string("Anton says: Hello, World!");

		/* Reset the screen scroll position: */
		ppu_set_scroll(0, 0);

		/* Enable the screen;
		 * By default, the screen and sprites were off.
		 * This turns on only bit 3 of the Control/Mask register, which
		 * activates backgrounds (i.e. rendering of the nametable).
		 * See: http://wiki.nesdev.com/w/index.php/PPU_registers#Mask_.28.242001.29_.3E_write */
		poke(PPU_CTRL2, 8);

		// /* Start making a noise, by turning on bit 0 of APU_STATUS,
		 // * which activates "pulse channel 1" */
		// apu_status(1);

		// /* Set the pulse timer for the first pulse channel (0), via registers
		 // * 2 and 3 (i.e. 0x4002 and 0x4003). Set the timer value to 0x208: */
		// apu_pulse(0, 2, 0x08); /* Set low 8 bits of pulse timer to 8 (00001000) */
		// apu_pulse(0, 3, 0x2); /* Set high 3 bits of pulse timer to 2 (010) */
		// /* A timer value of 0x208 means a frequency of about 214.7Hz. */

		// /* Set:
		 // * 10...... = Duty Cycle is 50%;
		 // * ..1..... = Disable length counter;
		 // * ...1.... = Constant volume option;
		 // * ....1111 = Maximum volume level.
		 // */
		// apu_pulse(0, 0, 0xBF);

		/* This is an endless loop that alternates a square wave tone between
		 * two different frequencies, at a rate of 0.5Hz (i.e. a high tone for
		 * 1 second, then a low tone for 1 second). */
		// while (1)
		// {
			// /* Wait until the current frame has finished drawing to the screen. */
			// waitvblank();
			// /* Increment our frame counter: */
			// frame++;
			// /* Check if we've reached a frame we need to take action on: */
			// if (frame == 60)
			// {
				// /* 60 frames have elapsed (1 second) so switch to a higher frequency.
				 // * 0x193 becomes about 276.9Hz. */
				// apu_pulse(0, 2, 0x93);
				// apu_pulse(0, 3, 0x1);
			// }
			// else if (frame == 120)
			// {
				// /* Another 60 frames have elapsed, so switch back to lower frequency,
				 // * and reset the frame counter. */
				// apu_pulse(0, 2, 0x08);
				// apu_pulse(0, 3, 0x2);
				// frame = 0;
			// }
		// }

		// /* NOTE: Though we never make it here because of the 'while' loop above,
		 // * this is an example of making the program hang at this point. The compiler
		 // * normally loops the main() function if it exits, so we put this in to stop
		 // * it from ever exiting. */
		// while(1);		
	}
 
	 
	// TODO: Sprite initialization (to be performed only once) should be separated from level generation
	void INIT_IMAGES(void)
	{		
	
			#if defined(__C64__) || defined(__C128__) || defined(__VIC20__)
				PLAYER_IMAGE._color = COLOR_BLUE;
				INVINCIBLE_GHOST_IMAGE._color = COLOR_BLUE;
				POWERUP_IMAGE._color = COLOR_BLUE;
				GUN_IMAGE._color = COLOR_BLACK;
				BOMB_IMAGE._color = COLOR_RED;
				DEAD_GHOST_IMAGE._color = COLOR_RED;
			#elif defined(__PET__) || defined(__APPLE2__)
				PLAYER_IMAGE._color = COLOR_BLACK;
				INVINCIBLE_GHOST_IMAGE._color = COLOR_BLACK;
				POWERUP_IMAGE._color = COLOR_BLACK;
				GUN_IMAGE._color = COLOR_BLACK;
				BOMB_IMAGE._color = COLOR_BLACK;
				DEAD_GHOST_IMAGE._color = COLOR_BLACK;
			#elif defined(__C16__) || defined(__PLUS4__)
				PLAYER_IMAGE._color = COLOR_BLACK;
				INVINCIBLE_GHOST_IMAGE._color = COLOR_BLACK;
				POWERUP_IMAGE._color = COLOR_GRAY1;
				GUN_IMAGE._color = COLOR_GRAY1;
				BOMB_IMAGE._color = COLOR_RED;
				DEAD_GHOST_IMAGE._color = COLOR_RED;
			#else
				PLAYER_IMAGE._color = COLOR_BLACK;
				INVINCIBLE_GHOST_IMAGE._color = COLOR_BLACK;
				POWERUP_IMAGE._color = COLOR_BLACK;
				GUN_IMAGE._color = COLOR_BLACK;
				BOMB_IMAGE._color = COLOR_RED;
				DEAD_GHOST_IMAGE._color = COLOR_RED;
			#endif
				

			GHOST_IMAGE._imageData = 'o';
			INVINCIBLE_GHOST_IMAGE._imageData = '+';
			BOMB_IMAGE._imageData = 'X';
			PLAYER_IMAGE._imageData = '*';
			POWERUP_IMAGE._imageData = 'S';
			GUN_IMAGE._imageData = '!';
			MISSILE_IMAGE._imageData = '.';
			DEAD_GHOST_IMAGE._imageData = BOMB_IMAGE._imageData;

			GHOST_IMAGE._color = COLOR_BLACK;
			MISSILE_IMAGE._color = COLOR_BLACK;

					
			LEFT_ENEMY_MISSILE_IMAGE._imageData = '>';
			LEFT_ENEMY_MISSILE_IMAGE._color = 0;
			RIGHT_ENEMY_MISSILE_IMAGE._imageData = '<';
			RIGHT_ENEMY_MISSILE_IMAGE._color = 0;
	}

	
	
#endif __NES_GRAPHICS
