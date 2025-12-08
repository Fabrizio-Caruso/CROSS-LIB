//
// Copyright (c) 2002 - 2019 Magnus Lind.
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//   1. The origin of this software must not be misrepresented// you must not
//   claim that you wrote the original software. If you use this software in a
//   product, an acknowledgment in the product documentation would be
//   appreciated but is not required.
//
//   2. Altered source versions must be plainly marked as such, and must not
//   be misrepresented as being the original software.
//
//   3. This notice may not be removed or altered from any source distribution.
//
//   4. The names of this software and/or it's copyright holders may not be
//   used to endorse or promote products derived from this software without
//   specific prior written permission.
//
// -------------------------------------------------------------------
// Known quirks:
//  Can't handle a sequence reference that ends at $ffff. It is left in
//  since it is a corner case and fixing it impacts negatively on
//  performance or backwards compatibility.
//  A simple way to work around this is to not decrunch to address $ffff.
// -------------------------------------------------------------------
// Controls if the shared get_bits routines should be inlined or not.
//#define INLINE_GET_BITS
// -------------------------------------------------------------------
// if literal sequences is not used (the data was crunched with the -c
// flag) then the following line can be uncommented for shorter and.
// slightly faster code.
//#define LITERAL_SEQUENCES_NOT_USED
// -------------------------------------------------------------------
// if the sequence length is limited to 256 (the data was crunched with
// the -M256 flag) then the following line can be uncommented for
// shorter and slightly faster code.
//#define MAX_SEQUENCE_LENGTH_256
// -------------------------------------------------------------------
// if the sequence length 3 has its own offset table (the data was
// crunched with the -P+16 flag) then the following
// line must be uncommented.
//#define EXTRA_TABLE_ENTRY_FOR_LENGTH_THREE
// -------------------------------------------------------------------
// if sequence offsets are not reused (the data was crunched with the
// -P-32 flag) then the following line must be uncommented. Uncommenting the
// line will also result in shorter and slightly faster code.
//#define DONT_REUSE_OFFSET
// -------------------------------------------------------------------
// if decrunching forwards then the following line must be uncommented.
//#define DECRUNCH_FORWARDS
// -------------------------------------------------------------------
// if split encoding is used (the data is crunched with the -E flag)
// then the following line must be uncommented.
//#define ENABLE_SPLIT_ENCODING
// -------------------------------------------------------------------
// The decruncher jsr:s to the exod_get_crunched_byte address when it wants to
// read a crunched byte into A. This subroutine has to preserve X and Y
// register and must not modify the state of the carry nor the overflow flag.
// -------------------------------------------------------------------
// -------------------------------------------------------------------
// The exod_decrunch function is the heart of the decruncher.
// It initializes the decruncher zeropage locations and precalculates the
// decrunch tables and decrunches the data
// This function will not change the interrupt status bit and it will not
// modify the memory configuration.
// -------------------------------------------------------------------
#if ENABLE_SPLIT_ENCODING
// -------------------------------------------------------------------
// To decrunch files crunched with the split feature (-E) you can't use the
// decrunch function. Instead you call the split_decrunch function. But you
// can only do this if the decrunch table contains the encoding used by the
// file you are decrunching. To generate the correct content for the decrunch
// table call set the get_crunched_byte function to point to the encoding data
// and then call the split_gentable function.
// -------------------------------------------------------------------
#endif
// -------------------------------------------------------------------
// zero page addresses used
// -------------------------------------------------------------------
.label exod_zp_len_lo = $9e
.label exod_zp_len_hi = $9f

.label exod_zp_src_lo  = $ae
.label exod_zp_src_hi  = exod_zp_src_lo + 1

.label exod_zp_bits_hi = $a7
#if !DONT_REUSE_OFFSET
.label exod_zp_ro_state = $a8
#endif

.label exod_zp_bitbuf  = $fd
.label exod_zp_dest_lo = exod_zp_bitbuf + 1      // dest addr lo
.label exod_zp_dest_hi = exod_zp_bitbuf + 2      // dest addr hi

#if EXTRA_TABLE_ENTRY_FOR_LENGTH_THREE
.label _encoded_entries = 68
#else
.label _encoded_entries = 52
#endif

.label _tabl_bi = exod_decrunch_table
.label _tabl_lo = exod_decrunch_table + _encoded_entries
.label _tabl_hi = exod_decrunch_table + _encoded_entries * 2

        //// refill bits is always inlined
.macro exod_mac_refill_bits() {
        pha
        jsr exod_get_crunched_byte
        rol
        sta exod_zp_bitbuf
        pla
}

.macro exod_mac_get_bits() {
#if INLINE_GET_BITS
        adc #$80                // needs c=0, affects v
        asl
        bpl _gb_skip
_gb_next:
        asl exod_zp_bitbuf
        bne _gb_ok
        :exod_mac_refill_bits()
_gb_ok:
        rol
        bmi _gb_next
_gb_skip:
        bvc _skip
_gb_get_hi:
        sec
        sta exod_zp_bits_hi
        jsr exod_get_crunched_byte
_skip:
#else
        jsr exod_get_bits
#endif
}

.macro exod_mac_init_zp() {
// -------------------------------------------------------------------
// init zeropage and x reg. (8 bytes)
//
_init_zp:
        jsr exod_get_crunched_byte
        sta exod_zp_bitbuf - 1,x
        dex
        bne _init_zp
}

#if !INLINE_GET_BITS
exod_get_bits:
        adc #$80                // needs c=0, affects v
        asl
        bpl _gb_skip
_gb_next:
        asl exod_zp_bitbuf
        bne _gb_ok
        :exod_mac_refill_bits()
_gb_ok:
        rol
        bmi _gb_next
_gb_skip:
        bvs _gb_get_hi
        rts
_gb_get_hi:
        sec
        sta exod_zp_bits_hi
        jmp exod_get_crunched_byte
#endif
// -------------------------------------------------------------------
// no code below this comment has to be modified in order to generate
// a working decruncher of this source file.
// However, you may want to relocate the tables last in the file to a
// more suitable address.
// -------------------------------------------------------------------

// -------------------------------------------------------------------
// jsr this label to decrunch, it will in turn init the tables and
// call the decruncher
// no constraints on register content, however the
// decimal flag has to be cleared (it almost always is, otherwise do a cld)
exod_decrunch:
#if ENABLE_SPLIT_ENCODING
        ldx #3
        jsr _internal_gentable
        jmp _normal_decrunch
exod_split_gentable:
        ldx #1
_internal_gentable:
        jsr _split_init_zp
#else
        ldx #3
        :exod_mac_init_zp()
#endif
// -------------------------------------------------------------------
// calculate tables (64 bytes) + get_bits macro
// x and y must be #0 when entering
//
        ldy #0
        clc
_table_gen:
        tax
        tya
        and #$0f
        sta _tabl_lo,y
        beq _shortcut            // start a new sequence
// -------------------------------------------------------------------
        txa
        adc _tabl_lo - 1,y
        sta _tabl_lo,y
        lda exod_zp_len_hi
        adc _tabl_hi - 1,y
_shortcut:
        sta _tabl_hi,y
// -------------------------------------------------------------------
        lda #$01
        sta <exod_zp_len_hi
        lda #$78                // %01111000
        :exod_mac_get_bits()
// -------------------------------------------------------------------
        lsr
        tax
        beq _rolled
        php
_rolle:
        asl exod_zp_len_hi
        sec
        ror
        dex
        bne _rolle
        plp
_rolled:
        ror
        sta _tabl_bi,y
        bmi _no_fixup_lohi
        lda exod_zp_len_hi
        stx exod_zp_len_hi
        .byte $24
_no_fixup_lohi:
        txa
// -------------------------------------------------------------------
        iny
        cpy #_encoded_entries
        bne _table_gen
// -------------------------------------------------------------------
#if ENABLE_SPLIT_ENCODING
        rts
exod_split_decrunch:
        ldx #3
        jsr _split_init_zp
// X reg must be 0 here
        sec
_normal_decrunch:
#endif
// -------------------------------------------------------------------
// prepare for main decruncher
#if !DONT_REUSE_OFFSET
        ror exod_zp_ro_state
        sec
#endif
        ldy exod_zp_dest_lo
        stx exod_zp_dest_lo
        stx exod_zp_bits_hi
// -------------------------------------------------------------------
// copy one literal byte to destination (11 bytes)
//
_literal_start1:
#if !DECRUNCH_FORWARDS
        tya
        bne _no_hi_decr
        dec exod_zp_dest_hi
#if !DONT_REUSE_OFFSET
        dec exod_zp_src_hi
#endif
_no_hi_decr:
        dey
#endif
        jsr exod_get_crunched_byte
        sta (exod_zp_dest_lo),y
#if DECRUNCH_FORWARDS
        iny
        bne skip_hi_incr
        inc exod_zp_dest_hi
#if !DONT_REUSE_OFFSET
        inc exod_zp_src_hi
#endif
skip_hi_incr:
#endif
// -------------------------------------------------------------------
// fetch sequence length index (15 bytes)
// x must be #0 when entering and contains the length index + 1
// when exiting or 0 for literal byte
_next_round:
#if !DONT_REUSE_OFFSET
        ror exod_zp_ro_state
#endif
        dex
        lda exod_zp_bitbuf
_no_literal1:
        asl
        bne _nofetch8
        jsr exod_get_crunched_byte
        rol
_nofetch8:
        inx
        bcc _no_literal1
        sta exod_zp_bitbuf
// -------------------------------------------------------------------
// check for literal byte (2 bytes)
//
        beq _literal_start1
// -------------------------------------------------------------------
// check for decrunch done and literal sequences (4 bytes)
//
        cpx #$11
#if INLINE_GET_BITS
        bcc _skip_jmp
        jmp _exit_or_lit_seq
_skip_jmp:
#else
        bcs _exit_or_lit_seq
#endif
// -------------------------------------------------------------------
// calulate length of sequence (zp_len) (18(11) bytes) + get_bits macro
//
        lda _tabl_bi - 1,x
        :exod_mac_get_bits()
        adc _tabl_lo - 1,x       // we have now calculated zp_len_lo
        sta exod_zp_len_lo
#if !MAX_SEQUENCE_LENGTH_256
        lda exod_zp_bits_hi
        adc _tabl_hi - 1,x       // c = 0 after this.
        sta exod_zp_len_hi
// -------------------------------------------------------------------
// here we decide what offset table to use (27(26) bytes) + get_bits_nc macro
// z-flag reflects zp_len_hi here
//
        ldx exod_zp_len_lo
#else
        tax
#endif
#if !MAX_SEQUENCE_LENGTH_256
        lda #0
#endif
#if !DONT_REUSE_OFFSET
// -------------------------------------------------------------------
// here we decide to reuse latest offset or not (13(15) bytes)
//
        bit <exod_zp_ro_state
        bmi _test_reuse
_no_reuse:
#endif
// -------------------------------------------------------------------
// here we decide what offset table to use (17(15) bytes)
//
#if !MAX_SEQUENCE_LENGTH_256
        sta <exod_zp_bits_hi
#endif
        lda #$e1
#if EXTRA_TABLE_ENTRY_FOR_LENGTH_THREE
        cpx #$04
#else
        cpx #$03
#endif
        bcs _gbnc2_next
        lda _tabl_bit - 1,x
_gbnc2_next:
        asl exod_zp_bitbuf
        bne _gbnc2_ok
        tax
        jsr exod_get_crunched_byte
        rol
        sta exod_zp_bitbuf
        txa
_gbnc2_ok:
        rol
        bcs _gbnc2_next
        tax
// -------------------------------------------------------------------
// calulate absolute offset (zp_src) (17 bytes) + get_bits macro
//
        lda _tabl_bi,x
        :exod_mac_get_bits()
#if !DECRUNCH_FORWARDS
        adc _tabl_lo,x
        sta exod_zp_src_lo
        lda exod_zp_bits_hi
        adc _tabl_hi,x
        adc exod_zp_dest_hi
        sta exod_zp_src_hi
#else
        clc
        adc _tabl_lo,x
        eor #$ff
        sta exod_zp_src_lo
        lda exod_zp_bits_hi
        adc _tabl_hi,x
        eor #$ff
        adc exod_zp_dest_hi
        sta exod_zp_src_hi
        clc
#endif
// -------------------------------------------------------------------
// prepare for copy loop (2 bytes)
//
        ldx exod_zp_len_lo
// -------------------------------------------------------------------
// main copy loop (30 bytes)
//
_copy_next:
#if !DECRUNCH_FORWARDS
        tya
        bne _copy_skip_hi
        dec exod_zp_dest_hi
        dec exod_zp_src_hi
_copy_skip_hi:
        dey
#endif
#if !LITERAL_SEQUENCES_NOT_USED
        bcs _get_literal_byte
#endif
        lda (exod_zp_src_lo),y
_literal_byte_gotten:
        sta (exod_zp_dest_lo),y
#if DECRUNCH_FORWARDS
        iny
        bne copy_skip_hi
        inc exod_zp_dest_hi
        inc exod_zp_src_hi
copy_skip_hi:
#endif
        dex
        bne _copy_next
#if !MAX_SEQUENCE_LENGTH_256
        lda exod_zp_len_hi
#if INLINE_GET_BITS
        bne _copy_next_hi
#endif
#endif
        stx exod_zp_bits_hi
#if !INLINE_GET_BITS
        beq _next_round
#else
        jmp _next_round
#endif
#if !MAX_SEQUENCE_LENGTH_256
_copy_next_hi:
        dec exod_zp_len_hi
        jmp _copy_next
#endif
#if !DONT_REUSE_OFFSET
// -------------------------------------------------------------------
// test for offset reuse (11 bytes)
//
_test_reuse:
        bvs _no_reuse
#if MAX_SEQUENCE_LENGTH_256
        lda #$00                // fetch one bit
#endif
        asl exod_zp_bitbuf
        bne _gbnc1_ok
        pha
        jsr exod_get_crunched_byte
        rol
        sta exod_zp_bitbuf
        pla
_gbnc1_ok:
        rol
        beq _no_reuse            // bit == 0 => C=0, no reuse
        bne _copy_next           // bit != 0 => C=0, reuse previous offset
#endif
// -------------------------------------------------------------------
// exit or literal sequence handling (16(12) bytes)
//
_exit_or_lit_seq:
#if !LITERAL_SEQUENCES_NOT_USED
        beq _decr_exit
        jsr exod_get_crunched_byte
#if !MAX_SEQUENCE_LENGTH_256
        sta exod_zp_len_hi
#endif
        jsr exod_get_crunched_byte
        tax
        bcs _copy_next
_decr_exit:
#endif
        rts
#if !LITERAL_SEQUENCES_NOT_USED
_get_literal_byte:
        jsr exod_get_crunched_byte
        bcs _literal_byte_gotten
#endif
#if EXTRA_TABLE_ENTRY_FOR_LENGTH_THREE
// -------------------------------------------------------------------
// the static stable used for bits+offset for lengths 1, 2 and 3 (3 bytes)
// bits 2, 4, 4 and offsets 64, 48, 32 corresponding to
// %10010000, %11100011, %11100010
_tabl_bit:
        .byte $90, $e3, $e2
#else
// -------------------------------------------------------------------
// the static stable used for bits+offset for lengths 1 and 2 (2 bytes)
// bits 2, 4 and offsets 48, 32 corresponding to %10001100, %11100010
_tabl_bit:
        .byte $8c, $e2
#endif

#if ENABLE_SPLIT_ENCODING
_split_init_zp:
        :exod_mac_init_zp()
        rts
#endif
// -------------------------------------------------------------------
// end of decruncher
// -------------------------------------------------------------------

// -------------------------------------------------------------------
// this 156 (204) byte table area may be relocated. It may also be
// clobbered by other data between decrunches.
// -------------------------------------------------------------------
exod_decrunch_table:
        .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
#if EXTRA_TABLE_ENTRY_FOR_LENGTH_THREE
        .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
#endif
        .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .byte 0,0,0,0,0,0,0,0,0,0,0,0
// -------------------------------------------------------------------
// end of decruncher
// -------------------------------------------------------------------
