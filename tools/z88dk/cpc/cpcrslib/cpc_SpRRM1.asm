; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_SpRRM1
;la entrada es un sprite con los dos primeros bytes indicando las dimensiones


.cpc_SpRRM1


;ld de,sprite
ld b,(hl)
;ld b,a	;ancho
inc hl
ld c,(hl)	;alto
xor a
inc hl		;apunta a datos sprite
.mult0	;repite el ancho
add c
djnz mult0
sla a

ld b,0
ld c,a
add hl,bc	;posiciona al final del sprite
;ld de,sprite_f ;como voy para atrás, me pongo en el último byte del sprite
ld b,c
;; de = start of line to scroll
.scroll_line3
;ld b,3*16*2	;ancho-1
ld d,h
ld e,l

dec hl
.scroll_line_02
dec hl
ld a,(hl)
and $11
rlca
rlca
rlca	;cojo solo los 2 bits que me interesan
ld c,a
ld a,(de)
and @11101110 ;está borrando el bit del pixel 1
srl a
or c
ld (de),a
dec de
djnz scroll_line_02
inc de
and @01110111
ld (de),a
ret