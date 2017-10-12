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


#define A_PERIOD_LOW 0
#define A_PERIOD_HI 1

#define B_PERIOD_LOW 2
#define B_PERIOD_HI 3

#define C_PERIOD_LOW 4
#define C_PERIOD_HI 5
	
#define NOISE 6
#define CONTROL 7

#define A_VOLUME 8
#define B_VOLUME 9
#define C_VOLUME 10
	
#define ENV_PERIOD_LOW 11
#define ENV_PERIOD_HI 12

#define ENV_WAVE 13


void _write_psg_aux(int value, int reg)
{
	#asm
	pop af
	pop hl ; reg
	pop bc ; value
	push bc
	push hl
	push af
	
	ld b,l
	
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

#define _write_psg(reg,value) _write_psg_aux(value,reg)

void ZAP_SOUND(void) 
{
	unsigned char i;
	_write_psg(NOISE,0);	
	_write_psg(A_VOLUME,15);
	_write_psg(A_PERIOD_LOW,255);
	_write_psg(A_PERIOD_HI, 1);		

	_write_psg(CONTROL, 0x38);
	
	for(i=0;i<220;++i)
	{
	}
	
	_write_psg(CONTROL, 0x3f);		
	_write_psg(A_VOLUME,0);			
	// unsigned char i;
	// unsigned char j;
	// _write_psg(NOISE,0);	
	// _write_psg(A_VOLUME,15);
	
	// _write_psg(A_PERIOD_LOW,255);
	// _write_psg(A_PERIOD_HI, 15);
	
	// _write_psg(CONTROL, 0x07, 0x38);
	
	// for(i=0;i<16;i++)
	// {
		// _write_psg(A_PERIOD_HI,15-i);		
		// for(j=0;j<150;++j)
		// {
		// }
	// }
	
	// for(i=0;i<16;i++)
	// {
		// _write_psg(A_VOLUME,15-i);		
		// for(j=0;j<200;++j)
		// {
		// }
	// }
	
	// _write_psg(CONTROL, 0x3ff);		
	// _write_psg(A_VOLUME,0);
}


void _ping_sound(unsigned char freq)
{ 
	unsigned char i;
	_write_psg(NOISE,0);	
	_write_psg(A_VOLUME,15);
	_write_psg(A_PERIOD_LOW,255);
	_write_psg(A_PERIOD_HI, 15 - (freq>>4));		

	_write_psg(CONTROL, 0x38);
	
	for(i=0;i<220;++i)
	{
	}
	
	_write_psg(CONTROL, 0x3f);		
	_write_psg(A_VOLUME,0);		
}			


void _explosion_sound(unsigned char freq)
{ 
	unsigned char i;
	unsigned char j;
	
	_write_psg(NOISE,15);
	_write_psg(A_VOLUME,15);
	
	_write_psg(A_PERIOD_LOW,0);
	_write_psg(A_PERIOD_HI, 15 - (freq>>4));
	

	_write_psg(CONTROL, 0x07, 0x07);
	
	for(i=0;i<13;i++)
	{
		_write_psg(A_VOLUME,15-i);		
		for(j=0;j<250;++j)
		{
		}		
	}
	for(j=0;j<250;++j)
		{
		}
		
	_write_psg(CONTROL, 0x3f);				
	_write_psg(A_VOLUME,0);		
}

