; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_PutTrSpriteTileMap2b

GLOBAL tiles_tocados
GLOBAL pantalla_juego					;datos de la pantalla, cada byte indica un tile
GLOBAL posiciones_super_buffer
GLOBAL tiles
GLOBAL ancho_pantalla_bytes 
GLOBAL posicion_inicial_superbuffer

GLOBAL mascara1
GLOBAL mascara2

.cpc_PutTrSpriteTileMap2b

;según las coordenadas x,y que tenga el sprite, se dibuja en el buffer
   
    ex de,hl
    LD IXH,d	;9
    LD IXL,e	;9 
    
  ;lo cambio para la rutina de multiplicar 
    ld l,(ix+4)
    ld h,(ix+5)





;en HL Tengo el valor
    
    
	ld e,(ix+0)
    ld d,(ix+1)	;HL apunta al sprite
	
    ;con el alto del sprite hago las actualizaciones necesarias a la rutina
    ld a,(de)
    ld (loop_alto_map_sbufferX+2),a
    ld b,a
    ld a,ancho_pantalla_bytes
    sub b
    ld (ancho_2X+1),a
	inc de
	ld a,(de)
	inc de
	
	;ld a,16		;necesito el alto del sprite
	
	
	
.sp_buffer_maskX
	ld b,0
.ancho_2X	
	ld c,ancho_pantalla_bytes-4 ;60	;;DEPENDE DEL ANCHO

	;defb $fD
	;LD H,A		;ALTO, SE PUEDE TRABAJAR CON HX DIRECTAMENTE
	ld ixh,a
.loop_alto_map_sbufferX
		;defb $fD
		;LD L,4		;ANCHO, SE PUEDE TRABAJAR CON HX DIRECTAMENTE
		ld ixl,4
		ex de,hl
.loop_ancho_map_sbufferX		

;**********************************************

;push bc
;ex de,hl
	ld A,(hl)
		
	and $aa
	cp mascara2
	jp z,sig_pixn_der_2X
	ld b,a ;B es el único registro libre
	ld a,(de) ;pixel actual donde pinto
	and $55		;0101 0101
	;and mascara1
	or b
	ld (de),a ;y lo pone en pantalla
.sig_pixn_der_2X
	ld a,(hl) ;pixel del sprite
	;and $55   ;0101 0101
	and $55
	cp mascara1
	jp z,pon_buffer_der_2X
	ld b,a ;B es el único registro libre
	ld a,(de) ;pixel actual donde pinto
	;and $aa		;1010 1010
	and $aa
	or b

	
	
		ld (de),a
	.pon_buffer_der_2X
	
;*************************************************	

		
		INC DE
		INC HL

		dec ixl
		JP NZ,loop_ancho_map_sbufferX

	   dec ixh
	   ret z
	   EX DE,HL
	   ld b,0

		add HL,BC	
		jp loop_alto_map_sbufferX