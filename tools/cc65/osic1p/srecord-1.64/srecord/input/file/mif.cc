//
// srecord - Manipulate EPROM load files
// Copyright (C) 2009-2011, 2013 Peter Miller
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
#include <srecord/input/file/mif.h>
#include <srecord/record.h>


srecord::input_file_mif::~input_file_mif()
{
}


srecord::input_file_mif::input_file_mif(const std::string &a_file_name) :
    srecord::input_file(a_file_name),
    state(state_header),
    address(0),
    lex_radix(10),
    lex_addr_radix(16),
    lex_data_radix(16),
    width(8),
    width_in_bytes(1)
{
}


srecord::input_file::pointer
srecord::input_file_mif::create(const std::string &a_file_name)
{
    return pointer(new srecord::input_file_mif(a_file_name));
}


srecord::input_file_mif::token_t
srecord::input_file_mif::lex_addr(void)
{
    lex_radix = lex_addr_radix;
    token_t result = lex();
    lex_radix = 10;
    return result;
}


srecord::input_file_mif::token_t
srecord::input_file_mif::lex_data(void)
{
    lex_radix = lex_data_radix;
    token_t result = lex();
    lex_radix = 10;
    return result;
}


srecord::input_file_mif::token_t
srecord::input_file_mif::lex(void)
{
    for (;;)
    {
        int c = get_char();
        if (c < 0)
            return token_eof;
        switch (c)
        {
        case ' ':
        case '\t':
        case '\r':
        case '\n':
            continue;

        case '.':
            c = get_char();
            if (c == '.')
                return token_dotdot;
            if (c >= 0)
                get_char_undo(c);
            return token_dot;

        case '=':
            return token_equals;

        case ':':
            return token_colon;

        case ';':
            return token_semicolon;

        case '%':
            for (;;)
            {
                c = get_char();
                if (c < 0 || c == '%')
                    break;
            }
            continue;

        case '-':
            if (peek_char() == '-')
            {
                for (;;)
                {
                    c = get_char();
                    if (c < 0 || c == '\n')
                        break;
                }
                continue;
            }
            return token_minus;

        case '[':
            return token_bracket_left;

        case ']':
            return token_bracket_right;

        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        case '_':
            {
                std::string id;
                for (;;)
                {
                    id += (char)c;
                    c = get_char();
                    if (c < 0)
                        break;
                    switch (c)
                    {
                    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                    case 'G': case 'H': case 'I': case 'J': case 'K': case 'L':
                    case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
                    case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
                    case 'Y': case 'Z':
                    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                    case 'g': case 'h': case 'i': case 'j': case 'k': case 'l':
                    case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
                    case 's': case 't': case 'u': case 'v': case 'w': case 'x':
                    case 'y': case 'z':
                    case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9':
                    case '_':
                        continue;

                    default:
                        get_char_undo(c);
                        break;
                    }
                    break;
                }

                //
                // See if we are looking at a number.
                //
                // The only place there is the possibility of ambiguity
                // is the "DEC" keyword, that could also be the 0xDEC
                // number.  However, this only ever occurs in the
                // radix-10 header portion, and will not be a problem
                // (because strtol will reject it).
                //
                const char *cp = id.c_str();
                char *ep = 0;
                token_value = strtol(cp, &ep, lex_radix);
                if (ep != cp && !*ep)
                    return token_number;

                //
                // See if it is a known identifier.
                //
                if (id == "ADDRESS_RADIX")
                    return token_address_radix;
                if (id == "BEGIN")
                    return token_begin;
                if (id == "BIN")
                    return token_bin;
                if (id == "CONTENT")
                    return token_content;
                if (id == "DATA_RADIX")
                    return token_data_radix;
                if (id == "DEC")
                    return token_dec;
                if (id == "DEPTH")
                    return token_depth;
                if (id == "END")
                    return token_end;
                if (id == "HEX")
                    return token_hex;
                if (id == "OCT")
                    return token_oct;
                if (id == "UNS")
                    return token_dec;
                if (id == "WIDTH")
                    return token_width;
            }
            return token_junk;

        default:
            return token_junk;
        }
    }
}


void
srecord::input_file_mif::syntax_error(const char *text)
{
    fatal_error("%s", text);
}


void
srecord::input_file_mif::get_equals(void)
{
    if (lex() != token_equals)
        syntax_error("equals (=) expected");
}


long
srecord::input_file_mif::get_number(void)
{
    if (lex() != token_number)
        syntax_error("decimal number expected");
    return token_value;
}


void
srecord::input_file_mif::get_semicolon(void)
{
    if (lex() != token_semicolon)
        syntax_error("semicolon (;) expected");
}


void
srecord::input_file_mif::get_colon(void)
{
    if (lex() != token_colon)
        syntax_error("colon (:) expected");
}


int
srecord::input_file_mif::get_radix(void)
{
    switch (lex())
    {
    case token_hex:
        return 16;

    case token_oct:
        return 8;

    case token_bin:
        return 2;

    case token_dec:
        break;

    default:
        syntax_error("radix specifier expected");
        // NOTREACHED
    }
    return 10;
}


bool
srecord::input_file_mif::read(srecord::record &record)
{
    unsigned char buffer[srecord::record::max_data_length];
    size_t bufpos = 0;
    unsigned address_range = 0;

    for (;;)
    {
        switch (state)
        {
        case state_eof:
            return false;

        case state_header:
            switch (lex())
            {
            default:
                syntax_error("header expected");

            case token_depth:
                get_equals();
                get_number();
                get_semicolon();
                break;

            case token_width:
                get_equals();
                width = get_number();
                if (width < 1 || width > sizeof(buffer) * 8)
                    syntax_error("width out of range");
                width_in_bytes = (width + 7) / 8;
                get_semicolon();
                break;

            case token_address_radix:
                get_equals();
                lex_addr_radix = get_radix();
                get_semicolon();
                break;

            case token_data_radix:
                get_equals();
                lex_data_radix = get_radix();
                get_semicolon();
                break;

            case token_content:
                if (lex() != token_begin)
                    syntax_error("BEGIN expected");
                state = state_address;
                break;
            }
            break;

        case state_address:
            address_range = 0;
            switch (lex_addr())
            {
            default:
                syntax_error("address expected");
                // NOTREACHED

            case token_number:
                address = token_value * width_in_bytes;
                get_colon();
                state = state_data;
                break;

            case token_end:
                state = state_eof;
                break;

            case token_bracket_left:
                {
                    if (lex_addr() != token_number)
                        syntax_error("start of address range expected");
                    unsigned long address_lo = token_value;
                    if (lex_addr() != token_dotdot)
                        syntax_error("dot dot (..) expected");
                    if (lex_addr() != token_number)
                        syntax_error("end of address range expected");
                    unsigned long address_hi = token_value;
                    if (address_hi < address_lo)
                        syntax_error("address range backwards");
                    address_range = address_hi + 1 - address_lo;
                    address_range *= width_in_bytes;
                    if (address_range > sizeof(buffer))
                        syntax_error("address range too large");
                    if (lex_addr() != token_bracket_right)
                        syntax_error("right bracket ']' expected");
                    get_colon();
                    address = address_lo;
                    state = state_data;
                }
                break;
            }
            break;

        case state_data:
            switch (lex_data())
            {
            default:
                syntax_error("data expected");
                // NOTREACHED

            case token_number:
                for (unsigned j = 0; j < width_in_bytes; ++j)
                {
                    unsigned jj = width_in_bytes - 1 - j;
                    buffer[bufpos + jj] = token_value >> (jj << 3);
                }
                bufpos += width_in_bytes;
                address += width_in_bytes;
                if (bufpos + width_in_bytes > sizeof(buffer))
                {
                    if (address_range != 0)
                        syntax_error("data too large for address range");
                    record =
                        srecord::record
                        (
                            srecord::record::type_data,
                            address - bufpos,
                            buffer,
                            bufpos
                        );
                    return true;
                }
                break;

            case token_semicolon:
                state = state_address;
                if (bufpos > 0)
                {
                    if (address_range != 0)
                    {
                        if (bufpos > address_range)
                            syntax_error("too much data for address range");
                        if (bufpos > 0 && bufpos < address_range)
                        {
                            // deliberately memcpy onto self
                            // to repeat the data in the buffer
                            size_t nbytes = address_range - bufpos;
                            const unsigned char *end = buffer + address_range;
                            const unsigned char *in = buffer;
                            unsigned char *out = buffer + bufpos;
                            while (out < end)
                                *out++ = *in++;
                            bufpos += nbytes;
                            address += nbytes;
                        }
                    }
                    record =
                        srecord::record
                        (
                            srecord::record::type_data,
                            address - bufpos,
                            buffer,
                            bufpos
                        );
                    return true;
                }
                break;
            }
            break;
        }
    }
}


const char *
srecord::input_file_mif::get_file_format_name(void)
    const
{
    return "Memory Initialization File (MIF, Altera)";
}


int
srecord::input_file_mif::format_option_number(void)
    const
{
    return arglex_tool::token_memory_initialization_file;
}


// vim: set ts=8 sw=4 et :
