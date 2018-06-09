; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_SpUpdY			;Actualiza X y coord según el incremento deseado y el signo
						;incrementos relativos





GLOBAL ancho_pantalla_bytes 


.cpc_SpUpdY
;first parameter sprite 
;second parameter value
;first parameter sprite 
;second parameter value


  

	ld ix,2
	add ix,sp
	ld a,(ix+0)
	;ld ,(ix+1)
	;A=x value
	or a ;	cp 0
	ret z	;si inc=0
	
		
	;ld l,(ix+2)
	;ld h,(ix+3)
		
    ;push hl
    ;pop ix
    
	ld e,(ix+2)
	ld d,(ix+3)		;sprite
		

  ;  push de		;10
  ;  pop ix		;14
    LD IXH,d	;9
    LD IXL,e	;9    

	
	ld bc,ancho_pantalla_bytes
	
    bit 7,a	;check if negative number
    jp nz, negative
  ;ix tiene el valor del sprite
  ;incrementar x
  ld l,a
  ld h,0
  ;ld b,(ix+9)			;19
  ;add b				;4
  add a,(ix+9)			;19
  ld (ix+9),a ;Y updated
  
  ld a,l
  
  ld l,(ix+4)
  ld h,(ix+5)
  
;  ld bc,ancho_pantalla_bytes
  ;ld b,0
.bucle_suma_y
	add hl,bc
	dec a
	jp nz,bucle_suma_y


  ld (ix+4),l
  ld (ix+5),h ;superbufer coord updated
  ret
  
  
  
.negative
  NEG A
  ld e,a
  ld d,0
  ld a,(ix+9)
  sub e
  ;sub (ix+9)
  ld (ix+9),a ;Y updated
  
  ld a,e
  
  ld l,(ix+4)
  ld h,(ix+5)
 
  ;ld c,ancho_pantalla_bytes
  ;ld b,0
.bucle_resta_y
	sbc hl,bc
	dec a
	jp nz,bucle_resta_y

  ld (ix+4),l
  ld (ix+5),h ;superbufer coord updated
  ret  
  
