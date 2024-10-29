	.include "atari7800.inc"
	.include "extzp.inc"
	.interruptor playsfx
	.constructor silence
	.export	_playsfx0
	.import	_zonecounter

        .include "extzp.inc"

        .segment "EXTZP" : zeropage

ptr7800:        .res 2
ptrtmp:         .res 2
cursorzone:     .res 2
sfxptr:         .res 2

	.bss
frames:	.res	1
cnt:	.res	1

	.code

_playsfx0:
	sta	sfxptr
	stx	sfxptr+1
	inc	sfxptr
	bne	@L11
	inc	sfxptr+1
@L11:	inc	sfxptr
	bne	@L12
	inc	sfxptr+1
@L12:	ldy	#0
	lda	(sfxptr),y
	sta	frames
	tya
	sta	cnt
	inc	sfxptr
	bne	@L13
	inc	sfxptr+1
@L13:	rts

silence:
	lda	#0
	sta	sfxptr
	sta	sfxptr+1
	rts

playsfx:
	lda	_zonecounter
	and	#$01
	beq	@L9		; Process audio while MARIA draws on screen
	lda	sfxptr+1
	beq	@L9		; Skip audio if high byte of address is zero

	lda	cnt
	bne	@L1
	lda	frames
	sta	cnt
	jmp	@L2
@L1:
	sec
	sbc	#1
	sta	cnt
	jmp	@L9

@L2:	ldx	#0		; Use X to detect end of sfx
	ldy	#0		; freq
	lda	(sfxptr),y
	sta	AUDF0
	beq	@L3
	inx			; Not end of sfx
@L3:	iny			; Channel
	lda	(sfxptr),y
	sta	AUDC0
	beq	@L4
	inx			; Not end of sfx
@L4:	iny			; Volume
	lda	(sfxptr),y
	sta	AUDV0
	beq	@L5
	inx			; Not end of sfx
@L5:	txa			; Check for end of sfx
	beq	@L8

	lda	sfxptr		; Increment sfx pointer by 3
	ldx	sfxptr+1
	clc
	adc	#3
	bcc	@L6
	inx
@L6:	sta	sfxptr
	stx	sfxptr+1
	jmp	@L9
@L8:	sta	sfxptr+1	; Stop sfx processing
@L9:	clc
	rts
