; Startup code for cc65/ca65

	.import _main
	.export __STARTUP__:absolute=1

; Linker generated symbols
	.import __STACK_START__, __STACKSIZE__
    .include "zeropage.inc"
	.import initlib, copydata


.segment "ZEROPAGE"


;no variables yet


.segment "HEADER"

    .byte $4e,$45,$53,$1a
	.byte 01
	.byte 01
	.byte 00
	.byte 00
	.res 8,0



.segment "STARTUP"

start:
	sei
	cld
	ldx #$40
	stx $4017
	ldx #$ff
	txs
	inx
	stx $2000
	stx $2001
	stx $4010
:
	lda $2002
	bpl :-
	lda #$00
Blankram:			;puts zero in all CPU RAM
	sta $00, x
	sta $0100, x
	sta $0200, x
	sta $0300, x
	sta $0400, x
	sta $0500, x
	sta $0600, x
	sta $0700, x
	inx
	bne Blankram
	
:
	lda $2002
	bpl :-

Isprites:
	jsr Blanksprite
	lda #$00		;pushes all sprites from 200-2ff
	sta $2003		;to the sprite memory
	lda #$02
	sta $4014
	
	jsr ClearNT		;puts zero in all PPU RAM

MusicInit:			;turns music channels off
	lda #0
	sta $4015
	
	lda #<(__STACK_START__+__STACKSIZE__)
    sta	sp
    lda	#>(__STACK_START__+__STACKSIZE__)
    sta	sp+1            ; Set the c stack pointer
	
	jsr	copydata
	jsr	initlib
	
	lda $2002		;reset the 'latch'
	jmp _main		;jumps to main in c code


	
	

	
_Blanksprite:
Blanksprite:
	ldy #$40
	ldx #$00
	lda #$f8
Blanksprite2:		;puts all sprites off screen
	sta $0200, x
	inx
	inx
	inx
	inx
	dey
	bne Blanksprite2
	rts
	


_ClearNT:
ClearNT:
	lda $2002
	lda #$20
	sta $2006
	lda #$00
	sta $2006
	lda #$00	;tile 00 is blank
	ldy #$10
	ldx #$00
BlankName:		;blanks screen
	sta $2007
	dex
	bne BlankName
	dey
	bne BlankName
	rts







nmi:
irq:
    rti

.segment "RODATA"

;none yet

.segment "VECTORS"

    .word nmi	;$fffa vblank nmi
    .word start	;$fffc reset
   	.word irq	;$fffe irq / brk


.segment "CHARS"

	.incbin "Alpha.chr"
