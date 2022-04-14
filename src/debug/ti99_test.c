#include <system.h>
#include <conio.h>

#define NUMBER_OF_STRINGS 5
#define MAX_STRING_SIZE 7

const char strings[NUMBER_OF_STRINGS][MAX_STRING_SIZE] = 
{
    "M" "i" "n" "e",
    "E" "n" "e" "r" "g" "y",
    "B" "o" "n" "u" "s",
    "P" "o" "i" "n" "t" "s",
    "A" "p" "p" "l" "e"
};

#define LINE_OFFSET 4
#define LINE_SKIP 2

int main(void)
{
    unsigned char i;
    
    set_graphics(VDP_SPR_16x16);                        // set video mode
    charsetlc();                                        // load character set with lowercase
    vdpmemset(gImage, 32, 768);                         // clear screen
    VDP_SET_REGISTER(VDP_REG_COL, COLOR_BLACK);      // set screen color
    

    
    for(i=0;i<NUMBER_OF_STRINGS;++i)
    {
        // _XL_PRINT(XSize/2-4,YSize/8+LINE_OFFSET+LINE_SKIP*i, (char *) strings[NUMBER_OF_STRINGS-1-i] );
        gotoxy(XSize/2-4, YSize/8+LINE_OFFSET+LINE_SKIP*i);
        cprintf((char *) strings[NUMBER_OF_STRINGS-1-i]);
    }
    
    
    while(1){};
    
    return 0;
}

