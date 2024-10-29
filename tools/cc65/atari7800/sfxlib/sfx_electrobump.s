	.rodata
	.export	_sfx_electrobump
_sfx_electrobump:
	.byte  $10,$10,$01 ; version, priority, frames per chunk
	.byte  $08,$08,$0a
	.byte  $08,$0c,$0a
	.byte  $08,$06,$0a
	.byte  $08,$0e,$0a
	.byte  $08,$06,$08
	.byte  $08,$06,$08
	.byte  $08,$0e,$06
	.byte  $08,$06,$04
	.byte  $08,$06,$02
	.byte  0,0,0
