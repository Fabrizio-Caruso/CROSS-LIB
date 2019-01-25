; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_ReadTile		;pone un byte en la posición de la pantalla indicada

GLOBAL pantalla_juego
GLOBAL ancho_pantalla_bytes

.cpc_ReadTile

	
	;ld ix,2
	;add ix,sp
	;ld e,(ix+0)
	;ld c,(ix+2)
	
	ld hl,2
    add hl,sp			; ¿Es la forma de pasar parámetros? ¿Se pasan en SP+2? ¿en la pila?			
	ld E,(hl)		;Y
	inc hl
	inc hl
	ld a,(hl)	;X
	
	include "multiplication2.asm"
	

	
			ld e,a
			ld d,0
		add hl,de		;SUMA X A LA DISTANCIA Y*ANCHO
	ld de,pantalla_juego
		add hl,de
		ld l,(hl)
		ld h,0
	ret
