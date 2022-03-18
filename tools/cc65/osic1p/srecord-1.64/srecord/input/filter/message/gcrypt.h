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

#ifndef SRECORD_INPUT_FILTER_MESSAGE_GCRYPT_H
#define SRECORD_INPUT_FILTER_MESSAGE_GCRYPT_H

#include <srecord/input/filter/message.h>

namespace srecord
{

/**
  * The srecord::input_filter_message_gcrypt class is used to represent a
  * filter that runs the data through one of the hashing algorithms in
  * the gcrypt library.
  *
  * http://freshmeat.net/projects/libgcrypt/
  * http://directory.fsf.org/project/libgcrypt/
  */
class input_filter_message_gcrypt:
    public input_filter_message
{
public:
    /**
      * The destructor.
      */
    virtual ~input_filter_message_gcrypt();

private:
    /**
      * The constructor.  It is private on purpose, use one of the
      * create_* class method instead.
      *
      * @param deeper
      *     The source of data to be filtered.
      * @param algo
      *     The algorithm number (private to class).
      * @param hmac
      *     Turn the hash into a HMAC.
      * @param address
      *     Where to place the hash in memory.
      * @param hmac
      *     Add a message authentication code
      */
    input_filter_message_gcrypt(const input::pointer &deeper,
        unsigned long address, int algo, bool hmac);

private:
    /**
      * The create class method is used to create a new dynamically
      * allocated instance of this class.
      *
      * @param deeper
      *     The source of data to be filtered.
      * @param algo
      *     The algorithm to be used.  This is for debug.
      *     The actual algorithm numbers are private to the class.
      *     Choose a random number, it will likely segfault.  Seriously.
      * @param address
      *     Where to place the hash in memory.
      * @param hmac
      *     Add a message authentication code
      */
    static pointer create(const input::pointer &deeper, unsigned long address,
        int algo, bool hmac = false);

    /**
      * The algorithm_from_name class method is used to translate an
      * algorithm name into an algorithm number.
      *
      * @param name
      *     The name of the algorithm.
      * @return
      *     an algorithm number.  It does not return if the name is
      *     unknown, it exits with a fatal error message instead.
      */
    static int algorithm_from_name(const char *name);

public:
    /**
      * The create class method is used to create a new dynamically
      * allocated instance of this class.
      *
      * @param deeper
      *     The source of data to be filtered.
      * @param address
      *     Where to place the hash in memory.
      * @param algo
      *     The algorithm to be used.  This is for debug.
      *     The actual algorithm numbers are private to the class.
      * @param hmac
      *     Turn the hash into a HMAC.
      */
    static pointer create(const input::pointer &deeper, unsigned long address,
        const char *algo, bool hmac = false);

    /**
      * The create_md5 class method is used to create a new dynamically
      * allocated instance of this class, that calculates MD5 hashes.
      *
      * @param deeper
      *     The source of data to be filtered.
      * @param address
      *     Where to place the hash in memory.
      */
    static pointer create_md5(const input::pointer &deeper,
        unsigned long address);

    /**
      * The create_sha1 class method is used to create a new dynamically
      * allocated instance of this class, that calculates SHA1 hashes.
      *
      * @param deeper
      *     The source of data to be filtered.
      * @param address
      *     Where to place the hash in memory.
      */
    static pointer create_sha1(const input::pointer &deeper,
        unsigned long address);

    /**
      * The create_rmd160 class method is used to create a new dynamically
      * allocated instance of this class, that calculates RMD160 hashes.
      *
      * @param deeper
      *     The source of data to be filtered.
      * @param address
      *     Where to place the hash in memory.
      */
    static pointer create_rmd160(const input::pointer &deeper,
        unsigned long address);

    /**
      * The create_md2 class method is used to create a new dynamically
      * allocated instance of this class, that calculates MD2 hashes.
      *
      * @param deeper
      *     The source of data to be filtered.
      * @param address
      *     Where to place the hash in memory.
      */
    static pointer create_md2(const input::pointer &deeper,
        unsigned long address);

    /**
      * The create_tiger class method is used to create a new dynamically
      * allocated instance of this class, that calculates TIGER/192 hashes.
      *
      * @param deeper
      *     The source of data to be filtered.
      * @param address
      *     Where to place the hash in memory.
      */
    static pointer create_tiger(const input::pointer &deeper,
        unsigned long address);

    /**
      * The create_haval class method is used to create a new dynamically
      * allocated instance of this class, that calculates HAVAL/160 hashes.
      *
      * @param deeper
      *     The source of data to be filtered.
      * @param address
      *     Where to place the hash in memory.
      */
    static pointer create_haval(const input::pointer &deeper,
        unsigned long address);

    /**
      * The create_sha256 class method is used to create a new dynamically
      * allocated instance of this class, that calculates SHA-256 hashes.
      *
      * @param deeper
      *     The source of data to be filtered.
      * @param address
      *     Where to place the hash in memory.
      */
    static pointer create_sha256(const input::pointer &deeper,
        unsigned long address);

    /**
      * The create_sha384 class method is used to create a new dynamically
      * allocated instance of this class, that calculates SHA-384 hashes.
      *
      * @param deeper
      *     The source of data to be filtered.
      * @param address
      *     Where to place the hash in memory.
      */
    static pointer create_sha384(const input::pointer &deeper,
        unsigned long address);

    /**
      * The create_sha512 class method is used to create a new dynamically
      * allocated instance of this class, that calculates SHA-512 hashes.
      *
      * @param deeper
      *     The source of data to be filtered.
      * @param address
      *     Where to place the hash in memory.
      */
    static pointer create_sha512(const input::pointer &deeper,
        unsigned long address);

    /**
      * The create_sha224 class method is used to create a new dynamically
      * allocated instance of this class, that calculates SHA-224 hashes.
      *
      * @param deeper
      *     The source of data to be filtered.
      * @param address
      *     Where to place the hash in memory.
      */
    static pointer create_sha224(const input::pointer &deeper,
        unsigned long address);

    /**
      * The create_md4 class method is used to create a new dynamically
      * allocated instance of this class, that calculates MD4 hashes.
      *
      * @param deeper
      *     The source of data to be filtered.
      * @param address
      *     Where to place the hash in memory.
      */
    static pointer create_md4(const input::pointer &deeper,
        unsigned long address);

    /**
      * The create_crc32 class method is used to create a new dynamically
      * allocated instance of this class, that calculates CRC-32 hashes.
      *
      * @param deeper
      *     The source of data to be filtered.
      * @param address
      *     Where to place the hash in memory.
      */
    static pointer create_crc32(const input::pointer &deeper,
        unsigned long address);

    /**
      * The create_crc32_rfc1510 class method is used to create a new
      * dynamically allocated instance of this class, that calculates
      * CRC32 RFC1510 hashes.
      *
      * @param deeper
      *     The source of data to be filtered.
      * @param address
      *     Where to place the hash in memory.
      */
    static pointer create_crc32_rfc1510(const input::pointer &deeper,
        unsigned long address);

    /**
      * The create_crc24_rfc2440 class method is used to create a new
      * dynamically allocated instance of this class, that calculates
      * CRC24 RFC2440 hashes.
      *
      * @param deeper
      *     The source of data to be filtered.
      * @param address
      *     Where to place the hash in memory.
      */
    static pointer create_crc24_rfc2440(const input::pointer &deeper,
        unsigned long address);

    /**
      * The create_whirlpool class method is used to create a new
      * dynamically allocated instance of this class, that calculates
      * WHIRLPOOL hashes.
      *
      * @param deeper
      *     The source of data to be filtered.
      * @param address
      *     Where to place the hash in memory.
      */
    static pointer create_whirlpool(const input::pointer &deeper,
        unsigned long address);

protected:
    // See base class for documentation.
    void process(const memory &input, record &output);

    // See base class for documentation.
    const char *get_algorithm_name() const;

private:
    /**
      * The algo instance variable is used to remember the algorithm
      * number.
      */
    int algo;

    /**
      * The hmac instance variable is used to remember whether or not to
      * turn the hash into a HMAC.
      */
    bool hmac;

    /**
      * The address instance variable is used to remember where to place
      * the hash in memory.
      */
    unsigned long address;

    /**
      * The default constructor.  Do not use.
      */
    input_filter_message_gcrypt();

    /**
      * The copy constructor.  Do not use.
      */
    input_filter_message_gcrypt(const input_filter_message_gcrypt &);

    /**
      * The assignment operator.  Do not use.
      */
    input_filter_message_gcrypt &operator=(const input_filter_message_gcrypt &);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_INPUT_FILTER_MESSAGE_GCRYPT_H
