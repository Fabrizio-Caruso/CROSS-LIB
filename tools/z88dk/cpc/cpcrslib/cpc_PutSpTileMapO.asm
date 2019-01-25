; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_PutSpTileMapO		;se encarga de actualizar los tiles que toca el sprite 

GLOBAL tiles_tocados
GLOBAL pantalla_juego					;datos de la pantalla, cada byte indica un tile
GLOBAL posiciones_super_buffer
GLOBAL tiles
GLOBAL solo_coordenadas 

GLOBAL bit_ancho
GLOBAL bit_alto

GLOBAL cpc_PutSpTileMap
GLOBAL cpc_UpdTileTable



.cpc_PutSpTileMapO

    ex de,hl	;4
	LD IXH,d	;9
    LD IXL,e	;9   
    ld e,(ix+0)
    ld d,(ix+1)
    
    LD A,(de)
    ld (bit_ancho+1),a
    
    inc de
    LD A,(de)
	ld (bit_alto+1),a
	ld e,(ix+10)
    ld d,(ix+11)
    jp solo_coordenadas    
    
 