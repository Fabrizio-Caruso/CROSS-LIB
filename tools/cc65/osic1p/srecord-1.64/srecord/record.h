//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2001-2003, 2005-2011 Peter Miller
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

#ifndef SRECORD_RECORD_H
#define SRECORD_RECORD_H

#include <cstddef>
#include <stdint.h>

#include <srecord/endian.h>

namespace srecord {

/**
  * The srecord::record class is used to represent a data record read
  * from a file.  (It is not limited to any particular file format.)
  * The records may be of various types.
  */
class record
{
public:
    /**
      * The destructor.  It isn't virtual, so don't derive anything
      * from this class.
      */
    ~record();

    /**
      * The type of the various record types.
      */
    enum type_t
    {
        type_unknown,
        type_header,
        type_data,
        type_data_count,
        type_execution_start_address
    };

    /**
      * The type of record addresses.
      */
    typedef uint32_t address_t;

    /**
      * The type of record data values.
      */
    typedef uint8_t data_t;

    /**
      * The default constructor.  The record will have an
      * indeterminate type, zero address, and no data.
      */
    record();

    /**
      * The copy constructor.
      */
    record(const record &);

    /**
      * A constructor.  The record will have the given type, a zero
      * address and no data.
      */
    record(type_t);

    /**
      * A constructor.  The record will have the given type, the
      * given address and no data.
      */
    record(type_t, address_t);

    /**
      * A constructor.  The record will have the given type, the
      * given address and a copy of the given data.
      *
      * @param the_type
      *     What kind of record this is
      * @param the_address
      *     The memory address of the first byte of data, the rest
      *     increase by one each.
      * @param the_data
      *     The bytes of data for the record.
      * @param the_data_length
      *     How long the data is.
      *     assert(the_data_length < max_data_length);
      */
    record(type_t the_type, address_t the_address, const data_t *the_data,
        size_t the_data_length);

    /**
      * The assignment operator.
      */
    record &operator=(const record &);

    /**
      * The get_address method is used to get the address of the
      * record.
      */
    address_t get_address(void) const { return address; }

    /**
      * The get_address_end method is used to get the address "off
      * the end" of this record.
      */
    address_t get_address_end(void) const { return (address + length); }

    /**
      * The address_range_fits_into_n_bits method is used to test whether or
      * not this record's address range fits within the given number of bits.
      *
      * @param nbits
      *     The number of bits, e.g. 16
      * @returns
      *     true if the address range will fit, or false if it will not fit
      */
    bool address_range_fits_into_n_bits(unsigned nbits) const;

    /**
      * The set_address method is used to set the address of the
      * record.
      */
    void set_address(address_t arg) { address = arg; }

    /**
      * The get_length method is used to get the length (number of
      * bytes) of the record data.
      */
    size_t get_length(void) const { return length; }

    /**
      * The set_length method is used to set the number of data
      * bytes in the record data.
      *
      * @param arg
      *      The new record length.  Note that you can reduce the
      *      length, but you can't increase it.
      */
    void
    set_length(size_t arg)
    {
        if (arg < length)
            length = arg;
    }

    /**
      * The get_data method is used to get a ponter to the baseof
      * the record data.
      *
      * Note: Accessing beyond get_length() bytes will give an
      * undefined value.
      */
    const data_t *get_data(void) const { return data; }

    /**
      * The get_data method is used to fetch the nth data value.
      *
      * Note: For perfoemance reasons, no range checking is
      * performed.  Accessing beyond get_length() bytes will give
      * an undefined value.
      *
      * @param n
      *     The index into the data array, zero based.
      *     Values when n is in excess of #length are undefined.
      */
    int get_data(size_t n) const { return data[n]; }

    /**
      * The is_all_zero method is used to determin if the record
      * contains data bytes which are all zero.
      */
    bool is_all_zero(void) const;

    /**
      * The set_data method is used to set values in the data array.
      * No range checking is performed.  The record length is not
      * consulted or adjusted.
      *
      * @param n
      *     The index into the data array, zero based.
      *     Results when n is in excess of #length are undefined.
      * @param d
      *     The new data value.
      */
    void set_data(size_t n, data_t d) { data[n] = d; }

    /**
      * The set_data_extend method is used to set values in the data array.
      * The record length is adjusted if necessary.
      *
      * @param n
      *     The index into the data array, zero based.
      *     If this is beyond #length, then #length will be extended.
      *     assert(n < max_data_length);
      * @param d
      *     The new data value.
      */
    void set_data_extend(size_t n, data_t d);

    /**
      * The get_type method is used to get the type of the record.
      */
    type_t get_type(void) const { return type; }

    /**
      * The set_type method is used to set the type of the record.
      */
    void set_type(type_t arg) { type = arg; }

    /**
      * The maximum_data_length method is used to determine the
      * maximum data length possible within a record, for a given
      * address.
      *
      * @param addr
      *     The address of the record.  Some formats trade data size of
      *     address size, for a constant maximum line length.
      */
    static size_t maximum_data_length(address_t addr);

    /**
      * The decode_big_endian method is used to extract 'len'
      * bytes from the given 'data' and assemble a big-endian value
      * (most significant byte first).
      *
      * @param data
      *     The data to be decodes
      * @param len
      *     Length of the data, in bytes
      * @returns
      *     the decoded value
      */
    static address_t decode_big_endian(const data_t *data, size_t len);

    /**
      * The decode_little_endian method is used to extract 'len' bytes
      * from the given 'data' and assemble a little-endian value (least
      * significant byte first).
      *
      * @param data
      *     The data to be decodes
      * @param len
      *     Length of the data, in bytes
      * @returns
      *     the decoded value
      */
    static address_t decode_little_endian(const data_t *data, size_t len);

    /**
      * The decode method is used to extract 'len' bytes
      * from the given 'data' and assemble a valu
      *
      * @param data
      *     The data to be decodes
      * @param len
      *     Length of the data, in bytes
      * @param end
      *     The byte order of the data.
      * @returns
      *     the decoded value
      */
    static address_t
    decode(const data_t *data, size_t len, endian_t end)
    {
        return
            (
                end == endian_big
            ?
                decode_big_endian(data, len)
            :
                decode_little_endian(data, len)
            );
    }

    /**
      * The encode_big_endian method is used to break down 'val' into
      * 'len' bytes of 'data' orderdd big-endian (most significan
      * byte first).
      *
      * @param data
      *     Where to place the encoded data
      * @param val
      *     The value to be encoded
      * @param len
      *     The number of bytes to use to encode the data.
      *     Bits above the 8*len resolution will be discarded.
      */
    static void encode_big_endian(data_t *data, address_t val, size_t len);

    /**
      * The encode_little_endian method is used to break down
      * 'val' into 'len' bytes of 'data' orderdd big-endian (least
      * significan byte first).
      *
      * @param data
      *     Where to place the encoded data
      * @param val
      *     The value to be encoded
      * @param len
      *     The number of bytes to use to encode the data.
      *     Bits above the 8*len resolution will be discarded.
      */
    static void encode_little_endian(data_t *data, address_t val, size_t len);

    /**
      * The encode method is used to break down 'val' into 'len' bytes
      * of 'data'
      *
      * @param data
      *     Where to place the encoded data
      * @param val
      *     The value to be encoded
      * @param len
      *     The number of bytes to use to encode the data.
      *     Bits above the 8*len resolution will be discarded.
      * @param end
      *     The byte order
      */
    static void
    encode(data_t *data, address_t val, size_t len, endian_t end)
    {
        if (end == endian_big)
            encode_big_endian(data, val, len);
        else
            encode_little_endian(data, val, len);
    }

    enum {
    /**
      * The max_data_length is the largest number of data bytes
      * which any record can hold.
      */
    max_data_length = 255 };

private:
    /**
      * The type instance variable is used to remember the type of
      * the record.
      */
    type_t type;

    /**
      * The address instance variable is used to remember the address
      * of the record.
      */
    address_t address;

    /**
      * The length instance variable is used to remember the number
      * of valid bytes in the #data array.
      */
    size_t length;

    /**
      * The data instance variable is used to remember the data
      * of the record.  Only the first #length bytes are valid,
      * the rest are undefined.
      */
    data_t data[max_data_length];
};

};

#endif // SRECORD_RECORD_H
