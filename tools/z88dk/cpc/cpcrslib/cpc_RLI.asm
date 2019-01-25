; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2009       **
; ******************************************************


GLOBAL cpc_RLI		;rota las líneas que se le digan hacia la izq y mete lo rotado por la derecha.


.cpc_RLI	
	ld ix,2
	add ix,sp
	
	ld l,(ix+4)	
	ld h,(ix+5)	;posición inicial
	
	ld a,(ix+2)	;lineas
	ld (_alto+1),a
	ld a,(ix+0)	;ancho
	ld (_ancho+1),a
	
	
dec hl	
;ld hl,$c001				;; parametro
._alto
ld a,8					;; parametro
.ciclo0
push af
push hl
;;
inc hl
ld a,(hl)
;;
ld d,h
ld e,l
dec hl
ld b, 0
._ancho
ld c,50	; parametro
lddr

inc hl

ld (hl),a

pop hl

pop af
dec a

ret z
;jp nz, ciclo0


;salto de línea, ojo salto caracter.
ld bc,$800
add hl,bc


jp nc,ciclo0 ;sig_linea_2zz		;si no desborda va a la siguiente linea
ld bc,$c050
add HL,BC
jp ciclo0
		
;.cont		
;pop af
;dec a
;jp nz, ciclo0


;ret

