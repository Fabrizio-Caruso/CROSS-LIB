	.rodata
	.export	_sfx_echo1
_sfx_echo1:
	.byte   $10,$08,$08 ; version, priority, frames per chunk
	.byte   $18,$06,$0a ; first chunk of freq,channel,volume data 
	.byte   $08,$06,$0a
	.byte   $01,$00,$00 
	.byte   $18,$06,$05
	.byte   $08,$06,$05
	.byte   $01,$00,$00 
	.byte   $18,$06,$02
	.byte   $08,$06,$02
	.byte   $00,$00,$00 
