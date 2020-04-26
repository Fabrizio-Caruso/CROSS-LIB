
#if __GNUC__ >= 8
#include "support/gcc8_c_support.h"
#define AMIGADEBUGGER 1
#else
#define AMIGADEBUGGER 0
#define warpmode(x)
#endif

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/graphics.h>
#include <graphics/gfxbase.h>
#include <graphics/view.h>
#include <exec/execbase.h>
#include <hardware/custom.h>
#include <hardware/intbits.h>


#define BPLHPTH_ADDR		0x1EC
#define BPLHPTL_ADDR		0x1EE
#define FMODE_ADDR		0x1FC

#define COP1LCH_ADDR		0x080
#define COP1LCL_ADDR		0x082
#define COP2LCH_ADDR		0x084
#define COP2LCL_ADDR		0x086
#define COPJMP1_ADDR		0x088
#define COPJMP2_ADDR		0x08A
#define COPINS_ADDR		0x08C
#define DIWSTRT_ADDR		0x08E
#define DIWSTOP_ADDR		0x090
#define DDFSTRT_ADDR		0x092
#define DDFSTOP_ADDR		0x094
#define BITPLNS_ADDR		0x0E0  // + 4*N bitplane.
#define BPLCON0_ADDR		0x100
#define BPLCON1_ADDR		0x102
#define BPLCON2_ADDR		0x104
#define BPLCON3_ADDR		0x106
#define BPL1MOD_ADDR		0x108
#define BPL2MOD_ADDR		0x10A
#define BPLCON4_ADDR		0x10C
#define COLOR00_ADDR		0x180

#define BLITTER_READY (1<<14)
#define BLITTER_ZERO (1<<13)
#define BLITTER_NASTY (1<<12)
#define DMA_ALL (1<<9)
#define BITPLANES_ON (1<<8)
#define COPPER_ON (1<<7)
#define BLITTER_ON (1<<6)
#define SPRITES_ON (1<<5)
#define DISKS_ON (1<<4)
#define AUDIO3_ON (1<<3)
#define AUDIO2_ON (1<<2)
#define AUDIO1_ON (1<<1)
#define AUDIO0_ON (1<<0)
#define DMACON_ADDR		0x096
 

struct ExecBase *SysBase;
volatile struct Custom *hw;
struct DosLibrary *DOSBase;
struct GfxBase *GfxBase;


void dma_On(int v)
{
 hw->dmacon=0x8000|v;
}
void dma_Off(int v)
{
 hw->dmacon=v;
}

short *copper_List = 0;
int copper_List_cursor=0;
int copper_List_bitplanes_cursor=0;
int copper_List_vx,copper_List_vy;
 int copper_Move(short a,short v) 
{
    int c=copper_List_cursor;
    copper_List[copper_List_cursor++]=a;
    copper_List[copper_List_cursor++]=v;
    return c;

}

 void copper_Set(void *copmem)
{
    copper_List=(short*)copmem;
    copper_List_cursor=0;
}
//(short sx = 0, short sy = 0, short ww = 320, short hh = 256, short wib = 0)
 int copper_SetView(short sx , short sy , short ww , short hh , short wib )
			{
				hh += sy;
				sx += 0x81; sy += 0x2c;
				ww += 0x81; hh += 0x2c;
				copper_List_vx = sx; copper_List_vy = sy;
				int r = copper_List_cursor;
				copper_Move(FMODE_ADDR, 0); // rimuovere se AGA
				copper_Move(DIWSTRT_ADDR, (sx) | (sy << 8));
				copper_Move(DIWSTOP_ADDR, (ww & 0xff) | ((hh & 0xff) << 8));
				copper_Move(DDFSTRT_ADDR, 0x38); // qui ci sarebbe da calcolare cose in modo da gestire overscan etc.
				copper_Move(DDFSTOP_ADDR, 0xd0);
				copper_Move(DDFSTOP_ADDR, 0xd0);
				copper_Move(BPLCON3_ADDR, 0x0c00); // AGA compatibility se dual playfield mode
				copper_Move(BPL1MOD_ADDR, wib);
				copper_Move(BPL2MOD_ADDR, wib);
				copper_Move(BPLCON1_ADDR, 0);

				// qua tocco ctrl se chiedo hires.. etc..
				//DIWSTRT, $5281; DIWSTOP, $06c1
				return r;
			}
			 int copper_OffsetX(short x)
			{
				x &= 0xf;
				return copper_Move(BPLCON1_ADDR, x | (x << 4));
			}
             int copper_Pointer(short addr, short n, void *pdata)
			{

				short* d = (short*)&pdata;
				short hi = d[0];
				short lo = d[1];

				short a = addr + (n << 2);
				// qui riswitcha da LE a BE
                copper_Move(a,hi);
				copper_Move(a+2,lo);
				return copper_List_cursor - 4;
			}

             int copper_Bitplane(short n, void *data)
			{
				return copper_Pointer(BITPLNS_ADDR, n, data);
			}
            
			 int copper_Bitplanes(short n, void *data, int count , int stride )
			{
				int r = copper_List_cursor;
				if(data)copper_List_bitplanes_cursor = copper_List_cursor;
				char *p = (char*)data;
				for (int i = 0; i < count; i++, p += stride)
					copper_Pointer(BITPLNS_ADDR, n + i, p);
				return r;
			}
			 int copper_RefreshBitplanes(short n, void *data, int count , int stride )
			{
				int oc = copper_List_cursor;
				copper_List_cursor = copper_List_bitplanes_cursor;
				copper_Bitplanes(n, data, count, stride);
				copper_List_cursor = oc;
				return copper_List_bitplanes_cursor;
			}


			 int copper_BitplanesNum(int n)
			{
				int v = 0;//playfields_mode ? (1 << 10) : 0;
				v |= n << 12;
				v |= 0x200;

				return copper_Move(BPLCON0_ADDR, v);
			}

             int copper_Jump(int cpl)
			{
				return copper_Move(COPJMP1_ADDR + (cpl<<1) , 0);
			}
			 int copper_Next(int cmdn, int cpl )
			{
				short *l = copper_List + copper_List_cursor + (cmdn<<1) + 4;
				return copper_Pointer(COP1LCH_ADDR, cpl, l);
			}

			

			 int copper_Color(short n, short  val)
			{
				return copper_Move(COLOR00_ADDR + (n << 1), val);
			}
			 int copper_Colors(short *c, int pos, int count )
			{
				int a = COLOR00_ADDR + (pos << 1);
				int r = copper_List_cursor;
				for (int i=0;i<count;i++)
				{
					copper_Move(a, c[i]);
					a += 2;
				}
				return r;
			}
            #define cWAITDEF      0xff,  0xff, 
             int copper_WaitRaw(int x , int y , int maskx , int masky, int blitwait )
			{	// calcolo in LE e copiando il res lo trasforma in BE
				copper_Move( (y << 8) | (x) | 1 , (blitwait ? 0x8000 : 0) | ((((masky & 0x7f) << 8) | maskx)&(~1)));
				return copper_List_cursor - 2;
			}

             //(int x = 0xdf, int y = 0xff, int maskx = 0xff, int masky = 0xff, bool blitwait = false)
			 int copper_Wait(int x , int y , int maskx , int masky , int blitwait )
			{
				x = ((x + copper_List_vx) >> 1) ;
				y = copper_List_vy + y;
				return copper_WaitRaw(x, y, maskx, masky, blitwait);
			}
			 int copper_End()
			{
				int r = copper_List_cursor;
				copper_Move(0xffdf, 0xfffe);
				copper_Move(0xffff, 0xfffe);
				return r;
			}
			 void copper_Load() //=0
			{
				hw->cop1lc = (long)copper_List;

			}
	
     void OpenDisplay(int w,int h, void* bitplanes, int nbitplanes,short *palette,int ncolors)
    {
           
            copper_List_cursor=0;
            copper_SetView(0,0,w,h,0);
            copper_Bitplanes(0,bitplanes,nbitplanes, (w>>3)*h);
            copper_Bitplanes(nbitplanes,0,6-nbitplanes, 0);
            copper_BitplanesNum(nbitplanes);
            copper_Colors(palette,0,ncolors);
            copper_End();
            copper_Load();
            dma_On(BITPLANES_ON|COPPER_ON|DMA_ALL);
			

    }
	


//backup
static UWORD SystemInts;
static UWORD SystemDMA;
static UWORD SystemADKCON;
volatile static APTR VBR=0;
static APTR SystemIrq;
 
struct View *ActiView;

static APTR GetVBR(void) {
	APTR vbr = 0;
	UWORD getvbr[] = { 0x4e7a, 0x0801, 0x4e73 }; // copper_MoveC.L VBR,D0 RTE

	if (SysBase->AttnFlags & AFF_68010) 
		vbr = (APTR)Supervisor((void*)getvbr);

	return vbr;
}

void SetInterruptHandler(APTR interrupt) {
	*(volatile APTR*)(((UBYTE*)VBR)+0x6c) = interrupt;
}

APTR GetInterruptHandler() {
	return *(volatile APTR*)(((UBYTE*)VBR)+0x6c);
}

//vblank begins at vpos 312 hpos 1 and ends at vpos 25 hpos 1
//vsync begins at line 2 hpos 132 and ends at vpos 5 hpos 18 
void WaitVbl() {
	while (1) {
		volatile ULONG vpos=*(volatile ULONG*)0xDFF004;
		vpos&=0x1ff00;
		if (vpos!=(311<<8))
			break;
	}
	while (1) {
		volatile ULONG vpos=*(volatile ULONG*)0xDFF004;
		vpos&=0x1ff00;
		if (vpos==(311<<8))
			break;
	}
}

 void WaitBlt() {
	UWORD tst=*(volatile UWORD*)&hw->dmaconr; //for compatiblity a1000
	(void)tst;
	while (*(volatile UWORD*)&hw->dmaconr&(1<<14)) {} //blitter busy wait
}

void TakeSystem() {


	warpmode(0);

	ActiView=GfxBase->ActiView; //store current view
	OwnBlitter();
	WaitBlit();	
	Disable();
	
	//Save current interrupts and DMA settings so we can restore them upon exit. 
	SystemADKCON=hw->adkconr;
	SystemInts=hw->intenar;
	SystemDMA=hw->dmaconr;
	hw->intena=0x7fff;//disable all interrupts
	hw->intreq=0x7fff;//Clear any interrupts that were pending
	
	WaitVbl();
	WaitVbl();
	hw->dmacon=0x7fff;//Clear all DMA channels
   

	//set all colors black
	for(int a=0;a<32;a++)
		hw->color[a]=0xf0;

	LoadView(0);
	WaitTOF();
	WaitTOF();

	WaitVbl();
	WaitVbl();

	VBR=GetVBR();
	SystemIrq=GetInterruptHandler(); //store interrupt register

	WaitVbl();

}

void FreeSystem() { 
	WaitVbl();
	WaitBlt();
	hw->intena=0x7fff;//disable all interrupts
	hw->intreq=0x7fff;//Clear any interrupts that were pending
	hw->dmacon=0x7fff;//Clear all DMA channels

	//restore interrupts
	SetInterruptHandler(SystemIrq);

	/*Restore system copper list(s). */
	hw->cop1lc=(ULONG)GfxBase->copinit;
	hw->cop2lc=(ULONG)GfxBase->LOFlist;
	hw->copjmp1=0x7fff; //start coppper

	/*Restore all interrupts and DMA settings. */
	hw->intena=SystemInts|0x8000;
	hw->dmacon=SystemDMA|0x8000;
	hw->adkcon=SystemADKCON|0x8000;

	LoadView(ActiView);
	WaitTOF();
	WaitTOF();
	WaitBlit();	
	DisownBlitter();
	Enable();
}

short MouseLeft(){return !((*(volatile UBYTE*)0xbfe001)&64);}	
short MouseRight(){return !((*(volatile UWORD*)0xdff016)&(1<<10));}


void *alloc_chip(unsigned long sz )
{
	long *r=(long*)AllocMem(sz+4, MEMF_CHIP);
	*r= sz;
	return (void*)(r+1);
}


void *free_chip(void *memtofree)
{
	long *r=((long*)memtofree)-1;

	FreeMem(r,*r);
	return r;
}

void InitializeSystem()
{
		SysBase = *((struct ExecBase**)4UL);
	hw = (struct Custom*)0xdff000;

	// We will use the graphics library only to locate and restore the system copper list once we are through.
	GfxBase = (struct GfxBase *)OpenLibrary("graphics.library",0);
	if (!GfxBase)
		Exit(0);

	// used for printing
	DOSBase = (struct DosLibrary*)OpenLibrary("dos.library", 0);
	if (!DOSBase)
		Exit(0);
}
void SetVBLInterrupt(APTR intr)
{
		SetInterruptHandler(intr);
	hw->intena=(1<<INTB_SETCLR)|(1<<INTB_INTEN)|(1<<INTB_VERTB);
	hw->intreq=1<<INTB_VERTB;//reset vbl req
}