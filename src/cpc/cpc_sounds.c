#include "cpc_sounds.h"

/*

  ld hl,sound_data
  call firmware
  dw 0bcaah
 
sound data:   
	db  01h,00h,00h,10h,01h,00h,0fh,50h

channel identifier
volume envelope
tone envelope
tone period low
tone period high (used to calculate the sound frequency) freq=(60000/tone period)
noise period
start volume (by using volume envelope 0 end volume also)
sound length (or envelope repeat count if envelope is used)
 
http://www.cpcwiki.eu/index.php/BIOS_Sound_Functions

 0  Channel (Bit0-2=ChannelA-C, Bit3-5=RendevousA..C, Bit6=Hold, Bit7=Flush)
 1  ENV Volume Envelope (1..15, or 0=none)
 2  ENT Tone Envelope   (1..15, or 0=none)
 3  Tone Frequency, lower 8bit (000h=none)
 4  Tone Frequency, upper 4bit
 5  Noise Frequency (01h..1Fh, or 00h=none)  ;shared for all 3 channels
 6  Volume (00h..0Fh)
 7  Duration, lower 8bit (in 1/100 seconds)  ;\0=endless/until end of ENV?
 8  Duration, upper 8bit                     ;/negative=repeat ENV -N times? 
*/

/*
 
ld bc,0x0738
call writepsg  ;enable all tone channels and disable noises (register 7)
ld bc,0x0100
call writepsg  ;set chn1 freq high to 0
ld bc,0x00c0
call writepsg  ;set chn1 freq low to 192
ld bc,0x080f
call writepsg  ;set chn1 volume to max
ret

*/

void __low_vol(void)
{
#asm 
	ld bc,0x0805
	call writepsg_low_vol 
	ret
	 
	writepsg_low_vol:
	  ld   a,b
	  di  
	  ld    b, 0xf4
	  out   (c), a
	  ld    b, 0xf6
	  in    a, (c)
	  or    0xc0
	  out   (c), a
	  and   0x3f
	  out   (c), a
	  ld    b, 0xf4
	  out   (c), c
	  ld    b,0xf6
	  ld    c, a
	  or    0x80
	  out   (c), a
	  out   (c), c
	  ei  
	  ret
		
#endasm	
}

void __stop_sound(void)
{
#asm 
	ld bc,0x0800
	call writepsg_stop 
	ret
	 
	writepsg_stop:
	  ld   a,b
	  di  
	  ld    b, 0xf4
	  out   (c), a
	  ld    b, 0xf6
	  in    a, (c)
	  or    0xc0
	  out   (c), a
	  and   0x3f
	  out   (c), a
	  ld    b, 0xf4
	  out   (c), c
	  ld    b,0xf6
	  ld    c, a
	  or    0x80
	  out   (c), a
	  out   (c), c
	  ei  
	  ret
		
#endasm	
}


void __ping_sound(void)
{
#asm
	ld bc,0x0738
	call writepsg_ping  
	ld bc,0x0100
	call writepsg_ping  
	ld bc,0x00c0
	call writepsg_ping  
	ld bc,0x080f
	call writepsg_ping  
	ret
	 
	writepsg_ping:
	  ld   a,b
	  di  
	  ld    b, 0xf4
	  out   (c), a
	  ld    b, 0xf6
	  in    a, (c)
	  or    0xc0
	  out   (c), a
	  and   0x3f
	  out   (c), a
	  ld    b, 0xf4
	  out   (c), c
	  ld    b,0xf6
	  ld    c, a
	  or    0x80
	  out   (c), a
	  out   (c), c
	  ei  
	  ret
		
#endasm	
}


void _ping_sound(void)
{
	unsigned char i=0;

	__ping_sound();
	for(;i<250;++i)
	{		
	}	
	__stop_sound();
}


void __explosion_sound(void)
{
#asm
	ld bc,0x0707
	call writepsg_explosion  
	ld bc,0x0618
	call writepsg_explosion  
	ld bc,0x080f
	call writepsg_explosion  
	ret
	 
	writepsg_explosion:
	  ld   a,b
	  di  
	  ld    b, 0xf4
	  out   (c), a
	  ld    b, 0xf6
	  in    a, (c)
	  or    0xc0
	  out   (c), a
	  and   0x3f
	  out   (c), a
	  ld    b, 0xf4
	  out   (c), c
	  ld    b,0xf6
	  ld    c, a
	  or    0x80
	  out   (c), a
	  out   (c), c
	  ei  
	  ret
		
#endasm		
}

void _explosion_sound(void)
{
	unsigned short i;


	__explosion_sound();
	for(i=0;i<600;++i)
	{
	}
	__low_vol();
	for(i=0;i<600;++i)
	{
	}	
	__stop_sound();
}



void _ZAP_SOUND(void)
{
#asm
	ld bc,0x0738
	call writepsg_ping  
	ld bc,0x0100
	call writepsg_ping  
	ld bc,0x0090
	call writepsg_ping  
	ld bc,0x080f
	call writepsg_ping  
	ret
	 
	writepsg_zap:
	  ld   a,b
	  di  
	  ld    b, 0xf4
	  out   (c), a
	  ld    b, 0xf6
	  in    a, (c)
	  or    0xc0
	  out   (c), a
	  and   0x3f
	  out   (c), a
	  ld    b, 0xf4
	  out   (c), c
	  ld    b,0xf6
	  ld    c, a
	  or    0x80
	  out   (c), a
	  out   (c), c
	  ei  
	  ret
		
#endasm	
}

void ZAP_SOUND(void)
{
	unsigned char i;
	

	_ZAP_SOUND();			
	for(i=0;i<250;++i)
	{}
	__stop_sound();
	
	__ping_sound();
	for(i=0;i<250;++i)
	{}	
	__stop_sound();
}

