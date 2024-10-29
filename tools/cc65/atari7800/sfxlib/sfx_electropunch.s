	.rodata
	.export	_sfx_electropunch
_sfx_electropunch:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $07,$06,$0f ; first chunk of freq,channel,volume
	.byte  $0f,$06,$08
	.byte  $1e,$04,$0a
	.byte  $0f,$06,$08
	.byte  $12,$04,$06
	.byte  $0f,$06,$05
	.byte  $0f,$06,$06
	.byte  $0c,$04,$03
	.byte  $07,$06,$01
	.byte  $0a,$04,$03
	.byte  $02,$0c,$01
	.byte  $00,$00,$00
