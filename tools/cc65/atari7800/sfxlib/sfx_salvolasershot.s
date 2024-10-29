	.rodata
	.export	_sfx_salvolasershot
_sfx_salvolasershot:
	.byte   $10,$18,$01 ; version, priority, frames per chunk
	.byte   $04,$08,$08 ; first chunk of freq,channel,volume data 
	.byte   $05,$08,$07
	.byte   $04,$08,$07
	.byte   $05,$08,$07
	.byte   $06,$08,$07
	.byte   $07,$08,$06
	.byte   $06,$08,$06
	.byte   $07,$08,$06
	.byte   $08,$08,$06
	.byte   $09,$08,$06
	.byte   $08,$08,$06
	.byte   $09,$08,$06
	.byte   $0a,$08,$04
	.byte   $09,$08,$04
	.byte   $0a,$08,$04
	.byte   $0b,$08,$04
	.byte   $0a,$08,$04
	.byte   $0b,$08,$04
	.byte   $0c,$08,$04
	.byte   $0b,$08,$02
	.byte   $0c,$08,$02
	.byte   $0d,$08,$02
	.byte   $00,$00,$00
