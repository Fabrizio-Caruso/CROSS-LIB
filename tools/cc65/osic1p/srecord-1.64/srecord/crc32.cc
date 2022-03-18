//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2002, 2006-2010 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see
// <http://www.gnu.org/licenses/>.
//
// (I got this copy from http://www.gator.net/~garyg/C/CONTRIB/SNIP/
// but as always on the Internet, it is now gone.  So use the
//      Wayback Machine, http://www.archive.org/, or google for "crc_32"
//      and you should find it fairly quickly.  The same code also
//      appears in the ZModem sources, as well, but they seem to abuse
//      it slightly, and the way they initialize and terminate the CRC
//      doesn't give the same answers as the way everyone else uses it.)
//
// 32 BIT ANSI X3.66 CRC checksum
//
// This class may be used to compute the 32-bit CRC used as the frame
// check sequence in ADCCP (ANSI X3.66, also known as FIPS PUB 71 and
// FED-STD-1003, the U.S. versions of CCITT's X.25 link-level protocol).
// The 32-bit FCS was added via the Federal Register, 1 June 1982,
// p.23798.  I presume but don't know for certain that this polynomial
// is or will be included in CCITT V.41, which defines the 16-bit CRC
// (often called CRC-CCITT) polynomial.  FIPS PUB 78 says that the
// 32-bit FCS reduces otherwise undetected errors by a factor of 10^-5
// over 16-bit FCS.
//


#include <srecord/crc32.h>


static unsigned long ccitt_seed = 0xFFFFFFFF;
static unsigned long xmodem_seed = 0;


//
// Portions of the code in this file are derived from code which is
// "Copyright (C) 1986 Gary S. Brown.  You may use this program, or
// code or tables extracted from it, as desired without restriction."
//
// First, the polynomial itself and its table of feedback terms.  The
// polynomial is
// X^32+X^26+X^23+X^22+X^16+X^12+X^11+X^10+X^8+X^7+X^5+X^4+X^2+X^1+X^0
// Note that we take it "backwards" and put the highest-order term in
// the lowest-order bit.  The X^32 term is "implied"; the LSB is the
// X^31 term, etc.  The X^0 term (usually shown as "+1") results in
// the MSB being 1.
//
#define POLYNOMIAL 0xedb88320

//
// Note that the usual hardware shift register implementation, which
// is what we're using (we're merely optimizing it by doing eight-bit
// chunks at a time) shifts bits into the lowest-order term.  In our
// implementation, that means shifting towards the right.  Why do we
// do it this way?  Because the calculated CRC must be transmitted in
// order from highest-order term to lowest-order term.  UARTs transmit
// characters in order from LSB to MSB.  By storing the CRC this way,
// we hand it to the UART in the order low-byte to high-byte; the UART
// sends each low-bit to hight-bit; and the result is transmission bit
// by bit from highest- to lowest-order term without requiring any bit
// shuffling on our part.  Reception works similarly.
//
// The feedback terms table consists of 256, 32-bit entries.
//
// The values must be right-shifted by eight bits by the "UPDC32"
// logic; the shift must be unsigned (bring in zeroes).
//

static unsigned long table[256];


static void
calculate_table()
{
    for (unsigned b = 0; b < 256; ++b)
    {
        unsigned long v = b;
        int i = 8;
        for (; --i >= 0; )
            v = (v & 1) ? ((v >> 1) ^ POLYNOMIAL) : (v >> 1);
        table[b] = v;
    }
}


static unsigned long
initial_state_from_seed_mode(srecord::crc32::seed_mode_t seed_mode)
{
    switch (seed_mode)
    {
    case srecord::crc32::seed_mode_ccitt:
        return ccitt_seed;

    case srecord::crc32::seed_mode_xmodem:
        return xmodem_seed;
    }
    return ccitt_seed;
}


srecord::crc32::crc32(seed_mode_t seed_mode) :
    state(initial_state_from_seed_mode(seed_mode))
{
    if (!table[1])
        calculate_table();
}


srecord::crc32::crc32(const crc32 &arg) :
    state(arg.state)
{
}


srecord::crc32 &
srecord::crc32::operator=(const crc32 &arg)
{
    if (this != &arg)
    {
        state = arg.state;
    }
    return *this;
}


srecord::crc32::~crc32()
{
}


static inline unsigned long
UPDC32(unsigned char octet, unsigned long crc)
{
    // The original code had this as a #define
    return table[(crc ^ octet) & 0xFF] ^ (crc >> 8);
}


void
srecord::crc32::next(unsigned char x)
{
    state = UPDC32(x, state);
}


void
srecord::crc32::nextbuf(const void *data, size_t nbytes)
{
    const unsigned char *dp = (const unsigned char *)data;
    while (nbytes > 0)
    {
        state = UPDC32(*dp, state);
        ++dp;
        --nbytes;
    }
}


unsigned long
srecord::crc32::get()
    const
{
#if 1
    return ~state;
#else
    //
    // The crc_32.c program floating around on the Internet prints
    // two numbers.  The first is calculated as follows (the second
    // is the CRC as returned 5 lines back).  It appears to be an
    // attempt to embed the crc into the data, and tell you what
    // the CRC should be if you calculate the CRC over the data and
    // the CRC.  However, it makes the assumption that you store
    // the CRC little-endian, and it doesn't do the final bit-not.
    //
    // To simulate this (or something very much like it) try
    //      srecord::cat <file> -lecrc32 <addr> -lecrc32 <addr+4>
    //
    unsigned long temp = state;
    temp = UPDC32( ~state    & 0xFF, temp);
    temp = UPDC32((~state >>  8) & 0xFF, temp);
    temp = UPDC32((~state >> 16) & 0xFF, temp);
    temp = UPDC32((~state >> 24) & 0xFF, temp);
    return temp; // I wonder why this isn't bit-not-ed?
#endif
}
