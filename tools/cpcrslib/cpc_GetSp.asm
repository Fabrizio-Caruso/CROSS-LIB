; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_GetSp

GLOBAL cpc_GetSp0
GLOBAL suma_siguiente_lineax
GLOBAL anchox


.cpc_GetSp	
	ld ix,2
	add ix,sp
	ld l,(ix+0)
	ld h,(ix+1)
	ld a,(ix+2)
	
   	ld e,(ix+6)
	ld d,(ix+7)
	
	;	defb $DD
   	;	LD H,c		;ALTO, SE PUEDE TRABAJAR CON HX DIRECTAMENTE
   		;defb $DD
   		;LD L,b      ;LD LX,B   ;ANCHO SPRITE
	ld (cpc_GetSp0+anchox+1),a
	
	
	sub 1
	cpl
	ld (cpc_GetSp0+suma_siguiente_lineax+1),a    ;comparten los 2 los mismos valores.		
	
	ld A,(ix+4)  
	jp cpc_GetSp0