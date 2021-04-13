#include <stdio.h>
#include <stdlib.h>

int main(void)
{

    while(1)
    {
        printf("%d\n", 1+(rand()&3));
        
        getchar();
    }
    return 0;
}
