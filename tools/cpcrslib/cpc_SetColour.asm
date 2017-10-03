; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007           **
; ******************************************************

GLOBAL cpc_SetColour

.cpc_SetColour		;El número de tinta 17 es el borde
    LD HL,2
    ADD HL,SP	
  	LD E,(HL)
    INC HL
  	INC HL
    LD A,(HL)
  	LD BC,$7F00                     ;Gate Array 
	OUT (C),A                       ;Número de tinta
	LD A,@01000000              	;Color (y Gate Array)
	ADD E
	OUT (C),A                       
	RET