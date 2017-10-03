; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_TouchTileSpXY

GLOBAL tiles
GLOBAL tiles_tocados


.cpc_TouchTileSpXY
;entran las coordenadas x,y y se marca el tile para redibujarlo en pantalla
;la coordenada x se divide entre 4 y da el tile
;la coordenada y se divide entre 16 y da el tile

	ld hl,2
    add hl,sp			; ¿Es la forma de pasar parámetros? ¿Se pasan en SP+2? ¿en la pila?			
	ld d,(hl)
	inc hl
	inc hl
	ld e,(hl)
	
	;E tiene X
	;d tiene Y
	
	LD A,E
	SRL A
	SRL A
	LD E,A	;E tiene el bloque en horizontal
	
	LD A,D
	SRL A
	SRL A
	SRL A
	SRL A
	LD D,A	;D tiene el bloque en vertical
	
	;la posición del bloque se calcula8 18*y+x
	
;	SLA A
;	LD D,A
;	SLA A
;	SLA A
;	SLA A
;	ADD D
;	ADD E	;A apunta a la posición
;	LD E,A
	
	LD HL,tiles_tocados
	;incorporo el tile en su sitio, guardo x e y
.bucle_recorrido_tiles_tocados
	LD A,(HL)
	CP $FF	
	JP Z,incorporar_tile	;Solo se incorpora al llegar a un hueco
	CP E
	JP Z, comprobar_segundo_byte	
	INC HL
	INC HL
	JP bucle_recorrido_tiles_tocados
.comprobar_segundo_byte
	INC HL
	LD A,(HL)
	CP D
	RET Z	;los dos bytes son iguales, es el mismo tile. No se añade
	INC HL
	JP bucle_recorrido_tiles_tocados
.incorporar_tile
	LD (HL),E	
	INC HL
	LD (HL),D
	INC HL
	RET