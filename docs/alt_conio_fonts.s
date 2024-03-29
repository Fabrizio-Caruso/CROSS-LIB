; The internal font structure for Atari7800 needs a full set of 128
; characters. Each character is 16 x 8 bits.
; The font consists of 8 rows of data:
; row7
; row6
; row5
; row4
; row3
; row2
; row1
; row0
; Each row is 256 bytes long
; As we have 2 bits per pixel we need 2 bytes to represent
; one character. So we can fit 128 characters in this font
; When you later use the font you must address the characters as
; 0, 2, 4, 6, ... 254
; Odd indices cannot be used.

    .export _conio_font
    .rodata
    .align 256
_conio_font:
    .byte $FF, $AA, $55, $FF, $55, $FF, $55, $FF ; SPACE
    .byte $0, $0, $0, $0, $1, $40, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $82, $82
    .byte $0, $0, $28, $0, $0, $0, $0, $0
    .byte $2, $80, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $c3, $c3, $0, $0, $3c, $0
    .byte $0, $0, $0, $0, $3, $c0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $0, $0
    .byte $0, $0, $0, $0, $0, $0, $ff, $ff
    .byte $AA, $FF, $55, $FF, $55, $FF, $55, $FF ; SPACE
    .byte $0, $0, $5, $0, $1, $40, $15, $50
    .byte $55, $50, $55, $50, $15, $40, $1, $54
    .byte $15, $40, $15, $40, $5, $0, $15, $40
    .byte $15, $0, $50, $50, $55, $50, $5, $50
    .byte $55, $40, $55, $54, $55, $0, $5, $54
    .byte $50, $50, $15, $40, $15, $40, $54, $14
    .byte $55, $54, $50, $14, $50, $14, $5, $40
    .byte $55, $0, $1, $50, $54, $14, $15, $40
    .byte $15, $40, $55, $50, $5, $0, $50, $14
    .byte $50, $14, $15, $40, $55, $54, $22, $88
    .byte $a, $0, $a, $0, $0, $0, $a, $0
    .byte $2, $80, $2a, $a0, $aa, $a0, $aa, $a0
    .byte $2a, $80, $2, $a8, $2a, $80, $2a, $80
    .byte $a, $0, $2a, $80, $2a, $0, $a0, $a0
    .byte $aa, $a0, $a, $a0, $aa, $80, $aa, $a8
    .byte $aa, $0, $a, $a8, $a0, $a0, $2a, $80
    .byte $2a, $80, $a8, $28, $aa, $a8, $a0, $28
    .byte $a0, $28, $a, $80, $aa, $0, $2, $a0
    .byte $a8, $28, $2a, $80, $2a, $80, $aa, $a0
    .byte $a, $0, $a0, $28, $a0, $28, $2a, $80
    .byte $aa, $a8, $33, $cc, $f, $0, $f, $0
    .byte $0, $0, $f, $0, $3, $c0, $3f, $f0
    .byte $ff, $f0, $ff, $f0, $3f, $c0, $3, $fc
    .byte $3f, $c0, $3f, $c0, $f, $0, $3f, $c0
    .byte $3f, $0, $f0, $f0, $ff, $f0, $f, $f0
    .byte $ff, $c0, $ff, $fc, $ff, $0, $f, $fc
    .byte $f0, $f0, $3f, $c0, $3f, $c0, $fc, $3c
    .byte $ff, $fc, $f0, $3c, $f0, $3c, $f, $c0
    .byte $ff, $0, $3, $f0, $fc, $3c, $3f, $c0
    .byte $3f, $c0, $ff, $f0, $f, $0, $f0, $3c
    .byte $f0, $3c, $3f, $c0, $ff, $fc, $0, $0
    .byte $FF, $AA, $55, $FF, $55, $FF, $55, $FF ; SPACE
    .byte $0, $0, $5, $0, $1, $40, $54, $14
    .byte $5, $0, $50, $50, $50, $50, $0, $50
    .byte $50, $50, $50, $50, $5, $0, $50, $50
    .byte $1, $40, $50, $50, $14, $14, $14, $14
    .byte $14, $50, $14, $4, $14, $0, $14, $14
    .byte $50, $50, $5, $0, $50, $50, $14, $14
    .byte $14, $14, $50, $14, $50, $14, $14, $50
    .byte $14, $0, $15, $40, $14, $14, $50, $50
    .byte $5, $0, $50, $50, $15, $40, $54, $54
    .byte $14, $50, $5, $0, $14, $14, $a, $a0
    .byte $0, $0, $a, $0, $0, $0, $a, $0
    .byte $2, $80, $a8, $28, $a, $0, $a0, $a0
    .byte $a0, $a0, $0, $a0, $a0, $a0, $a0, $a0
    .byte $a, $0, $a0, $a0, $2, $80, $a0, $a0
    .byte $28, $28, $28, $28, $28, $a0, $28, $8
    .byte $28, $0, $28, $28, $a0, $a0, $a, $0
    .byte $a0, $a0, $28, $28, $28, $28, $a0, $28
    .byte $a0, $28, $28, $a0, $28, $0, $2a, $80
    .byte $28, $28, $a0, $a0, $a, $0, $a0, $a0
    .byte $2a, $80, $a8, $a8, $28, $a0, $a, $0
    .byte $28, $28, $f, $f0, $0, $0, $f, $0
    .byte $0, $0, $f, $0, $3, $c0, $fc, $3c
    .byte $f, $0, $f0, $f0, $f0, $f0, $0, $f0
    .byte $f0, $f0, $f0, $f0, $f, $0, $f0, $f0
    .byte $3, $c0, $f0, $f0, $3c, $3c, $3c, $3c
    .byte $3c, $f0, $3c, $c, $3c, $0, $3c, $3c
    .byte $f0, $f0, $f, $0, $f0, $f0, $3c, $3c
    .byte $3c, $3c, $f0, $3c, $f0, $3c, $3c, $f0
    .byte $3c, $0, $3f, $c0, $3c, $3c, $f0, $f0
    .byte $f, $0, $f0, $f0, $3f, $c0, $fc, $fc
    .byte $3c, $f0, $f, $0, $3c, $3c, $0, $0
    .byte $AA, $FF, $55, $FF, $55, $FF, $55, $FF ; SPACE
    .byte $55, $55, $0, $0, $1, $40, $55, $14
    .byte $5, $0, $14, $0, $0, $50, $55, $54
    .byte $0, $50, $50, $50, $5, $0, $50, $50
    .byte $0, $50, $55, $50, $14, $14, $50, $0
    .byte $14, $14, $14, $40, $14, $40, $50, $54
    .byte $50, $50, $5, $0, $50, $50, $14, $50
    .byte $14, $4, $51, $14, $50, $54, $50, $14
    .byte $14, $0, $51, $50, $14, $50, $1, $50
    .byte $5, $0, $50, $50, $50, $50, $55, $54
    .byte $5, $40, $5, $0, $5, $4, $a8, $2a
    .byte $a, $0, $0, $0, $aa, $aa, $0, $0
    .byte $2, $80, $aa, $28, $a, $0, $28, $0
    .byte $0, $a0, $aa, $a8, $0, $a0, $a0, $a0
    .byte $a, $0, $a0, $a0, $0, $a0, $aa, $a0
    .byte $28, $28, $a0, $0, $28, $28, $28, $80
    .byte $28, $80, $a0, $a8, $a0, $a0, $a, $0
    .byte $a0, $a0, $28, $a0, $28, $8, $a2, $28
    .byte $a0, $a8, $a0, $28, $28, $0, $a2, $a0
    .byte $28, $a0, $2, $a0, $a, $0, $a0, $a0
    .byte $a0, $a0, $aa, $a8, $a, $80, $a, $0
    .byte $a, $8, $fc, $3f, $f, $0, $0, $0
    .byte $ff, $ff, $0, $0, $3, $c0, $ff, $3c
    .byte $f, $0, $3c, $0, $0, $f0, $ff, $fc
    .byte $0, $f0, $f0, $f0, $f, $0, $f0, $f0
    .byte $0, $f0, $ff, $f0, $3c, $3c, $f0, $0
    .byte $3c, $3c, $3c, $c0, $3c, $c0, $f0, $fc
    .byte $f0, $f0, $f, $0, $f0, $f0, $3c, $f0
    .byte $3c, $c, $f3, $3c, $f0, $fc, $f0, $3c
    .byte $3c, $0, $f3, $f0, $3c, $f0, $3, $f0
    .byte $f, $0, $f0, $f0, $f0, $f0, $ff, $fc
    .byte $f, $c0, $f, $0, $f, $c, $0, $0
    .byte $FF, $AA, $55, $FF, $55, $FF, $55, $FF ; SPACE
    .byte $55, $55, $0, $0, $1, $40, $51, $54
    .byte $5, $0, $5, $40, $5, $40, $50, $50
    .byte $0, $50, $55, $40, $1, $40, $15, $40
    .byte $15, $50, $50, $50, $15, $50, $50, $0
    .byte $14, $14, $15, $40, $15, $40, $50, $0
    .byte $55, $50, $5, $0, $0, $50, $15, $40
    .byte $14, $0, $55, $54, $51, $54, $50, $14
    .byte $15, $50, $50, $50, $15, $50, $15, $0
    .byte $5, $0, $50, $50, $50, $50, $51, $14
    .byte $5, $40, $15, $40, $1, $40, $a8, $2a
    .byte $2, $80, $0, $0, $aa, $aa, $0, $0
    .byte $2, $80, $a2, $a8, $a, $0, $a, $80
    .byte $a, $80, $a0, $a0, $0, $a0, $aa, $80
    .byte $2, $80, $2a, $80, $2a, $a0, $a0, $a0
    .byte $2a, $a0, $a0, $0, $28, $28, $2a, $80
    .byte $2a, $80, $a0, $0, $aa, $a0, $a, $0
    .byte $0, $a0, $2a, $80, $28, $0, $aa, $a8
    .byte $a2, $a8, $a0, $28, $2a, $a0, $a0, $a0
    .byte $2a, $a0, $2a, $0, $a, $0, $a0, $a0
    .byte $a0, $a0, $a2, $28, $a, $80, $2a, $80
    .byte $2, $80, $fc, $3f, $3, $c0, $0, $0
    .byte $ff, $ff, $0, $0, $3, $c0, $f3, $fc
    .byte $f, $0, $f, $c0, $f, $c0, $f0, $f0
    .byte $0, $f0, $ff, $c0, $3, $c0, $3f, $c0
    .byte $3f, $f0, $f0, $f0, $3f, $f0, $f0, $0
    .byte $3c, $3c, $3f, $c0, $3f, $c0, $f0, $0
    .byte $ff, $f0, $f, $0, $0, $f0, $3f, $c0
    .byte $3c, $0, $ff, $fc, $f3, $fc, $f0, $3c
    .byte $3f, $f0, $f0, $f0, $3f, $f0, $3f, $0
    .byte $f, $0, $f0, $f0, $f0, $f0, $f3, $3c
    .byte $f, $c0, $3f, $c0, $3, $c0, $0, $0
    .byte $AA, $FF, $55, $FF, $55, $FF, $55, $FF ; SPACE
    .byte $0, $0, $0, $0, $1, $40, $50, $54
    .byte $5, $0, $0, $50, $0, $50, $14, $50
    .byte $55, $40, $50, $0, $0, $50, $50, $50
    .byte $50, $50, $50, $50, $14, $14, $50, $0
    .byte $14, $14, $14, $40, $14, $40, $50, $0
    .byte $50, $50, $5, $0, $0, $50, $14, $50
    .byte $14, $0, $55, $54, $55, $14, $50, $14
    .byte $14, $14, $50, $50, $14, $14, $54, $0
    .byte $5, $0, $50, $50, $50, $50, $50, $14
    .byte $14, $50, $50, $50, $40, $50, $a, $a0
    .byte $0, $a0, $0, $0, $0, $0, $0, $0
    .byte $2, $80, $a0, $a8, $a, $0, $0, $a0
    .byte $0, $a0, $28, $a0, $aa, $80, $a0, $0
    .byte $0, $a0, $a0, $a0, $a0, $a0, $a0, $a0
    .byte $28, $28, $a0, $0, $28, $28, $28, $80
    .byte $28, $80, $a0, $0, $a0, $a0, $a, $0
    .byte $0, $a0, $28, $a0, $28, $0, $aa, $a8
    .byte $aa, $28, $a0, $28, $28, $28, $a0, $a0
    .byte $28, $28, $a8, $0, $a, $0, $a0, $a0
    .byte $a0, $a0, $a0, $28, $28, $a0, $a0, $a0
    .byte $80, $a0, $f, $f0, $0, $f0, $0, $0
    .byte $0, $0, $0, $0, $3, $c0, $f0, $fc
    .byte $f, $0, $0, $f0, $0, $f0, $3c, $f0
    .byte $ff, $c0, $f0, $0, $0, $f0, $f0, $f0
    .byte $f0, $f0, $f0, $f0, $3c, $3c, $f0, $0
    .byte $3c, $3c, $3c, $c0, $3c, $c0, $f0, $0
    .byte $f0, $f0, $f, $0, $0, $f0, $3c, $f0
    .byte $3c, $0, $ff, $fc, $ff, $3c, $f0, $3c
    .byte $3c, $3c, $f0, $f0, $3c, $3c, $fc, $0
    .byte $f, $0, $f0, $f0, $f0, $f0, $f0, $3c
    .byte $3c, $f0, $f0, $f0, $c0, $f0, $0, $0
    .byte $FF, $AA, $55, $FF, $55, $FF, $55, $FF ; SPACE
    .byte $55, $FF, $55, $FF, $55, $FF, $50, $14
    .byte $55, $FF, $55, $FF, $55, $FF, $5, $50
    .byte $50, $0, $14, $0, $50, $50, $50, $50
    .byte $50, $50, $15, $40, $14, $14, $14, $14
    .byte $14, $50, $14, $4, $14, $4, $14, $14
    .byte $50, $50, $5, $0, $0, $50, $14, $14
    .byte $14, $0, $54, $54, $54, $14, $14, $50
    .byte $14, $14, $50, $50, $14, $14, $50, $50
    .byte $45, $10, $50, $50, $50, $50, $50, $14
    .byte $50, $14, $50, $50, $50, $14, $22, $88
    .byte $a0, $a0, $0, $0, $0, $0, $0, $0
    .byte $2, $80, $a0, $28, $2a, $0, $a0, $a0
    .byte $a0, $a0, $a, $a0, $a0, $0, $28, $0
    .byte $a0, $a0, $a0, $a0, $a0, $a0, $2a, $80
    .byte $28, $28, $28, $28, $28, $a0, $28, $8
    .byte $28, $8, $28, $28, $a0, $a0, $a, $0
    .byte $0, $a0, $28, $28, $28, $0, $a8, $a8
    .byte $a8, $28, $28, $a0, $28, $28, $a0, $a0
    .byte $28, $28, $a0, $a0, $8a, $20, $a0, $a0
    .byte $a0, $a0, $a0, $28, $a0, $28, $a0, $a0
    .byte $a0, $28, $33, $cc, $f0, $f0, $0, $0
    .byte $0, $0, $0, $0, $3, $c0, $f0, $3c
    .byte $3f, $0, $f0, $f0, $f0, $f0, $f, $f0
    .byte $f0, $0, $3c, $0, $f0, $f0, $f0, $f0
    .byte $f0, $f0, $3f, $c0, $3c, $3c, $3c, $3c
    .byte $3c, $f0, $3c, $c, $3c, $c, $3c, $3c
    .byte $f0, $f0, $f, $0, $0, $f0, $3c, $3c
    .byte $3c, $0, $fc, $fc, $fc, $3c, $3c, $f0
    .byte $3c, $3c, $f0, $f0, $3c, $3c, $f0, $f0
    .byte $cf, $30, $f0, $f0, $f0, $f0, $f0, $3c
    .byte $f0, $3c, $f0, $f0, $f0, $3c, $0, $0
    .byte $AA, $FF, $55, $FF, $55, $FF, $55, $FF ; SPACE
    .byte $55, $FF, $55, $FF, $55, $FF, $15, $50
    .byte $55, $FF, $55, $FF, $55, $FF, $1, $50
    .byte $55, $50, $5, $40, $55, $50, $15, $40
    .byte $15, $40, $5, $0, $55, $50, $5, $50
    .byte $55, $40, $55, $54, $55, $54, $5, $50
    .byte $50, $50, $15, $40, $1, $54, $54, $14
    .byte $55, $0, $50, $14, $50, $14, $5, $40
    .byte $55, $50, $15, $40, $55, $50, $15, $40
    .byte $55, $50, $50, $50, $50, $50, $50, $14
    .byte $50, $14, $50, $50, $55, $54, $82, $82
    .byte $2a, $80, $0, $0, $0, $0, $0, $0
    .byte $2, $80, $2a, $a0, $a, $0, $2a, $80
    .byte $2a, $80, $2, $a0, $aa, $a0, $a, $80
    .byte $aa, $a0, $2a, $80, $2a, $80, $a, $0
    .byte $aa, $a0, $a, $a0, $aa, $80, $aa, $a8
    .byte $aa, $a8, $a, $a0, $a0, $a0, $2a, $80
    .byte $2, $a8, $a8, $28, $aa, $0, $a0, $28
    .byte $a0, $28, $a, $80, $aa, $a0, $2a, $80
    .byte $aa, $a0, $2a, $80, $aa, $a0, $a0, $a0
    .byte $a0, $a0, $a0, $28, $a0, $28, $a0, $a0
    .byte $aa, $a8, $c3, $c3, $3f, $c0, $0, $0
    .byte $0, $0, $0, $0, $3, $c0, $3f, $f0
    .byte $f, $0, $3f, $c0, $3f, $c0, $3, $f0
    .byte $ff, $f0, $f, $c0, $ff, $f0, $3f, $c0
    .byte $3f, $c0, $f, $0, $ff, $f0, $f, $f0
    .byte $ff, $c0, $ff, $fc, $ff, $fc, $f, $f0
    .byte $f0, $f0, $3f, $c0, $3, $fc, $fc, $3c
    .byte $ff, $0, $f0, $3c, $f0, $3c, $f, $c0
    .byte $ff, $f0, $3f, $c0, $ff, $f0, $3f, $c0
    .byte $ff, $f0, $f0, $f0, $f0, $f0, $f0, $3c
    .byte $f0, $3c, $f0, $f0, $ff, $fc, $0, $0
