#include <psg.h>
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
void ZAP_SOUND(void) {
	uint8_t i;
	uint8_t j;
	set_psg(A_VOLUME,15);
	set_psg(A_PERIOD_LOW,255);
	set_psg(A_PERIOD_HI, 15);
	set_psg(CONTROL, 0xFF  - 0x01);
	for(i=0;i<16;i++) {
		set_psg(A_PERIOD_HI,15-i);		
		for(j=0;j<150;++j){}
	}
	set_psg(A_VOLUME,0);		
}

int main()
{
    ZAP_SOUND();
    
    while(1){};
    
    return 0;
}

