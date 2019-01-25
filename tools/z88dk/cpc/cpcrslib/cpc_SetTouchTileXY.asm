; ******************************************************

GLOBAL cpc_SetTouchTileXY		;marca un tile indicando las coordenadas del tile

GLOBAL tiles
GLOBAL tiles_tocados
GLOBAL pantalla_juego
GLOBAL ancho_pantalla_bytes

	GLOBAL cpc_UpdTileTable

.cpc_SetTouchTileXY

	ld ix,2
	add ix,sp
	
	ld e,(ix+2)
	ld c,(ix+4)
	
	ld d,e
	ld e,c
	call cpc_UpdTileTable

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
	
	
	
;entran las coordenadas x,y y se marca el tile para redibujarlo en pantalla
;la coordenada x se divide entre 4 y da el tile
;la coordenada y se divide entre 16 y da el tile



