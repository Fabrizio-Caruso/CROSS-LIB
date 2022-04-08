
#include <conio.h>
#include <vdp.h>

int main(void)
{
    set_graphics(VDP_SPR_16x16);                        // set video mode
    charsetlc();                                        // load character set with lowercase
    vdpmemset(gImage, 32, 768);                         // clear screen
    VDP_SET_REGISTER(VDP_REG_COL, COLOR_MEDGREEN);      // set screen color
    vdpmemset(gColor, 0x10, 32);                        // set colors to black on transparent
    
    gotoxy(2,2);
    
    cprintf("Hello");

    while(1){};
    return 0;
}

