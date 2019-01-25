; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	                                              **
; ******************************************************

GLOBAL cpc_SetMode

.cpc_SetMode
  ld a,l
  LD BC,$7F00          ;Gate array port
  LD D,@10001100	   ;Mode  and  rom  selection  (and Gate Array function)
  ADD D
  OUT (C),A  
  RET

 	 

	