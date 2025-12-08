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
; this file is intended to be assembled and linked with the cc65 toolchain.
; It has not been tested with any other assemblers or linkers.
; -------------------------------------------------------------------

; -------------------------------------------------------------------
; the circular buffer needs to be page aligned, so only the high-bytes
; of the buffer start and len is imported to enforce this.
; the data that are to be decrunched must not have been crunched with
; a maximum offset size greater than the buffer size. use the -m option
; with a value of (buffer_len_hi * 256) or less.
; Please observe that the uncrunched file size must be a multiple of the
; chunk size. If not the final uncomplete chunk will be skipped.
; -------------------------------------------------------------------
.import buffer_start_hi:	absolute
.import buffer_len_hi:	absolute
.import decrunched_chunk_size:	absolute

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
; want a chunk of decrunched data. the zero page pointer $fe will point to it.
; Calling this function will also destroy the values in y and x reg and mess up
; the status flags. This function will not change the interrupt status bit and
; it will not modify the memory configuration.
; -------------------------------------------------------------------
.export get_decrunched_chunk

; -------------------------------------------------------------------
; zero page addresses used
; -------------------------------------------------------------------
zp_src_lo = $a7
zp_src_hi = zp_src_lo + 1

zp_len_lo  = $ae
zp_len_hi  = zp_len_lo + 1

zp_bits_lo = $fb
zp_bits_hi = zp_bits_lo + 1

zp_bitbuf = $fd
zp_dest_lo = $fe		; dest addr lo
zp_dest_hi = zp_dest_lo + 1	; dest addr hi

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
; -------------------------------------------------------------------
; jsr this label to init the decruncher, it will init used zeropage
; zero page locations and the decrunch tables
; no constraints on register content, however the
; decimal flag has to be #0 (it almost always is, otherwise do a cld)
; -------------------------------------------------------------------
init_decruncher:
	jsr get_crunched_byte
	sta zp_bitbuf

	ldx #<buffer_start_hi
	stx zp_dest_hi
	ldx #0
	stx zp_dest_lo
	stx zp_len_lo
	stx zp_len_hi
	ldy #0
; -------------------------------------------------------------------
; calculate tables (49 bytes)
; x and y must be #0 when entering
;
_init_nextone:
	tya
	and #$0f
	beq _init_shortcut		; starta på ny sekvens

	txa			; this clears reg a
	sec			; and sets the carry flag
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
; decrunch chunk
;
; -------------------------------------------------------------------
get_decrunched_chunk:
	ldx zp_dest_lo
	bne _seq_dest_dec_lo
	ldx zp_dest_hi
	cpx #<buffer_start_hi
	bne _seq_dest_dec_hi
; ------- handle buffer wrap problematics here ----------------------
	ldx #<buffer_end_hi
	stx zp_dest_hi
_seq_dest_dec_hi:
	dec zp_dest_hi
_seq_dest_dec_lo:
; -------------------------------------------------------------------
_decrunched_chunk_loop:
	dec zp_dest_lo

	ldy zp_len_lo
	bne _do_sequence
	ldx zp_len_hi
	bne _do_sequence2
; -------------------------------------------------------------------
; count zero bits + 1 to get length table index (10 bytes)
; y = x = 0 when entering
; -------------------------------------------------------------------
_seq_bit_next:
	lsr zp_bitbuf
	bne _seq_bit_ok
	jsr get_crunched_byte
	ror
	sta zp_bitbuf
_seq_bit_ok:
	iny
	bcc _seq_bit_next
	dey
	beq _get_literal
	cpy #$11
	bcs _do_exit
; -------------------------------------------------------------------
; calulate length of sequence (zp_len) (17 bytes)
;
	txa
	sta zp_bits_lo
	sta zp_bits_hi
	ldx tabl_bi - 1,y
	beq _seq_skip_len_bits1
	jsr _bit_get_bits_noclear
_seq_skip_len_bits1:
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
	cpy #$03
	bcc _seq_size123
_seq_nots123:
	ldy #$02
_seq_size123:
	ldx tabl_bit,y

	lda tabl_off,y
	sta zp_bits_lo

	jsr _bit_get_bits_noclear
	tay
; -------------------------------------------------------------------
; calulate absolute offset (zp_src) (27 bytes)
;
	txa
	sta zp_bits_lo
	sta zp_bits_hi
	ldx tabl_bi,y
	beq _seq_skip_len_bits2
	jsr _bit_get_bits_noclear
_seq_skip_len_bits2:
	adc tabl_lo,y
	bcc _seq_skipcarry
	inc zp_bits_hi
_seq_skipcarry:
	sec
	adc zp_dest_lo
	sta zp_src_lo
	lda zp_bits_hi
	adc tabl_hi,y
	adc zp_dest_hi
; -------------------------------------------------------------------
	cmp #<buffer_end_hi
	bcc _seq_offset_ok
	sbc #<buffer_len_hi
; -------------------------------------------------------------------
_seq_offset_ok:
	sta zp_src_hi
	jmp _seq_shortcut
; -------------------------------------------------------------------
_get_literal:
	jsr get_crunched_byte
	bcs _do_literal
; -------------------------------------------------------------------
; literal handling (13 bytes)
;
_do_sequence2:
	dec zp_len_hi
_do_sequence:
	dec zp_len_lo
; -------------------------------------------------------------------
	ldx zp_src_lo
	bne _seq_src_dec_lo
	ldx zp_src_hi
	cpx #<buffer_start_hi
	bne _seq_src_dec_hi
; ------- handle buffer wrap problematics here ----------------------
	ldx #<buffer_end_hi
	stx zp_src_hi
; -------------------------------------------------------------------
_seq_src_dec_hi:
	dec zp_src_hi
_seq_src_dec_lo:
	dec zp_src_lo
; -------------------------------------------------------------------
_seq_shortcut:
	ldy #0
	lda (zp_src_lo),y
; -------------------------------------------------------------------
_do_literal:
; -------------------------------------------------------------------
	sta (zp_dest_lo),y
	lda <zp_dest_lo
	and #<(decrunched_chunk_size - 1)
	beq do_exit2
	jmp _decrunched_chunk_loop
do_exit2:
	clc
	rts
; -------------------------------------------------------------------
; two small static tables (6 bytes)
;
tabl_bit:
	.byte 2,4,4
tabl_off:
	.byte %0001100, %00000010, %00000001
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
	lda #0
	sta zp_bits_lo
	sta zp_bits_hi
_bit_get_bits_noclear:
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
	rts
; -------------------------------------------------------------------
; end of decruncher
; -------------------------------------------------------------------
