; -------------------------------------------------------------------
; this file is intended to be assembled and linked with the cc65 toolchain.
; It has not been tested with any other assemblers or linkers.
; -------------------------------------------------------------------
; -------------------------------------------------------------------
; example usage do the stream decruncher
; this program decrunches data to memory
; -------------------------------------------------------------------
.import init_decruncher
.import get_decrunched_chunk
.export get_crunched_byte

.export buffer_start_hi:	absolute
.export buffer_len_hi:		absolute
.export decrunched_chunk_size:	absolute

decrunched_chunk_size = 128

	.byte $01,$08,$0b,$08,<2003,>2003,$9e,'2','0','6','1',0,0,0
; -------------------------------------------------------------------
; we begin here
; -------------------------------------------------------------------
	lda $04
	sta _byte_lo
	lda $05
	sta _byte_hi
	jsr init_decruncher
_sample_next:
	jsr get_decrunched_chunk
	bcs _sample_end
	;; do whatever you wish with the value in the accumulator
.if 1
	lda store_lo
	sec
	sbc #<(decrunched_chunk_size)
	sta store_lo
	bcs skip_store_dec_hi
	dec store_hi
skip_store_dec_hi:

	ldy #<(decrunched_chunk_size - 1)
_next_byte_in_chunk:
	lda ($fe),y
store_lo = * + 1
store_hi = * + 2
	.byte $99, 0, 0 		;sta $0000
	dey
	bpl _next_byte_in_chunk

.endif
	jmp _sample_next
_sample_end:
	rts
; -------------------------------------------------------------------
; for this get_crunched_byte routine to work the crunched data has to be
; crunched using the -m <buffersize> and possibly the -l flags. Any other
; flag will just mess things up.
get_crunched_byte:
	lda _byte_lo
	bne _byte_skip_hi
	dec _byte_hi
_byte_skip_hi:
	dec _byte_lo
_byte_lo = * + 1
_byte_hi = * + 2
	lda $ffff		; needs to be set correctly before
	rts			; decrunch_file is called.
; end_of_data needs to point to the address just after the address
; of the last byte of crunched data.
; -------------------------------------------------------------------
buffer_len_hi	= 4            ; 1k
unaligned_buffer:
	.res (buffer_len_hi * 256) + 255
buffer_start_hi = (unaligned_buffer + 255) / 256

