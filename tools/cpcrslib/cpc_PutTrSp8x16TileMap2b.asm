; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_PutTrSp8x16TileMap2b

GLOBAL tiles_tocados
GLOBAL pantalla_juego					;datos de la pantalla, cada byte indica un tile
GLOBAL posiciones_super_buffer
GLOBAL tiles
GLOBAL ancho_pantalla_bytes 
GLOBAL posicion_inicial_superbuffer

GLOBAL mascara1
GLOBAL mascara2

.cpc_PutTrSp8x16TileMap2b

;según las coordenadas x,y que tenga el sprite, se dibuja en el buffer
   
    ex de,hl	;4
    LD IXH,d	;9
    LD IXL,e	;9 

    ;push hl		;10
    ;pop ix			;14
    
  ;lo cambio para la rutina de multiplicar 
    ld e,(ix+8)		;x
    ld l,(ix+9)		;y


.pasa_bloque_a_sbuffer

.transferir_sprite


ld h,0
ld d,h

add hl,hl	;2
add hl,hl	;4
ld b,h
ld c,l
add hl,hl	;8
add hl,hl	;16
add hl,hl	;32
add hl,hl	;64
sbc hl,bc	;60
	
	add hl,de
	ld de,posicion_inicial_superbuffer
	add hl,de
	;hl apunta a la posición en buffer (destino)

			
;	ld (ix+4),l		;update superbuffer address
;    ld (ix+5),h
    
    
	ld e,(ix+0)
    ld d,(ix+1)	;HL apunta al sprite

	inc de
	inc de
	
	
.sp_buffer_mask
	;ld b,0
.ancho_2	
	ld bc,$38
	ld ixh,16
.loop_alto_map_sbuffer
		ex de,hl
.loop_ancho_map_sbuffer		

;**********************************************
	ld A,(hl)		;sprite
	and $aa
	cp mascara2
	jp z,sig_pixn_der_2
	
	ld b,a 
	ld a,(de)		;fondo
	and $55	
	or b
	ld (de),a 
	
.sig_pixn_der_2
	ld a,(hl) 
	and $55
	cp mascara1
	jp z,pon_buffer_der_2
	
	ld b,a 
	ld a,(de)
	and $aa
	or b
	ld (de),a
	
.pon_buffer_der_2
		INC DE
		INC HL
;*************************************************			
;**********************************************
	ld A,(hl)

	and $aa			;deja solo los bits a comprobar
	
	cp mascara2		;los comprueba comparando con mascara2
	jp z,sig_pixn_der_2a
	ld b,a 
	ld a,(de)		;fondo	
	and $55	
	or b
	ld (de),a 
.sig_pixn_der_2a
	ld a,(hl) 
	and $55
	cp mascara1
	jp z,pon_buffer_der_2a
	ld b,a 
	ld a,(de)
	and $aa
	or b
	ld (de),a
.pon_buffer_der_2a
		INC DE
		INC HL
;*************************************************		
;**********************************************
	ld A,(hl)
	and $aa
	cp mascara2
	jp z,sig_pixn_der_2b
	ld b,a 
	ld a,(de)
	and $55	
	or b
	ld (de),a 
.sig_pixn_der_2b
	ld a,(hl) 
	and $55
	cp mascara1
	jp z,pon_buffer_der_2b
	ld b,a 
	ld a,(de)
	and $aa
	or b
	ld (de),a
.pon_buffer_der_2b
		INC DE
		INC HL
;*************************************************		
;**********************************************
	ld A,(hl)
	and $aa
	cp mascara2
	jp z,sig_pixn_der_2c
	ld b,a 
	ld a,(de)
	and $55	
	or b
	ld (de),a 
.sig_pixn_der_2c
	ld a,(hl) 
	and $55
	cp mascara1
	jp z,pon_buffer_der_2c
	ld b,a 
	ld a,(de)
	and $aa
	or b
	ld (de),a
.pon_buffer_der_2c
		INC DE
		INC HL
;*************************************************		
		
;		dec ixl
;		JP NZ,loop_ancho_map_sbuffer

	   dec ixh
	   ret z

	   EX DE,HL
	   ld b,0
       add HL,BC	
	   jp loop_alto_map_sbuffer

		
		
		
		
		
		