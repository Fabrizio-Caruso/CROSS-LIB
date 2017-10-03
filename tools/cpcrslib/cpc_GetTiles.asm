; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2009       **
; ******************************************************

GLOBAL cpc_GetTiles		;pone un byte en la posición de la pantalla indicada

GLOBAL pantalla_juego
GLOBAL ancho_pantalla_bytes

GLOBAL captura
GLOBAL cpc_PutTiles

.cpc_GetTiles

	ld ix,2
	add ix,sp
	
	ld l,(ix+0)	
	ld h,(ix+1)	
	
	ld c,(ix+2)	;h
	;ld b,(ix+4)	;w

	ld e,(ix+6)	;y
	ld d,(ix+8)	;x	
	
	
	;lee el rectángulo y lo mueve al buffer
	
	.bucle_alto
	ld b,(ix+4)
	.bucle_ancho
	push de

	;e+c
	ld a,c
	add e
	ld e,a
	;d+b
	ld a,b
	add d
	push hl
	call captura
	ld a,(hl)
	pop hl
	ld (hl),a
	inc hl		
	pop de	
	djnz bucle_ancho	
	
	dec c
	jp nz,bucle_alto
	ret

	;e=y
	;a=x
	

