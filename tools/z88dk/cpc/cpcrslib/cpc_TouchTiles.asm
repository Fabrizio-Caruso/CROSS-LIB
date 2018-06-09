; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2009       **
; ******************************************************

GLOBAL cpc_TouchTiles		;pone un byte en la posición de la pantalla indicada

GLOBAL cpc_UpdTileTable


.cpc_TouchTiles

	ld ix,2
	add ix,sp

	ld c,(ix+0)	;h
	;ld b,(ix+2)	;w

	ld L,(ix+4)	;y
	ld H,(ix+6)	;x	
	
	
	;lee el rectángulo y lo mueve al buffer
	
	.bucle_alto
	ld b,(ix+2)
	.bucle_ancho
	push HL
	;e+c
	ld a,c		;alto
	add L
	LD d,a
	;d+b
	ld a,b		;ancho
	add H
	LD e,a
	call cpc_UpdTileTable
	pop HL	
	djnz bucle_ancho	
	dec c
	jp nz,bucle_alto
	ret


