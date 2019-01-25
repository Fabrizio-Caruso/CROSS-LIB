; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Augusto Ruiz, 	           2007       **
; ******************************************************

GLOBAL cpc_SetInk

.cpc_SetInk
  ld hl,2
  add hl,sp	
  ld b,(hl)
  ld c,b
  inc hl
  inc hl
  ld a,(hl)
  call $bc32
ret