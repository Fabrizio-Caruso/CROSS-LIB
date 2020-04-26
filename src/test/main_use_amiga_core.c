#include "amiga_core.h"
// DEMO - INCBIN
volatile int frameCounter = 0;
//INCBIN(colors, "colors.bin")

static __attribute__((interrupt)) void interruptHandler() {
	hw->intreq=(1<<INTB_VERTB); hw->intreq=(1<<INTB_VERTB); //reset vbl req. twice for a4000 bug.
	// DEMO - increment frameCounter
	frameCounter++;
}


int main() {

	InitializeSystem();


	char *mem = (char*)alloc_chip(320*256*3/8+1024);
	short* copperlist=(short*)mem;
	char *video = mem+1024;
	copper_Set(copperlist);

	//KPrintF("Hello debugger from Amiga!\n");
	Write(Output(), "Hello console!\n", 15);
	Delay(50);

	warpmode(1);
	// TODO: precalc stuff here
	warpmode(0);

	TakeSystem();

	// DEMO
	SetVBLInterrupt((APTR)interruptHandler);
	
	short palette[8]={0x888,0xf00,0x0f0,0x00f,0xff0,0x0ff,0xf0f,0xfff};
	OpenDisplay(320,256,video,3,palette,8);

	int bplsz= 40*256;

	for(int i=0;i<40*256;i++)
		video[i]= 0xff; //i+palette[i&7];
	for(int i=0;i<40*256*2;i++)
		video[i+bplsz]= 0x0; //i+palette[i&7];
	

	while(!MouseLeft()) {
		WaitVbl();
		video[bplsz+0]=0xff;
		video[40*100]=frameCounter;
		video[40*200]=0x0;
		// DEMO - set colors from INCBIN (contains 64 colors)
		//hw->color[0] = ((UWORD*)colors)[frameCounter & 63];
	}

	free_chip(mem);
	// END
	FreeSystem();

	CloseLibrary((struct Library*)DOSBase);
	CloseLibrary((struct Library*)GfxBase);
}
