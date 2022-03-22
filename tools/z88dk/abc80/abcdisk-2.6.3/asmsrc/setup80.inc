	org 0			; This code must be position-independent

	defc END=00C9h		; END routine in BASIC ROM
	defc RLINEo=0D71h	; Run line in BASIC ROM (11273)
	defc RLINEn=0D6Fh	; Run line in BASIC ROM (9913)
	defc CHAINPTR=0E64h	; Pointer to CHAIN
	defc BOFA=65052
	defc EOFA=65054
	defc ONERR=65076	; Target for ON ERROR GOTO

	; Encode explicitly as this has to be a contiguous BASIC statement
	defb 0xcb		; String expression
	defb '"'
	defb _enddata - _data	; Length of string data
_data:
