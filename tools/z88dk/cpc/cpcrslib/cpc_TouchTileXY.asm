; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_TouchTileXY		;marca un tile indicando las coordenadas del tile

GLOBAL tiles
GLOBAL tiles_tocados

GLOBAL cpc_UpdTileTable


.cpc_TouchTileXY
;entran las coordenadas x,y y se marca el tile para redibujarlo en pantalla
;la coordenada x se divide entre 4 y da el tile
;la coordenada y se divide entre 16 y da el tile
	ld hl,2
    add hl,sp			; ¿Es la forma de pasar parámetros? ¿Se pasan en SP+2? ¿en la pila?			
	ld D,(hl)
	inc hl
	inc hl
	ld E,(hl)
JP cpc_UpdTileTable
