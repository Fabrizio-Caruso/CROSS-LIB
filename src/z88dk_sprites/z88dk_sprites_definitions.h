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
	 defb    8,7
	 defb    @00011000
	 defb    @00100100
	 defb    @00011000
	 defb    @01100110
	 defb    @10011001
	 defb    @00100100
	 defb    @01100110

	 defb    8,7
	 defb    @00011000
	 defb    @00111100
	 defb    @00011000
	 defb    @01100110
	 defb    @10011001
	 defb    @00100100
	 defb    @01100110

	 defb    8,7
	 defb    @00011000
	 defb    @00101100
	 defb    @10011000
	 defb    @01101100
	 defb    @00011011
	 defb    @00101000
	 defb    @00101000
	 
	 defb    8,7
	 defb    @00011000
	 defb    @00110100
	 defb    @00011001
	 defb    @00110110
	 defb    @11011000
	 defb    @00010100
	 defb    @00010100	 
	 
	 defb    8,7
	 defb    @01111110
	 defb    @11000011
	 defb    @10100101
	 defb    @10011001
	 defb    @10100101
	 defb    @11000011	 
	 defb    @01111110  
	  
	 defb    8,7
	 defb    @10111101
	 defb    @01000010
	 defb    @01100110
	 defb    @01000010
	 defb    @01011010
	 defb    @01000010	 
	 defb    @00111100  
	 
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

