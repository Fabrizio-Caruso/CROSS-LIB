#include "c264_sounds.h"


void EXPLOSION_SOUND(void)
	{ 
		unsigned char volume = 0; unsigned char j; 
		for(;volume<=MAX_VOLUME;++volume) 
		{ 
			for(j=0;j<255;++j){};
			POKE(SELECT,volume+NOISE); 
		} 
		POKE(SELECT,128); 
	};
	
void SHOOT_SOUND(void) 
{ 
	unsigned char volume; unsigned char j; 
	POKE(SELECT,MAX_VOLUME+NOISE); 
	for(volume=0;volume<=MAX_VOLUME;++volume) 
	{ 
		for(j=50;j<255;++j){} 
		POKE(SELECT,volume+NOISE); 
	} 
	for(volume=0;volume<=MAX_VOLUME;++volume) 
	{ 
		for(j=0;j<255;++j){} 
		for(j=0;j<255;++j){} 
		POKE(SELECT,MAX_VOLUME-volume+NOISE); 
	} 
	POKE(SELECT,128); 
};

void PING_SOUND(void) 
{ 
	unsigned char freq; unsigned char j; 
	POKE(SELECT,MAX_VOLUME+VOICE_1); 
	POKE(HI_FREQ_1,PEEK(HI_FREQ_1) | 1); 
	for(freq=100;freq<120;++freq) 
	{ 
		for(j=0;j<150;++j) {}; 
		POKE(LO_FREQ_1,freq); 
	} 
	POKE(HI_FREQ_1,PEEK(HI_FREQ_1) & (255-3)); 
	POKE(SELECT,128); 
};


void ZAP_SOUND(void) 
{ 
	unsigned char freq; unsigned char j; 
	POKE(SELECT,MAX_VOLUME+VOICE_1); 
	POKE(HI_FREQ_1,PEEK(HI_FREQ_1) & (255-3)); 
	for(freq=100;freq<255;++freq) 
	{ 
		for(j=0;j<25;++j) {}; 
		POKE(LO_FREQ_1,freq); 
	} 
	POKE(SELECT,128); 
};	


void TOCK_SOUND(void) 
{ 
	unsigned char freq; unsigned char j; 
	POKE(SELECT,MAX_VOLUME+VOICE_1); 
	POKE(HI_FREQ_1,PEEK(HI_FREQ_1) & (255-3)); 
	for(freq=12;freq<16;++freq) 
	{ 
		for(j=0;j<150;++j) {}; 
		POKE(LO_FREQ_1,freq); 
	} 
	POKE(SELECT,128); 
};


void TICK_SOUND(void) 
{ 
	unsigned char freq; unsigned char j; 
	POKE(SELECT,MAX_VOLUME+VOICE_1); 
	POKE(HI_FREQ_1,PEEK(HI_FREQ_1) & (255-3)); 
	for(freq=16;freq<20;++freq) 
	{ 
		for(j=0;j<150;++j) {}; 
		POKE(LO_FREQ_1,freq); 
	} 
	POKE(SELECT,128); 
};