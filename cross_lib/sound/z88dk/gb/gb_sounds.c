// see https://github.com/bwhitman/pushpin/blob/master/src/gbsound.txt
// chanel 1 register 0, Frequency sweep settings
// 7	Unused
// 6-4	Sweep time(update rate) (if 0, sweeping is off)
// 3	Sweep Direction (1: decrease, 0: increase)
// 2-0	Sweep RtShift amount (if 0, sweeping is off)
// 0001 0110 is 0x16, sweet time 1, sweep direction increase, shift ammount per step 110 (6 decimal)
// NR10_REG = 0x16; 

// chanel 1 register 1: Wave pattern duty and sound length
// Channels 1 2 and 4
// 7-6	Wave pattern duty cycle 0-3 (12.5%, 25%, 50%, 75%), duty cycle is how long a quadrangular  wave is "on" vs "of" so 50% (2) is both equal.
// 5-0 sound length (higher the number shorter the sound)
// 01000000 is 0x40, duty cycle 1 (25%), wave length 0 (long)
// NR11_REG = 0x40;

// chanel 1 register 2: Volume Envelope (Makes the volume get louder or quieter each "tick")
// On Channels 1 2 and 4
// 7-4	(Initial) Channel Volume
// 3	Volume sweep direction (0: down; 1: up)
// 2-0	Length of each step in sweep (if 0, sweeping is off)
// NOTE: each step is n/64 seconds long, where n is 1-7	
// 0111 0011 is 0x73, volume 7, sweep down, step length 3
// NR12_REG = 0x73;  

// chanel 1 register 3: Frequency LSbs (Least Significant bits) and noise options
// for Channels 1 2 and 3
// 7-0	8 Least Significant bits of frequency (3 Most Significant Bits are set in register 4)
// NR13_REG = 0x00;   

// chanel 1 register 4: Playback and frequency MSbs
// Channels 1 2 3 and 4
// 7	Initialize (trigger channel start, AKA channel INIT) (Write only)
// 6	Consecutive select/length counter enable (Read/Write). When "0", regardless of the length of data on the NR11 register, sound can be produced consecutively.  When "1", sound is generated during the time period set by the length data contained in register NR11.  After the sound is ouput, the Sound 1 ON flag, at bit 0 of register NR52 is reset.
// 5-3	Unused
// 2-0	3 Most Significant bits of frequency
// 1100 0011 is 0xC3, initialize, no consecutive, frequency = MSB + LSB = 011 0000 0000 = 0x300
// NR14_REG = 0xC3;	

#include "standard_libs.h"    

#include "gb_sounds.h"
#include <gb/gb.h>


void _XL_INIT_SOUND(void)
{
    NR52_REG = 0x80;
    NR50_REG = 0x77;
    NR51_REG = 0xFF;
}


void _XL_ZAP_SOUND(void)
{
    uint8_t i;
    uint8_t j;
    
    NR10_REG = 0x36;
    NR11_REG = 0x4F;
    NR12_REG = 0x73;
    NR13_REG = 0x80;
    NR14_REG = 0xC4;
    for(i=0;i<0x80;++i)
    {
        NR13_REG = 0x80+i;
        for(j=0;j<8;++j)
        {
        }
    }
}

void sound(uint8_t freq_msb)
{
    NR10_REG = 0x16;
    NR11_REG = 0x4F;
    NR12_REG = 0x73;
    NR13_REG = 0x00;
    NR14_REG = 0xC0 | freq_msb;

}

