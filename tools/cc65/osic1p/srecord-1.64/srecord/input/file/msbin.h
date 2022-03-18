//
// srecord - manipulate eprom load files
// Copyright (C) 2009-2011, 2013 Peter Miller
//
// Code contribution by David Kozub <zub@linux.fjfi.cvut.cz>
// Copyright assigned to Peter Miller 27-Jan-2010.
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
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

#ifndef SRECORD_INPUT_FILE_MSBIN_H
#define SRECORD_INPUT_FILE_MSBIN_H

#include <stdint.h>
#include <srecord/input/file.h>
#include <srecord/record.h>
#include <boost/static_assert.hpp>

namespace srecord {

/**
  * The srecord::input_file_binary class is used to represent the parse
  * state of msbin (Windows CE Binary Image Data Format) input stream.
  *
  * See http://msdn.microsoft.com/en-us/library/ms924510.aspx
  * for a description of the format.
  */
class input_file_msbin:
    public input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~input_file_msbin();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be read.
      * @returns
      *     smart pointer to new instance
      */
    static pointer create(const std::string &file_name);

protected:
    // See base class for documentation.
    bool read(record &record);

    // See base class for documentation.
    const char *get_file_format_name(void) const;

    // See base class for documentation.
    int format_option_number(void) const;

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    input_file_msbin(const std::string &file_name);

    /**
      * The header_read instance variable is used to remember whether
      * the file header was read already.
      */
    bool header_read;

    /**
      * The first_record_read instance variable is used to remember
      * whether we read the header of at least a single record.
      *
      * While false, lowest_address and highest_address are not valid.
      */
    bool first_record_read;

    /**
      * The execution_start_record_seen instance variable is used to
      * remember whether we already read the execution start address
      * record, which is supposed to be the last record in a file.
      */
    bool execution_start_record_seen;

    /**
      * The last_record_warning instance variable is used to remember
      * whether a warning about extra records after the execution start
      * record was already issued.
      */
    bool last_record_warning;

    /**
      * The address instance variable is used to remember the current
      * address.
      */
    record::address_t address;

    /**
      * The remaining instance variable is used to remember the number
      * of bytes remaining in this record to be read.
      */
    uint32_t remaining;

    /**
      * The record_checksum instance variable is used to remember the
      * checksum of the current record, as read from the record header.
      */
    uint32_t record_checksum;

    /**
      * The running_checksum instance variable is used to remember the
      * checksum of the data read from this record so far.
      */
    uint32_t running_checksum;

    /**
      * The image_start instance variable is used to remember the image
      * start, the lowest data address in the file, as read from the
      * file header.
      */
    record::address_t image_start;

    /**
      * The image_length instance variable is used to remember the image
      * length, as read from the file header.
      */
    record::address_t image_length;

    /**
      * The lowest_address instance variable is used to remember the
      * lowest address encountered in records read so far.
      */
    record::address_t lowest_address;

    /**
      * The highest_address instance variable is used to remember the
      * highest address encountered in records read so far.
      */
    record::address_t highest_address;

    /**
      * The read_file_header method is used to read the (optional) magic
      * and the file header.
      */
    void read_file_header(void);

    // Just to be sure we can fit uint32_t in address_t.
    BOOST_STATIC_ASSERT(sizeof(record::address_t) >= sizeof(uint32_t));

    /**
      * The read_dword_le method is used to read a little endian double
      * word from the input.
      */
    uint32_t read_dword_le(void);

    /**
      * The checksum method is used to calculate the checksum of a given
      * data block.
      *
      * The checksum is additive, so:
      *  checkum([Block1,Block2]) = checksum(Block1) + checksum(Block2)
      *
      * @param data
      *     The data to be check-summed.
      * @param len
      *     The length in bytes of the data to be check-summed.
      */
    static uint32_t checksum(const unsigned char *data, size_t len);

    // See base class for documentation.
    bool is_binary(void) const;

    /**
      * The default constructor.  Do not use.
      */
    input_file_msbin();

    /**
      * The copy constructor.  Do not use.
      */
    input_file_msbin(const input_file_msbin &);

    /**
      * The assignment operator.  Do not use.
      */
    input_file_msbin &operator=(const input_file_msbin &);
};

};

#endif // SRECORD_INPUT_FILE_MSBIN_H
// vim: set ts=8 sw=4 et :
