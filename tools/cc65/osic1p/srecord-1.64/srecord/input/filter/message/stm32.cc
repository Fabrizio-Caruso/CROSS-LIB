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

#include <srecord/arglex/tool.h>
#include <srecord/input/filter/message/stm32.h>
#include <srecord/memory/walker/stm32.h>
#include <srecord/record.h>


srecord::input_filter_message_stm32::~input_filter_message_stm32()
{
}


srecord::input_filter_message_stm32::input_filter_message_stm32(
    const input::pointer &a_deeper,
    unsigned long a_address,
    endian_t a_end
) :
    input_filter_message(a_deeper),
    address(a_address),
    end(a_end)
{
}


srecord::input::pointer
srecord::input_filter_message_stm32::create(const input::pointer &a_deeper,
    unsigned long a_address, endian_t a_end)
{
    return
        pointer
        (
            new input_filter_message_stm32(a_deeper, a_address, a_end)
        );
}


void
srecord::input_filter_message_stm32::command_line(arglex_tool *)
{
}


void
srecord::input_filter_message_stm32::process(const memory &input,
    record &output)
{
    //
    // Now STM32 the bytes in order from lowest address to highest.
    // (Holes are ignored, not filled, warning already issued.)
    //
    memory_walker_stm32::pointer w = memory_walker_stm32::create();
    input.walk(w);
    unsigned long crc = w->get();

    //
    // Turn the CRC into the first data record.
    //
    unsigned char chunk[4];
    record::encode(chunk, crc, sizeof(chunk), end);
    output = record(record::type_data, address, chunk, sizeof(chunk));
}


const char *
srecord::input_filter_message_stm32::get_algorithm_name(void)
    const
{
    return "STM32";
}


unsigned
srecord::input_filter_message_stm32::get_minimum_alignment(void)
    const
{
    return 4;
}


// vim: set ts=8 sw=4 et :
