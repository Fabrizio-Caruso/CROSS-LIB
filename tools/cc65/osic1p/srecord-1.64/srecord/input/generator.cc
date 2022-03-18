//
// srecord - The "srecord" program.
// Copyright (C) 2007-2010, 2013, 2014 Peter Miller
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

#include <cstring>

#include <srecord/arglex/tool.h>
#include <srecord/input/generator.h>
#include <srecord/input/generator/constant.h>
#include <srecord/input/generator/random.h>
#include <srecord/input/generator/repeat.h>
#include <srecord/record.h>
#include <srecord/string.h>


srecord::input_generator::~input_generator()
{
}


srecord::input_generator::input_generator(const interval &a_range) :
    range(a_range)
{
}


bool
srecord::input_generator::read(srecord::record &result)
{
    //
    // If there is not data left to generate,
    // signal end-of-file
    //
    if (range.empty())
        return false;

    //
    // Calculate the address range for this chunk of data.  Use the
    // biggest record size available.
    //
    unsigned long addr = range.get_lowest();
    interval partial(addr, addr + srecord::record::max_data_length);
    partial *= range;

    //
    // Only supply the first sub-interval, if the generation range has
    // holes in it.
    //
    partial.first_interval_only();

    //
    // Generate the data and build the result record.
    //
    result.set_type(srecord::record::type_data);
    result.set_address(addr);
    result.set_length(1);
    interval::data_t size = partial.get_highest() - addr;
    for (interval::data_t j = 0; j < size; ++j)
    {
        result.set_data_extend(j, generate_data(addr + j));
    }

    //
    // Reduce the amount of data left to be generated.
    //
    range -= partial;

    //
    // Report that another record is available.
    //
    return true;
}


srecord::input::pointer
srecord::input_generator::create(srecord::arglex_tool *cmdln)
{
    interval range = cmdln->get_interval_small("--generate");
    srecord::input::pointer result;
    switch (cmdln->token_cur())
    {
    case srecord::arglex_tool::token_constant_be:
        {
            cmdln->token_next();
            unsigned long datum =
                cmdln->get_number("--generate --b-e-constant <datum>");
            int length =
                cmdln->get_number("--generate --b-e-constant <length>");
            if (length <= 0 || length > 4)
            {
                cmdln->fatal_error
                (
                    "length %d out of range [1..4]",
                    length
                );
            }
            unsigned long over = (1uL << (8u * length)) - 1;
            if (length < 4 && datum > over)
            {
                cmdln->fatal_error
                (
                    "datum %lu out of range [0..%lu]",
                    datum,
                    over
                );
            }
            unsigned char data[4];
            data[3] = datum;
            data[2] = datum >> 8;
            data[1] = datum >> 16;
            data[0] = datum >> 24;
            result =
                srecord::input_generator_repeat::create
                (
                    range,
                    data + 4 - length,
                    length
                );
        }
        break;

    case srecord::arglex_tool::token_constant_le:
        {
            cmdln->token_next();
            unsigned long datum =
                cmdln->get_number("--generate --l-e-constant <datum>");
            int length =
                cmdln->get_number("--generate --l-e-constant <length>");
            if (length <= 0 || length > 4)
            {
                cmdln->fatal_error
                (
                    "length %d out of range [1..4]",
                    length
                );
            }
            unsigned long over = (1uL << (8u * length)) - 1;
            if (length < 4 && datum > over)
            {
                cmdln->fatal_error
                (
                    "datum %lu out of range [0..%lu]",
                    datum,
                    over
                );
            }
            unsigned char data[4];
            data[0] = datum;
            data[1] = datum >> 8;
            data[2] = datum >> 16;
            data[3] = datum >> 24;
            result = input_generator_repeat::create(range, data, length);
        }
        break;

    case srecord::arglex_tool::token_constant:
        {
            cmdln->token_next();
            int n = cmdln->get_number("--generate --constant");
            if (n < 0 || n > 255)
            {
                cmdln->fatal_error
                (
                    "data byte %d out of range [0..255]",
                    n
                );
            }
            result = srecord::input_generator_constant::create(range, n);
        }
        break;

    case srecord::arglex_tool::token_random:
        {
            cmdln->token_next();
            result = srecord::input_generator_random::create(range);
        }
        break;

    case srecord::arglex_tool::token_repeat_data:
        {
            cmdln->token_next();
            size_t length = 0;
            size_t maxlen = 16;
            unsigned char *data = new unsigned char [maxlen];
            for (;;)
            {
                //
                // Get the next byte value from the command line
                // (there must be at least one).
                //
                long byte_value = cmdln->get_number("--repeat-data");

                //
                // Make sure there is room for the next byte in the data array
                //
                if (length >= maxlen)
                {
                    size_t new_maxlen = maxlen * 2 + 16;
                    unsigned char *new_data = new unsigned char [new_maxlen];
                    memcpy(new_data, data, length);
                    delete [] data;
                    data = new_data;
                    maxlen = new_maxlen;
                }
                if (byte_value < 0 || byte_value > 255)
                {
                    cmdln->fatal_error
                    (
                        "data byte %ld out of range [0..255]",
                        byte_value
                    );
                }

                //
                // Stash this byte
                //
                data[length++] = byte_value;

                //
                // If there are no more numbers on the command line, we
                // are done.
                //
                if (!cmdln->can_get_number())
                    break;
            }

            //
            // Build our new input data source.
            //
            if (length == 1)
            {
                result = input_generator_constant::create(range, data[0]);
            }
            else
            {
                result = input_generator_repeat::create(range, data, length);
            }
            delete [] data;
        }
        break;

    case srecord::arglex_tool::token_repeat_string:
        {
            cmdln->token_next();
            std::string s = cmdln->get_string("--repeat-string");

            // The users may use %nn on the command line,
            // but we store a byte 0xnn in the header.
            s = srecord::string_url_decode(s);

            size_t len = s.size();
            switch (len)
            {
            case 0:
                cmdln->fatal_error("--repeat-string value may not be empty");
                // NOTREACHED

            case 1:
                result = srecord::input_generator_constant::create(range, s[0]);
                break;

            default:
                result =
                    srecord::input_generator_repeat::create
                    (
                        range,
                        (unsigned char *)s.c_str(),
                        s.size()
                    );
                break;
            }
        }
        break;

    default:
        cmdln->fatal_error
        (
            "the --generate option needs to be followed by a generation "
                "type (e.g. --constant)"
        );
        // NOTREACHED
    }
    return result;
}


void
srecord::input_generator::disable_checksum_validation()
{
    // Do nothing.
    // None of the generators have checksums.
}


// vim: set ts=8 sw=4 et :
