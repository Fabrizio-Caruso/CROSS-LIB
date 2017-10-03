; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_PrintGphStrM1


GLOBAL cpc_PrintGphStr0M1
GLOBAL direcc_destino0_m1

.cpc_PrintGphStrM1
;preparación datos impresión. El ancho y alto son fijos!
	ld ix,2
	add ix,sp
	
	ld l,(ix+0)
	ld h,(ix+1)	;destino
	
	
   	ld e,(ix+2)
	ld d,(ix+3)	;texto origen
	xor a

 JP cpc_PrintGphStr0M1