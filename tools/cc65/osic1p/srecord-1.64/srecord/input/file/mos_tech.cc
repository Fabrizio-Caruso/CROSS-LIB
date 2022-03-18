//
// srecord - manipulate eprom load files
// Copyright (C) 2000, 2002, 2003, 2005-2008, 2010-2013 Peter Miller
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
#include <srecord/input/file/mos_tech.h>
#include <srecord/record.h>


srecord::input_file_mos_tech::~input_file_mos_tech()
{
}


srecord::input_file_mos_tech::input_file_mos_tech(
    const std::string &a_file_name
) :
    srecord::input_file(a_file_name),
    garbage_warning(false),
    seen_some_input(false),
    data_record_count(0)
{
}


srecord::input_file::pointer
srecord::input_file_mos_tech::create(const std::string &a_file_name)
{
    return pointer(new srecord::input_file_mos_tech(a_file_name));
}


bool
srecord::input_file_mos_tech::read_inner(srecord::record &record)
{
    for (;;)
    {
        int c = get_char();
        if (c < 0)
            return false;
        if (c == 0x11)
        {
            // XOFF in the file also indicates end-of-file
            while (get_char() >= 0)
                ;
            return false;
        }
        if (c == '\0')
            continue;
        if (c == ';')
            break;
        if (c == '\n')
            continue;
        if (!garbage_warning)
        {
            warning("ignoring garbage lines");
            garbage_warning = true;
        }
        for (;;)
        {
            c = get_char();
            if (c < 0)
                return false;
            if (c == '\n')
                break;
        }
    }
    checksum_reset();
    int length = get_byte();
    if (length == 0)
    {
        //
        // This is a data count record,
        // and also the end-of-file record.
        //
        if (peek_char() != '\n' && peek_char() != '\r')
        {
            //
            // Only check the data count if it is present.
            //
            int nrecs = get_word_be();
            int csumX = checksum_get16();
            int csum = get_word_be();
            // In the only file example I have, the count is repeated
            // in the checksum, which would you make you think that the
            // address field is added as a 16-bit value, except that
            // only the data count line is wrong.  Sheesh.
            if (use_checksums() && csumX != csum && csum != nrecs)
                fatal_error("checksum mismatch (%04X != %04X)", csumX, csum);
            if (nrecs != data_record_count)
            {
                fatal_error
                (
                    "data record count mismatch (%d != %d)",
                    nrecs,
                    data_record_count
                );
            }
        }
        if (get_char() != '\n')
            fatal_error("end-of-line expected");
        while (get_char() >= 0)
            ;
        return false;
    }

    unsigned long address = get_word_be();
    unsigned char buffer[256];
    for (int j = 0; j < length; ++j)
        buffer[j] = get_byte();
    int csumX = checksum_get16();
    int csum = get_word_be();
    if (use_checksums() && csumX != csum)
        fatal_error("checksum mismatch (%04X != %04X)", csumX, csum);
    if (get_char() != '\n')
        fatal_error("end-of-line expected");

    srecord::record::type_t type = srecord::record::type_data;
    record = srecord::record(type, address, buffer, length);
    ++data_record_count;
    return true;
}


bool
srecord::input_file_mos_tech::read(srecord::record &record)
{
    if (!read_inner(record))
    {
        if (!seen_some_input && garbage_warning)
            fatal_error("file contains no data");
        return false;
    }
    seen_some_input = true;
    return true;
}


const char *
srecord::input_file_mos_tech::get_file_format_name(void)
    const
{
    return "MOS Technology";
}


int
srecord::input_file_mos_tech::format_option_number(void)
    const
{
    return arglex_tool::token_mos_tech;
}


// vim: set ts=8 sw=4 et :
