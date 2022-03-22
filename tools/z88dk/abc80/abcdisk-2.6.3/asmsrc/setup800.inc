;
; Common setup code for ABC800
;

	defc BOFA=65286
	defc EOFA=65288
	defc HEAP=65290
	defc BOTM=65292
	defc TOPM=65294
	defc TOPP=65296

	defc HLBOFA=003Bh	; LD HL,BOFA
	defc IXBOFA=003Fh	; LD IX,BOFA
	defc HLPROG=0044h	; LD HL,BOFA+20 (after header)

	;; First, the ABC800 BAC file header
_header:
	defb 8Fh		; BASIC II
	defb 0
	defb 21h		; Program flags (meaning unclear)
	defb 0
PRGSZ:	defw _end + 7		; Adjust to total file length (7 = trailer)
	defw 0			; Scalar variable area size
	defw 0			; Address for scalar variable area
	defw 0			; Variable count
COMSZ:	defw 0			; COMMON area size (memory reserved @ BOTM)
	defw 0			; COMMON declarations checksum
	defw 0			; Address for the first DEF statement
	defw 0			; Address for the first DATA statement
