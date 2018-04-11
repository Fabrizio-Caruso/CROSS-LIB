#ifndef _Z88DK_SPRITES_DEFINITIONS
#define _Z88DK_SPRITES_DEFINITIONS




#if SPRITE_X_SIZE==8 && SPRITE_Y_SIZE==8
	#asm
	._sprites
	 defb    8,8
	 defb    @00011000
	 defb    @00100100
	 defb    @00011000
	 defb    @01100110
	 defb    @10011001
	 defb    @00011000
	 defb    @00100100
	 defb    @01100110

	 defb    8,8
	 defb    @00011000
	 defb    @00111100
	 defb    @00011000
	 defb    @01100110
	 defb    @10011001
	 defb    @00011000
	 defb    @00100100
	 defb    @01100110

	 defb    8,8
	 defb    @00011000
	 defb    @00101100
	 defb    @10011000
	 defb    @11101110
	 defb    @00011001
	 defb    @00011000
	 defb    @00101000
	 defb    @00101000

	 defb    8,8
	 defb    @00011000
	 defb    @00110100
	 defb    @00011001
	 defb    @01101111
	 defb    @10011000
	 defb    @00011000
	 defb    @00010100
	 defb    @00010100

	 defb    8,8
	 defb    @00111100
	 defb    @01000010
	 defb    @10100101
	 defb    @10011001
	 defb    @10011001
	 defb    @10100101
	 defb    @01000010
	 defb    @00111100 
	 
	 defb    8,8
	 defb    @01111110
	 defb    @10000001
	 defb    @11100111
	 defb    @10000001
	 defb    @10000001
	 defb    @10111101
	 defb    @10000001
	 defb    @01111110 
	 
	 defb    8,8
	 defb    @01111110
	 defb    @10000001
	 defb    @11100111
	 defb    @10100101
	 defb    @01000010
	 defb    @01011010
	 defb    @01000010
	 defb    @00111100 	 

	 defb    8,8
	 defb    @00000000
	 defb    @00000100
	 defb    @10011000
	 defb    @11111110
	 defb    @01111111
	 defb    @00011001
	 defb    @00100000
	 defb    @00000000	 
	 
	 defb    8,8
	 defb    @11111111
	 defb    @10000001
	 defb    @10011101
	 defb    @10110001
	 defb    @10011001
	 defb    @10110001
	 defb    @01000010
	 defb    @00111100 	 

	 defb    8,8
	 defb    @00000000
	 defb    @00000000
	 defb    @01111111
	 defb    @11111111
	 defb    @11101000
	 defb    @01110000
	 defb    @01100000	 
	 defb    @00000000
	 
	 defb    8,8
	 defb    @00101100
	 defb    @00101111
	 defb    @00101011
	 defb    @00101000
	 defb    @00101000
	 defb    @11101000
	 defb    @11101000
	 defb    @00101000	 

	 defb    8,8
	 defb    @01111110
	 defb    @10000001
	 defb    @10100101
	 defb    @10100101
	 defb    @10000001
	 defb    @10011001
	 defb    @10100101
	 defb    @01111110 
	 
	 defb    8,8
	 defb    @00011000
	 defb    @00011000
	 defb    @01111000
	 defb    @00011000
	 defb    @00011000
	 defb    @00011110
	 defb    @00011000
	 defb    @00011000
	 
	 defb    8,8
	 defb    @00000000
	 defb    @00000000
	 defb    @00000000
	 defb    @11111111
	 defb    @11111111
	 defb    @00000000
	 defb    @00000000
	 defb    @00000000
	 
	 defb    8,8
	 defb    @11111111
	 defb    @11111111
	 defb    @11111111
	 defb    @11111111
	 defb    @11111111
	 defb    @11111111
	 defb    @11111111
	 defb    @11111111
	#endasm
#else
	#asm
	._sprites
	
	 ; 0 up
	 defb    8,7
	 defb    @00011000
	 defb    @00100100
	 defb    @00011000
	 defb    @01100110
	 defb    @10011001
	 defb    @00100100
	 defb    @01100110

	 ; 1 down
	 defb    8,7
	 defb    @00011000
	 defb    @00111100
	 defb    @00011000
	 defb    @01100110
	 defb    @10011001
	 defb    @00100100
	 defb    @01100110

	 ; 2 left
	 defb    8,7
	 defb    @00011000
	 defb    @00101100
	 defb    @10011000
	 defb    @01101100
	 defb    @00011011
	 defb    @00101000
	 defb    @00101000
	 
	 ; 3 right
	 defb    8,7
	 defb    @00011000
	 defb    @00110100
	 defb    @00011001
	 defb    @00110110
	 defb    @11011000
	 defb    @00010100
	 defb    @00010100	 
	 
	 ; 4 bomb
	 defb    8,7
	 defb    @01111110
	 defb    @11000011
	 defb    @10100101
	 defb    @10011001
	 defb    @10100101
	 defb    @11000011	 
	 defb    @01111110  
	  
	 ; 5 ghost
	 defb    8,7
	 defb    @10111101
	 defb    @01000010
	 defb    @01100110
	 defb    @01000010
	 defb    @01011010
	 defb    @01000010	 
	 defb    @00111100  
	 
	 ; 6 invincible_ghost
	 defb    8,7
	 defb    @01111110
	 defb    @10000001
	 defb    @11100111
	 defb    @10100101
	 defb    @01000010
	 defb    @01011010
	 defb    @01111110 	 

	 ; 7 missile
	 defb    8,7
	 defb    @00000000
	 defb    @00000100
	 defb    @10001000
	 defb    @11111000
	 defb    @00011111
	 defb    @00010001
	 defb    @00100000	 
	 
	 ; 8 power_up
	 defb    8,7
	 defb    @11111111
	 defb    @10000001
	 defb    @10011101
	 defb    @10110001
	 defb    @10011001
	 defb    @10110001
	 defb    @01111110 	 

	 ; 9 gun
	 defb    8,7
	 defb    @00000000
	 defb    @01111111
	 defb    @11111111
	 defb    @11101000
	 defb    @01110000
	 defb    @01100000	 
	 defb    @00000000
	 
	 ; 10 extra_points
	 defb    8,7
	 defb    @00101100
	 defb    @00101111
	 defb    @00101011
	 defb    @00101000
	 defb    @11101000
	 defb    @11101000
	 defb    @00101000	 

	 ; 11 dead_ghost
	 defb    8,7
	 defb    @01111110
	 defb    @10000001
	 defb    @10100101
	 defb    @10100101
	 defb    @10000001
	 defb    @10011001
	 defb    @11111111 
	 	 
	 ; 12 vertical_bar
	 defb    8,7
	 defb    @00011000
	 defb    @00011000
	 defb    @01111000
	 defb    @00011000
	 defb    @00011110
	 defb    @00011000
	 defb    @00011000

	 ; 13 horizontal_bar
	 defb    8,7
	 defb    @00000000
	 defb    @00000000
	 defb    @11111111
	 defb    @11111111
	 defb    @11111111
	 defb    @00000000
	 defb    @00000000	 
	 
	 ; 14 space
	 defb    8,7
	 defb    @11111111
	 defb    @11111111
	 defb    @11111111
	 defb    @11111111
	 defb    @11111111
	 defb    @11111111
	 defb    @11111111
	#endasm
#endif


#endif // _Z88DK_SPRITES_DEFINITIONS

