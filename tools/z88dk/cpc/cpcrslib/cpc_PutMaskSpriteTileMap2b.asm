; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_PutMaskSpriteTileMap2b
; Put a masked sprite in superbuffer
; The sprite must have coord0 updated because this routine doesn't calculate
; the superbuffer address using cx,cy 

GLOBAL tiles_tocados
GLOBAL pantalla_juego					;datos de la pantalla, cada byte indica un tile
GLOBAL posiciones_super_buffer
GLOBAL tiles
GLOBAL ancho_pantalla_bytes 
GLOBAL posicion_inicial_superbuffer

.cpc_PutMaskSpriteTileMap2b
;según las coordenadas x,y que tenga el sprite, se dibuja en el buffer
   
   ; ld ix,2
   ; add ix,sp
    
    
  ;  ld l,(ix+0)
  ;  ld h,(ix+1)	;HL apunta al sprite
        
    ;push hl
    ;pop ix
    
    ex de,hl
    LD IXH,d	;9
    LD IXL,e	;9 
    
    ld l,(ix+4)
    ld h,(ix+5)
    
  ;lo cambio para la rutina de multiplicar 
  ;  ld a,(ix+8)
  ;  ld e,(ix+9)
    ;ld (coord2),de    
   
    ;ld l,(ix+0)
    ;ld h,(ix+1)	;HL apunta al sprite


	;CALL transferir_sprite

	;RET
	
	


.pasa_bloque_a_sbuffer



	
.transferir_sprite

; según coordenadas, se busca la posición en buffer.

	;ld de,(coord2)		;E ancho D alto

	;NUEVA FORMA. LAS POSICIONES VIENEN PREVIAMENTE CALCULADAS DESDE FUERA.
	
;multiplica ExH


	

;en HL Tengo el valor

	;ld b,0
	;ld c,a
	;add hl,bc
	;ld de,posicion_inicial_superbuffer
	;add hl,de
	;hl apunta a la posición en buffer (destino)
	
	ld e,(ix+0)
    ld d,(ix+1)	;HL apunta al sprite
	
    ;con el ancho del sprite hago las actualizaciones necesarias a la rutina
    ld a,(de)
    ld (loop_alto_map_sbuffer+2),a	;marco el ancho del sprite para la rutina de impresión
    ld b,a
    ld a,ancho_pantalla_bytes
    sub b
    ld (ancho_2+1),a	;calculo el salto a la siguiene línea en función del ancho
    					;de la pantalla y del sprite
	inc de
	ld a,(de)	;alto sprite en A
	inc de
	
	;ld a,16		;necesito el alto del sprite
	
	
	
.sp_buffer_mask
	ld b,0
.ancho_2	
	ld c,ancho_pantalla_bytes-4 ;60	;;DEPENDE DEL ANCHO

	;defb $fd
	;LD H,A		;ALTO, SE PUEDE TRABAJAR CON HX DIRECTAMENTE
	ld ixh,a
.ancho_sprite		
.loop_alto_map_sbuffer
		;defb $fd
		;LD L,4		;ANCHO, SE PUEDE TRABAJAR CON HX DIRECTAMENTE
		ld ixl,4
		ex de,hl
		
.loop_ancho_map_sbuffer		

			
		LD A,(DE)	;leo el byte del fondo
		AND (HL)	;lo enmascaro		
		INC HL
		OR (HL)		;lo enmascaro 
		LD (DE),A	;actualizo el fondo
		INC DE
		INC HL
		
		;defb $fd
		;DEC L
		dec ixl
		JP NZ,loop_ancho_map_sbuffer
		
	   ;defb $fd
	   ;dec H
	   dec ixh
	   ret z
	   EX DE,HL
;hay que sumar 72 bytes para pasar a la siguiente línea
		add HL,BC	
		jp loop_alto_map_sbuffer
