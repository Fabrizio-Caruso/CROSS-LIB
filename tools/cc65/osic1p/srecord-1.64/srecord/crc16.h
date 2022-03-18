//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2003, 2006-2010, 2012 Peter Miller
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
//
// See http://www.joegeluso.com/software/articles/ccitt.htm for a great
// write-up on the CRC16 calculation.
//

#ifndef SRECORD_CRC16_H
#define SRECORD_CRC16_H

#include <stddef.h>

namespace srecord
{

/**
  * The crc16 class is used to represent the running value of a 16-bit
  * cyclic redundancy check of series of bytes.
  *
  * Note: this CRC16 works for systems that process the leaast
  * significant bit of each byte first, and working to wards the most
  * significant bit.
  *
  * If you were expecting MSB first (you may not even know it) this
  * code will give you "wrong" answers.  Try using the bitrev filter.
  */
class crc16
{
public:
    // See http://en.wikipedia.org/wiki/Cyclic_redundancy_check
    // for table of polynomials.
    enum
    {
        /**
          * X.25, V.41, HDLC, XMODEM, Bluetooth, SD, CRC-CCITT
          */
        polynomial_ccitt = 0x1021,

        /**
          * CRC-16-IBM, Bisync, Modbus, USB, ANSI X3.28, CRC-16-ANSI
          */
        polynomial_ansi = 0x8005,

        /**
          * CRC-16-DNP, DNP, IEC 870, M-Bus
          */
        polynomial_dnp = 0x3D65,

        /**
          * CRC-16-DECT
          */
        polynomial_dect = 0x0589,

        /**
          * CRC-16-T10-DIF, SCSI DIF
          */
        polynomial_t10_dif = 0x8BB7,
    };

    /**
      * The polynomial_by_name class method is used to take the name
      * of a CRC-16 polynamlial, and return the value of the CRC-16
      * polynomial corresponding to that name.
      */
    static int polynomial_by_name(const char *name);

    enum seed_mode_t
    {
        seed_mode_ccitt,
        seed_mode_xmodem,
        seed_mode_broken
    };

    /**
      * The destructor.
      */
    virtual ~crc16();

    enum bit_direction_t
    {
        bit_direction_most_to_least,
        bit_direction_least_to_most,
    };

    /**
      * The default constructor.
      *
      * @param seed_mode
      *     This selects which CRC16 calculation seed is to be used.
      * @param augment
      *     This is true if the 16-zero-bit augmentation is desired.
      *     This is the default.  False if no augmentation is desired.
      * @param polynomial
      *     The checksum polynomial.
      * @param bitdir
      *     The direction of bits in a characters as they pass through
      *     the algorithm.
      */
    crc16(seed_mode_t seed_mode = seed_mode_ccitt, bool augment = true,
        unsigned short polynomial = polynomial_ccitt,
        bit_direction_t bitdir = bit_direction_most_to_least);

    /**
      * The copy constructor.
      */
    crc16(const crc16 &);

    /**
      * The assignment operator.
      */
    crc16 &operator=(const crc16 &);

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
      */
    void nextbuf(const void *, size_t);

    /**
      * The print_table method may be used to print the table being used.
      * This is principally for debugging the table generation process.
      */
    void print_table(void) const;

private:
    /**
      * The state instance variable is used to remember the running
      * value of the 16-bit cyclic redundancy check.
      */
    unsigned short state;

    /**
      * The augment instance variable is used to remember whether or
      * not the 16 zero bits of augmentation are to be processed before
      * reporting the result.
      */
    bool augment;

    /**
      * The polynomial instance variable is used to remember the CRC
      * polynomial being calculated with.  Note that if the bitdir is
      * low-to-high then the bits in the polynomial will be reversed.
      */
    unsigned short polynomial;

    /**
      * The bitdir instance variable is sued to remember the direction
      * of bits in a characters as they pass through the algorithm.
      */
    bit_direction_t bitdir;

    /**
      * The table instance variable is used to remember the results of 8
      * shift-and-process operations for each byte value.  This is used
      * to improve efficiency.  It is filled in the the #calculate_table
      * method, called from the constructor.
      */
    unsigned short table[256];

    /**
      * The calculate_table method is called by the constructor to fill
      * in the #table instance variable.
      */
    void calculate_table(void);

    /**
      * The updcrc method is to add another byte of data to the running
      * CRC state.  It is called by the #next and next_buf methods.
      */
    inline unsigned short updcrc(unsigned char c, unsigned short state) const;
};

};

#endif // SRECORD_CRC16_H
