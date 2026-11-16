#include <stdio.h>
#include <stdlib.h>

int main() {
        int c;

        printf("Going\n");
        while ( 1 ) {
                c = getk();
                printf(".");
                if ( c != 0 ) printf("Got key %d ", c);
                msleep(100);
        }

}


