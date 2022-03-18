//
// srecord - manipulate eprom load files
// Copyright (C) 2001-2003, 2005-2008, 2010-2013 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <srecord/arglex/tool.h>
#include <srecord/input/file/dec_binary.h>
#include <srecord/record.h>


srecord::input_file_dec_binary::~input_file_dec_binary()
{
}


srecord::input_file_dec_binary::input_file_dec_binary(
        const std::string &a_file_name) :
    srecord::input_file(a_file_name),
    current_pos(0),
    current_length(0),
    current_address(0)
{
}


srecord::input_file::pointer
srecord::input_file_dec_binary::create(const std::string &a_file_name)
{
    return pointer(new srecord::input_file_dec_binary(a_file_name));
}


bool
srecord::input_file_dec_binary::skip_nul()
{
    for (;;)
    {
        int c = peek_char();
        if (c < 0)
            return false;
        if (c)
            return true;
        get_char();
    }
}


int
srecord::input_file_dec_binary::get_byte()
{
    int c = get_char();
    if (c < 0)
        fatal_error("premature end-of-file");
    checksum_add(c);
    return c;
}


bool
srecord::input_file_dec_binary::read(srecord::record &record)
{
    //
    // The records in the file are much larger than SRecord usually
    // handles, so we keep track of where we are within the current
    // record.  When we run out, we go on to the next one.
    //
    if (current_pos >= current_length)
    {
        if (current_length)
        {
            // Verify the checksum at end of the record.
            get_byte();
            if (use_checksums() && checksum_get())
            {
                fatal_error("checksum mismatch (%02X)", checksum_get());
            }
        }
        current_pos = 0;
        current_length = 0;
        current_address = 0;

        //
        // You are allowed to have NUL characters between records.
        // Presumably this is used to get recordss onto even
        // byte boundardies and/or whole block boundaries.
        //
        if (!skip_nul())
            return false;

        checksum_reset();
        int tag = get_word_le();
        if (tag != 1)
            fatal_error("record type %d unknown", tag);
        int length = get_word_le();
        if (length < 6)
            fatal_error("record length (%d) invalid", length);
        length -= 6;
        current_address = get_word_le();
        if (length == 0)
        {
            get_byte();
            if (use_checksums() && checksum_get())
            {
                fatal_error("checksum mismatch (%02X)", checksum_get());
            }
            srecord::record::type_t type =
                srecord::record::type_execution_start_address;
            record = srecord::record(type, current_address, 0, 0);
            seek_to_end();
            return true;
        }
        current_length = length;
    }

    //
    // Read as many bytes as possible into the buffer.
    //
    unsigned char buffer[srecord::record::max_data_length];
    int nbytes = srecord::record::max_data_length;
    if (current_pos + nbytes > current_length)
        nbytes = current_length - current_pos;
    for (int j = 0; j < nbytes; ++j)
        buffer[j] = get_byte();

    //
    // Create a data record and return.
    //
    srecord::record::type_t type = srecord::record::type_data;
    record = srecord::record(type, current_address, buffer, nbytes);
    current_address += nbytes;
    current_pos += nbytes;
    return true;
}


bool
srecord::input_file_dec_binary::is_binary(void)
    const
{
    return true;
}


const char *
srecord::input_file_dec_binary::get_file_format_name()
    const
{
    return "DEC Binary (XXDP)";
}


int
srecord::input_file_dec_binary::format_option_number(void)
    const
{
    return arglex_tool::token_dec_binary;
}


// vim: set ts=8 sw=4 et :
