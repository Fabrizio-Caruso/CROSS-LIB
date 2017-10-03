; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007           **
; ******************************************************

GLOBAL cpc_WyzTestPlayer

GLOBAL cpc_WyzPlayer
GLOBAL INTERRUPCION

.cpc_WyzTestPlayer
	LD HL,INTERRUPCION+cpc_WyzPlayer
	LD A,(HL)
	LD L,A
	LD H,0
	RET