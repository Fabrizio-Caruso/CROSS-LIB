; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	                                              **
; ******************************************************

GLOBAL cpc_SetModo

.cpc_SetModo
  ld a,l
  call $bc0e
  RET