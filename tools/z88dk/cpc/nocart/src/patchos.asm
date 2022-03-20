;; Patch CPC6128 English OS ROM
org &0000
incbin "original/os.rom"

org &67d
;; original:
;;call &0723
;;call &06fc
;;ld hl,&0688
;;jr   &6fc

;; new:
call &b900 	;; enable upper rom

ld bc,&df82	;; select disc page (compatible with GX4000)
out (c),c

;; poke BASIC command into RAM
jp &dc00	;; call into DOS rom

;; this is placed over the copyright message; the message is no longer displayed so it's free to use
copy_high_jumpblock
ld      hl,&03a6            ; copy high kernel jumpblock from ROM into RAM
ld      de,&b900
ld      bc,&01e4
ldir

;; copy our extra code for remapping ROM access
ld hl,bae4
ld de,&bae4
ld bc,end_bae4-bae4
ldir
ret

;; this code is located at bae4 in ram and fits in a small area.
;; replaces the ld b,&df: out (c),a code.

bae4:
;; C = rom
push af
ld a,c
call bae4b-bae4+&bae4
pop af
ret

;; replaces the ld b,&df: out (c),c
bae4b:
push bc
ld c,&82   ;; if disc rom use this page (not normal disc page)
cp &7      ;; disc rom selected?
jr z,sel_disc_rom
ld c,&81 ;; choose basic page
sel_disc_rom:
ld b,&df
out (c),c ;; select page
pop bc
ret
end_bae4:


;; replace LDIR to copy high kernal jumpblock, we want to do more.
org &0051
call copy_high_jumpblock
defs &005c-$

;;------------------------------------------------------------------------
;; patch RST 3 - LOW: FAR CALL
;;
;; we redirect ROM numbers in the range 0-15 with cartridge pages.
;; In this case, we recognise ROM 7 (AMSDOS) and direct it to page we have chosen
;; for DOS, all other numbers go to the BASIC page. This makes the OS on the cartridge
;; think we have BASIC and AMSDOS only. The OS will not initialise ROMs on an expansion
;; board unless they respond to cartridge pages. This allows games to run as if expansion
;; ROMs had been disabled making the cartridge more compatible.

;; patch RST 3 - LOW: FAR CALL
org &480
;; replacing this:

;; replacing this (direct ROM select using hardware):
;; 0480 06df      ld      b,$df            ; ROM select I/O port
;; 0482 ed79      out     (c),a            ; select upper rom
;; with this:
call bae4b-bae4+&bae4
nop

;; patch RST 3 - LOW: FAR CALL
;; replacing this:
;;04b1 06df      ld      b,$df            ; ROM select I/O port
;;04b3 ed79      out     (c),a            ; restore upper rom selection

org &4b1
call bae4b-bae4+&bae4
nop

;; common upper rom selection code.
org &535
;; replace this:
;;0535 06df      ld      b,$df            ;; ROM select I/O port
;;0537 ed49      out     (c),c            ;; select upper rom
;; with this:
call &bae4
nop


;; patch palettes used by OS - these are toggled
;; to make flashing colours.

;; patch palette set 1
org &1052
defb &14		;; border -> black
defb &14		;; pen 0 -> black

;; patch palette set 2
org &1063
defb &14		;; border -> black
defb &14		;; pen 0 -> black
