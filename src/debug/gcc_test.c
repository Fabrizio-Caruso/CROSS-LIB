
unsigned char level = 42;

unsigned char bossLevel(void)
{
    return !(level%5);
}
    
int main(void)
{
    if(bossLevel())
    {
        level=1;
    }
    else
    {
        level =0;
    }
    return 0;
}
