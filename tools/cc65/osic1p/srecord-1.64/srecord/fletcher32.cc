//
// srecord - Manipulate EPROM load files
// Copyright (C) 2009-2011 Peter Miller
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
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <srecord/fletcher32.h>


srecord::fletcher32::~fletcher32()
{
}


srecord::fletcher32::fletcher32() :
    sum1(0xFFFF),
    sum2(0xFFFF)
{
    // See notes in the nextbuf() method, below.
}


srecord::fletcher32::fletcher32(const fletcher32 &rhs) :
    sum1(rhs.sum1),
    sum2(rhs.sum2)
{
}


srecord::fletcher32 &
srecord::fletcher32::operator=(const fletcher32 &rhs)
{
    if (this != &rhs)
    {
        sum1 = rhs.sum1;
        sum2 = rhs.sum2;
    }
    return *this;
}


void
srecord::fletcher32::next(unsigned char x)
{
    sum1 += x;
    sum2 += sum1;
    // reduction step to reduce sums to 16 bits
    sum1 = (sum1 & 0xFFFF) + (sum1 >> 16);
    sum2 = (sum2 & 0xFFFF) + (sum2 >> 16);
}


void
srecord::fletcher32::nextbuf(const void *vdata, size_t nbytes)
{
    //
    // A few tricks, well-known to implementors of the IP checksum, are
    // used here for efficiency:
    //
    // * This reduces to the range 1..65535 rather than 0..65534.
    // Modulo 65535, the values 65535 = 0xFFFF and 0 are equivalent, but
    // it is easier to detect overflow if the former convention is used.
    // This also provides the guarantee that the resultant checksum will
    // never be zero, so that value is available for a special flag,
    // such as "checksum not yet computed".
    //
    // * 65536 ≡ 1 mod 65535, so the end-around carry expression (x
    // & 0xFFFF) + (x >> 16) reduces x modulo 65535.  Only doing it
    // once is not guaranteed to be complete, but it will be less than
    // 0x1FFFE.  A second repetition guarantees a fully reduced sum in
    // the range of 1..65535.
    //
    // * This uses a 32-bit accumulator to perform a number of sums
    // before doing any modular reduction.  The magic value 360 is the
    // largest number of sums that can be performed without numeric
    // overflow.  Any smaller value is also permissible; 256 may be
    // convenient in many cases.
    //
    const unsigned char *data = (const unsigned char *)vdata;
    size_t len = nbytes;
    while (len)
    {
        unsigned tlen = len > 360 ? 360 : len;
        len -= tlen;
        for (;;)
        {
            sum1 += *data++;
            sum2 += sum1;
            --tlen;
            if (!tlen)
                break;
        }
        sum1 = (sum1 & 0xFFFF) + (sum1 >> 16);
        sum2 = (sum2 & 0xFFFF) + (sum2 >> 16);
    }
    // Second reduction step to reduce sums to 16 bits
    sum1 = (sum1 & 0xFFFF) + (sum1 >> 16);
    sum2 = (sum2 & 0xFFFF) + (sum2 >> 16);
}


unsigned long
srecord::fletcher32::get()
    const
{
    return ((sum2 << 16) | sum1);
}


// vim: set ts=8 sw=4 et :
