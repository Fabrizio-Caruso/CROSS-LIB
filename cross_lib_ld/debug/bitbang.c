#include <sound.h>
    
#include <stdio.h>

int main(void)
{

    while(1)
    {
        bit_fx(7);
        getchar();
        
        bit_fx(1);
        getchar();
        
        bit_fx3(0);
        getchar();
    }
    
    return 0;
}

