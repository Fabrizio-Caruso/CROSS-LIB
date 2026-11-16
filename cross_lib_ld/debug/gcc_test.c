#include <system.h>
#include <conio.h>

#define NUMBER_OF_STRINGS 5
#define MAX_STRING_SIZE 7

const char strings[NUMBER_OF_STRINGS][MAX_STRING_SIZE] = 
{
    "Mine",
    "Energy",
    "Bonus",
    "Points",
    "Apple"
};


int main(void)
{
    uint8_i;
    
    set_graphics(VDP_SPR_16x16);                        // set video mode
    charsetlc();                                        // load character set with lowercase
    vdpmemset(gImage, 32, 768);                         // clear screen
    VDP_SET_REGISTER(VDP_REG_COL, COLOR_BLACK);      // set screen color
    

    
    for(i=0;i<NUMBER_OF_STRINGS;++i)
    {
        gotoxy(2,2+i);
        cprintf(strings[i]);
    }
    
    
    while(1){};
    
    return 0;
}

