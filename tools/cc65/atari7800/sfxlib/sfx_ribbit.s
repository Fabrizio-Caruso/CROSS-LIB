	.rodata
	.export	_sfx_ribbit
_sfx_ribbit:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $0c,$06,$04 ; first chunk of freq,channel,volume
	.byte  $19,$06,$0f
	.byte  $19,$06,$0f
	.byte  $19,$06,$0f
	.byte  $0c,$06,$09
	.byte  $19,$06,$0f
	.byte  $19,$06,$0f
	.byte  $08,$06,$06
	.byte  $19,$06,$0f
	.byte  $19,$06,$0f
	.byte  $19,$06,$0f
	.byte  $19,$06,$0f
	.byte  $08,$06,$07
	.byte  $19,$06,$0f
	.byte  $19,$06,$0d
	.byte  $19,$06,$0f
	.byte  $19,$06,$0c
	.byte  $19,$06,$0d
	.byte  $19,$06,$05
	.byte  $19,$06,$09
	.byte  $19,$06,$03
	.byte  $19,$06,$04
	.byte  $00,$00,$00
