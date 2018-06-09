 ; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_PrintGphStrXYM12X

GLOBAL cpc_GetScrAddress0

GLOBAL cpc_PrintGphStr0M1

GLOBAL direcc_destino0_m1

.cpc_PrintGphStrXYM12X
;preparación datos impresión. El ancho y alto son fijos!
	ld ix,2
	add ix,sp
	

 	ld L,(ix+0)
	ld A,(ix+2)	;pantalla
	
	call cpc_GetScrAddress0   

	;destino
	
   	ld e,(ix+4)
	ld d,(ix+5)	;texto origen
	ld a,1
    
 JP cpc_PrintGphStr0M1