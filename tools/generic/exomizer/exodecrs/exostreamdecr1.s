;
; Copyright (c) 2002, 2003 Magnus Lind.
;
; This software is provided 'as-is', without any express or implied warranty.
; In no event will the authors be held liable for any damages arising from
; the use of this software.
;
; Permission is granted to anyone to use this software for any purpose,
; including commercial applications, and to alter it and redistribute it
; freely, subject to the following restrictions:
;
;   1. The origin of this software must not be misrepresented; you must not
;   claim that you wrote the original software. If you use this software in a
;   product, an acknowledgment in the product documentation would be
;   appreciated but is not required.
;
;   2. Altered source versions must be plainly marked as such, and must not
;   be misrepresented as being the original software.
;
;   3. This notice may not be removed or altered from any source distribution.
;
;   4. The names of this software and/or it's copyright holders may not be
;   used to endorse or promote products derived from this software without
;   specific prior written permission.
;

; -------------------------------------------------------------------
; the circular buffer needs to be page aligned, so only the high-bytes
; of the buffer start and len is imported to enforce this.
; the data that are to be decrunched must not have been crunched with
; a maximum offset size greater than the buffer size. use the -m option
; with a value of (buffer_len_hi * 256) or less.
; -------------------------------------------------------------------
.import buffer_start_hi:	absolute
.import buffer_len_hi:		absolute

; -------------------------------------------------------------------
; The decruncher jsr:s to the get_crunched_byte address when it wants to
; read a crunched byte. This subroutine has to preserve x and y register
; and must not modify the state of the carry flag.
; -------------------------------------------------------------------
.import get_crunched_byte

; -------------------------------------------------------------------
; this subsoutine is called before decrunching. It initializes the
; decruncher zeropage locations and precalculates the decrunch tables.
; -------------------------------------------------------------------
.export init_decruncher

; -------------------------------------------------------------------
; this function is the heart of the decruncher. Call this whenever you
; want a decrunched byte. It will return it in the accumulator. It will
; also destroy the values in y and x reg and mess up the status flags.
; This function will not change the interrupt status bit and it will not
; modify the memory configuration.
; -------------------------------------------------------------------
.export get_decrunched_byte

; -------------------------------------------------------------------
; zero page addresses used
; -------------------------------------------------------------------
zp_src_hi = $a7
zp_src_lo = zp_src_hi + 1
zp_src_bi = zp_src_hi + 2
zp_bitbuf = zp_src_hi + 3

zp_len_lo  = $ae
zp_len_hi  = zp_len_lo + 1

zp_bits_lo = $fb
zp_bits_hi = zp_bits_lo + 1

zp_dest_hi = $fd
zp_dest_lo = zp_dest_hi + 1	; dest addr lo
zp_dest_bi = zp_dest_hi + 2	; dest addr hi

; -------------------------------------------------------------------
; symbolic names for constants
; -------------------------------------------------------------------
buffer_end_hi = buffer_start_hi + buffer_len_hi

tabl_bi = decrunch_table
tabl_lo = decrunch_table + 52
tabl_hi = decrunch_table + 104
; -------------------------------------------------------------------
; no code below this comment has to be modified in order to generate
; a working decruncher of this source file.
; However, you may want to relocate the tables last in the file to a
; more suitable address.
; -------------------------------------------------------------------

; -------------------------------------------------------------------
; jsr this label to init the decruncher, it will init used zeropage
; zero page locations and the decrunch tables
; no constraints on register content, however the
; decimal flag has to be #0 (it almost always is, otherwise do a cld)
; -------------------------------------------------------------------
init_decruncher:
	jsr get_crunched_byte
	sta zp_bitbuf

	ldx #0
	stx zp_dest_lo
	stx zp_dest_hi
	stx zp_len_lo
	stx zp_len_hi
	ldy #0
; -------------------------------------------------------------------
; calculate tables (49 bytes)
; x and y must be #0 when entering
;
_init_nextone:
	inx
	tya
	and #$0f
	beq _init_shortcut		; starta på ny sekvens

	txa			; this clears reg a
	lsr a			; and sets the carry flag
	ldx zp_bits_lo
_init_rolle:
	rol a
	rol zp_bits_hi
	dex
	bpl _init_rolle		; c = 0 after this (rol zp_bits_hi)

	adc tabl_lo-1,y
	tax

	lda zp_bits_hi
	adc tabl_hi-1,y
_init_shortcut:
	sta tabl_hi,y
	txa
	sta tabl_lo,y

	ldx #4
	jsr _bit_get_bits		; clears x-reg.
	sta tabl_bi,y
	iny
	cpy #52
	bne _init_nextone
_do_exit:
	rts
; -------------------------------------------------------------------
; decrunch one byte
;
get_decrunched_byte:
	ldy zp_len_lo
	bne _do_sequence
	ldx zp_len_hi
	bne _do_sequence2

	jsr _bit_get_bit1
	beq _get_sequence
; -------------------------------------------------------------------
; literal handling (13 bytes)
;
	jsr get_crunched_byte
	bcc _do_literal
; -------------------------------------------------------------------
; count zero bits + 1 to get length table index (10 bytes)
; y = x = 0 when entering
;
_get_sequence:
_seq_next1:
	iny
	jsr _bit_get_bit1
	beq _seq_next1
	cpy #$11
	bcs _do_exit
; -------------------------------------------------------------------
; calulate length of sequence (zp_len) (17 bytes)
;
	ldx tabl_bi - 1,y
	jsr _bit_get_bits
	adc tabl_lo - 1,y
	sta zp_len_lo
	lda zp_bits_hi
	adc tabl_hi - 1,y
	sta zp_len_hi
; -------------------------------------------------------------------
; here we decide what offset table to use (20 bytes)
; x is 0 here
;
	bne _seq_nots123
	ldy zp_len_lo
	cpy #$04
	bcc _seq_size123
_seq_nots123:
	ldy #$03
_seq_size123:
	ldx tabl_bit - 1,y
	jsr _bit_get_bits
	adc tabl_off - 1,y
	tay
; -------------------------------------------------------------------
; calulate absolute offset (zp_src) (27 bytes)
;
	ldx tabl_bi,y
	jsr _bit_get_bits;
	adc tabl_lo,y
	bcc _seq_skipcarry
	inc zp_bits_hi
	clc
_seq_skipcarry:
	adc zp_dest_lo
	sta zp_src_lo
	lda zp_bits_hi
	adc tabl_hi,y
	adc zp_dest_hi
; -------------------------------------------------------------------
	cmp #<buffer_len_hi
	bcc _seq_offset_ok
	sbc #<buffer_len_hi
	clc
; -------------------------------------------------------------------
_seq_offset_ok:
	sta zp_src_hi
	adc #<buffer_start_hi
	sta zp_src_bi
_do_sequence:
	ldy #0
_do_sequence2:
	ldx zp_len_lo
	bne _seq_len_dec_lo
	dec zp_len_hi
_seq_len_dec_lo:
	dec zp_len_lo
; -------------------------------------------------------------------
	ldx zp_src_lo
	bne _seq_src_dec_lo
	ldx zp_src_hi
	bne _seq_src_dec_hi
; ------- handle buffer wrap problematics here ----------------------
	ldx #<buffer_len_hi
	stx zp_src_hi
	ldx #<buffer_end_hi
	stx zp_src_bi
; -------------------------------------------------------------------
_seq_src_dec_hi:
	dec zp_src_hi
	dec zp_src_bi
_seq_src_dec_lo:
	dec zp_src_lo
; -------------------------------------------------------------------
	lda (zp_src_lo),y
; -------------------------------------------------------------------
_do_literal:
	ldx zp_dest_lo
	bne _seq_dest_dec_lo
	ldx zp_dest_hi
	bne _seq_dest_dec_hi
; ------- handle buffer wrap problematics here ----------------------
	ldx #<buffer_len_hi
	stx zp_dest_hi
	ldx #<buffer_end_hi
	stx zp_dest_bi
; -------------------------------------------------------------------
_seq_dest_dec_hi:
	dec zp_dest_hi
	dec zp_dest_bi
_seq_dest_dec_lo:
	dec zp_dest_lo
; -------------------------------------------------------------------
	sta (zp_dest_lo),y
	clc
	rts
; -------------------------------------------------------------------
; two small static tables (6 bytes)
;
tabl_bit:
	.byte 2,4,4
tabl_off:
	.byte 48,32,16
; -------------------------------------------------------------------
; get x + 1 bits (1 byte)
;
_bit_get_bit1:
	inx
; -------------------------------------------------------------------
; get bits (31 bytes)
;
; args:
;   x = number of bits to get
; returns:
;   a = #bits_lo
;   x = #0
;   c = 0
;   zp_bits_lo = #bits_lo
;   zp_bits_hi = #bits_hi
; notes:
;   y is untouched
;   other status bits are set to (a == #0)
; -------------------------------------------------------------------
_bit_get_bits:
	lda #$00
	sta zp_bits_lo
	sta zp_bits_hi
	cpx #$01
	bcc _bit_bits_done
	lda zp_bitbuf
_bit_bits_next:
	lsr a
	bne _bit_ok
	jsr get_crunched_byte
	ror a
_bit_ok:
	rol zp_bits_lo
	rol zp_bits_hi
	dex
	bne _bit_bits_next
	sta zp_bitbuf
	lda zp_bits_lo
_bit_bits_done:
	rts
; -------------------------------------------------------------------
; end of decruncher
; -------------------------------------------------------------------

; -------------------------------------------------------------------
; this 156 byte table area may be relocated. It may also be clobbered
; by other data between decrunches.
; -------------------------------------------------------------------
decrunch_table:
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0
; -------------------------------------------------------------------
; end of decruncher
; -------------------------------------------------------------------
