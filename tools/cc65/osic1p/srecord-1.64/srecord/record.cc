//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2001, 2002, 2006-2011 Peter Miller
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


#include <cassert>
#include <cstring>

#include <srecord/record.h>


srecord::record::record():
    type(type_unknown),
    address(0),
    length(0)
{
}


srecord::record::record(const srecord::record &arg) :
    type(arg.type),
    address(arg.address),
    length(arg.length)
{
    if (arg.length > 0)
        memcpy(data, arg.data, arg.length);
}


srecord::record::record(type_t arg) :
    type(arg),
    address(0),
    length(0)
{
}


srecord::record::record(type_t a1, address_t a2) :
    type(a1),
    address(a2),
    length(0)
{
}


srecord::record::record(type_t a1, address_t a2, const data_t *a3, size_t a4) :
    type(a1),
    address(a2),
    length(a4)
{
    assert(length <= max_data_length);
    if (length > 0)
        memcpy(data, a3, length);
}


srecord::record &
srecord::record::operator=(const srecord::record &arg)
{
    if (this != &arg)
    {
        type = arg.type;
        address = arg.address;
        length = arg.length;
        if (arg.length > 0)
            memcpy(data, arg.data, arg.length);
    }
    return *this;
}


srecord::record::~record()
{
}


size_t
srecord::record::maximum_data_length(address_t address)
{
    // FIXME: this is format specific
    if (address < ((address_t)1 << 16))
        return 252;
    if (address < ((address_t)1 << 24))
        return 251;
    return 250;
}


srecord::record::address_t
srecord::record::decode_big_endian(const data_t *buffer, size_t length)
{
    assert(length <= sizeof(address_t));
    address_t result = 0;
    while (length-- > 0)
        result = (result << 8) | *buffer++;
    return result;
}


srecord::record::address_t
srecord::record::decode_little_endian(const data_t *buffer, size_t length)
{
    assert(length <= sizeof(address_t));
    address_t result = 0;
    buffer += length;
    while (length > 0)
    {
        --length;
        --buffer;
        result = (result << 8) | *buffer;
    }
    return result;
}


void
srecord::record::encode_big_endian(data_t *buffer, address_t value,
    size_t length)
{
    assert(length <= sizeof(address_t));
    while (length > 0)
    {
        --length;
        buffer[length] = value;
        value >>= 8;
    }
}


void
srecord::record::encode_little_endian(data_t *buffer, address_t value,
    size_t length)
{
    assert(length <= sizeof(address_t));
    while (length-- > 0)
    {
        *buffer++ = value;
        value >>= 8;
    }
}


bool
srecord::record::is_all_zero()
    const
{
    for (size_t j = 0; j < length; ++j)
        if (data[j])
            return false;
    return true;
}


void
srecord::record::set_data_extend(size_t n, data_t d)
{
    assert(n < max_data_length);
    if (n < max_data_length)
    {
        data[n] = d;
        if (length <= n)
            length = n + 1;
    }
}


bool
srecord::record::address_range_fits_into_n_bits(unsigned nbits)
    const
{
    assert(nbits > 0);
    assert(nbits <= 32);
    return ((address >> nbits) < 1 && ((address + length - 1) >> nbits) < 1);
}
