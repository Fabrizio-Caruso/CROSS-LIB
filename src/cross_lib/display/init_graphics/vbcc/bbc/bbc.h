#if !defined(_BBC_MACROS)
#define _BBC_MACROS

typedef struct {
  int channel;
  int amplitude;
  int pitch;
  int duration;
} bbc_sound;


// https://blog.mousefingers.com/post/bbc/bbc_colour_cycling/

char OSBYTE0(__reg("a") char)="\tjsr\t$fff4\n\ttxa";
char OSBYTE1(__reg("a") char,__reg("r0") char)="\tldx\tr0\n\tjsr\t$fff4\n\ttxa";
char OSBYTE1RI(__reg("a") char,__reg("r0") char)="\tldx\tr0\n\tjsr\t$fff4\n\ttxa\n\tpha\n\ttya\n\ttax\n\tpla";
char OSBYTE2(__reg("a") char,__reg("r0") char,__reg("r1") char)="\tldx\tr0\n\tldy\tr1\n\tjsr\t$fff4\n\ttxa";
char OSBYTE2I(__reg("a") char,__reg("r0/r1") unsigned int)="\tldx\tr0\n\tldy\tr1\n\tjsr\t$fff4\n\ttxa";
void OSBYTE0NR(__reg("a") char)="\tjsr\t$fff4";
void OSBYTE1NR(__reg("a") char,__reg("r0") char)="\tldx\tr0\n\tjsr\t$fff4";
void OSBYTE2NR(__reg("a") char,__reg("r0") char,__reg("r1") char)="\tldx\tr0\n\tldy\tr1\n\tjsr\t$fff4";

#define OSVER() OSBYTE0(0)
#define OSUSRFLG(x,y) OSBYTE2(1,(x),(y))
#define OSSELINPUT(x) OSBYTE1NR(2,(x))
#define OSSELOUTPUT(x) OSBYTE2(3,(x),(y))
#define OSCURSORED(x) OSBYTE2(4,(x),(y))
#define OSSELADC(x) OSBYTE1(16,(x))
#define OSFRCADC(x) OSBYTE1NR(17,(x))
#define OSVSYNC() OSBYTE0(19)
#define OSRDVDU() OSBYTE0(117)
#define OSSCANKEY(x) OSBYTE1(121,(x))
#define OSSCANKEY16() OSBYTE0(122)
#define OSADVAL(x) OSBYTE1RI(128,(x))
#define OSINKEY(x) OSBYTE2I(129,(x)) 




void OSWORD(__reg("a") char,__reg("r0/r1") void *)="\tldx\tr0\n\tldy\tr1\n\tjsr\t$fff1";

#define OSRDSYSCLK(x) OSWORD(1,x)
#define OSWRSYSCLK(x) OSWORD(2,x)
#define OSRDTIMER(x) OSWORD(3,x)
#define OSWRTIMER(x) OSWORD(4,x)
#define OSRDIO(x) OSWORD(5,x)
#define OSWRIO(x) OSWORD(6,x)
#define OSSOUND(x) OSWORD(7,x)
#define OSENVELOPE(x) OSWORD(8,x)
#define OSRDPIXEL(x) OSWORD(9,x)
#define OSRDCHARDEF(x) OSWORD(10,x)
#define OSRDPALETTE(x) OSWORD(11,x)
#define OSWRPALETTE(x) OSWORD(12,x)
#define OSRDGRCURSOR(x) OSWORD(13,x)




#endif // _BBC_MACROS
