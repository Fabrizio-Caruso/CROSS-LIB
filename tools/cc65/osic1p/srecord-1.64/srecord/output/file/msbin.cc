//
// srecord - manipulate eprom load files
// Copyright (C) 2009-2011 Peter Miller
//
// Code contribution by David Kozub <zub@linux.fjfi.cvut.cz>
// Copyright assigned to Peter Miller 27-Jan-2010.
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

#include <srecord/output/file/msbin.h>


srecord::output_file_msbin::~output_file_msbin()
{
    flush_pending_records();

    if (start_address_set && enable_goto_addr_flag)
    {
        if (beginning_of_file)
        {
            // This could be possible to support, but then a file
            // header with fake data would have to be generated.
            fatal_error("no data, only the execution start record present");
        }
        write_record_header(0, start_address, 0);
    }
    else
    {
        warning
        (
            "no execution start address record present, although "
            "it's required by format specification"
        );
    }
}


srecord::output_file_msbin::output_file_msbin(
    const std::string &a_file_name
) :
    srecord::output_file(a_file_name),
    start_address_set(false),
    start_address(0),
    beginning_of_file(true)
{
    line_termination = line_termination_binary;
}


srecord::output::pointer
srecord::output_file_msbin::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_msbin(a_file_name));
}


void
srecord::output_file_msbin::write_dword_le(uint32_t d)
{
    unsigned char c[sizeof(uint32_t)];

    srecord::record::encode_little_endian(c, d, sizeof(c));

    for (size_t i = 0; i < sizeof(c); ++i)
        put_char(c[i]);
}


uint32_t
srecord::output_file_msbin::checksum(const unsigned char *data, size_t len)
{
    uint32_t sum = 0;

    for (size_t i = 0; i < len; ++i)
        sum += data[i];

    return sum;
}


void
srecord::output_file_msbin::write_file_header(uint32_t start, uint32_t length)
{
    // Write magic
    static const unsigned char Magic[7] =
        { 'B', '0', '0', '0', 'F', 'F', '\n' };
    for (size_t i = 0; i < sizeof(Magic); ++i)
        put_char(Magic[i]);

    // Write header itself
    write_dword_le(start);
    write_dword_le(length);
}


void
srecord::output_file_msbin::write_record_header(uint32_t addr, uint32_t length,
    uint32_t checksum)
{
    write_dword_le(addr);
    write_dword_le(length);
    write_dword_le(checksum);
}


void
srecord::output_file_msbin::write_data(const record &r)
{
    const unsigned char *data = r.get_data();
    size_t length = r.get_length();
    while (length-- > 0)
        put_char(*data++);
}


void
srecord::output_file_msbin::flush_pending_records(const record *r)
{
    if (!pending_records.empty() || r != NULL)
    {
        // write header
        const record::address_t start =
            (
                pending_records.empty()
            ?
                r->get_address()
            :
                pending_records.front()->get_address()
            );

        const record::address_t end =
            (
                r
            ?
                r->get_address_end()
            :
                pending_records.back()->get_address_end()
            );

        record_vector::const_iterator it;
        uint32_t chksum = 0;
        for (it = pending_records.begin(); it != pending_records.end(); ++it)
            chksum += checksum((*it)->get_data(), (*it)->get_length());

        if (r)
            chksum += checksum(r->get_data(), r->get_length());

        write_record_header(start, end - start, chksum);

        // write data
        for (it = pending_records.begin(); it != pending_records.end(); ++it)
            write_data(*(*it));

        if (r)
            write_data(*r);

        pending_records.clear();
    }
}


void
srecord::output_file_msbin::append_pending_record(const record &r)
{
    static const char *warning_size_exceeded =
        "cannot concatenate records: internal memory limit "
        "exceeded; creating a new record";

    if (r.get_address() == 0)
    {
        fatal_error
        (
            "the MsBin format cannot express a data value at "
            "the address 0"
        );
    }

    // check if record can be appended
    if
    (
        pending_records.empty()
    ||
        pending_records.back()->get_address_end() == r.get_address()
    )
    {
        // can be possibly appended, check size constraints
        size_t pending_size = 0;
        for (record_vector::const_iterator it = pending_records.begin();
            it != pending_records.end(); ++it)
            pending_size += (*it)->get_length();

        if (pending_size + r.get_length() > MAX_PENDING_DATA_SIZE)
        {
            // cannot append the record due to max pending data size
            // limit, but r immediatelly follows pending_records =>
            // flush (pending_records, r)
            warning("%s", warning_size_exceeded);
            flush_pending_records(&r);
        }
        else
            pending_records.push_back(boost::shared_ptr<record>(new record(r)));
    }
    else
    {
        flush_pending_records();
        assert(pending_records.empty());

        if (r.get_length() > MAX_PENDING_DATA_SIZE)
        {
            // r cannot be stored at all => write it out immediatelly
            warning("%s", warning_size_exceeded);
            flush_pending_records(&r);
        }
        else
            pending_records.push_back(boost::shared_ptr<record>(new record(r)));
    }
}


void
srecord::output_file_msbin::notify_upper_bound(unsigned long addr)
{
    upper_bound = addr;
}


void
srecord::output_file_msbin::write(const srecord::record &record)
{
    switch (record.get_type())
    {
    case srecord::record::type_execution_start_address:
        // Just note down the execution start address. It must be
        // the last record in the MsBin format.
        start_address = record.get_address();
        start_address_set = true;
        break;

    case srecord::record::type_data:
        if (beginning_of_file)
        {
            const unsigned long start = record.get_address();
            const unsigned long length = upper_bound - start;
            write_file_header(start, length);
            beginning_of_file = false;
        }
        assert
        (
            record.get_length() == 0
        ||
            record.get_address() + record.get_length() <= upper_bound
        );
        append_pending_record(record);
        break;

    case srecord::record::type_header:
    case srecord::record::type_data_count:
    case srecord::record::type_unknown:
        // Ignore.
        break;
    }
}


void
srecord::output_file_msbin::line_length_set(int)
{
    //
    // Irrelevant.  Ignore.
    //
}


void
srecord::output_file_msbin::address_length_set(int)
{
    //
    // Irrelevant.  Ignore.
    //
}


bool
srecord::output_file_msbin::preferred_block_size_set(int nbytes)
{
    return (nbytes >= 1 && nbytes <= record::max_data_length);
}


int
srecord::output_file_msbin::preferred_block_size_get()
    const
{
    //
    // Irrelevant.  Use the largest we can get.
    //
    return srecord::record::max_data_length;
}


const char *
srecord::output_file_msbin::format_name()
    const
{
    return "Windows CE Binary Image Data Format";
}


bool
srecord::output_file_msbin::is_binary(void)
    const
{
    return true;
}
