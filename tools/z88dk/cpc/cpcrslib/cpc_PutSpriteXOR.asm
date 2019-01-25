; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_PutSpriteXOR		;el sprite viene con dimensiones


GLOBAL cpc_PutSpXOR0
GLOBAL suma_siguiente_lineax0
GLOBAL anchox0


.cpc_PutSpriteXOR	; dibujar en pantalla el sprite
		; Entradas	bc-> Alto Ancho
		;			de-> origen
		;			hl-> destino
		; Se alteran hl, bc, de, af			

		
pop af
pop de
pop hl
push af

				
;	ld ix,2
;	add ix,sp
	
;	ld e,(ix+0)
;	ld d,(ix+1)		;destino
	
	;ld a,(ix+2)
	
;   	ld l,(ix+2)
;	ld h,(ix+3)		;origen
	
	ld a,(hl)		;ancho
	inc hl
    ld (cpc_PutSpXOR0+anchox0+1),a		;actualizo rutina de captura
    ;ld (anchot+1),a	;actualizo rutina de dibujo 
	sub 1
	cpl
	ld (cpc_PutSpXOR0+suma_siguiente_lineax0+1),a    ;comparten los 2 los mismos valores.		

	ld a,(hl)	;alto
	inc hl	
	ex de,hl
	
	;ld A,(ix+4)  
	jp cpc_PutSpXOR0