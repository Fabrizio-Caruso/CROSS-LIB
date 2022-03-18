//
// srecord - manipulate eprom load files
// Copyright (C) 1998-2003, 2006-2008, 2010, 2012-2014 Peter Miller
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

#ifndef SRECORD_MEMORY_H
#define SRECORD_MEMORY_H

#include <string>

#include <srecord/defcon.h>
#include <srecord/input.h>
#include <srecord/memory/chunk.h>
#include <srecord/memory/walker.h>
#include <srecord/string.h>

namespace srecord {

class record; // forward

/**
  * The srecord::memory class is used to simulate memory contents.
  * This allows staging of reord adta, and also validation of contents.
  */
class memory
{
public:
    /**
      * The default constructor.
      */
    memory();

    /**
      * The copy constructor.
      */
    memory(const memory &);

    /**
      * The assignment operator.
      */
    memory &operator=(const memory &);

    /**
      * The destructor.
      */
    ~memory();

    /**
      * The set method is used to set the byte at the given `address'
      * to the given `value'.
      *
      * Uses the find() method to locate the chunk, and then calls
      * the memory_chunk::set method, to set the byte within
      * the chunk.
      */
    void set(unsigned long address, int value);

    /**
      * The get method is used to fetch the value of the byte at
      * the given 'address'.
      *
      * If you fo a get on an address which has not been set() yet,
      * the results are undefined.
      *
      * Uses the find() method to locate the chunk, and then calls
      * the memory_chunk::get method, to get the byte within
      * the chunk.
      */
    int get(unsigned long address) const;

    /**
      * The set_p method is used to determine whether the byte at
      * the given address has been set() yet.  Returns true if
      * already set, false if never been set.
      *
      * Uses the find() method to locate the chunk, and then calls
      * the memory_chunk::set_p method, to get the status of
      * the byte within the chunk.
      */
    bool set_p(unsigned long address) const;

    /**
      * The walk method is used to apply a memory_walker derived
      * class to every byte of memory.
      */
    void walk(memory_walker::pointer) const;

    /**
      * The reader method is used to read the given `input' source
      * into memory.  This method may be called multiple times,
      * for multiple input files.
      *
      * If the input format supports headers, and the file has a
      * header, the first header will be remembered, if set_header()
      * was not called previously.
      *
      * @param input
      *     The source of the byte stream
      * @param redundant_bytes
      *     ignore: do nothing
      *     warning: issue a warning and continue (default)
      *     error: issuse a atal error message, and exit failure.
      * @param contradictory_bytes
      *     If the value at any address is set more than once,
      *     Thid argument controls what happeens.
      *     ignore: do nothing
      *     warning: issue a warning and continue (default)
      *     error: issuse a atal error message, and exit failure (default).
      */
    void reader(const input::pointer &input, defcon_t redundant_bytes,
        defcon_t contradictory_bytes);

    /**
      * The equal method may be used to determine if two memory
      * instances are equal.
      */
    static bool equal(const memory &, const memory &);

    /**
      * The compare method may be used to determine if two memory
      * instances are equal.  Verbose messages concerning the exact
      * differences are printed if they are not equal.
      */
    static bool compare(const memory &, const memory &);

    /**
      * The find_next_data method may be used to locate data at or
      * following the `address' given.  Returns false if there is
      * no data meeting this criteria.
      *
      * If there is data, the `address' will be set to the start
      * address of the data block.      At most `nbytes' of data will
      * be transfetrred into the `data' array.  Then `nbytes' wil
      * be set to the number of bytes transferred.      Returns true.
      *
      * Calls the find_next_chunk() method.
      */
    bool find_next_data(unsigned long &address, void *data,
        size_t &nbytes) const;

    /**
      * The get_header method is used to determine the value of the
      * header record set by either the reader() or set_header()
      * methods.  If neither has set a header, a NULL is returned.
      */
    record *get_header(void) const;

    /**
      * The set_header method may be used to set the header command.
      *
      * @param value
      *     The `value' string, to set the header.  The user may use URL
      *     encoding for of the more interesting non-printable byte values
      *     (eg. "%3F").
      */
    void set_header(const std::string &value);

    /**
      * The get_execution_start_address method is used to determine the
      * value of the execution start address record set by either the
      * reader() or set_execution_start_address() methods.  If neither
      * has set an execution start address, NULL is returned.
      */
    record *get_execution_start_address(void) const;

    /**
      * The set_execution_start_address method may be used to set the
      * execution start address record.
      */
    void set_execution_start_address(unsigned long value);

    /**
      * The has_holes method may be used to determine whether or not the
      * memory data has "holes" in it.
      *
      * @returns
      *     true if the memory data is not continuous (has at least
      *     one hole), false if the memory data is continuous (has no
      *     holes).
      */
    bool has_holes(void) const;

    /**
      * The get_lower_bound method is used to obtain the lower bound
      * (lowest address) of the memory image.
      */
    unsigned long get_lower_bound() const;

    /**
      * The get_upper_bound method is used to obtain the upper bound
      * (maximum address plus one) of the memory image.
      */
    unsigned long get_upper_bound() const;

   /**
      * The is_well_aligned method is used to test whether or not
      * all of the data in the memory buffer is well aligned, on the
      * given byte boundary.
      *
      * @param multiple
      *     The desired byte alignment.
      *
      * @note
      *     This isn't about holes, it's about the beginnings and
      *     endings of each consecitice run of bytes.
      */
    bool is_well_aligned(unsigned multiple) const;

    /**
      * The empty method is used to determine whether or not this memory
      * image has no data in it.
      */
    bool
    empty(void)
        const
    {
        return (nchunks == 0);
    }

private:
    /**
      * The nchunks instance variable is used to member how large
      * our pool of memory chunks is.  It is always <= nchunks_max;
      */
    mutable int nchunks;

    /**
      * The max_chunks instance variable is used to rememberf the
      * size of the chunk array, which holds the pool of memory chunks.
      */
    mutable int nchunks_max;

    /**
      * The chunk instance variable is used to hold the pointer to
      * the base of the chunk array.  These chunks remember the
      * settings of the various bytes.  By using a sparse array,
      * we can cope with arbitrary memory usages.
      */
    mutable memory_chunk **chunk;

    /**
      * The find method is used to find the chunk which contains
      * the given `address'.  The chunk will be created if it
      * doesn't exist.
      *
      * Called by the get(), set() and set_p() methods.
      */
    memory_chunk *find(unsigned long address) const;

    /**
      * The cache instance variable is used to accellerate the find()
      * method, based on the fact that most memory accesses are
      * sequential, in the same chunk.
      */
    mutable memory_chunk *cache;

    /**
      * The find_next_chunk method is used to visit each and every
      * byte, in cases where the walk() method is not appropriate.
      * Called by the find_next_data() method.
      */
    memory_chunk *find_next_chunk(unsigned long) const;

    /**
      * The find_next_chunk_index instance variable is used by
      * the find_next_chunk() method to keep track of where it is
      * positioned across the chunk array.
      */
    mutable int find_next_chunk_index;

    /**
      * The header instance variable is used to track the file header.
      * It is set by the reader() and set_header() methods.  It is
      * read by the get_header() method.
      */
    record *header;

    /**
      * The execution_start_address instance variable is used to track
      * the execution start address.  It is set by the reader() and
      * set_execution_start_address() methods.  It is read by the
      * get_execution_start_address() method.
      */
    record *execution_start_address;

    /**
      * The clear method is used to discard all data, as if when
      * the instance was first constructed. Alsu used by the destructor.
      */
    void clear(void);

    /**
      * The copy method is used to copy the chunks from the `src' into
      * this object.  Only to be used the the assignment operator.
      */
    void copy(const memory &src);
};

};

/**
  * The equality operator.  Used to test if two srecord::memory objects
  * are equal.  Calls the srecord::memory::equal() method.
  */
bool operator == (const srecord::memory &, const srecord::memory &);

/**
  * The inequality operator.  Used to test if two srecord::memory objects
  * are not equal.  Calls the srecord::memory::equal() method.
  */
bool operator != (const srecord::memory &, const srecord::memory &);

// vim: set ts=8 sw=4 et :
#endif // SRECORD_MEMORY_H
