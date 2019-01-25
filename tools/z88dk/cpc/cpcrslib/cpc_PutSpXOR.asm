; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_PutSpXOR


GLOBAL cpc_PutSpXOR0
GLOBAL suma_siguiente_lineax0
GLOBAL anchox0


.cpc_PutSpXOR	; dibujar en pantalla el sprite
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

    ld (cpc_PutSpXOR0+anchox0+1),a		;actualizo rutina de captura
    ;ld (anchot+1),a	;actualizo rutina de dibujo 
	sub 1
	cpl
	ld (cpc_PutSpXOR0+suma_siguiente_lineax0+1),a    ;comparten los 2 los mismos valores.		
	
	ld A,(ix+4)  
	jp cpc_PutSpXOR0
	