; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_PutMaskSp


GLOBAL cpc_PutMaskSp0
GLOBAL suma_siguiente_linea_m0
GLOBAL ancho_m0


.cpc_PutMaskSp	; dibujar en pantalla el sprite
		; Entradas	bc-> Alto Ancho
		;			de-> origen
		;			hl-> destino
		; Se alteran hl, bc, de, af			
		
	ld ix,2
	add ix,sp
	
	ld l,(ix+0)
	ld h,(ix+1)
	
	ld a,(ix+2)
	
   	ld e,(ix+6)
	ld d,(ix+7)

    ld (cpc_PutMaskSp0+ancho_m0+1),a		;actualizo rutina de captura
    ;ld (anchot+1),a	;actualizo rutina de dibujo 
	sub 1
	cpl
	ld (cpc_PutMaskSp0+suma_siguiente_linea_m0+1),a    ;comparten los 2 los mismos valores.		
	
	ld A,(ix+4)  
	jp cpc_PutMaskSp0
	