#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


struct ClassStruct
{
    int data;
    void (*set_data)(struct ClassStruct*, int);
};
typedef struct ClassStruct class_t;


void _set_data(struct ClassStruct* this, int val)
{
    this->data = val;
}



int main()
{
    class_t foo;
   
    foo.set_data = _set_data;
    foo.set_data(&foo, 42);
    
    if(foo.data==42)
    {
        printf("OK  -  FOO.DATA == 42\n");
    }
    else
    {
        printf("KO  -  FOO.DATA != 42\n");
    }


    return EXIT_SUCCESS;
}
