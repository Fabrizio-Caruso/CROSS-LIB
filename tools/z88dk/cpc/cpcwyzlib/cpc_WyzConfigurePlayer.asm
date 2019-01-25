; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007           **
; ******************************************************

GLOBAL cpc_WyzConfigurePlayer

GLOBAL cpc_WyzPlayer
GLOBAL INTERRUPCION

.cpc_WyzConfigurePlayer
	;LD HL,2
	;ADD HL,SP
	;LD A,(HL)
	LD a,l
	;LD HL,INTERRUPCION+cpc_WyzPlayer
	;AND (HL)
	LD (INTERRUPCION+cpc_WyzPlayer),A
	RET
