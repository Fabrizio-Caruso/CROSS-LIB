; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************
GLOBAL cpc_PutSprite	
;*************************************
; SPRITE ROUTINE WITHOUT TRANSPARENCY
; Supplied by Tim Riemann
; from a German forum
; DE = source address of the sprite
;      (includes header with 1B width [64byte maximum!], 1B height)
; HL = destination address
;*************************************
.cpc_PutSprite

pop af
pop de	;destination address
pop hl	;sprite data	
push af



        ;ex de,hl
        ld a,64
        sub (hl)
        add a
        ld (width1+1),a
        xor a
        sub (hl)
        ld (width2+1),a
        inc hl
        ld a,(hl)
        inc hl 
		
.width0 
		;ex de,hl 
.width1
		jr width1 ;cada LDI es un byte 
;se hace el salto al byte correspondiente (64-ancho) 
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        
                ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        
                ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        
                ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        
                ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        
                ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        ldi
        
        ldi
        ldi
        ldi
        ldi
  
.width2
   ld bc,$700 
   ex de,hl 
        add hl,bc 
        jp nc,width3 
        ld bc,$c050 
        add hl,bc 
  .width3
  	ex de,hl
    dec a 
    jp nz, width1
    
   ret ; z  
        
        