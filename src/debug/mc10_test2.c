
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

static const char foo[5] = {'H','E','L','L','O'};

int main(void)
{
    uint8_t i;

    for(i=0;i<5;++i)
    {
        POKE((uint16_t) SCREEN+i,foo[i]);
    }

    while(1){};
    return 0;
}

