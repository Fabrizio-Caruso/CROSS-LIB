;
; Karri Kaksonen, 2022-04-05
;
; zeropage locations for exclusive use by the library
;

        .include "extzp.inc"

        .segment "EXTZP" : zeropage

ptr7800:        .res 2
ptrtmp:         .res 2
cursorzone:     .res 2
sfxptr:         .res 2
