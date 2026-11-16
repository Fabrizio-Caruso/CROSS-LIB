
#if defined(__MC10__)
    #define SCREEN 0x4000
    #define YSize 16
    #define XSize 32
#elif defined(__C64__)
    #define SCREEN 0x0400
    #define YSize 25
    #define XSize 40
#elif defined(__VIC20__)
    #define SCREEN 7680
    #define YSize 23
    #define XSize 22
#endif

#define POKE(addr,val) (*(unsigned char*) (addr) = (val))
#define PEEK(addr) (*(unsigned char*) (addr))
#include <stdint.h>

/*

__getc:
	pshx
getc_wait:
	ldx $FFDC			; poll keyboard
	jsr ,x
	beq getc_wait
	pulx
	tab
	rts

*/




// uint8_t getk(void)
// {
    // __asm__("pshx");
    // __asm__("ldx $FFDC");
    // __asm__("jsr ,x");
    // __asm__("pulx");
    // __asm__("tab");
// }

#include <stdio.h>

extern int getk(void);

int main(void)
{

    uint8_t c = 0;
    uint8_t ch;
    
    while(1)
    {
        POKE(SCREEN,PEEK(49151U));
        POKE(SCREEN+1,PEEK(17023U));
        POKE(SCREEN+2,PEEK(2));
        ch = (uint8_t) getk();
        POKE(SCREEN+4,(uint8_t) ch);
        if(ch)
        {
            POKE(SCREEN+32+c++,ch);
        }
    }

    while(1){};
    return 0;
}

