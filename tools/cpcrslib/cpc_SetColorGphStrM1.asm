; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_SetInkGphStrM1

GLOBAL cpc_PrintGphStr0M1
GLOBAL colores_cambM1

.cpc_SetInkGphStrM1
;preparación datos impresión. El ancho y alto son fijos!
	ld ix,2
	add ix,sp
	

 	ld a,(ix+0) ;valor
	ld c,(ix+2)	;color
	
ld hl,colores_camb+cpc_PrintGphStr0M1
ld b,0
add hl,bc
ld (hl),a

	
ret