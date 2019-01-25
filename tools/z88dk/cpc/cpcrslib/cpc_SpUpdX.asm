; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_SpUpdX			;Actualiza X y coord según el incremento deseado y el signo
						;incrementos relativos


.cpc_SpUpdX
;first parameter sprite 
;second parameter value

	ld ix,2
	add ix,sp
	ld a,(ix+0)		;value
	;ld ,(ix+1)
	;A=x value
	
	ld e,(ix+2)
	ld d,(ix+3)		;sprite
		

    push de		;10
    pop ix		;14
  
    ;LD IXH,d	;9
    ;LD IXL,e	;9
    
    
    
	ld h,0
    bit 7,a	;check if negative number
    jp z, negative1
    dec h
    .negative1
  ;ix tiene el valor del sprite
  ;incrementar x
  ld l,a
  ;ld h,0
  ;ld b,(ix+8)
  ;add b
  add a,(ix+8)
  ld (ix+8),a ;X updated
  ld e,(ix+4)
  ld d,(ix+5)
  add hl,de
  ld (ix+4),l
  ld (ix+5),h ;superbufer coord updated
  ret
  
  
  ;Decrapted
;.negative
;  NEG A	;pone positivo el número...
  
;  ld e,a
;  ld d,0
;  ld a,(ix+8)
;  sub e
;  ld (ix+8),a ;X updated
  
;  ld l,(ix+4)
;  ld h,(ix+5)
;  sbc hl,de
;  ld (ix+4),l
;  ld (ix+5),h ;superbufer coord updated
;  ret  
  
 