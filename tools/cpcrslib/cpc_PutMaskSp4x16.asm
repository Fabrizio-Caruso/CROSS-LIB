; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_PutMaskSp4x16

.cpc_PutMaskSp4x16

	ld ix,2
	add ix,sp


	ld l,(ix+0)
	ld h,(ix+1)
		
	ld e,(ix+2)
	ld d,(ix+3)
	
		;ld a,16		;alto
		defb $fD
   		LD H,16		;ALTO, SE PUEDE TRABAJAR CON HX DIRECTAMENTE
		ld b,7
	.loop_alto_mask_4x16
	.loop_ancho_mask_4x16
		EX DE,HL
		LD A,(DE)	;leo el byte del fondo
		AND (HL)	;lo enmascaro		
		INC HL
		OR (HL)		;lo enmascaro 
		LD (DE),A	;actualizo el fondo
		INC DE
		INC HL
;COMO SOLO SON 4 BYTES, es más rápido y económico desplegar la rutina

		LD A,(DE)	;leo el byte del fondo
		AND (HL)	;lo enmascaro		
		INC HL
		OR (HL)		;lo enmascaro 
		LD (DE),A	;actualizo el fondo
		INC DE
		INC HL

		LD A,(DE)	;leo el byte del fondo
		AND (HL)	;lo enmascaro		
		INC HL
		OR (HL)		;lo enmascaro 
		LD (DE),A	;actualizo el fondo
		INC DE
		INC HL

		LD A,(DE)	;leo el byte del fondo
		AND (HL)	;lo enmascaro		
		INC HL
		OR (HL)		;lo enmascaro 
		LD (DE),A	;actualizo el fondo
		INC DE
		INC HL

	   defb $fD
	   dec H
	   ret z
	   EX DE,HL
.suma_siguiente_linea_mask_4x16		;
		LD C,$fc			;$07f6 			;salto linea menos ancho
		ADD HL,BC
		jp nc,loop_alto_mask_4x16 ;sig_linea_2zz		;si no desborda va a la siguiente linea
		ld bc,$c050
		add HL,BC
		ld b,7			;sólo se daría una de cada 8 veces en un sprite
		jp loop_alto_mask_4x16
