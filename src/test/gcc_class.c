#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


struct ParentStruct
{
    int data;
    void (*set_parent_data)(struct ParentStruct*, int);
};

typedef struct ParentStruct parent_t;

struct ChildStruct
{
    parent_t _parent;
    int more_data;
    void (*set_child_data)(struct ChildStruct* this, int val);
};

typedef struct ChildStruct child_t;


void _set_parent_data(struct ParentStruct* this, int val)
{
    this->data = val;
}


void _set_child_data(child_t* this, int val)
{
    this->more_data = val;
}


int main()
{
    parent_t foo;
    child_t bar;
    parent_t *bar_p = (parent_t*) &bar;
   
    bar._parent.set_parent_data = _set_parent_data;
   
    foo.set_parent_data = _set_parent_data;
   
    foo.set_parent_data(&foo, 42);
   
    bar_p->set_parent_data(bar_p, 43);
   
   
    printf("value in parent: %d\n", foo.data);
    printf("value in child: %d\n", bar_p->data);
    
    if(foo.data==42)
    {
        printf("OK  -  FOO.DATA == 42 OK\n");

    }
    else
    {
        printf("KO  -  FOO.DATA != 42\n");
    }
    
    if(bar_p->data==43)
    {
        printf("OK  -  BAR_P->DATA == 43 OK\n");
    }
    else
    {
        printf("KO  -  BAR_P->DATA != 43 OK\n");
    }

    return EXIT_SUCCESS;
}
