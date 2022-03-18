//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2003, 2006-2010, 2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see
// <http://www.gnu.org/licenses/>.
//

#include <srecord/arglex/tool.h>
#include <srecord/input/filter/message/crc16.h>
#include <srecord/memory.h>
#include <srecord/memory/walker/crc16.h>
#include <srecord/record.h>


srecord::input_filter_message_crc16::~input_filter_message_crc16()
{
}


srecord::input_filter_message_crc16::input_filter_message_crc16(
        const input::pointer &deeper_arg, unsigned long address_arg,
        endian_t a_end) :
    input_filter_message(deeper_arg),
    address(address_arg),
    end(a_end),
    seed_mode(crc16::seed_mode_ccitt),
    augment_flag(true),
    polynomial(crc16::polynomial_ccitt),
    bitdir(crc16::bit_direction_most_to_least)
{
}


srecord::input::pointer
srecord::input_filter_message_crc16::create(const input::pointer &a_deeper,
    unsigned long a_address, endian_t a_end)
{
    return
        pointer
        (
            new input_filter_message_crc16(a_deeper, a_address, a_end)
        );
}


void
srecord::input_filter_message_crc16::command_line(arglex_tool *cmdln)
{
    for (;;)
    {
        switch (cmdln->token_cur())
        {
        case arglex::token_number:
            polynomial = cmdln->value_number();
            cmdln->token_next();
            break;

        case arglex_tool::token_crc16_most_to_least:
            bitdir = crc16::bit_direction_most_to_least;
            cmdln->token_next();
            break;

        case arglex_tool::token_crc16_least_to_most:
            bitdir = crc16::bit_direction_least_to_most;
            cmdln->token_next();
            break;

        case arglex_tool::token_crc16_xmodem:
            seed_mode = crc16::seed_mode_xmodem;
            cmdln->token_next();
            break;

        case arglex_tool::token_crc16_ccitt:
            seed_mode = crc16::seed_mode_ccitt;
            polynomial = crc16::polynomial_ccitt;
            cmdln->token_next();
            break;

        case arglex_tool::token_polynomial:
            // polynomial = crc16::polynomial_ansi;
            switch (cmdln->token_next())
            {
            case arglex::token_number:
                polynomial = cmdln->value_number();
                break;

            case arglex_tool::token_crc16_ccitt:
                polynomial = crc16::polynomial_ccitt;
                break;

            case arglex::token_string:
                polynomial =
                    crc16::polynomial_by_name(cmdln->value_string().c_str());
                break;

            default:
                fatal_error("expected --polynomial <name>");
            }
            cmdln->token_next();
            break;

        case arglex_tool::token_crc16_broken:
            seed_mode = crc16::seed_mode_broken;
            cmdln->token_next();
            break;

        case arglex_tool::token_crc16_augment:
            augment_flag = true;
            cmdln->token_next();
            break;

        case arglex_tool::token_crc16_augment_not:
            augment_flag = false;
            cmdln->token_next();
            break;

        default:
            return;
        }
    }
}


void
srecord::input_filter_message_crc16::process(const memory &buffer,
    record &result)
{
    //
    // Now CRC16 the bytes in order from lowest address to highest.
    // (Holes are ignored, not filled, warning already issued.)
    //
    memory_walker_crc16::pointer w =
        memory_walker_crc16::create
        (
            seed_mode,
            augment_flag,
            polynomial,
            bitdir
        );
    buffer.walk(w);
    unsigned crc = w->get();

    //
    // Turn the CRC into the first data record.
    //
    unsigned char chunk[2];
    record::encode(chunk, crc, sizeof(chunk), end);
    result = record(record::type_data, address, chunk, sizeof(chunk));
}


const char *
srecord::input_filter_message_crc16::get_algorithm_name()
    const
{
    return "CRC16";
}
