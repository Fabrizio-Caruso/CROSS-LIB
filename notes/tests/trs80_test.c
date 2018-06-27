#include <stdio.h>
// int main() {
    // while ( 1 ) {
        // if ( kbhit() ) {
            // int c = getch();

            // printf("%c",c);
        // }
    // }
// }


int main() {
    while ( 1 ) {
        if ( kbhit() ) {
            printf("%c",getch());
        }
    }
}