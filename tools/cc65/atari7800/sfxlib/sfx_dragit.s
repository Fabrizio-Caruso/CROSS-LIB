	.rodata
	.export	_sfx_dragit
_sfx_dragit:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $1c,$07,$02 ; first chunk of freq,channel,volume
	.byte  $02,$07,$06
	.byte  $1e,$08,$09
	.byte  $11,$08,$09
	.byte  $1e,$08,$09
	.byte  $11,$07,$06
	.byte  $1e,$08,$09
	.byte  $11,$07,$06
	.byte  $0e,$07,$03
	.byte  $06,$07,$02
	.byte  $00,$00,$00
