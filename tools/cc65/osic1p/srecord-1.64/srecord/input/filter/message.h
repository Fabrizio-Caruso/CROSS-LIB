//
// srecord - Manipulate EPROM load files
// Copyright (C) 2009-2012 Peter Miller
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

#ifndef SRECORD_INPUT_FILTER_MESSAGE_H
#define SRECORD_INPUT_FILTER_MESSAGE_H

#include <srecord/input/filter.h>
#include <srecord/memory.h>

namespace srecord
{

/**
  * The srecord::input_filter_message class is used to represent an abstract
  * base class for filters that must operate on the complete data, in
  * order, in order to functions (e.g. CRC, message digest, etc).
  *
  * All of the machinery for accumulating the input data and eventually
  * forwarding it on are in this common base class.  The only methods
  * that a derived class must supply are data block processing and
  * results creation.
  */
class input_filter_message:
    public input_filter
{
public:
    /**
      * The destructor.
      */
    virtual ~input_filter_message();

protected:
    /**
      * The constructor.
      *
      * @param deeper
      *     The data to be filtered.
      * @param naked
      *     Whether or not to forward the data: true means result only
      *     with no data, false means result and data.
      */
    input_filter_message(const input::pointer &deeper, bool naked = false);

    // See base class for documentation.
    bool read(record &record);

    /**
      * The process method is used to process the data from the input.
      *
      * @param input
      *     The memory representation to be processed.
      * @param output
      *     The filter's output.
      */
    virtual void process(const memory &input, record &output) = 0;

    /**
      * The get_algorithm_name method is used in error messages.
      */
    virtual const char *get_algorithm_name() const = 0;

    /**
      * The get_minimum_alignment method is used to obtain the minium
      * require dbyte alignment.  Returns 0 if irrelevant.
      */
    virtual unsigned get_minimum_alignment(void) const;

private:
    /**
      * The naked instance variable is used to remember whether or not
      * to forward the data: true means result only with no data, false
      * means result and data.
      */
    bool naked;

    /**
      * The buffer instance variable is used to remember the contents
      * of the deeper file.  The deeper file must be read completely
      * in order to calculate the result, and the input may be out of
      * address order, necessitating this buffer.
      */
    memory buffer;

    /**
      * The buffer_pos instance variable is used to remember where we
      * are up to in processing 'buffer'.
      */
    unsigned long buffer_pos;

    /**
      * The have_forwarded_header instance variable is used to remember
      * whether we have returned the file header to our reader yet.
      */
    bool have_forwarded_header;

    /**
      * The have_given_result instance variable is used to remember
      * whether we have returned the result to our reader yet.
      */
    bool have_given_result;

    /**
      * The have_forwarded_start_address instance variable is used to
      * remember whether we have returned the execution start address to
      * our reader yet.
      */
    bool have_forwarded_start_address;

    /**
      * The default constructor.  Do not use.
      */
    input_filter_message();

    /**
      * The copy constructor.  Do not use.
      */
    input_filter_message(const input_filter_message &);

    /**
      * The assignment operator.  Do not use.
      */
    input_filter_message &operator=(const input_filter_message &);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_INPUT_FILTER_MESSAGE_H
