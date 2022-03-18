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
#include <srecord/input/file/emon52.h>
#include <srecord/record.h>


srecord::input_file_emon52::~input_file_emon52()
{
}


srecord::input_file_emon52::input_file_emon52(const std::string &a_file_name) :
    srecord::input_file(a_file_name)
{
}


srecord::input_file::pointer
srecord::input_file_emon52::create(const std::string &a_file_name)
{
    return pointer(new srecord::input_file_emon52(a_file_name));
}


void
srecord::input_file_emon52::skip_white_space(void)
{
    for (;;)
    {
        int c = peek_char();
        if (c != ' ')
            return;
        get_char();
    }
}


bool
srecord::input_file_emon52::read(srecord::record &record)
{
    //
    // This format has no execution start address record type, and no
    // magic start character.  So look ahead to see if there is anything
    // more.
    //
    if (peek_char() < 0)
        return false;

    //
    // Looks like there should be a record.  Read it all in.
    //
    int length = get_byte();
    if (length == 0)
        fatal_error("data length of zero is not valid");
    skip_white_space();
    unsigned long address = get_word_be();
    if (get_char() != ':')
        fatal_error("colon expected");
    checksum_reset();
    unsigned char buffer[256];
    for (int j = 0; j < length; ++j)
    {
        skip_white_space();
        buffer[j] = get_byte();
    }
    skip_white_space();
    int csumX = checksum_get16();
    int csum = get_word_be();
    if (use_checksums() && csumX != csum)
        fatal_error("checksum mismatch (%04X != %04X)", csumX, csum);
    if (get_char() != '\n')
        fatal_error("end-of-line expected");

    srecord::record::type_t type = srecord::record::type_data;
    record = srecord::record(type, address, buffer, length);
    return true;
}


const char *
srecord::input_file_emon52::get_file_format_name(void)
    const
{
    return "Elektor Monitor (EMON52)";
}


int
srecord::input_file_emon52::format_option_number(void)
    const
{
    return arglex_tool::token_emon52;
}


// vim: set ts=8 sw=4 et :
