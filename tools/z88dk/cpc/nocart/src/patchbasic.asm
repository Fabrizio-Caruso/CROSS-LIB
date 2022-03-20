org &c000
incbin "original/basic.rom"

org &c033
;; truncate BASIC's 'BASIC v1.1' startup message.
defb 0

org &c0d7
;; truncate BASIC's 'Ready' prompt
defb 0

org &caf9
call &d583	

org &d583
ld hl,&ac8a
ld a,&aa
cp (hl)
ret nz
pop af
ld (hl),' '
scf
ret