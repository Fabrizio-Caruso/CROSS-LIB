
#include <tgi.h>
#include <conio.h>

#define CLI() asm("\tcli")

int main(void)
{
    uint16_t i;
    uint8_t str[2];
    
    str[1] = '\0';
    tgi_install (tgi_static_stddrv);

    tgi_init ();        
    CLI();

    tgi_clear();

    while (tgi_busy())  {  };

    tgi_setpalette(tgi_getdefpalette());
    tgi_setbgcolor(TGI_COLOR_GREY);

    tgi_setcolor(TGI_COLOR_RED);

    tgi_outtextxy(8,8*0,"press a key"); 
    for(i=0;i<30000;++i){}
    if(kbhit())
    {
    }
    else
    {
    }    
    
    str[0] = cgetc();   
    tgi_outtextxy(8,8*1,"step 1"); 
    tgi_outtextxy(8*8,8*1,str); 
    for(i=0;i<30000;++i){}

    if(kbhit())
    {
    }
    else
    {
    }

    str[0] = cgetc();   
    tgi_outtextxy(8,8*2,"step 2"); 
    tgi_outtextxy(8*8,8*2,str); 
    for(i=0;i<30000;++i){}
    
    str[0] = cgetc();   
    tgi_outtextxy(8,8*3,"step 3");
    tgi_outtextxy(8*8,8*3,str);     
    for(i=0;i<30000;++i){}
    tgi_outtextxy(8,8*4,"step 4");
    
    while(1)
    {
        if(kbhit())
        {
            str[0] = cgetc();
            tgi_outtextxy(8*8,8*4,str);
        }
        else
        {
            tgi_outtextxy(8*8,8*6,"*");
            for(i=0;i<4000;++i){}
            tgi_outtextxy(8*8,8*6,"+");            
            tgi_outtextxy(8*8,8*4," ");            
        }
    }
    
    return 0;
}
