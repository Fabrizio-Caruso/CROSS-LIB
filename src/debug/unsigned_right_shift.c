#include <stdio.h>
#include <stdint.h>

uint8_t foo;
uint8_t bar; // Without this there is no bug

int main(void)
{            
    uint8_t i; 

    foo = 1U;
    bar = 3U;  // Without this there is no bug

    for(i=0;i<foo>>0;++i)
    {
        printf("foo\n");
    }

    printf("OK");

    return 0;
}

