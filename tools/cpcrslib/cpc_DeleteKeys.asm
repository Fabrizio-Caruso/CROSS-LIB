; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_DeleteKeys

;borra la tabla de las teclas para poder redefinirlas todas

GLOBAL cpc_KeysData
GLOBAL  tabla_teclas

.cpc_DeleteKeys
LD HL,cpc_KeysData+tabla_teclas
LD DE,cpc_KeysData+tabla_teclas+1
LD BC, 24
LD (HL),$FF
LDIR
RET
