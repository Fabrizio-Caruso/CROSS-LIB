; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2009       **
; ******************************************************


GLOBAL cpc_DisableFirmware		
GLOBAL backup

.cpc_DisableFirmware
	DI
	LD HL,($0038)
	LD (backup),HL				
	LD HL,$0038
	LD (hl),$FB		;EI
	inc hl
	LD (hl),$C9	;RET
	EI
	RET

.backup defw  0