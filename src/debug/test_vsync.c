#include <peekpoke.h>
#include <conio.h>

#if defined(__VIC20__)
    #define SET_DEBUG_BORDER()     POKE(36879u, 12)
    #define UNSET_DEBUG_BORDER()   POKE(36879u,  8)
#elif defined(__C64__)
    #define SET_DEBUG_BORDER()     POKE(53280u,  1)
    #define UNSET_DEBUG_BORDER()   POKE(53280u,  0)
#else
    #define SET_DEBUG_BORDER() 
    #define UNSET_DEBUG_BORDER() 
#endif


int main()
{

    int i;
    
    while(1)
    {
        waitvsync();
        SET_DEBUG_BORDER();
        
        // Waste some time
        for(i=0;i<100;++i){};
        
        UNSET_DEBUG_BORDER();
    }
    
    return 0;
}

