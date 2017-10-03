; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_SpRLM1
;la entrada es un sprite con los dos primeros bytes indicando las dimensiones


.cpc_SpRLM1


;ld de,sprite
ld b,(hl)
;ld b,a	;ancho
inc hl
ld c,(hl)	;alto
inc hl		;apunta a datos sprite
xor a
.mult0	;repite el ancho
add c
djnz mult0

sla a
ld b,a


;; de = start of line to scroll

;ld b,3*16*2	;voy a hacer esto en todos los bytes del sprite
ld d,h
ld e,l

inc hl			;usando máscaras
.scroll_line_0
inc hl

ld a,(hl)
and $88
rrca
rrca
rrca
ld c,a
ld a,(de)
and $77
add a
or c
ld (de),a
inc de
djnz scroll_line_0
dec de
and @11101110
ld (de),a
ret

