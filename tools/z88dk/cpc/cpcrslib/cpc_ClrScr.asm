; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2009       **
; ******************************************************


GLOBAL cpc_ClrScr		

.cpc_ClrScr
	XOR A
	LD HL,$c000
	LD DE,$c001
	LD BC,16383
	LD (HL),A
	LDIR
	RET

