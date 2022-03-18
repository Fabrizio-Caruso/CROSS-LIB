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
#include <srecord/input/file/four_packed_code.h>
#include <srecord/record.h>


srecord::input_file_four_packed_code::~input_file_four_packed_code()
{
}


srecord::input_file_four_packed_code::input_file_four_packed_code(
    const std::string &a_file_name
) :
    srecord::input_file(a_file_name),
    garbage_warning(false),
    seen_some_input(false),
    running_address(0)
{
}


srecord::input_file::pointer
srecord::input_file_four_packed_code::create(const std::string &a_file_name)
{
    return pointer(new srecord::input_file_four_packed_code(a_file_name));
}


int
srecord::input_file_four_packed_code::get_digit(void)
{
    int c = get_char();
    switch (c)
    {
    case '%':   return  0;
    case '&':   return  1;
    case '\'':  return  2;
    case '(':   return  3;
    case ')':   return  4;
    case '+':   return  5;
    case ',':   return  6;
    case '-':   return  7;
    case '.':   return  8;
    case '/':   return  9;
    case '0':   return 10;
    case '1':   return 11;
    case '2':   return 12;
    case '3':   return 13;
    case '4':   return 14;
    case '5':   return 15;
    case '6':   return 16;
    case '7':   return 17;
    case '8':   return 18;
    case '9':   return 19;
    case ':':   return 20;
    case ';':   return 21;
    case '<':   return 22;
    case '=':   return 23;
    case '>':   return 24;
    case '?':   return 25;
    case '@':   return 26;
    case 'A':   return 27;
    case 'B':   return 28;
    case 'C':   return 29;
    case 'D':   return 30;
    case 'E':   return 31;
    case 'F':   return 32;
    case 'G':   return 33;
    case 'H':   return 34;
    case 'I':   return 35;
    case 'J':   return 36;
    case 'K':   return 37;
    case 'L':   return 38;
    case 'M':   return 39;
    case 'N':   return 40;
    case 'O':   return 41;
    case 'P':   return 42;
    case 'Q':   return 43;
    case 'R':   return 44;
    case 'S':   return 45;
    case 'T':   return 46;
    case 'U':   return 47;
    case 'V':   return 48;
    case 'W':   return 49;
    case 'X':   return 50;
    case 'Y':   return 51;
    case 'Z':   return 52;
    case '[':   return 53;
    case '\\':  return 54;
    case ']':   return 55;
    case '^':   return 56;
    case '_':   return 57;
    case '`':   return 58;
    case 'a':   return 59;
    case 'b':   return 60;
    case 'c':   return 61;
    case 'd':   return 62;
    case 'e':   return 63;
    case 'f':   return 64;
    case 'g':   return 65;
    case 'h':   return 66;
    case 'i':   return 67;
    case 'j':   return 68;
    case 'k':   return 69;
    case 'l':   return 70;
    case 'm':   return 71;
    case 'n':   return 72;
    case 'o':   return 73;
    case 'p':   return 74;
    case 'q':   return 75;
    case 'r':   return 76;
    case 's':   return 77;
    case 't':   return 78;
    case 'u':   return 79;
    case 'v':   return 80;
    case 'w':   return 81;
    case 'x':   return 82;
    case 'y':   return 83;
    case 'z':   return 84;
    }
    fatal_error("base85 digit expected");
    // NOTREACHED
    return -1;
}


int
srecord::input_file_four_packed_code::get_byte(void)
{
    if (get_byte_pos >= 4)
        get_byte_pos = 0;
    if (get_byte_pos == 0)
    {
        get_byte_value = (((get_digit() * 85 + get_digit()) * 85 +
            get_digit()) * 85 + get_digit()) * 85 + get_digit();
    }
    unsigned char c = get_byte_value >> ((3 - get_byte_pos++) * 8);
    checksum_add(c);
    return c;
}


bool
srecord::input_file_four_packed_code::read_inner(srecord::record &record)
{
    for (;;)
    {
        int c = get_char();
        if (c < 0)
            return false;
        if (c == '$')
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
    get_byte(); // discard the checksum byte
    int length = get_byte();
    int format_code = get_word_be();
    switch (length)
    {
    case 0:
        if (format_code != 0)
                fatal_error("format code must be zero");
        if (get_char() != '\n')
                fatal_error("end-of-line expected");
        while (get_char() >= 0)
                ;
        return false;

    case 1:
    case 2:
    case 3:
        fatal_error("bad length");
        length = 0;
        break;

    default:
        length -= 4;
        break;
    }

    unsigned long address = get_4bytes_be();
    switch (format_code)
    {
    case 0:
        break;

    case 1:
        // Ignore any address specified.
        address = running_address;
        break;

    case 2:
        // "Relative"  Just guessing...
        address += running_address;
        break;

    default:
        fatal_error("format code %d unknown", format_code);
    }

    unsigned char buffer[256];
    for (int j = 0; j < length; ++j)
        buffer[j] = get_byte();
    if (use_checksums() && checksum_get() != 0)
        fatal_error("checksum mismatch");
    if (get_char() != '\n')
        fatal_error("end-of-line expected");

    srecord::record::type_t type = srecord::record::type_data;
    if (length == 0)
        type = srecord::record::type_execution_start_address;
    record = srecord::record(type, address, buffer, length);
    running_address = address + length;
    return true;
}


bool
srecord::input_file_four_packed_code::read(srecord::record &record)
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
srecord::input_file_four_packed_code::get_file_format_name(void)
    const
{
    return "Four Packed Code (FPC)";
}


int
srecord::input_file_four_packed_code::format_option_number(void)
    const
{
    return arglex_tool::token_four_packed_code;
}


// vim: set ts=8 sw=4 et :
