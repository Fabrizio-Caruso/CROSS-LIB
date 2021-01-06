
#define POKE(addr,val)     (*(uint8_t*) (addr) = (val))
#define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
#define PEEK(addr)         (*(uint8_t*) (addr))
#define PEEKW(addr)        (*(unsigned*) (addr))

void GRAPHICS_INIT(void)
{
    // Set 256x192x16 mode
    POKE(0xFF99,0x1A);

    // Set graphics ON
    POKE(0xFF98,0x80);

    // Set actual screen ram at $600000
    POKE(0xFF9D,0xC0);
    POKE(0xFF9E,0x00);

    // Set palette
    /*
    POKE(0xFFB0,...);
    ...
    POKE(0xFFBF,...);
    */
    // Set MMU windows at $8000
    POKE(0xFFA4,0x30);
    POKE(0xFFA5,0x31);
    POKE(0xFFA6,0x32);
        
        
    // TODO: Remove this
    
    while(1){}
}

