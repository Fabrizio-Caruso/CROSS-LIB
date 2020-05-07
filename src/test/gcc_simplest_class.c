#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


struct ClassStruct
{
    void (*method_t)(void);
};
typedef struct ClassStruct class_t;


void _print_42(void)
{
    printf("42\n");
}



int main()
{
    class_t foo;
   
    foo.method_t = _print_42;

    foo.method_t();

    return EXIT_SUCCESS;
}
