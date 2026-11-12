.include "lynx.inc"
.export _chibisound
.importzp	sp

.code

.proc _chibisound			; %NVPPPPPP	N=Noise  V=Volume  P=Pitch
	cmp #0
	beq silent

	pha
	and #%01000000			; Mask for volume		
	ora #%00111111
	sta AUD0VOL   			; Volume (127=max 0=min)	
	pla 
	
	pha
	and #%00111111			; Mask for pitch
	sta AUD0BKUP			; Effectively frequency
	pla						
	
	and #%10000000			; Noise Bit
	beq nonoise
	lda #%11110000			; 15 = Noise
	jmp noiseset
nonoise:	
	lda #%00010000			; 1 = Good tone
noiseset:
	sta AUD0FEED			; FFFFFFFF	Effective sound of instrument
	
	stz AUD0SHIFT			; SSSSSSSS	Shift Register L
	stz AUD0CTLB			; SSSS-CBB	S=Shift Register H, 
										;C=Clock state B=Borrow
	stz MSTEREO				; LLLLRRRR	LR Vol - 0=all on 255=all off

	lda #$80				; Silent
	sta AUD0OUT				; Reset Audio Output Value 
	
	lda #%00011110 			; FTIRCKKK	F=Feedback bit 7 , reset Timer done, 
							; enable Integrate, enable Reload enable Count,
							; clock select
	sta AUD0CTLA			; Audio Control bits
	rts
	
silent:
	stz AUD0VOL				; Volume (127=max 0=min)
	rts

.endproc