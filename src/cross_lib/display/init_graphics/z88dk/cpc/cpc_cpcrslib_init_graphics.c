
void set_palette()
{ 
#asm
    EXTERN firmware

	ld bc,$0000
    call firmware
	defw 0xbc38	
 
    ld  a,$00
    ld  bc,$0000
    call firmware
    defw 0xbc32
	
    ld  a,$02
    ld  bc,$1a1a
    call firmware
    defw 0xbc32
	
    ld  a,$03
    ld  bc,$0606
    call firmware
    defw 0xbc32
	
    ld  a,$01
    ld  bc,$1818
    call firmware
    defw 0xbc32
#endasm 
}

void INIT_GRAPHICS(void)
{
	set_palette();
}
