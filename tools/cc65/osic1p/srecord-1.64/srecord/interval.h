//
// srecord - manipulate eprom load files
// Copyright (C) 1998-2000, 2002-2010 Peter Miller
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

#ifndef SRECORD_INTERVAL_H
#define SRECORD_INTERVAL_H

#include <cstddef>
#include <stdint.h>
#include <iostream>

namespace srecord
{

/**
  * The interval class is used to represent a set of integer values,
  * usually composed of runs of adjacent value.  Set arithmetic is
  * implemented on these intervals.
  */
class interval
{
public:
    /**
      * The destructor.  It isn't virtual, so don't derive from this class.
      */
    ~interval();

    /**
      * The data_t type is used to parameterize the integr type used in
      * the interval.
      */
    typedef uint32_t data_t;
    typedef uint64_t long_data_t;

    /**
      * The default constructor.  The interval is empty.
      */
    interval();

    /**
      * The constructor.  The interval contains the single integer supplied.
      *
      * \param val
      *    The interval is constructed to contain the single interger
      *    value supplied.
      */
    interval(data_t val);

    /**
      * The constructor.  The interval contains all values >= lo and < hi.
      *
      * \param lo
      *     The lower bound of the integers in the initial interval.
      * \param hi
      *     The upper bound of the integers in the initial interval;
      *     this value is not included.
      */
    interval(data_t lo, data_t hi);

    /**
      * The copy constructor.
      */
    interval(const interval &);

    /**
      * The assignment operator.
      */
    interval &operator=(const interval &);

    /**
      * The union_ class method is used to calculate the set union of
      * two intervals.
      */
    static interval union_(const interval &, const interval &);

    /**
      * The intersection class method is used to calculate the set
      * intersection of two intervals.
      */
    static interval intersection(const interval &, const interval &);

    /**
      * The difference class method is used to calculate the set
      * difference of two intervals.
      */
    static interval difference(const interval &, const interval &);

    /**
      * The equal class method is used to test the equality of two
      * intervals.
      */
    static bool equal(const interval &, const interval &);

    /**
      * The member method is used to test whether a given value is a
      * member of the interval.
      *
      * \param val
      *     The value to test for membership
      * \returns
      *     True if the given value is a member of the interval,
      *     false if it is not.
      */
    bool member(data_t val) const;

    /**
      * The empty method is used to test whether the interval is empty.
      *
      * \returns
      *     True if the interval is empty,
      *     false if the interval is not empty.
      */
    bool empty() const;

    /**
      * The first_interval_only method is used to crop the interval to the
      * first (numerically least) run of consecutive integers in the set.
      */
    void first_interval_only();

    /**
      * The interval_scan_begin method is used to start traversing every
      * integer value in the interval.
      */
    void scan_begin();

    /**
      * The interval_scan_next method is used to traverse every integer
      * value in the interval.
      */
    bool scan_next(data_t &);

    /**
      * The interval_scan_end method is used to finish traversing every
      * integer value in the interval.
      */
    void scan_end();

    /**
      * The get_lowest method is used to obtain the lower bound of
      * the interval.  It is inclusive.
      */
    data_t get_lowest() const;

    /**
      * The get_highest method is used to obtain the upper bound of
      * the interval.  It is exclusive (i.e. one beyond the highest
      * integer in the set).
      */
    data_t get_highest() const;

    /**
      * The print method is used to print an interval on an output stream.
      */
    void print(std::ostream &) const;

    /**
      * The pad method is used to expand an interval by padding each
      * sub-interval.  For each sub-interval the minimum is rounded
      * down to a multiple of x, and the upper bound is rounded up to a
      * multiple of x.
      *
      * @param x
      *     The multiple to expand to.  No action is taken if x < 2.
      * @returns
      *     new padded interval.
      */
    interval pad(int x) const;

    /**
      * The representation method is used to manufacture a textural
      * representation of this interval.
      */
    std::string representation() const;

    /**
      * The flatten method is used to convert an interval with holes
      * in it to a simple interval with no holes in it, by using the
      * mimimum and maximum values.
      */
    interval flatten() const;

    /**
      * The coverage method is used to obtain the size of the interval
      * in bytes.
      *
      * @returns
      *     The number of bytes cobered by the interval, NOT including
      *     holes.  Because the result could be 2**32, the 64-bit result
      *     is used so that this is representable.
      */
    long_data_t coverage() const;

private:
    /**
      * The length instance variable is used to remember the length of
      * the data instance variable.  This is almost always even, because
      * the internal is a series of [lo, hi) sub-intervals.
      */
    size_t length;

    /**
      * The size instance variable is used to remember the maximum size
      * of the data instance variable.  The length can go up and down
      * depending on the calculation, but the size only ever rises.
      */
    size_t size;

    /**
      * The scan_index instance variable is used to remember where the
      * scan us up to.  Used by the scan_next method, et al.
      */
    size_t scan_index;

    /**
      * The scan_next_datum instance variable is used to remember where
      * the scan us up to.  Used by the scan_next method, et al.
      */
    data_t scan_next_datum;

    /**
      * The data instance variable is used to remember a pointer to
      * the base of an array of interger values.  They come in [lo, hi)
      * pairs.  As a sanity check, there is an extra item, wich contains
      * the same value as the length instance variable.
      */
    data_t *data;

    /**
      * The valid method is used to test whether the interval is
      * internally self-consistent.  Principally of use when debugging.
      */
    bool valid() const;

    /**
      * The append method is used to append another value to the end
      * of an interval under construction.  This breaks the "length is
      * even" assertion which usually applies; for this reason it is
      * only to be used by the arithmetic operator implementations when
      * calculating their results.
      */
    void append(data_t);
};

/**
  * The equality operator is used to determine if two intervals are the
  * same.
  */
inline bool
operator == (const interval &lhs, const interval &rhs)
{
    return interval::equal(lhs, rhs);
}

/**
  * The inequality operator is used to determine if two intervals are
  * different.
  */
inline bool
operator != (const interval &lhs, const interval &rhs)
{
    return !interval::equal(lhs, rhs);
}

/**
  * The binary star operator is used to calculate the intersection of
  * two intervals.
  */
inline interval
operator * (const interval &lhs, const interval &rhs)
{
    return interval::intersection(lhs, rhs);
}

/**
  * The star-and-replace operator is used to calculate the intersection
  * of two intervals, and assign the result to the left-hand-side.
  */
inline interval &
operator *= (interval &lhs, const interval &rhs)
{
    lhs = interval::intersection(lhs, rhs);
    return lhs;
}

/**
  * The binary plus operator is used to calculate the union of two
  * intervals.
  */
inline interval
operator + (const interval &lhs, const interval &rhs)
{
    return interval::union_(lhs, rhs);
}

/**
  * The plus-and-replace operator is used to calculate the union of two
  * intervals, and assign the result to the left-hand-side.
  */
inline interval &
operator += (interval &lhs, const interval &rhs)
{
    lhs = interval::union_(lhs, rhs);
    return lhs;
}

/**
  * The binary minus operator is used to calculate the difference of two
  * intervals.
  */
inline interval
operator - (const interval &lhs, const interval &rhs)
{
    return interval::difference(lhs, rhs);
}

/**
  * The minus-and-replace operator is used to calculate the difference
  * of two intervals, and assign the result to the left-hand-side.
  */
inline interval &
operator -= (interval &lhs, const interval &rhs)
{
    lhs = interval::difference(lhs, rhs);
    return lhs;
}

/**
  * The unary minus operator is used to calculate the logical complement
  * (inverse, negative) of an interval.
  */
inline interval
operator - (const interval &arg)
{
    return (interval(0, 0) - arg);
}

/**
  * The binary left-shift operator is used to print an interval on an
  * output stream.
  */
inline std::ostream &
operator << (std::ostream &os, const interval &val)
{
    val.print(os);
    return os;
}

};

#endif // SRECORD_INTERVAL_H
