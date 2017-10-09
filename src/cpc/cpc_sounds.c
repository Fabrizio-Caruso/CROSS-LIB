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
// void __stop_sound(void)
// {
// #asm
	// EXTERN firmware
	
	// jr stop_code
	
	// stop_data: 
		// defb 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  
		
	// stop_code:
		// ld hl,stop_data
		// call firmware
		// defw 0xbcaa
// #endasm	
// }


void __ping_sound(void)
{
#asm
	EXTERN firmware
	
	jr ping_code
	
	ping_data: 
		defb 1,0,0,142,0,0,12,50,0  
		
	ping_code:
		ld hl,ping_data
		call firmware
		defw 0xbcaa
#endasm	
}

void _ping_sound(void)
{
	unsigned char i=0;

	__ping_sound();
	for(;i<250;++i)
	{		
	}	
	// __stop_sound();
}

void __explosion_sound(void)
{
#asm
	EXTERN firmware

	jr explosion_code
	
	explosion_data: 
		defb 1,0,0,142,0,0,12,50,0
		
	explosion_code:	
		ld hl,explosion_data
		call firmware
		defw 0xbcaa	
#endasm		
}

void _explosion_sound(void)
{
	unsigned char i=0;

	__explosion_sound();
	for(;i<240;++i)
	{
	}
    // __stop_sound
}


void _ZAP_SOUND(void)
{
#asm
	EXTERN firmware

	jr zap_code
	
	zap_data: 
		defb 1,0,0,142,0,0,12,50,0
	
	zap_code:	
		ld hl,zap_data
		call firmware
		defw 0xbcaa		
#endasm	
}

void ZAP_SOUND(void)
{
	unsigned char i=0;
	

	_ZAP_SOUND();			
	
	for(;i<250;++i)
	{}
	// __stop_sound
}

