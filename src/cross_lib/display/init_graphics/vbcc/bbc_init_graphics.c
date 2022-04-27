#include <stdint.h>
#include <stdio.h>

static uint8_t stripes[] = {255,0,255,0,255,0,255,0};

void redefine(uint8_t ch, uint8_t *data)
{
    uint8_t i;
    
    putchar(23);
    putchar(ch);
    for(i=0;i<8;++i)
    {
        putchar(data[i]);
    }
    putchar('\n');
}

void _XL_INIT_GRAPHICS(void)
{
    redefine(240,stripes);
    redefine(241,stripes);
    
    putchar('O');
    putchar('K');
    
    printf("hello world\n");
    
    putchar(240);
    
    putchar(241);
    
    printf("\nhello world\n");
    
    while(1){};
}
