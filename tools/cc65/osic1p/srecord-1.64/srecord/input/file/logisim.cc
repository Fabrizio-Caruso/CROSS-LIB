//
// srecord - Manipulate EPROM load files
// Copyright (C) 2013 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
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

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <srecord/arglex/tool.h>
#include <srecord/input/file/logisim.h>
#include <srecord/string.h>


srecord::input_file_logisim::~input_file_logisim()
{
}


srecord::input_file_logisim::input_file_logisim(
    const std::string &a_file_name
) :
    input_file(a_file_name),
    address(0),
    state(state_line_one)
{
}


srecord::input_file::pointer
srecord::input_file_logisim::create(const std::string &a_file_name)
{
    return pointer(new input_file_logisim(a_file_name));
}


static std::string
quote_char(int c)
{
     if (c == EOF)
        return "EOF";
     if (c == '\0')
        return "\\0";
     char buf[2] = { static_cast<char>(c), '\0' };
     return srecord::string_quote_c(buf);
}


void
srecord::input_file_logisim::read_inner_one(void)
{
    char buffer[512];
    char *bp = buffer;
    char *end = buffer + sizeof(buffer) - 1;
    for (;;)
    {
        int c = get_char();
        switch(c)
        {
        case EOF:
            return;

        case '\n':
            break;

        default:
            if (bp > end)
                fatal_error("line too long");
            *bp++ = c;
            continue;
        }
        break;
    }
    *bp = '\0';
    if (strcmp(buffer, "v2.0 raw"))
        fatal_error("bad magic number");
    state = state_line_two;
}


void
srecord::input_file_logisim::read_inner_two(void)
{
    bool warned = false;
    for (;;)
    {
        int c = get_char();
        switch (c)
        {
        case EOF:
            return;

        case '\n':
            break;

        case ' ':
        case '\t':
        case '\r':
        case '\f':
        case '\v':
            continue;

        default:
            if (!warned)
            {
                warning("second line must be completely empty");
                warned = true;
            }
            continue;
        }
        break;
    }
    state = state_lines_of_data;
}


void
srecord::input_file_logisim::read_inner_job(void)
{
    char buffer[200];
    char *bp = buffer;
    char *end = buffer + (sizeof(buffer) - 1);
    char *seen_star = 0;
    for(;;)
    {
        if (jobs.size() > 20)
            return;
        int c = get_char();
        switch (c)
        {
        case EOF:
            return;

        case ' ':
        case '\t':
        case '\r':
        case '\n':
        case '\f':
            // ignore leading white space
            continue;

        default:
            warning("ignoring junk %s", quote_char(c).c_str());
            break;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
            bp = buffer;
            seen_star = 0;
            for (;;)
            {
                if (bp >= end)
                    fatal_error("datum too long");
                *bp++ = c;

                c = get_char();
                switch(c)
                {
                case '*':
                    seen_star = bp + 1;
                    // Fall through...

                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                case 'G': case 'H': case 'I': case 'J': case 'K': case 'L':
                    continue;

                case ' ':
                case '\n':
                case '\t':
                case '\r':
                case '\f':
                default:
                    get_char_undo(c);
                    break;
                }
                break;
            }
            *bp = '\0';

            // now push it on the back of the jobs list
            {{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{
            if (seen_star)
            {
                char *ep = 0;
                long count = strtol(buffer, &ep, 10);
                if (buffer == ep || *ep != '*')
                    fatal_error("malformed datum");

                // The doco does not reveal a way to distingush
                // 1-byte vs 2-byte or 4-byte items.  Also it
                // doesn't specify endian-ness.
                ep = 0;
                unsigned long value = strtoul(seen_star, &ep, 16);
                if (seen_star == ep || *ep != '\0')
                    fatal_error("malformed datum");
                datum_t job(address, count, value);
                jobs.push_back(job);
                address += count;
                return;
            }

            // no star
            char *ep = 0;
            long n = strtol(buffer, &ep, 16);
            if (buffer == ep || *ep != '\0')
                fatal_error("malformed datum");
            datum_t job(address, 1, n);
            jobs.push_back(job);
            ++address;
            }}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}
            break;
        }
    }
}


bool
srecord::input_file_logisim::read(class record &rec)
{
    for (;;)
    {
        if (state == state_line_one)
            read_inner_one();
        if (state == state_line_two)
            read_inner_two();
        assert(state == state_lines_of_data);

        // So now we unwrap the run-length encoding
        if (!jobs.empty())
        {
            datum_t job(jobs.front());
            jobs.pop_front();

            size_t max_data_size = record::maximum_data_length(address);
            size_t data_size =
                ((size_t)job.count > max_data_size ? max_data_size : job.count);
            record::data_t data[data_size];
            memset(data, job.value, data_size);
            rec = record(record::type_data, job.address, data, data_size);
            job.address += data_size;
            job.count -= data_size;
            if (job.count > 0)
            {
                jobs.push_back(job);
            }
            return true;
        }
        read_inner_job();

        if (jobs.empty())
            return false;
    }
}


const char *
srecord::input_file_logisim::get_file_format_name(void)
    const
{
    return "Logisim";
}


int
srecord::input_file_logisim::format_option_number(void)
    const
{
    return srecord::arglex_tool::token_logisim;
}


std::string
srecord::input_file_logisim::datum_t::representation(void)
    const
{
    char buffer[200];
    snprintf
    (
        buffer,
        sizeof(buffer),
        "{ address = 0x%04X, count = %ld, value = 0x%02X }",
        address,
        count,
        value
    );
    return buffer;
}


// vim: set ts=8 sw=4 et :
