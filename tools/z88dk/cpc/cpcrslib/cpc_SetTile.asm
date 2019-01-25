; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_SetTile		;pone un byte en la posición de la pantalla indicada

GLOBAL pantalla_juego
GLOBAL ancho_pantalla_bytes

.cpc_SetTile

	ld ix,2
	add ix,sp
	
	ld e,(ix+2)
	ld a,(ix+4)
	
include "multiplication2.asm"




	
		ld e,a
		ld d,0
		add hl,de
		
	ld de,pantalla_juego
	add hl,de
	ld a,(ix+0)
	ld (hl),a
	ret
