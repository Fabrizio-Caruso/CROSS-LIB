#include <stdio.h>
#include <stdint.h>

uint8_t level;

#define WORD_SIZE 5
#define INITIAL_ROWS 3

#if WORD_SIZE>20
    #define SMALL_SCREEN_REDUCTION 1
#else
    #define SMALL_SCREEN_REDUCTION 0
#endif

#define MIN_INITIAL_DROP ((((WORD_SIZE)*INITIAL_ROWS))+SMALL_SCREEN_REDUCTION)

void initial_letter_drop(void)
{
    uint8_t i;
   
    for(i=0;i<(uint8_t) MIN_INITIAL_DROP + (level>>0);++i)
    {
        // drop_letter();
        // short_pause();
    } 
}

int main(void)
{

    level = 1;

    initial_letter_drop();
    
    printf("end\n");
    while(1){};
    
    return 0;
}
