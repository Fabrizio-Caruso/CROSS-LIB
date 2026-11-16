.export _getk
.setcpu 6803

.code

_getk:
    pshx
    ldx $FFDC
    jsr ,x
    pulx
    tab
    clra
    rts




