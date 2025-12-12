; Bootloader for 256byte sectors
;
        opt     h-

DATA_INDEX      equ     $0043
MAP_INDEX       equ     $0044
IO_ADDRESS      equ     $0045
IO_LENGTH       equ     $0047
PTR             equ     $0049

RUNAD           equ     $02E0
INITAD          equ     $02E2
DCB             equ     $0300
ICCOM           equ     $0342
ICSTA           equ     $0343
ICBAL           equ     $0344
ICBAH           equ     $0345
ICBLL           equ     $0348
ICBLH           equ     $0349
ICAX1           equ     $034A
ICAX2           equ     $034B
CIOV            equ     $E456
SIOV            equ     $E459


; Specify BASE_ADDRESS in command line, as -d:BASE_ADDRESS=\$2100
;BASE_ADDRESS    equ     $2000

MAP_BUFFER      equ     BASE_ADDRESS
DATA_BUFFER     equ     BASE_ADDRESS + $100
LOAD_ADDRESS    equ     BASE_ADDRESS + $200

        org     LOAD_ADDRESS

        .byte   0
        .byte   3       ; Number of sectors to load
        .word   LOAD_ADDRESS   ; Load address
        .word   $0000
        jmp     START   ; Jump to START (must be XX80 for SDX compatibility)

        .word   $0000   ; MAIN DIR MAP SECTOR
        .word   $0000   ; MAX SECTOR
        .word   $0000   ; FREE SECTORS
        .byte   $00     ; SECTORS PER BITMAP
        .word   $0000   ; BITMAP START SECTOR
        .word   $0000   ; LAST ALLOCATED FILE SECTOR
        .word   $0000   ; LAST ALLOCATED DIR SECTOR
        .byte   'DSKLABEL' ; DISK LABEL
        .byte   40      ; NUMBER OF TRACKS
        .byte   $80     ; SECTOR SIZE
        .byte   $20     ; DISK FORMAT VERSION (2.0)
        .byte   $00,$00,$00,$00,$00 ; RESERVED
        .byte   $00     ; DISK SEQ NUMBER
        .byte   $00     ; DISK RANDOM ID
BOOT_MAP:
        .word   $0000   ; BOOT FILE MAP SECTOR
        .byte   $00,$00,$00,$00,$00,$00 ; RESERVED

ERROR_MSG:
        .byte   'DOS Error', $9B
ERROR_MSG_LEN   equ     * - ERROR_MSG

SIO_READ_BUF:
        .if     <MAP_BUFFER != <DATA_BUFFER
        .error  "Error, MAP_BUFFER and DATA_BUFFER must be $XX00"
        .endif
        ldy     #<MAP_BUFFER

SIO_READ:
        sty     DCB+4           ; Store address
        stx     DCB+5
        sta     DCB+10          ; Store and check sector number
        ora     DCB+11
        beq     READ_BLANK      ; If sector == 0, read zeroes
        lda     #$40
        sta     DCB+3
        jsr     SIOV
        bpl     RTS_OK

DOS_ERROR:
        lda     #<ERROR_MSG
        sta     ICBAL
        .if     <ERROR_MSG < ERROR_MSG_LEN
        .error  "Error, low byte of message address should be bigger than message length"
        .endif
        sta     ICBLL
        lda     #>ERROR_MSG
        sta     ICBAH
        lda     #$09
        sta     ICCOM
        ldx     #$00
        jsr     CIOV
@loop:  jmp     @loop

READ_BLANK:
        stx     PTR+1   ; Store destination address
        sty     PTR
        tay             ; A is already 0
@copy:
        sta     (PTR),y
        iny
        bne     @copy
RTS_OK:
        rts

GO_INITAD:
        jmp     (INITAD)

END_LOAD:
        jmp     (RUNAD)

        ; START must be at $XX80
        .align  $80, $ea
        .if     START != LOAD_ADDRESS + $80
        .error  "Error, START symbol must be at $XX80, currently at ", START
        .endif
START:
        ; Set index to end of sector data
        ldx     #$0
        stx     MAP_INDEX
        stx     DATA_INDEX
        ; Set read size to sector size
        stx     DCB+8
        inx
        stx     DCB+9

        ; Read binary header: "FF FF"
        jsr     GET_2_BYTES
        inx
        bne     DOS_ERROR
        cmp     #$FF
        bne     DOS_ERROR

        ; Read first address, copy to RUNAD
        jsr     GET_2_BYTES
        sta     RUNAD
        stx     RUNAD+1
        jmp     FIRST_SEG

        ; Reads a data block
READ_DATA_BLOCK:
        ; Get start address
        jsr     GET_2_BYTES
FIRST_SEG:
        sta     IO_ADDRESS
        stx     IO_ADDRESS+1
        ora     IO_ADDRESS+1
        beq     END_LOAD

        ; Skip $FFFF header in the middle of file
        txa
        and     IO_ADDRESS
        cmp     #$FF
        beq     READ_DATA_BLOCK

        ; Get end address
        jsr     GET_2_BYTES
        sec
        sbc     IO_ADDRESS
        sta     IO_LENGTH
        txa
        sbc     IO_ADDRESS+1
        sta     IO_LENGTH+1

        ; Setup INITAD to an "rts" instruction.
        lda     #<RTS_OK
        sta     INITAD
        lda     #>RTS_OK
        sta     INITAD+1

        ; Read data
        jsr     GET_DATA
        ; Execute INITAD
        jsr     GO_INITAD
        ; GO to next block
        jmp     READ_DATA_BLOCK

GET_2_BYTES:
        jsr     GET_BYTE
        pha
        jsr     GET_BYTE
        tax
        pla
        rts


DEC_IO_LENGTH_1:
        dec     IO_LENGTH+1
DEC_IO_LENGTH:
        dec     IO_LENGTH

GET_DATA:
        jsr     GET_BYTES
        ldy     #$00
        sta     (IO_ADDRESS),y
        inc     IO_ADDRESS
        bne     @skip
        inc     IO_ADDRESS+1
@skip:  lda     IO_LENGTH
        bne     DEC_IO_LENGTH
        lda     IO_LENGTH+1
        bne     DEC_IO_LENGTH_1
        rts

GET_BYTE:
        lda     #$00
        sta     IO_LENGTH+1
        sta     IO_LENGTH
GET_BYTES:
        ldx     DATA_INDEX
        beq     NEXT_SECTOR
GET_BYTE_X:
        lda     DATA_BUFFER,x
        inc     DATA_INDEX
        rts

GET_FULL_SECTOR:
        ldy     IO_ADDRESS
        ldx     IO_ADDRESS+1
        jsr     SIO_READ
        inc     IO_ADDRESS+1
        dec     IO_LENGTH+1

        ; Fall through
;;;        jmp     NEXT_SECTOR

NEXT_SECTOR:
        ldy     MAP_INDEX
        beq     NEXT_MAP
NEXT_SECTOR_Y:
        lda     MAP_BUFFER,y
        ldx     MAP_BUFFER+1,y
        stx     DCB+11
        iny
        iny
        sty     MAP_INDEX

        ldy     IO_LENGTH+1
        bne     GET_FULL_SECTOR
        ldx     #>DATA_BUFFER
        jsr     SIO_READ_BUF
        ldx     #0
        beq     GET_BYTE_X

NEXT_MAP:
        lda     BOOT_MAP
        ldx     BOOT_MAP+1
        stx     DCB+11
        ldx     #>MAP_BUFFER
        jsr     SIO_READ_BUF
        lda     MAP_BUFFER
        sta     BOOT_MAP
        lda     MAP_BUFFER+1
        sta     BOOT_MAP+1
        ldy     #$04
        bne     NEXT_SECTOR_Y

