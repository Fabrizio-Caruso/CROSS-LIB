//
// srecord - manipulate eprom load files
// Copyright (C) 2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
// for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//
//      From: Hendrik Beijeman <hbeyeman@gmail.com>
//      To: pmiller@opensource.org.au
//      Subject: Re: patch for srecord 1.59
//      Date: Sat, 05 May 2012 06:26:02 +0200 (05/05/12 14:26:02)
//
//      I hereby formally assign all copyright to the author of srecord,
//      Peter Miller.
//

#include <iostream>
#include <srecord/stm32.h>

//
// This code is mostly copied from the CRC32 implementation. The only
// difference is that the STM32 operates on WORDs whereas the CRC32 is
// byte-oriented.  In addition the STM32 uses a different polynomial.
//
#define POLYNOMIAL 0x04C11DB7


srecord::stm32::~stm32()
{
}


srecord::stm32::stm32() :
    state(0xFFFFFFFF),
    cnt(0)
{
}


srecord::stm32::stm32(const stm32 &arg) :
    state(arg.state),
    cnt(0)
{
    while (cnt < arg.cnt)
    {
        buf[cnt] = arg.buf[cnt];
        ++cnt;
    }
}


srecord::stm32 &
srecord::stm32::operator=(const stm32 &arg)
{
    if (this != &arg)
    {
        state = arg.state;

        cnt = 0;
        while (cnt < arg.cnt)
        {
            buf[cnt] = arg.buf[cnt];
            ++cnt;
        }
    }
    return *this;
}


//
// The STM32 hardware CRC calculation uses CRC polynomial 0x04C11DB7
// and operates only on words.
//
static unsigned long
stm32_crc(unsigned long crc, unsigned long data)
{
    crc ^= data;
    for (int j = 0; j < 32; ++j)
    {
        if (crc & 0x80000000)
            crc = (crc << 1) ^ POLYNOMIAL;
        else
            crc = (crc << 1);
    }
    return crc;
}


void
srecord::stm32::generator(void)
{
    unsigned long data = 0;
    for (size_t j = 0; j < wordsize; j++)
        data |= (buf[j] << (8 * j));
    state = stm32_crc(state, data);
    cnt = 0;
}


void
srecord::stm32::next(unsigned char x)
{
    buf[cnt++] = x;
    if (cnt == wordsize)
        generator();
}


void
srecord::stm32::nextbuf(const void *data, size_t nbytes)
{
    const unsigned char *dp = (const unsigned char *)data;
    while (nbytes > 0)
    {
        --nbytes;
        next(*dp++);
    }
}


unsigned long
srecord::stm32::get(void)
    const
{
    return state;
}


// vim: set ts=8 sw=4 et :
