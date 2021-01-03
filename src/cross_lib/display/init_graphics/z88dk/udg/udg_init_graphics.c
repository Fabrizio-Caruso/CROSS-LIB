
#include <graphics.h>

#include "display_macros.h"

#include <sys/ioctl.h>

extern uint8_t udgs[];

// #if defined(__CPC__) && !defined(CPCRSLIB)
    // void set_palette()
    // { 
    // #asm
        // EXTERN firmware

        // ld bc,$0000
        // call firmware
        // defw 0xbc38
     
        // ; black
        // ld  a,$00
        // ld  bc,$0000
        // call firmware
        // defw 0xbc32
        
        // ; white
        // ld  a,$01
        // ld  bc,$1a1a
        // call firmware
        // defw 0xbc32
        
        // ld  a,$03
        // ld  bc,$0909
        // call firmware
        // defw 0xbc32
        
        // ; yellow
        // ld  a,$02
        // ld  bc,$1818
        // call firmware
        // defw 0xbc32
        
        // ld  a,$04
        // ld  bc,$0909
        // call firmware
        // defw 0xbc32
        

        // defw 0xbc32
    // #endasm 
    // }
// #endif

void INIT_GRAPHICS(void)
{
	void *param = &udgs;
	console_ioctl(IOCTL_GENCON_SET_FONT32, &param);
	
	#if defined(FORCE_SCREEN_MODE) || defined(__MC1000__) || defined(__PC6001__) || defined(__CPC__) \
    || defined(__MULTI8__) || defined(__SPC1000__) || defined(__GB__) || defined(__SMS__) \
    || defined(__GAL_PLUS__) || defined(__GAME_GEAR__) || defined(__MTX__) || defined(__MSX__)
	{
		#if defined(FORCE_SCREEN_MODE)
			#define SCREEN_MODE FORCE_SCREEN_MODE
		#elif defined(__MC1000__) || defined(__GB__) || defined(__GAL_PLUS__) 
			#define SCREEN_MODE 1
		#else
			#define SCREEN_MODE 2
		#endif
		int mode = SCREEN_MODE;
		console_ioctl(IOCTL_GENCON_SET_MODE, &mode);
        
	}
	#elif defined(__SPECTRUM__)
	printf("\x01\x20");
    zx_border(0);
    textbackground(COLOR_BLACK);
	#endif
    
    // #if defined(__CPC__) && !defined(CPCRSLIB)
        // set_palette();
    // #endif
}


