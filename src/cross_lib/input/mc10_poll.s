	.export _mc10_poll

	.setcpu 6803
	.code

_mc10_poll:
	pshx
	ldx $FFDC			; poll keyboard
	jsr ,x
	pulx
	tab
	rts

