; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_AssignKey


GLOBAL cpc_KeysData
GLOBAL  tabla_teclas

.cpc_AssignKey

	ld hl,2
    add hl,sp	
    
    ld a,(hl)	;linea, byte
    inc hl
    ld b,(hl)	;DE tiene el valor de la tecla a escribir en la tabla
    inc hl
    ld e,(hl)
; En A se tiene el valor de la tecla seleccionada a comprobar [0..11]
	;A*2
;______________________________________________________________________________________________
;	;En A viene la tecla a redefinir (0..11)
	;srl	e 	;A*2	;
	sla e
	;ld e,a
	ld d,0
	ld hl,tabla_teclas+cpc_KeysData

; gracias a Mochilote por detectar un error aquí:	
	add hl,de ;Nos colocamos en la tecla a redefinir y la borramos
	ld (hl),$ff
	inc hl
	ld (hl),$ff
	dec hl
	push hl
	;call ejecutar_deteccion_teclado ;A tiene el valor del teclado
	;ld a,d
; A tiene el byte (<>0)
; B tiene la linea	
	;guardo linea y byte
	pop hl
	;ld a,(linea)
	ld (hl),a ;byte
	inc hl
	;ld a,b	  ;linea
	;ld a,(bte)
	ld (hl),b
	ret
