; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2009       **
; ******************************************************

GLOBAL cpc_ScrollRight





GLOBAL tiles
GLOBAL ancho_pantalla_bytes 
GLOBAL ColumnScr
GLOBAL alto_pantalla_bytes
GLOBAL pantalla_juego
GLOBAL posiciones_pantalla
GLOBAL posicion_inicial_superbuffer


.cpc_ScrollRight

;;scrollea el superbuffer
ld hl,posicion_inicial_superbuffer+alto_pantalla_bytes*ancho_pantalla_bytes-2 ; pantalla_juego+alto_pantalla_bytes*ancho_pantalla_bytes/16-1
ld de,posicion_inicial_superbuffer+alto_pantalla_bytes*ancho_pantalla_bytes ;pantalla_juego+alto_pantalla_bytes*ancho_pantalla_bytes/16
ld bc,alto_pantalla_bytes*ancho_pantalla_bytes-1 ;-1
LDDR
RET
xor a
ld (ntile),a
add 8
ld (contador),a
ld ix,ColumnScr
ld a,alto_pantalla_bytes
.aaa
push af

ld h,(ix+1)
ld l,(ix+0)

;Se suma el ancho de pantalla a hl
ld bc,ancho_pantalla_bytes-1
add hl,bc
;dec l
ld e,l
ld d,h


dec l
dec l

ld bc,ancho_pantalla_bytes-2
;dec bc
;dec bc
lddr
ld a,(contador)
dec a
call z,tocatile
ld (contador),a

pop af
dec a
inc ix
inc ix
jp nz,aaa



;ld hl,pantalla_juego+alto_pantalla_bytes*ancho_pantalla_bytes/16-1
;ld de,pantalla_juego+alto_pantalla_bytes*ancho_pantalla_bytes/16
;ld bc,alto_pantalla_bytes*ancho_pantalla_bytes/16 ;-1
;LDDR


ret


.tocatile

;(a+1)*30-1

ld de,$3800-1
sbc hl,de
;ld hl,pantalla_juego
;dec hl
;dec hl
;ld bc,$50
;sbc hl,bc

;ld bc,14256+$50+2
;sbc hl,bc
push hl
;Se selecciona el tile correspondiente
ld a,(ntile) ;(a)*30
ld e,a
inc a
ld (ntile),a
call calcula_entradaPantalla
;En HL está el tile a dibujar
;HL * ntile*16
add hl,hl
add hl,hl
add hl,hl
add hl,hl
ld de,tiles
add hl,de
ex de,hl
pop hl

call cpc_PutTile2x8b

ld a,8
ret

.contador  defb 0
.ntile defb 0



.calcula_entradaPantalla

include "multiplication2.asm"


	    ;dec hl
		ld de,pantalla_juego
		add hl,de
		ld l,(HL)
		ld h,0
	    ret

.cpc_PutTile2x8b ;siempre se dibujan en posicion caracter así que no hay saltos raros en HL, se puede quitar la última parte
;hl screen address
;de data

		defb $fD
   		LD H,8		;ALTO, SE PUEDE TRABAJAR CON HX DIRECTAMENTE
		ld b,7
		
	.loop_alto_tile_2x8
	.loop_ancho_tile_2x8		
		ld A,(DE)
		ld (hl),a
		inc de
		inc hl
		
		ld A,(DE)
		ld (hl),a
		inc de
;		inc hl

	   defb $fD
	   dec H
	   ret z
	   
.suma_siguiente_linea0_tile_2x8		;
		LD C,$ff			;&07f6 			;salto linea menos ancho
		ADD HL,BC
		jp nc,loop_alto_tile_2x8 ;sig_linea_2zz		;si no desborda va a la siguiente linea
		ld bc,$c050
		
		add HL,BC
		ld b,7			;sólo se daría una de cada 8 veces en un sprite
		jp loop_alto_tile_2x8		    