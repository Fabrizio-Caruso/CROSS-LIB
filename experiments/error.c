
struct ParentStruct
{
	int a;
};

typedef struct ParentStruct Parent;

struct ChildStruct
{
	Parent parent;
        void (*method)(void);	
};

typedef struct ChildStruct Child;

void foo()
{
}

int main()
{
	Child c;
	c.method = &foo;

	Parent *pPtr = (Parent *) &c;

	pPtr->a = 42;

	return 0;
}
