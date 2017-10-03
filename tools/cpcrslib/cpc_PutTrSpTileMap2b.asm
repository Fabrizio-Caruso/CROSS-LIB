; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_PutTrSpTileMap2b

GLOBAL tiles_tocados
GLOBAL pantalla_juego					;datos de la pantalla, cada byte indica un tile
GLOBAL posiciones_super_buffer
GLOBAL tiles
GLOBAL ancho_pantalla_bytes 
GLOBAL posicion_inicial_superbuffer

GLOBAL mascara1
GLOBAL mascara2

.cpc_PutTrSpTileMap2b

;según las coordenadas x,y que tenga el sprite, se dibuja en el buffer
   
    ex de,hl
    
  
    LD IXH,d	;9
    LD IXL,e	;9 
    
;_______________________________________________________
;____ GENERIC MULTIPLICATION ___________________________      
  ;lo cambio para la rutina de multiplicar 
    ld a,(ix+8)			;x
    ld e,(ix+9)			;y
;hl = ExH

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
	
    ;con el alto del sprite hago las actualizaciones necesarias a la rutina
    ld a,(de)
    ld (loop_alto_map_sbuffer+2),a
    ld b,a
    ld a,ancho_pantalla_bytes
    sub b
    ld (ancho_2+1),a
	inc de
	ld a,(de)
	inc de
	
	;ld a,16		;necesito el alto del sprite
	
	
	
.sp_buffer_mask
	ld b,0
.ancho_2	
	ld c,ancho_pantalla_bytes-4 ;60	;;DEPENDE DEL ANCHO

	;defb $fD
	;LD H,A		;ALTO, SE PUEDE TRABAJAR CON HX DIRECTAMENTE
	ld ixh,a
.loop_alto_map_sbuffer
		;defb $fD
		;LD L,4		;ANCHO, SE PUEDE TRABAJAR CON HX DIRECTAMENTE
		ld ixl,4
		ex de,hl
.loop_ancho_map_sbuffer		

;**********************************************

;push bc
;ex de,hl
	ld A,(hl)
		
	and $aa
	cp mascara2
	jp z,sig_pixn_der_2
	ld b,a ;B es el único registro libre
	ld a,(de) ;pixel actual donde pinto
	and $55		;0101 0101
	;and mascara1
	or b
	ld (de),a ;y lo pone en pantalla
.sig_pixn_der_2
	ld a,(hl) ;pixel del sprite
	;and $55   ;0101 0101
	and $55
	cp mascara1
	jp z,pon_buffer_der_2
	ld b,a ;B es el único registro libre
	ld a,(de) ;pixel actual donde pinto
	;and $aa		;1010 1010
	and $aa
	or b

	
	
		ld (de),a
	.pon_buffer_der_2
	
;*************************************************	

		
		INC DE
		INC HL
	;ex de,hl	
;pop bc		

		dec ixl
		JP NZ,loop_ancho_map_sbuffer

	   dec ixh
	   ret z
	   EX DE,HL
	   ld b,0

		add HL,BC	
		jp loop_alto_map_sbuffer
		
		
		
		
		
		
		