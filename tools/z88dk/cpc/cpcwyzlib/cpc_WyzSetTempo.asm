; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007           **
; ******************************************************

GLOBAL cpc_WyzSetTempo

GLOBAL cpc_WyzPlayer
GLOBAL direcc_tempo

.cpc_WyzSetTempo
	ld a,l
	ld (direcc_tempo+cpc_WyzPlayer+1),a
	ret
