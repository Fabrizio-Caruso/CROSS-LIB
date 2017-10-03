; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2009       **
; ******************************************************

GLOBAL cpc_TestKeyF

GLOBAL cpc_KeysData
GLOBAL keymap
GLOBAL tabla_teclas


.cpc_TestKeyF
	SLA L
	inc l
	ld h,0
	ld de,cpc_KeysData+tabla_teclas
	add hl,de
	
	ld a,(HL)
	sub $40

	ex de,hl
	ld hl,cpc_KeysData+keymap	;; lee la línea buscada del keymap
	ld c,a
	ld b,0
	add hl,bc
	ld a,(hl)
	ex de,hl
	

	DEC hl						; pero sólo nos interesa una de las teclas.
	and (HL) ;para filtrar por el bit de la tecla (puede haber varias pulsadas)
	CP (hl)	;comprueba si el byte coincide
	ld h,0
	jp nz,pulsado
	ld l,h
	ret
.pulsado
	ld l,1
	ret
	
	