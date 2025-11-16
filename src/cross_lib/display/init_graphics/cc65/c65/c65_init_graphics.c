
#include <peekpoke.h>
#include <conio.h>

#include "standard_libs.h"

#define CRAM2K 0xD030
    // lda #%00000001  ; Enable CRAM2K
    // tsb cram2k
    
void _XL_INIT_GRAPHICS(void)
{
    // __asm__("SEI");
    
    // BLACK background and border
    POKE(53281U,0U);
    POKE(53280U,0U);
    
    #if defined(__CRAM2K)
        POKE(CRAM2K,PEEK(CRAM2K)|(0x01));
    #endif
        
    #if defined(__MEGA65_40COL)
        // gotoxy(0,0);
        // cputc(27);
        // cputc('4');
        
// lda #27
// jsr $ffd2
// lda #52
// jsr $ffd2
        __asm__("lda #27");
        __asm__("jsr $ffd2");
        __asm__("lda #52");
        __asm__("jsr $ffd2");

        
        // 40 column mode
        // POKE(0xD031,PEEK(0xD031)&(0x7F));
        
        // CRAM2K

    #endif
    
    // {
        // uint16_t i;
        
        // for(i=0;i<2000;++i)
        // {
            // POKE(2048+i,65);
        // }
    // };
    // while(1){};
}
