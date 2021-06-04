
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
#include <stdint.h>

#define FOO 10
#define BAR 1
uint8_t bar;

int main(void)
{

    bar = BAR;
    POKE(SCREEN,65);
    while(1 && (bar < FOO + 1))
    {
        POKE(SCREEN+1,66);
    }
    POKE(SCREEN+2,67);

    while(1){};
    return 0;
}

