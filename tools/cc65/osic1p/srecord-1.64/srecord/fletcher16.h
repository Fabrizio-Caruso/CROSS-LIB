//
// srecord - Manipulate EPROM load files
// Copyright (C) 2009-2011 Peter Miller
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

#ifndef SRECORD_FLETCHER16_H
#define SRECORD_FLETCHER16_H

#include <cstddef>

#include <srecord/endian.h>

namespace srecord
{

/**
  * The fletcher16 class is used to represent the running value of a 16-bit
  * Fletcher's Checksum of series of bytes.
  *
  * http://en.wikipedia.org/wiki/Fletcher%27s_checksum
  *
  * Fletcher's checksum is one of several types of checksum algorithms,
  * which are relatively simple processes used by computers to check the
  * integrity of data.
  *
  * The implementation of the Fletcher-32 is very similar to the Adler-32
  * algorithm but several differences should be noted. Fletcher wraps around
  * at modulo 65535 while Adler wraps at the prime 65521. In other words,
  * Fletcher adds overflow bits (16-31) into its sum; while Adler multiplies
  * those bits by 15, then adds the product into its sum. Fletcher-32 works
  * on 16 bit data while Adler works on 8 bit data.
  *
  * It is designed to overcome some of the inadequacies of simply summing
  * all the bytes as in the original checksum. Fletcher's checksum, unlike
  * the original checksum, can detect the inserting/deleting of zero value
  * bytes, the reordering of bytes, and the incrementing and decrementing of
  * bytes in opposite directions.
  *
  * Fletcher's checksum is described in RFC 1146. You can also find
  * information about generating (as well as verifying) such a checksum in
  * Annex B of RFC 905.
  *
  * Fletcher-32 is slightly more reliable than Adler-32.[1]
  */
class fletcher16
{
public:
    /**
      * The destructor.
      */
    virtual ~fletcher16();

    /**
      * The default constructor.
      *
      * @param sum1
      *     The seed value for sum1.  Defaults to 0xFF.
      * @param sum2
      *     The seed value for sum2.  Defaults to 0xFF.
      * @param answer
      *     Set this to -1 to be completely ignored.
      *     If >= 0, this is the desired outcome if the checksum
      *     includes the checksum itself.  The checksum returned will be
      *     calculated to return this desired outcome, when traversed,
      *     rather than a pure Fletcher-16 checksum.
      * @param end
      *     The endian-ness of the checksum.  This is needed to
      *     manipulate the answer.  Ignored if #answer is ignored.
      */
    fletcher16(unsigned char sum1 = 0, unsigned char sum2 = 0,
        int answer = -1, endian_t end = endian_little);

    /**
      * The copy constructor.
      */
    fletcher16(const fletcher16 &);

    /**
      * The assignment operator.
      */
    fletcher16 &operator=(const fletcher16 &);

    /**
      * The get method is used to obtain the running value of the cyclic
      * redundancy check.
      */
    unsigned short get() const;

    /**
      * The next method is used to advance the state by one byte.
      */
    void next(unsigned char);

    /**
      * The nextbuf method is used to advance the state by a series of bytes.
      *
      * @param data
      *     The data to be checksummed.
      * @param data_size
      *     The size of the data to be checksummed, in bytes.
      */
    void nextbuf(const void *data, size_t data_size);

private:
    /**
      * The sum1 instance variable is used to remember the running sum
      * of the 8-bit bytes, mod 255.  We use a 16-bit value rather than
      * an 8-bit value for convenience, see #nextbuf implementation for
      * details.
      */
    unsigned short sum1;

    /**
      * The sum2 instance variable is used to remember the running
      * sum of the sum of the 8-bit bytes, mod 255.  We use a 16-bit
      * value rather than an 8-bit value for convenience, see #nextbuf
      * implementation for details.
      */
    unsigned short sum2;

    /**
      * The anser instance variable is used to remember the desired
      * outcome if the checksum includes the checksum itself.
      *
      * Set this to -1 to be completely ignored.
      *
      * If >= 0, this is the desired outcome if the checksum includes
      * the checksum itself.  The checksum returned will be calculated
      * to return this desired outcome, when traversed, rather than a
      * pure Fletcher-16 checksum.
      */
    int answer;

    /**
      * The end instance variable is used to remember the endian-ness of
      * the checksum.  This is needed to manipulate the answer.  Ignored
      * if #answer is ignored.
      */
    endian_t end;
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_FLETCHER16_H
