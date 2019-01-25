#define _CPCWYZGLOBAL_H

extern void  __FASTCALL__	__GLOBAL__  	cpc_WyzLoadSong(unsigned char numero);
extern void  __GLOBAL__   				cpc_WyzStartEffect(unsigned char canal, unsigned char efecto);
extern void  __CALLEE__	__GLOBAL__  		cpc_WyzSetPlayerOn(void);
extern void  __CALLEE__	__GLOBAL__   		cpc_WyzSetPlayerOff(void);
extern void  __FASTCALL__	__GLOBAL__ 	cpc_WyzConfigurePlayer(char valor);
extern char  __CALLEE__	__GLOBAL__   		cpc_WyzTestPlayer(void);
extern void  __GLOBAL__   				cpc_WyzInitPlayer(int sonidos, int pautas, int efectos, int canciones);
extern void  __FASTCALL__	__GLOBAL__  	cpc_WyzSetTempo(unsigned char tempo);

