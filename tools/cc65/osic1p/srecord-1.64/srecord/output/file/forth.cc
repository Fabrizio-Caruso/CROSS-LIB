//
// srecord - manipulate eprom load files
// Copyright (C) 2008-2010, 2012 Peter Miller
// Copyright (C) 2008 Torsten Sadowski
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

#include <srecord/output/file/forth.h>
#include <srecord/record.h>
#include <srecord/arglex/tool.h>


srecord::output_file_forth::~output_file_forth()
{
    emit_end_of_file();
}


srecord::output_file_forth::output_file_forth(
    const std::string &a_file_name
) :
    srecord::output_file(a_file_name),
    store_cmd("C!")
{
}


srecord::output::pointer
srecord::output_file_forth::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_forth(a_file_name));
}


void
srecord::output_file_forth::emit_end_of_file()
{
    put_char('\n');

    if (enable_footer_flag)
    {
        put_stringf("( THE END )\n");
    }
}


void
srecord::output_file_forth::write(const srecord::record &record)
{
    switch (record.get_type())
    {
    case srecord::record::type_header:
        put_string("HEX\n");
        break;

    case srecord::record::type_data:
        {
            record::address_t address = record.get_address();
            //
            // Now write out the new address.  It is important not to
            // disturb the checksum, so don't use the put_byte method.
            //
            for (size_t j = 0; j < record.get_length(); ++j)
            {
                put_byte(record.get_data(j));
                put_char(' ');
                put_word_be(address);
                put_char(' ');
                put_string(store_cmd);
                ++address;
                put_char('\n');
            }
        }
        break;

    case srecord::record::type_data_count:
        // ignore
        break;

    case srecord::record::type_execution_start_address:
        // Ignore.
        // The destructor sill call emit_end_of_file();
        break;

    case srecord::record::type_unknown:
        fatal_error("can't write unknown record type");
    }
}


const char *
srecord::output_file_forth::format_name()
    const
{
    return "FORTH";
}

void
srecord::output_file_forth::command_line(srecord::arglex_tool *cmdln)
{
    for (;;)
    {
        switch (cmdln->token_cur())
        {
        case srecord::arglex_tool::token_ram:
            cmdln->token_next();
            // Store into RAM
            store_cmd = "C!";
            break;

        case srecord::arglex_tool::token_eeprom:
            cmdln->token_next();
            // Store into EEPROM
            store_cmd = "EEC!";
            break;

        default:
            return;
        }
    }
}


void
srecord::output_file_forth::line_length_set(int)
{
}


void
srecord::output_file_forth::address_length_set(int)
{
}


bool
srecord::output_file_forth::preferred_block_size_set(int nbytes)
{
    return (nbytes >= 1 && nbytes <= record::max_data_length);
}


int
srecord::output_file_forth::preferred_block_size_get() const
{
  return 16;
}


// vim: set ts=8 sw=4 et :
