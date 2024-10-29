	.rodata
	.export	_sfx_jump2
_sfx_jump2:
	.byte  $10,$10,$00 ; version, priority, frames per chunk
	.byte  $10,$0c,$0c ; first chunk of freq,channel,volume
	.byte  $10,$0c,$0f
	.byte  $0f,$0c,$0f
	.byte  $0f,$0c,$0f
	.byte  $00,$06,$07
	.byte  $0e,$0c,$0f
	.byte  $0e,$0c,$0d
	.byte  $0e,$0c,$07
	.byte  $0f,$0c,$0f
	.byte  $0f,$0c,$08
	.byte  $11,$0c,$08
	.byte  $12,$0c,$06
	.byte  $03,$06,$08
	.byte  $15,$0c,$07
	.byte  $15,$0c,$04
	.byte  $17,$0c,$06
	.byte  $04,$06,$06
	.byte  $04,$06,$01
	.byte  $17,$0c,$01
	.byte  $00,$00,$00
