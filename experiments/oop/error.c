
struct ParentStruct
{
	int a;
};

typedef struct ParentStruct Parent;

struct ChildStruct
{
	Parent parent;
    
	void (*method)(void);	
	int b;
};

typedef struct ChildStruct Child;

void foo(void)
{
}

int main()
{
	Child c;

	Parent *pPtr = (Parent *) &c;

	c.method = &foo;

	pPtr->a = 42;
	
	c.b = 43;
	
	c.method();

	return 0;
}
