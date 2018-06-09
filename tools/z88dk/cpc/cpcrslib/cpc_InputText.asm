; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_InputText		;marca un tile indicando las coordenadas del tile

GLOBAL cpc_TestKeyboard


.cpc_InputText	


.ejecutar_deteccion_teclado
	ld b,10
	
.bucle_deteccion_teclado
	ld a,b
	cp 10
	jr nz, continua_deteccion_x
	xor a
	ld b,a
	;ld b,0
	;ld a,b
.continua_deteccion_x
	push bc
	;ld (cpc_TestKeyboard+linea_a_buscar+1),a
	add $40

	call cpc_TestKeyboard	;en A vuelve los valores de la linea

;_A_devuelto
	pop bc
	;cp 0 ;0 no se ha pulsado tecla...
	or a
	jr nz,key_pressed	;no se ha pulsado nada??
; A tiene el byte (<>0)
; B tiene la linea
	

	;ret
.continua_deteccion2

;	ret nz	;vuelve si las teclas son las anteriores a=d

	inc b		
	jr bucle_deteccion_teclado
	ld L,0
	ld h,l
;_A_devuelto
.key_pressed
;a tiene columna
;b tiene la fila
	ret
	;cp 0 ;0 no se ha pulsado tecla...
	
	


	LD HL,keyboard_table
	;incorporo el tile en su sitio, guardo x e y
.bucle_keyboard_table
	LD A,(HL)
	CP $FF	
	JP Z,tecla_invalida	; No es una tecla válida
	CP E
	JP Z, comprobar_segundo_byte	
	INC HL
	INC HL
	INC HL	;son 3 bytes por cada tecla
	JP bucle_keyboard_table
.comprobar_segundo_byte
	INC HL
	LD A,(HL)
	CP D
	JP Z, tecla_valida	;los dos bytes son iguales, es el mismo tile. No se añade
	INC HL
	INC HL
	JP bucle_keyboard_table

.tecla_valida
	INC HL
	LD A,(HL)	;mira ASCII
	RET	
.tecla_invalida
	LD A,$FF
	RET	
		
.keyboard_table	
defb 20,8,'A'
defb 40,6,'B'
defb 40,7,'C'
defb 20,7,'D'
defb 4,7,'E'
defb 20,6,'F'
defb 10,6,'G'
defb 10,5,'H'
defb 8,4,'I'
defb 20,5,'J'
defb 20,4,'K'
defb 19,4,'L'
defb 40,4,'M'
defb 40,5,'N'
defb ,-40,'Ñ'
defb 4,4,'O'
defb 8,3,'P'
defb 8,8,'Q'
defb 4,6,'R'
defb 10,7,'S'
defb 8,6,'T'
defb 4,5,'U'
defb 80,6,'V'
defb 8,7,'W'
defb 80,7,'X'
defb 8,5,'Y'
defb 80,8,'Z'
defb 1,4,'0'
defb 1,8,'1'
defb 2,8,'2'
defb 2,7,'3'
defb 1,7,'4'
defb 2,6,'5'
defb 1,6,'6'
defb 2,5,'7'
defb 1,5,'8'
defb 2,4,'9'
defb 80,5,'SPC'
defb 4,2,'ENTER'
defb 4,8,'ESC'

defb $ff	