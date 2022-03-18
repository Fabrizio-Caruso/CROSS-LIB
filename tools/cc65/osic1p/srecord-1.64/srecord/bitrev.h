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

#ifndef SRECORD_BITREV_H
#define SRECORD_BITREV_H

namespace srecord
{

/**
  * The bitrev8 function is used to reverse the order of the bits in an
  * 8-bit number.  This function is its own inverse.
  *
  * @param value
  *     The value to be reversed.
  */
unsigned char bitrev8(unsigned char value);

/**
  * The bitrev16 function is used to reverse the order of the bits in an
  * 16-bit number.  This function is its own inverse.
  *
  * @param value
  *     The value to be reversed.
  */
unsigned short bitrev16(unsigned short value);

/**
  * The bitrev24 function is used to reverse the order of the bits in an
  * 24-bit number.  This function is its own inverse.
  *
  * @param value
  *     The value to be reversed.
  */
unsigned long bitrev24(unsigned long value);

/**
  * The bitrev32 function is used to reverse the order of the bits in an
  * 32-bit number.  This function is its own inverse.
  *
  * @param value
  *     The value to be reversed.
  */
unsigned long bitrev32(unsigned long value);

/**
  * The bitrev40 function is used to reverse the order of the bits in an
  * 40-bit number.  This function is its own inverse.
  *
  * @param value
  *     The value to be reversed.
  */
unsigned long long bitrev40(unsigned long long value);

/**
  * The bitrev48 function is used to reverse the order of the bits in an
  * 48-bit number.  This function is its own inverse.
  *
  * @param value
  *     The value to be reversed.
  */
unsigned long long bitrev48(unsigned long long value);

/**
  * The bitrev56 function is used to reverse the order of the bits in an
  * 56-bit number.  This function is its own inverse.
  *
  * @param value
  *     The value to be reversed.
  */
unsigned long long bitrev56(unsigned long long value);

/**
  * The bitrev64 function is used to reverse the order of the bits in an
  * 64-bit number.  This function is its own inverse.
  *
  * @param value
  *     The value to be reversed.
  */
unsigned long long bitrev64(unsigned long long value);

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_BITREV_H
