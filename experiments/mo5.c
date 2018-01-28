#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))
#define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
#define PEEK(addr)         (*(unsigned char*) (addr))
#define PEEKW(addr) (*(unsigned*) (addr))

#define VIDEO_ADDR 0x0000

int main()
{

unsigned short i;
unsigned char j = 0;

do
{
	for(i=0;i<8000;++i)
	{
		POKE(VIDEO_ADDR+i,j);
	}
	++j;
}
while(j!=0);


return 0;
}

