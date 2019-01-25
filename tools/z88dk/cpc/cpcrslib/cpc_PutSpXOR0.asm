; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_PutSpXOR0
GLOBAL anchox0 
GLOBAL suma_siguiente_lineax0 

		
.cpc_PutSpXOR0
		defb $fD
   		LD H,a		;ALTO, SE PUEDE TRABAJAR CON HX DIRECTAMENTE
		ld b,7
	.loop_alto_2x
	   ld c,4
	.loop_ancho_2x		
		ld A,(DE)
		XOR (HL)
		ld (hl),a
		inc de
		inc hl
		dec c
		jp nz,loop_ancho_2x
	   defb $fD
	   dec H
	   ret z

	   	   
.salto_lineax
		LD C,$ff			;&07f6 			;salto linea menos ancho
		ADD HL,BC
		jp nc,loop_alto_2x ;sig_linea_2zz		;si no desborda va a la siguiente linea
		ld bc,$c050
		
		add HL,BC
		ld b,7			;sólo se daría una de cada 8 veces en un sprite
		jp loop_alto_2x	


				
DEFC anchox0 = loop_alto_2x
DEFC suma_siguiente_lineax0 = salto_lineax