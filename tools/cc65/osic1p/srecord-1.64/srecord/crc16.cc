//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2002, 2006-2010, 2012, 2014 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//
//
// Implemented from scratch from
// "A painless guide to CRC error detection algorithms"
// http://www.repairfaq.org/filipg/LINK/F_crc_v3.html
//
// See also http://www.joegeluso.com/software/articles/ccitt.htm (gone)
//          http://srecord.sourceforge.net/crc16-ccitt.html (duplicate)
//                         etc/crc16-ccitt.html (in the source tarball).
//
// See test/01/t0150a.sh for test vectors.
//

#include <cstring>
#include <string>

#include <srecord/bitrev.h>
#include <srecord/crc16.h>
#include <srecord/quit.h>
#include <srecord/sizeof.h>


//
// The Chapter numbers come from Williams, N. (1993) "A painless guide
// to CRC error detection algorithms"
// http://www.repairfaq.org/filipg/LINK/F_crc_v3.html
//
#define IMPL_CH9   9
#define IMPL_CH10 10
#define IMPL_CH11 11    // doesn't work yet (i.e. not same results as 9 and 10)
                        // but it "almost" works, according to test/01/t0150a.sh

// So, for now we will be conservative, and use Chapter 10
#define IMPL IMPL_CH10


//
// Use a seed of 0xFFFF when augmenting manually (i.e. augmenting by 16
// zero bits by processing two zero bytes at the end of the data), but a
// seed of 0x1D0F when the augmenting is done by shifting where the XOR
// appears in the updcrc function.
//
// The 0x1D0F value is calculated by using the manual augmentation
// updcrc function:
//     updcrc(0, updcrc(0, 0xFFFF))
//
static unsigned short const ccitt_seed = 0xFFFF;
static unsigned short const broken_seed = 0x84CF;
static unsigned short const xmodem_seed = 0;


void
srecord::crc16::calculate_table(void)
{
    if (polynomial == 0)
        polynomial = polynomial_ccitt;
    if (bitdir == bit_direction_most_to_least)
    {
        for (unsigned b = 0; b < 256; ++b)
        {
            unsigned short v = b << 8;
            for (unsigned j = 0; j < 8; ++j)
                v = (v & 0x8000) ? ((v << 1) ^ polynomial) : (v << 1);
            table[b] = v;
        }
    }
    else
    {
        polynomial = bitrev16(polynomial);
        for (unsigned b = 0; b < 256; ++b)
        {
            unsigned short v = b;
            for (unsigned j = 0; j < 8; ++j)
                v = (v & 1) ? ((v >> 1) ^ polynomial) : (v >> 1);
            table[b] = v;
        }
    }
}


static int
state_from_seed_mode(srecord::crc16::seed_mode_t seed_mode)
{
    switch (seed_mode)
    {
    case srecord::crc16::seed_mode_ccitt:
        return ccitt_seed;

    case srecord::crc16::seed_mode_xmodem:
        return xmodem_seed;

    case srecord::crc16::seed_mode_broken:
        return broken_seed;
    }
    return ccitt_seed;
}


srecord::crc16::crc16(
    seed_mode_t seed_mode,
    bool a_augment,
    unsigned short a_polynomial,
    bit_direction_t a_bitdir
) :
    state(state_from_seed_mode(seed_mode)),
    augment(a_augment),
    polynomial(a_polynomial),
    bitdir(a_bitdir)
{
#if (IMPL == IMPL_CH11)
  #if 1
    // the "pre-augment" crc seed
    state = 0x1D0F;
  #else
    // The above nukes the seed value the user may have provided,
    // making this look more likely:
    state = updcrc(0, updcrc(0, state));
  #endif
#endif
    calculate_table();
}


srecord::crc16::crc16(const crc16 &rhs) :
    state(rhs.state),
    augment(rhs.augment),
    polynomial(rhs.polynomial),
    bitdir(rhs.bitdir)
{
    for (size_t j = 0; j < 256; ++j)
        table[j] = rhs.table[j];
}


srecord::crc16 &
srecord::crc16::operator=(const crc16 &rhs)
{
    if (this != &rhs)
    {
        state = rhs.state;
        augment = rhs.augment;
        polynomial = rhs.polynomial;
        bitdir = rhs.bitdir;
        for (size_t j = 0; j < 256; ++j)
            table[j] = rhs.table[j];
    }
    return *this;
}


srecord::crc16::~crc16()
{
}


#if (IMPL == IMPL_CH9)

//
// This is the simplest possible implementation.  It can be used to
// validate the two following table-driven implementations.
//
// See "A painless guide to CRC error detection algorithms",
// Chapter 9, http://www.repairfaq.org/filipg/LINK/F_crc_v33.html#CRCV_001
// 'A Straightforward CRC Implementation', for an explanation.
//

inline unsigned short
srecord::crc16::updcrc(unsigned char c, unsigned short state)
    const
{
    if (bitdir == bit_direction_most_to_least)
    {
        for (unsigned i = 0; i < 8; ++i)
        {
            bool xor_flag = !!(state & 0x8000);
            state <<= 1;
            if (c & 0x80)
                state |= 1;
            if (xor_flag)
                state ^= polynomial;
            c <<= 1;
        }
    }
    else
    {
        // note: calculate_table() already reversed the bits in the polynomial
        for (unsigned i = 0; i < 8; ++i)
        {
            bool xor_flag = !!(state & 1);
            state >>= 1;
            if (c & 1)
                state |= 0x8000;
            if (xor_flag)
                state ^= polynomial;
            c >>= 1;
        }
    }
    return state;
}

#endif // IMPL_CH9
#if (IMPL == IMPL_CH10)

//
// This version of updcrc doesn't augment automagically, you must
// do it explicitly in the get() method.  It is a more intuitave
// implementation than the "augmentation included" implementation below.
//
// See "A painless guide to CRC error detection algorithms",
// Chapter 10, http://www.repairfaq.org/filipg/LINK/F_crc_v33.html#CRCV_002
// 'A Table-Driven Implementation', for an explanation.
//

inline unsigned short
srecord::crc16::updcrc(unsigned char c, unsigned short state)
    const
{
    if (bitdir == bit_direction_least_to_most)
    {
        return (((state >> 8) & 0xFF) | (c << 8)) ^ table[state & 0xFF];
    }
    else
    {
        return ((state << 8) | c) ^ table[state >> 8];
    }
}

#endif // IMPL_CH10
#if (IMPL == IMPL_CH11)

//
// This version of updcrc means that the 16-zero-bit augmentation has
// already happened.  There is no need to explicitly do it in the get()
// method.  This is deep voodoo even for folks who actually understand
// XOR arithmetic.
//
// See "A painless guide to CRC error detection algorithms",
// Chapter 11, http://www.repairfaq.org/filipg/LINK/F_crc_v33.html#CRCV_003
// 'A Slightly Mangled Table-Driven Implementation', for an explanation.
//

inline unsigned short
srecord::crc16::updcrc(unsigned char c, unsigned short state)
    const
{
    if (bitdir == bit_direction_least_to_most)
        return (state >> 8) ^ table[(state ^ c) & 0xFF];
    else
        return (state << 8) ^ table[((state >> 8) ^ c) & 0xFF];
}

#endif // IMPL_CH11


void
srecord::crc16::next(unsigned char ch)
{
    state = updcrc(ch, state);
}


void
srecord::crc16::nextbuf(const void *data, size_t nbytes)
{
    unsigned char *dp = (unsigned char *)data;
    while (nbytes > 0)
    {
        state = updcrc(*dp++, state);
        --nbytes;
    }
}


unsigned short
srecord::crc16::get(void)
    const
{
#if (IMPL < IMPL_CH11)
    // The whole idea is that Ch.11 technique is "pre-auugmented"
    if (augment)
    {
        return updcrc(0, updcrc(0, state));
    }
#endif
    return state;
}


#include <cstdio>


void
srecord::crc16::print_table(void)
    const
{
    printf("/*\n");
    printf
    (
        " * Bit order: %s\n",
        (
            bitdir == bit_direction_most_to_least
        ?
            "most to least"
        :
            "least to most"
        )
    );
    printf(" * Polynomial: 0x");
    if (bitdir == bit_direction_most_to_least)
        printf("%04X", polynomial);
    else
        printf("%04X", bitrev16(polynomial));
    printf("\n */\n");
    printf("const unsigned short table[256] =\n{\n");
    for (size_t j = 0; j < 256; ++j)
    {
        if ((j & 7) == 0)
            printf("    /* %02X */", int(j));
        printf(" 0x%04X,", table[j]);
        if ((j & 7) == 7)
            printf("\n");
    }
    printf("};\n");
}


int
srecord::crc16::polynomial_by_name(const char *name)
{
    struct table_t
    {
        const char *name;
        int value;
    };

    static const table_t table[] =
    {
        { "ansi", polynomial_ansi },
        { "bisync", polynomial_ansi },
        { "bluetooth", polynomial_ccitt },
        { "ccitt", polynomial_ccitt },
        { "dnp", polynomial_dnp },
        { "hdlc", polynomial_ccitt },
        { "ibm", polynomial_ansi },
        { "iec-870", polynomial_dnp },
        { "m-bus", polynomial_dnp },
        { "modbus", polynomial_ansi },
        { "scsi-dif", polynomial_t10_dif },
        { "sd", polynomial_ccitt },
        { "t10-dif", polynomial_t10_dif },
        { "usb", polynomial_ansi },
        { "v.41", polynomial_ccitt },
        { "x.25", polynomial_ccitt },
        { "x3.28", polynomial_ansi },
        { "xmodem", polynomial_ccitt },
    };

    std::string names;
    for (const table_t *tp = table; tp < ENDOF(table); ++tp)
    {
        if (0 == strcasecmp(name, tp->name))
            return tp->value;
        if (!names.empty())
            names += ", ";
        names += tp->name;
    }

    quit_default.fatal_error
    (
        "CRC-16 polynomial name \"%s\" unknown (known names are %s)",
        name,
        names.c_str()
    );
    return polynomial_ccitt;
}


// vim: set ts=8 sw=4 et :
