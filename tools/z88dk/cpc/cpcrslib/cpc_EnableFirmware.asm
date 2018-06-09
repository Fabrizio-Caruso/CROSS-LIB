; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2009       **
; ******************************************************


GLOBAL cpc_EnableFirmware		
GLOBAL cpc_DisableFirmware
GLOBAL backup

.cpc_EnableFirmware
	DI
	LD de,(backup)
				
	LD HL,$0038
	LD (hl),e		;EI
	inc hl
	LD (hl),d	;RET
	EI
	RET

