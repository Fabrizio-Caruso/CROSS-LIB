; ******************************************************
; **       Librería de rutinas para Amstrad CPC       **
; **	   Raúl Simarro, 	  Artaburu 2007       **
; ******************************************************

GLOBAL cpc_CollSp			;Test if sprite1 and sprite2 are colliding

.cpc_CollSp
;first parameter sprite 
;second parameter value

	ld ix,2
	add ix,sp
	ld e,(ix+0)
	ld d,(ix+1)
	;A=x value
	ld l,(ix+2)
	ld h,(ix+3)
	push de
	pop iy	;ix sprite2 data
			
    push hl
    pop ix	;iy sprite1 data
    
    ;Sprite coords & sprite dims
				
;COLISION_sprites



;entran sprite1 y sprite 2 y se actualizan los datos
;ix apunta a sprite1
;iy apunta a sprite2

;coordenadas
	ld l,(ix+8)
	ld h,(ix+9)
	LD (SPR2X),HL
	
	ld l,(iy+8)
	ld h,(iy+9)
	LD (SPR1X),HL	

;dimensiones sprite 1
	ld l,(ix+0)
	ld h,(ix+1)
	ld b,(hl)
	inc hl
	ld c,(hl)
;dimensiones sprite 12
	ld l,(iy+0)
	ld h,(iy+1)
	ld d,(hl)
	inc hl
	ld e,(hl)	
	
	
	;ld e,(ix+6)
	;ld d,(ix+7)	
	
	

;ld de,DIMENSIONES_SP_PPAL	;dimensiones sprite 2
;ld bc,DIMENSIONES_SP_PPAL	;dimensiones sprite 1
CALL TOCADO
;RET NC ;vuelve si no hay colision
ld h,0
JP nc,no_colision
;Aquí hay colisión
ld l,1
RET		

defb 'm','m','o'
.no_colision
ld l,0
ret
	
.TOCADO
LD HL,SPR2X	
LD A,(SPR1X)
CP (HL)
jp C,C1
LD A,(HL)
ADD A,B	;alto del sprite1
LD B,A
LD A,(SPR1X)
SUB B
RET NC
jp COMPROBAR
.C1
ADD A,D	;alto sprite2
LD D,A
LD A,(HL)
SUB D
RET NC
.COMPROBAR
INC HL
LD A,(SPR1Y)
CP (HL)
jp C,C2
LD A,(HL)
ADD A,C
LD C,A
LD A,(SPR1Y)
SUB C
RET
.C2
ADD A,E
LD E,A
LD A,(HL)
SUB E
RET

.SPR1X defb 0
.SPR1Y defb 0
.SPR2X defb 0
.SPR2Y defb 0

	
