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

#include <srecord/arglex/tool.h>
#include <srecord/input/filter/message/fletcher16.h>
#include <srecord/memory/walker/fletcher16.h>
#include <srecord/record.h>


srecord::input_filter_message_fletcher16::~input_filter_message_fletcher16()
{
}


srecord::input_filter_message_fletcher16::input_filter_message_fletcher16(
    const input::pointer &a_deeper,
    unsigned long a_address,
    endian_t a_end
) :
    input_filter_message(a_deeper),
    address(a_address),
    end(a_end),
    sum1(0xFF),
    sum2(0xFF),
    answer(-1)
{
}


srecord::input::pointer
srecord::input_filter_message_fletcher16::create(
    const input::pointer &a_deeper, unsigned long a_address,
    endian_t a_end)
{
    return
        pointer
        (
            new input_filter_message_fletcher16(a_deeper, a_address, a_end)
        );
}


void
srecord::input_filter_message_fletcher16::command_line(
    srecord::arglex_tool *cmdln)
{
    if (cmdln->token_cur() == arglex::token_number)
    {
        sum1 = 0xFF & cmdln->get_number("sum1");
        sum2 = 0xFF & cmdln->get_number("sum2");

        if (cmdln->token_cur() == arglex::token_number)
            answer = 0xFFFF & cmdln->get_number("answer");
    }
}


void
srecord::input_filter_message_fletcher16::process(const memory &input,
    record &output)
{
    //
    // Now calculate the Fletcher 16 checksum the bytes in order from
    // lowest address to highest.  (Holes are ignored, not filled,
    // warning issued already.)
    //
    memory_walker_fletcher16::pointer w =
        memory_walker_fletcher16::create(sum1, sum2, answer, end);
    input.walk(w);
    unsigned short fletcher = w->get();

    //
    // Turn the Fletcher-16 checksum into the first data record.
    //
    unsigned char chunk[2];
    record::encode(chunk, fletcher, sizeof(chunk), end);
    output = record(record::type_data, address, chunk, sizeof(chunk));
}


const char *
srecord::input_filter_message_fletcher16::get_algorithm_name()
    const
{
    return "Fletcher-16";
}


// vim: set ts=8 sw=4 et :
