#include <psg.h>

int main(void)
{
    unsigned char i;
    unsigned char j;
    
    psg_init();                       
    psg_volume(0, 15);        
    // psg_channels(chan0, chanNone); // set first channel to tone generation, reset noise channels
          
    for(i=0;i<16;i++)
    {
        psg_tone(0, 500-(i*16));
        for(j=0;j<150;++j) {}
    }         
    psg_volume(0, 0);
    return 0;
}