
#include <coco.h>
#include <cmoc.h>

char strings[5][7] = 
{
    "Mine",
    "Energy",
    "Bonus",
    "Points",
    "Apple",
};


int main(void)
{
    unsigned char i;
    
    for(i=0;i<5;++i)
    {
        printf("%s\n", strings[i]);
    }
    
    while(1){};
    
    return 0;
}

