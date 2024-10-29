	.rodata
	.export	_sfx_spaceinvshoot
_sfx_spaceinvshoot:
	.byte   $10,$10,$03 ; version, priority, frames per chunk
	.byte   $18,$08,$08 ; first chunk of freq,channel,volume data 
	.byte   $19,$08,$05
	.byte   $19,$08,$05
	.byte   $19,$08,$05
	.byte   $19,$08,$05
	.byte   $1C,$08,$02
	.byte   $1C,$08,$02
	.byte   $1C,$08,$02
	.byte   $1C,$08,$02
	.byte   $1C,$08,$02
	.byte   $1E,$08,$01
	.byte   $1E,$08,$01
	.byte   $1E,$08,$01
	.byte   $1E,$08,$01
	.byte   $1E,$08,$01
	.byte   $00,$00,$00
