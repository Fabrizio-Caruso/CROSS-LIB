	.rodata
	.export	_sfx_poof1
_sfx_poof1:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $07,$08,$04 ; first chunk of freq,channel,volume
	.byte  $07,$08,$06
	.byte  $07,$08,$08
	.byte  $09,$08,$08
	.byte  $0c,$08,$0a
	.byte  $09,$08,$0a
	.byte  $0c,$08,$0a
	.byte  $09,$08,$0a
	.byte  $0d,$08,$08
	.byte  $0e,$08,$06
	.byte  $09,$08,$04
	.byte  $09,$08,$02
	.byte  $00,$00,$00
