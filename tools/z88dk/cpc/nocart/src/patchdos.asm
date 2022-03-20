org &c000

incbin "original/amsdos.rom"

org &c5f2
defs &c601-$

;;--------------------------------------------------------
org &c56c
;; force DATA format disc auto-detect
ld a,&c1		;; sector ID for data format
defb &18
defb &c581-$-1

;;--------------------------------------------------------
;; do not send specify to FDC
;; GX4000 will hang if attempt is made to send specify
org &c615
ret

;;--------------------------------------------------------
;; patch BIOS: get drive status
;; check current track
;; report if on track 0.
;; report write protected
;; report ready
org &c630
or a
jr z,get_dr_status0

ld hl,&be4b
ld (hl),1
inc hl
ld a,%01000001 ;; drive 1, not ready, not write protect
ld (hl),a
dec hl
or a
ret

get_dr_status0:
push hl
push de
ld e,a						;; drive into E
ld a,&16					;; XDPB: current track
call &ca63
ld a,(hl)
or a
ld d,%00010000
jr z,gds
ld d,%00000000
gds:
or d
or %01100000					;; write protect, drive ready
pop de
pop hl
scf
ret

;;--------------------------------------------------------

;; disable BIOS: write sector
;; indicate fail because it's write protected
org &c64e
jp &c652

;;--------------------------------------------------------
;; disable BIOS: format track
;; indicate fail because it's write protected
org &c652
inc e
dec e
jr z,write_drive0
;; Drive B is not ready.
ld a,&48
ld hl,&be4b
push hl
ld (hl),2
inc hl
ld (hl),&68
inc hl
ld (hl),0
pop hl
or a
ret

write_drive0:
;; Drive A is ready but write protected
ld a,&42
ld hl,&be4c
push hl
ld (hl),7
inc hl
ld (hl),&40
inc hl
ld (hl),&2
inc hl
ld (hl),0
inc hl
ld (hl),d
inc hl
ld (hl),0
inc hl
ld (hl),c
inc hl
ld (hl),2
pop hl
or a
ret

;;--------------------------------------------------------
;; patch BIOS: read sector
org &c666
jp read_sector

;;--------------------------------------------------------
;; patch BIOS: seek track
org &c763
inc e
dec e
jr nz,seek_dr0
ld a,&48
ld hl,&be4b
push hl
ld (hl),2
inc hl
ld (hl),&68
inc hl
ld (hl),0
pop hl
or a
ret

seek_dr0:
push hl
ld a,&16					;; XDPB: current track
call &ca63
ld (hl),d					;; store requested track
pop hl
xor a
scf
ret

;;--------------------------------------------------------
org &dc00
jp install_basic_command

;; this is the command to run
command_buffer:
defb &aa			;; indicates we have command
defb 'RUN"FILENAME.BAS',0
end_command_buffer:

install_basic_command:
;; install basic run command
ld      hl,command_buffer
ld      de,&ac8a
ld      bc,end_command_buffer-command_buffer
ldir    
ret


;;HL contains address of sector buffer 
;;E contains drive number 
;;D contains track number 
;;C contains sector number

;; 

read_sector:
inc e
dec e
jr z,read_dr0
;; drive B is not ready
ld a,&48
ld hl,&be4b
push hl
ld (hl),2
inc hl
ld (hl),&68
inc hl
ld (hl),0
pop hl
or a
ret

read_dr0:
di
push    bc
push    de
push    hl

push    hl			;; destination

ld      a,c			;; sector 
and     &f			;; c1->1, c2->2, c9->9
dec     a			;; 1->0, 2->1... 9->8

ld      e,a

ld      h,0
ld      l,d	;; track
ld 		d,h
ld      b,h
ld 		c,l
add     hl,hl	;; x2
add     hl,hl	;; x4
add     hl,hl	;; x8
add     hl,bc	;; x9
add		hl,de
add     hl,hl	;; x18

;; address
ld      a,l		
or      &c0
ld      d,a
ld      e,&0

add     hl,hl	;; x2
add     hl,hl	;; x4
ld      a,h		;; page

push    de

;; copy the sector copy function to ram
ld      hl,sector_copy
ld      de,&be44
ld      bc,end_sector_copy-sector_copy
ldir    

add a,&83		;; base page 
pop     hl		;; source
pop     de		;; destination
ld      b,&df
call    &be44		;; copy sector function in ram
pop     hl
pop     de
pop     bc
ld a,0
scf     
ei      
ret       


;; C = 0
;; DE = destination
;; HL = source (in cartridge page)
sector_copy:
out     (c),a					;; select cartridge page
ld      bc,&200					;; 512 bytes
ldir    
ld      bc,&df82 ;; restore disc rom
out     (c),c
ret     
end_sector_copy:
