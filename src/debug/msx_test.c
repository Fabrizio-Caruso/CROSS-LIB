#include <msx/gfx.h>
int main()
{
    uint8_t i;
    set_color(15, 1, 1);
    set_mode(mode_0);

    for(i=0;i<254;++i)
    {
        vpoke(0x0000+i,i);
    }    
    while(1);
    return 0;
}