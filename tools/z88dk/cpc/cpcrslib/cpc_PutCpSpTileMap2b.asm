; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2009       **
; ******************************************************

GLOBAL cpc_PutCpSpTileMap2b

GLOBAL tiles_tocados
GLOBAL pantalla_juego					;datos de la pantalla, cada byte indica un tile
GLOBAL posiciones_super_buffer
GLOBAL tiles
GLOBAL ancho_pantalla_bytes 
GLOBAL posicion_inicial_superbuffer

.cpc_PutCpSpTileMap2b
;según las coordenadas x,y que tenga el sprite, se dibuja en el buffer
   

    ex de,hl
    LD IXH,d	;9
    LD IXL,e	;9 
    
  ;lo cambio para la rutina de multiplicar 
    ld a,(ix+8)
    ld e,(ix+9)


include "multiplication1.asm"
	

	
	ld b,0
	ld c,a
	add hl,bc
	ld de,posicion_inicial_superbuffer
	add hl,de
	;hl apunta a la posición en buffer (destino)

			
	ld (ix+4),l		;update superbuffer address
    ld (ix+5),h
    
    
	ld e,(ix+0)
    ld d,(ix+1)	;HL apunta al sprite
	
ex de,hl
inc hl
inc hl
    jp (hl)
    
 	

		
	