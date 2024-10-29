	.rodata
	.export	_sfx_nonobounce
_sfx_nonobounce:
	.byte  $10,$10,$01 ; version, priority, frames per chunk
	.byte  $0f,$0c,$04
	.byte  $00,$0e,$08
	.byte  $10,$0c,$08
	.byte  $02,$06,$06
	.byte  $10,$0c,$06
	.byte  $02,$06,$06
	.byte  $00,$0e,$08
	.byte  $10,$0c,$08
	.byte  $02,$06,$08
	.byte  $0f,$0c,$06
	.byte  $10,$0c,$06
	.byte  $02,$06,$06
	.byte  $10,$0c,$06
	.byte  $0f,$0c,$04
	.byte  $10,$0c,$04
	.byte  $0f,$0c,$04
	.byte  $00,$0e,$04
	.byte  $10,$0c,$04
	.byte  $10,$0c,$02
	.byte  0,0,0
