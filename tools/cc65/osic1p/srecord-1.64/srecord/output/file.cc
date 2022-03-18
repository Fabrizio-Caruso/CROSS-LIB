//
// srecord - manipulate eprom load files
// Copyright (C) 1998-2002, 2005-2012, 2014 Peter Miller
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

#include <cerrno>
#include <cstdio>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

#include <srecord/arglex.h>
#include <srecord/sizeof.h>
#include <srecord/output/file.h>
#include <srecord/record.h>


bool srecord::output_file::enable_header_flag = true;
bool srecord::output_file::enable_data_count_flag = true;
bool srecord::output_file::enable_goto_addr_flag = true;
bool srecord::output_file::enable_footer_flag = true;
bool srecord::output_file::enable_optional_address_flag = false;


srecord::output_file::~output_file()
{
    FILE *fp = (FILE *)get_fp();
    if (fflush(fp))
        fatal_error_errno("write");
    if (fp != stdout && fclose(fp))
        fatal_error_errno("close");
}


srecord::output_file::output_file() :
    file_name("standard output"),
    line_number(1),
    vfp(0),
    checksum(0),
    position(0),
    is_regular(true)
{
    vfp = stdout;
    set_is_regular();
    line_termination = line_termination_binary;
}


srecord::output_file::output_file(const std::string &a_file_name) :
    file_name(a_file_name),
    line_number(1),
    vfp(0),
    checksum(0),
    position(0),
    is_regular(true)
{
    if (file_name == "-")
    {
        file_name = "standard output";
        vfp = stdout;
        set_is_regular();
        line_termination = line_termination_binary;
    }
    else
    {
        //
        // The call to fopen is deferred until the constructor has
        // completed.  This is so that the line_termination can be set
        // correctly.
        //
    }
}


bool
srecord::output_file::is_binary(void)
    const
{
    return false;
}


void *
srecord::output_file::get_fp(void)
{
    if (!vfp)
    {
        //
        // The call to fopen is deferred until the constructor has
        // completed.  This is so that the line_termination can be set
        // correctly.
        //
#ifdef __CYGWIN__
        if (line_termination == line_termination_native && !is_binary())
        {
            vfp = fopen(file_name.c_str(), "w");
            if (!vfp)
                fatal_error_errno("open");
            line_termination = line_termination_binary;
        }
        else
#endif
        {
            vfp = fopen(file_name.c_str(), "wb");
            if (!vfp)
                fatal_error_errno("open");
        }
        set_is_regular();
    }
    return vfp;
}


const std::string
srecord::output_file::filename()
    const
{
    char buffer[20];
    sprintf(buffer, ": %d", line_number);
    return (file_name + buffer);
}


void
srecord::output_file::put_char(int c)
{
    FILE *fp = (FILE *)get_fp();
    if (c == '\n' && !is_binary())
    {
        ++line_number;
        for (;;)
        {
            switch (line_termination)
            {
            case line_termination_native:
                line_termination = line_termination_guess();
                continue;

            case line_termination_primos:
                putc('\n', fp);
                ++position;
                if (position & 1)
                {
                    putc(0, fp);
                    ++position;
                }
                break;

            case line_termination_nl:
                putc('\n', fp);
                ++position;
                break;

            case line_termination_cr:
                putc('\r', fp);
                ++position;
                break;

            case line_termination_crlf:
                putc('\r', fp);
                ++position;
                putc('\n', fp);
                ++position;
                break;
            }
            break;
        }
    }
    else
    {
        putc(c, fp);
        ++position;
    }
    if (ferror(fp))
        fatal_error_errno("write");
}


void
srecord::output_file::put_nibble(int n)
{
    put_char("0123456789ABCDEF"[n & 15]);
}


void
srecord::output_file::put_byte(unsigned char n)
{
    put_nibble(n >> 4);
    put_nibble(n);
    checksum_add(n);
}


void
srecord::output_file::put_word_be(int n)
{
    put_byte(n >> 8);
    put_byte(n);
}


void
srecord::output_file::put_word_le(int n)
{
    put_byte(n);
    put_byte(n >> 8);
}


void
srecord::output_file::put_3bytes_be(unsigned long n)
{
    put_byte(n >> 16);
    put_byte(n >> 8);
    put_byte(n);
}


void
srecord::output_file::put_3bytes_le(unsigned long n)
{
    put_byte(n);
    put_byte(n >> 8);
    put_byte(n >> 16);
}


void
srecord::output_file::put_4bytes_be(unsigned long n)
{
    put_byte(n >> 24);
    put_byte(n >> 16);
    put_byte(n >> 8);
    put_byte(n);
}


void
srecord::output_file::put_4bytes_le(unsigned long n)
{
    put_byte(n);
    put_byte(n >> 8);
    put_byte(n >> 16);
    put_byte(n >> 24);
}


int
srecord::output_file::checksum_get(void)
{
    return (checksum & 0xFF);
}


int
srecord::output_file::checksum_get16(void)
{
    return (checksum & 0xFFFF);
}


void
srecord::output_file::checksum_reset(void)
{
    checksum = 0;
}

void
srecord::output_file::checksum_add(unsigned char n)
{
    checksum += n;
}


void
srecord::output_file::seek_to(unsigned long address)
{
    //
    // Seeking on non-regular files is problematic.  Avoid doing
    // this if possible.  (Usually we can, srecord::cat emits records
    // in ascending address order.)
    //
    if (!is_regular)
    {
        while (position < address)
            put_char(0);
    }
    if (address == position)
        return;

    //
    // We'll have to try a seek.
    //
    FILE *fp = (FILE *)get_fp();
    errno = 0;
    if (fseek(fp, address, 0) < 0)
    {
        if (errno == EINVAL && address >= 0x80000000uL)
        {
            warning
            (
                "It appears that the implementation of fseek on your "
                "system is unable to cope with addresses which have "
                "the most significant bit set (this is POSIX and ANSI "
                "C conforming behaviour).  You probably did not intend "
                "to create a %3.1fGB file.  See the manual for a "
                "description of the --offset filter, remembering that "
                "you can give negative offsets.",
                ((double)address / (double)(1uL << 30))
            );
        }
        fatal_error_errno("seek 0x%lX", address);
    }
    position = address;
}


void
srecord::output_file::put_string(const char *s)
{
    while (*s)
        put_char(*s++);
}


void
srecord::output_file::put_string(const std::string &s)
{
    const char *cp = s.c_str();
    const char *ep = cp + s.size();
    while (cp < ep)
        put_char(*cp++);
}


void
srecord::output_file::put_stringf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char buffer[1000];
    vsprintf(buffer, fmt, ap);
    va_end(ap);
    put_string(buffer);
}


void
srecord::output_file::enable_header(bool yesno)
{
    enable_header_flag = yesno;
}


void
srecord::output_file::enable_data_count(bool yesno)
{
    enable_data_count_flag = yesno;
}


void
srecord::output_file::enable_goto_addr(bool yesno)
{
    enable_goto_addr_flag = yesno;
}


void
srecord::output_file::enable_footer(bool yesno)
{
    enable_footer_flag = yesno;
}


void
srecord::output_file::enable_optional_address(bool yesno)
{
    enable_optional_address_flag = yesno;
}


bool
srecord::output_file::enable_by_name(const std::string &name, bool yesno)
{
    struct table_t
    {
        const char *name;
        void (*func)(bool);
    };

    static const table_t table[] =
    {
        { "Header", &srecord::output_file::enable_header },
        { "Data_Count", &srecord::output_file::enable_data_count },
        { "Execution_Start_Address", &srecord::output_file::enable_goto_addr },
        { "Footer", &srecord::output_file::enable_footer },
        { "Optional_Address",
          &srecord::output_file::enable_optional_address },
    };

    for (const table_t *tp = table; tp < ENDOF(table); ++tp)
    {
        if (arglex::compare(tp->name, name.c_str()))
        {
            tp->func(yesno);
            return true;
        }
    }
    return false;
}


void
srecord::output_file::set_is_regular(void)
{
    FILE *fp = (FILE *)vfp;
    struct stat st;
    is_regular = fstat(fileno(fp), &st) == 0 && S_ISREG(st.st_mode);
}


void
srecord::output_file::fatal_hole_error(unsigned long lo, unsigned long hi)
{
    fatal_error
    (
        "The %s output format is unable to cope with holes in the data,"
        "however there is a hole at 0x%04lX..0x%04lX.",
        format_name(),
        lo,
        hi - 1
    );
}


void
srecord::output_file::fatal_alignment_error(int multiple)
{
    if (multiple > 4)
    {
        fatal_error
        (
            "The %s output format uses %d-byte alignment, but unaligned "
                "data is present.  Use a \"--fill 0xNN --within <input> "
                "--range-padding %d\" filter to fix this problem.",
            format_name(),
            multiple,
            multiple
        );
    }
    else
    {
        fatal_error
        (
            "The %s output format uses %d-bit data, but unaligned "
                "data is present.  Use a \"--fill 0xNN --within "
                "<input> --range-padding %d\" filter to fix this "
                "problem.",
            format_name(),
            multiple * 8,
            multiple
        );
    }
}


void
srecord::output_file::data_address_too_large(const srecord::record &record,
    unsigned nbits) const
{
    unsigned long lo = record.get_address();
    unsigned long hi = lo + record.get_length() - 1;
    assert(nbits <= 32);
    if (nbits > 0)
    {
        int prec = (nbits + 3) / 4;
        fatal_error
        (
            "data address range (0x%.*lX..0x%.*lX) is too large, "
                "the available range is only (0x%.*lx..0x%.*lX)",
            prec,
            lo,
            prec,
            hi,
            prec,
            0uL,
            prec,
            (1uL << nbits) - 1
        );
    }
    fatal_error("data address (0x%lX..0x%lX) too large", lo, hi);
}


// vim: set ts=8 sw=4 et :
