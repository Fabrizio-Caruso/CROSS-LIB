;;
;; ABC80 .bac loader loading at a fixed address
;;

	include "setup80.inc"

_start:
	include "start80.inc"

set_load_addr:
	defc LOAD=set_load_addr+1
	ld de,0			; Load address

	include "copy80.inc"

	;; All done, jump to entry point
	;; The stack is all set up for return to CHAIN, NEW or END
done:
	defc ENTRY=done+1	; Entry point address
	jp 0

	include "end80.inc"
