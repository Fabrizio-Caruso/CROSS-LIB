#include <stdio.h>

#include <stdint.h>

int main(void)
{
    uint8_t i;
    uint8_t offset;
    uint8_t new_arrow_x;
    uint8_t bow_x;
    uint8_t number_of_arrows_per_shoot;
    
    number_of_arrows_per_shoot = 2;
    bow_x = 20;
    new_arrow_x = (bow_x>>1)+(bow_x&1);
    
    for(i=0;i<number_of_arrows_per_shoot;++i)
    {
        printf("i: %d ", i);
        if((number_of_arrows_per_shoot==2)&&i)
        {
            offset = i-2*(bow_x&1);
        }
        else
        {
            offset = i;
        }
        printf("| offset: %d\n", offset);
        
        new_arrow_x+=offset;
        if(i==2)
        {
            if(new_arrow_x>=4)
            {
                new_arrow_x-=4; 
            }
            else
            {
                // printf("new_arrow_x for %d: %d\n", i, new_arrow_x);

                continue;
            }
        }
        printf("new_arrow_x for %d: %d\n\n", i, new_arrow_x);
    }
    
    return 0;
}