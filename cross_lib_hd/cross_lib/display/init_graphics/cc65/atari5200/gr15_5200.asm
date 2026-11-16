	.import __SCREEN_RUN__, __SCR_SIZE__
	.import __DLIST_LOAD__, __DLIST_SIZE__, __DLIST_RUN__

; Taken from atari5200.inc
SDLSTL  =  $05     ; DLISTL Shadow
SDLSTH  =  $06     ; DLISTH  "

	.export  __CART_NAME__: absolute = 1
	.macpack atari
	.segment "CARTNAME"
		scrcode " TEXT DEMONSTRATION "

	.export  __CART_YEAR__: absolute = 1
	.segment "CARTYEAR"
		.byte   '9' + 32,'9' + 32       ; "99", using playfield 1

	.segment "SCREEN"

	.segment "DLIST"
DlistGr15:
	.byte $70, $70, $70
	.byte $4E, <__SCREEN_RUN__, >__SCREEN_RUN__
	.res 94-1, $0E
	.byte $4E, $00, >((__SCREEN_RUN__+$1000)&$3000)
	.res 98-1, $0E
	.byte $41, <DlistGr15, >DlistGr15
	
	.code
	
	.importzp ptr1
	.import pusha, incsp1
	.export __graphics
__graphics:
	JSR pusha
	TXA
	PHA
	TYA
	PHA
	LDX #0
@0:	LDA __DLIST_LOAD__,X
	STA __DLIST_RUN__,X
	INX
	CPX #<__DLIST_SIZE__
	BNE @0
	LDA #<__SCREEN_RUN__
	LDX #>__SCREEN_RUN__
	STA ptr1
	STX ptr1+1
	LDX #>__SCR_SIZE__
	LDY #0
	TYA
@1:	STA (ptr1),Y
	INY
	BNE @1
	INC ptr1+1
	DEX
	BNE @1
	LDA #<DlistGr15
	LDX #>DlistGr15
	STA SDLSTL
	STX SDLSTH
	PLA
	TAY
	PLA
	TAX
	JMP incsp1
