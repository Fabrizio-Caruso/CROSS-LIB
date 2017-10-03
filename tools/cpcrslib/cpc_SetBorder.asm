; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Augusto Ruiz, 	           2007       **
; ******************************************************

GLOBAL cpc_SetBorder

.cpc_SetBorder
  ;ld hl,2
  ;add hl,sp
  ;ld b,(hl)
  ld b,l
  ld c,b
  JP $bc38
;ret


