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
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <cassert>

#include <srecord/fletcher16.h>

//
// http://en.wikipedia.org/wiki/Fletcher%27s_checksum
// RFC 1146
// RFC 905 Annex B
//


srecord::fletcher16::~fletcher16()
{
}


srecord::fletcher16::fletcher16(
    unsigned char a_sum1,
    unsigned char a_sum2,
    int a_answer,
    endian_t a_end
) :
    sum1(a_sum1),
    sum2(a_sum2),
    answer(a_answer),
    end(a_end)
{
    // because 0 == 255 mod 255, adjust accordingly
    if (sum1 == 0xFF)
        sum1 = 0;
    if (sum2 == 0xFF)
        sum2 = 0;
    if (answer >= 0)
    {
        if ((answer & 0x00FF) == 0x00FF)
            answer &= 0xFF00;
        if ((answer & 0xFF00) == 0xFF00)
            answer &= 0x00FF;
    }
}


srecord::fletcher16::fletcher16(const fletcher16 &rhs) :
    sum1(rhs.sum1),
    sum2(rhs.sum2),
    answer(rhs.answer),
    end(rhs.end)
{
}


srecord::fletcher16 &
srecord::fletcher16::operator=(const fletcher16 &rhs)
{
    if (this != &rhs)
    {
        sum1 = rhs.sum1;
        sum2 = rhs.sum2;
        answer = rhs.answer;
        end = rhs.end;
    }
    return *this;
}


void
srecord::fletcher16::next(unsigned char ch)
{
    // reduction step to reduce sums to 8 bits
    sum1 += ch;
    sum2 += sum1;
    sum1 = (sum1 & 0xFF) + (sum1 >> 8);
    sum2 = (sum2 & 0xFF) + (sum2 >> 8);
}


void
srecord::fletcher16::nextbuf(const void *vdata, size_t nbytes)
{
    //
    // A few tricks, well-known to implementors of the IP checksum, are
    // used here for efficiency:
    //
    // * This reduces to the range 1..255 rather than 0..254.  Modulo
    //   255, the values 255 = 0xFF and 0 are equivalent, but it is easier
    //   to detect overflow if the former convention is used.  This also
    //   provides the guarantee that the resultant checksum will never
    //   be zero, so that value is available for a special flag, such as
    //   "checksum not yet computed".
    //
    // * 256 = 1 mod 255, so the end-around carry expression (x &
    //   0xFF) + (x >> 8) reduces x modulo 255.  Only doing it once is not
    //   guaranteed to be complete, but it will be less than 0x1fe.  A
    //   second repetition guarantees a fully reduced sum in the range of
    //   1..255.
    //
    // * This uses a 16-bit accumulator to perform a number of sums
    //   before doing any modular reduction. The magic value 21 is the
    //   largest number of sums that can be performed without numeric
    //   overflow in 16 bits. Any smaller value is also permissible; 16
    //   may be convenient in many cases.
    //
    const unsigned char *data = (const unsigned char *)vdata;
    size_t len = nbytes;
    while (len)
    {
        size_t tlen = len > 21 ? 21 : len;
        len -= tlen;
        for (;;)
        {
            sum1 += *data++;
            sum2 += sum1;
            --tlen;
            if (!tlen)
                break;
        }
        sum1 = (sum1 & 0xFF) + (sum1 >> 8);
        sum2 = (sum2 & 0xFF) + (sum2 >> 8);
    }
    // Second reduction step to reduce sums to 8 bits
    sum1 = (sum1 & 0xFF) + (sum1 >> 8);
    sum2 = (sum2 & 0xFF) + (sum2 >> 8);
}


unsigned short
srecord::fletcher16::get()
    const
{
    //
    // An empty "answer" on command line defaults to answer == -1
    // so, in this case, don't calculate adjusted sums.
    //
    if (answer >= 0)
    {
        //
        // Adjust checksum so it results in "answer" when the checksum
        // itself is included in firmware's fletcher sum.  Typical final
        // value would be 0x0000 so firmware's fletcher result of zero
        // indicates good eprom when adjusted sum place in last two
        // bytes of eeprom.  (Note: 0xFF in final byte (f1 or f2) is
        // equivalent to 0x00 due to modulo 255 arithmetic.)
        //
        unsigned char f2;
        unsigned char f1;
        if (end == endian_big)
        {
            f1 = answer >> 8;
            f2 = answer;
        }
        else
        {
            f1 = answer;
            f2 = answer >> 8;
        }

#if 1
        // The following code assumes sum1a and sum2a are 16-bits, or
        // more.  If you make them char or unsigned char, you will get
        // the wrong answer.
        unsigned short sum1a = sum1;
        unsigned short sum2a = sum2;

        int tmp = int(f2) - int(f1) - int(sum2a) - int(sum1a);
        tmp = (tmp & 0xff) + (tmp >> 8);
        sum2a = f1 - sum1a - tmp;
        sum2a = (sum2a & 0xff) + (sum2a >> 8);
        sum1a = tmp;

        return (((sum1a & 0xFF) << 8) | (sum2a & 0xFF));
#else
        // exhaustive search
        for (unsigned short c1 = 0; c1 < 256; ++c1)
        {
            unsigned short sum1a = sum1 + c1;
            unsigned short sum2a = sum2 + sum1a;
            sum1a = (sum1a & 0xFF) + (sum1a >> 8);
            sum2a = (sum2a & 0xFF) + (sum2a >> 8);

            for (unsigned short c2 = 0; c2 < 256; ++c2)
            {

                unsigned short sum1b = sum1a + c2;
                unsigned short sum2b = sum2a + sum1b;
                sum1b = (sum1b & 0xFF) + (sum1b >> 8);
                sum2b = (sum2b & 0xFF) + (sum2b >> 8);

                sum1b &= 0xFF;
                sum2b &= 0xFF;

                if (f1 == sum1b && f2 == sum2b)
                    return ((sum1b << 8) | sum2b);
            }
        }
        // This is common when one or both of f1 and f2 are zero.
        assert(!"exhaustive search failed");
#endif
    }

    return (((sum1 & 0xFF) << 8) | (sum2 & 0xFF));
}

// vim: set ts=8 sw=4 et :
