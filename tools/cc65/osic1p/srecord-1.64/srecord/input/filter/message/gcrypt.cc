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

#include <srecord/quit.h>
#include <srecord/sizeof.h>
#include <srecord/input/filter/message/gcrypt.h>
#include <srecord/memory/walker/gcrypt.h>
#include <srecord/record.h>


srecord::input_filter_message_gcrypt::~input_filter_message_gcrypt()
{
}


srecord::input_filter_message_gcrypt::input_filter_message_gcrypt(
    const input::pointer &a_deeper,
    unsigned long a_address,
    int a_algo,
    bool a_hmac
) :
    input_filter_message(a_deeper),
    algo(a_algo),
    hmac(a_hmac),
    address(a_address)
{
}


srecord::input::pointer
srecord::input_filter_message_gcrypt::create(const input::pointer &a_deeper,
    unsigned long a_address, int algo, bool hmac)
{
    return
        pointer
        (
            new input_filter_message_gcrypt
            (
                a_deeper,
                a_address,
                algo,
                hmac
            )
        );
}


srecord::input::pointer
srecord::input_filter_message_gcrypt::create(const input::pointer &a_deeper,
    unsigned long a_address, const char *name, bool a_hmac)
{
    return create(a_deeper, a_address, algorithm_from_name(name), a_hmac);
}


srecord::input::pointer
srecord::input_filter_message_gcrypt::create_md5(
    const input::pointer &a_deeper, unsigned long a_address)
{
#ifdef HAVE_LIBGCRYPT
    return create(a_deeper, a_address, GCRY_MD_MD5);
#else
    (void)a_deeper;
    (void)a_address;
    quit_default.fatal_error("libgcrypt not available");
    return pointer();
#endif
}


srecord::input::pointer
srecord::input_filter_message_gcrypt::create_sha1(
    const input::pointer &a_deeper, unsigned long a_address)
{
#ifdef HAVE_LIBGCRYPT
    return create(a_deeper, a_address, GCRY_MD_SHA1);
#else
    (void)a_deeper;
    (void)a_address;
    quit_default.fatal_error("libgcrypt not available");
    return pointer();
#endif
}


srecord::input::pointer
srecord::input_filter_message_gcrypt::create_rmd160(
    const input::pointer &a_deeper, unsigned long a_address)
{
#ifdef HAVE_LIBGCRYPT
    return create(a_deeper, a_address, GCRY_MD_RMD160);
#else
    (void)a_deeper;
    (void)a_address;
    quit_default.fatal_error("libgcrypt not available");
    return pointer();
#endif
}


srecord::input::pointer
srecord::input_filter_message_gcrypt::create_md2(
    const input::pointer &a_deeper, unsigned long a_address)
{
#ifdef HAVE_LIBGCRYPT
    return create(a_deeper, a_address, GCRY_MD_MD2);
#else
    (void)a_deeper;
    (void)a_address;
    quit_default.fatal_error("libgcrypt not available");
    return pointer();
#endif
}


srecord::input::pointer
srecord::input_filter_message_gcrypt::create_tiger(
    const input::pointer &a_deeper, unsigned long a_address)
{
#ifdef HAVE_LIBGCRYPT
    return create(a_deeper, a_address, GCRY_MD_TIGER);
#else
    (void)a_deeper;
    (void)a_address;
    quit_default.fatal_error("libgcrypt not available");
    return pointer();
#endif
}


srecord::input::pointer
srecord::input_filter_message_gcrypt::create_haval(
    const input::pointer &a_deeper, unsigned long a_address)
{
#ifdef HAVE_LIBGCRYPT
    return create(a_deeper, a_address, GCRY_MD_HAVAL);
#else
    (void)a_deeper;
    (void)a_address;
    quit_default.fatal_error("libgcrypt not available");
    return pointer();
#endif
}


srecord::input::pointer
srecord::input_filter_message_gcrypt::create_sha256(
    const input::pointer &a_deeper, unsigned long a_address)
{
#ifdef HAVE_LIBGCRYPT
    return create(a_deeper, a_address, GCRY_MD_SHA256);
#else
    (void)a_deeper;
    (void)a_address;
    quit_default.fatal_error("libgcrypt not available");
    return pointer();
#endif
}


srecord::input::pointer
srecord::input_filter_message_gcrypt::create_sha384(
    const input::pointer &a_deeper, unsigned long a_address)
{
#ifdef HAVE_LIBGCRYPT
    return create(a_deeper, a_address, GCRY_MD_SHA384);
#else
    (void)a_deeper;
    (void)a_address;
    quit_default.fatal_error("libgcrypt not available");
    return pointer();
#endif
}


srecord::input::pointer
srecord::input_filter_message_gcrypt::create_sha512(
    const input::pointer &a_deeper, unsigned long a_address)
{
#ifdef HAVE_LIBGCRYPT
    return create(a_deeper, a_address, GCRY_MD_SHA512);
#else
    (void)a_deeper;
    (void)a_address;
    quit_default.fatal_error("libgcrypt not available");
    return pointer();
#endif
}


srecord::input::pointer
srecord::input_filter_message_gcrypt::create_sha224(
    const input::pointer &a_deeper, unsigned long a_address)
{
#ifdef HAVE_LIBGCRYPT_SHA224
    return create(a_deeper, a_address, GCRY_MD_SHA224);
#else
    (void)a_deeper;
    (void)a_address;
    quit_default.fatal_error("libgcrypt SHA224 not available");
    return pointer();
#endif
}


srecord::input::pointer
srecord::input_filter_message_gcrypt::create_md4(
    const input::pointer &a_deeper, unsigned long a_address)
{
#ifdef HAVE_LIBGCRYPT
    return create(a_deeper, a_address, GCRY_MD_MD4);
#else
    (void)a_deeper;
    (void)a_address;
    quit_default.fatal_error("libgcrypt not available");
    return pointer();
#endif
}


srecord::input::pointer
srecord::input_filter_message_gcrypt::create_crc32(
    const srecord::input::pointer &a_deeper, unsigned long a_address)
{
#ifdef HAVE_LIBGCRYPT
    return create(a_deeper, a_address, GCRY_MD_CRC32);
#else
    (void)a_deeper;
    (void)a_address;
    quit_default.fatal_error("libgcrypt not available");
    return pointer();
#endif
}


srecord::input::pointer
srecord::input_filter_message_gcrypt::create_crc32_rfc1510(
    const input::pointer &a_deeper, unsigned long a_address)
{
#ifdef HAVE_LIBGCRYPT
    return create(a_deeper, a_address, GCRY_MD_CRC32_RFC1510);
#else
    (void)a_deeper;
    (void)a_address;
    quit_default.fatal_error("libgcrypt not available");
    return pointer();
#endif
}


srecord::input::pointer
srecord::input_filter_message_gcrypt::create_crc24_rfc2440(
    const input::pointer &a_deeper, unsigned long a_address)
{
#ifdef HAVE_LIBGCRYPT
    return create(a_deeper, a_address, GCRY_MD_CRC24_RFC2440);
#else
    (void)a_deeper;
    (void)a_address;
    quit_default.fatal_error("libgcrypt not available");
    return pointer();
#endif
}


srecord::input::pointer
srecord::input_filter_message_gcrypt::create_whirlpool(
    const input::pointer &a_deeper, unsigned long a_address)
{
#ifdef HAVE_LIBGCRYPT_WHIRLPOOL
    return create(a_deeper, a_address, GCRY_MD_WHIRLPOOL);
#else
    (void)a_deeper;
    (void)a_address;
    quit_default.fatal_error("libgcrypt WHIRLPOOL not available");
    return pointer();
#endif
}


int
srecord::input_filter_message_gcrypt::algorithm_from_name(const char *name)
{
#ifdef HAVE_LIBGCRYPT
    static int table[] =
    {
        GCRY_MD_MD5,
        GCRY_MD_SHA1,
        GCRY_MD_RMD160,
        GCRY_MD_MD2,
        GCRY_MD_TIGER,
        GCRY_MD_HAVAL,
        GCRY_MD_SHA256,
        GCRY_MD_SHA384,
        GCRY_MD_SHA512,
#ifdef HAVE_LIBGCRYPT_SHA224
        GCRY_MD_SHA224,
#endif
        GCRY_MD_MD4,
        GCRY_MD_CRC32,
        GCRY_MD_CRC32_RFC1510,
        GCRY_MD_CRC24_RFC2440,
#ifdef HAVE_LIBGCRYPT_WHIRLPOOL
        GCRY_MD_WHIRLPOOL,
#endif
    };

    for (const int *tp = table; tp < ENDOF(table); ++tp)
    {
        int algo = *tp;
        if (0 == strcasecmp(name, gcry_md_algo_name(algo)))
            return algo;
    }
#endif
    quit_default.fatal_error("gcrypt algorithm \"%s\" unknown", name);
    return -1;
}


void
srecord::input_filter_message_gcrypt::process(const memory &input,
    record &output)
{
#ifdef HAVE_LIBGCRYPT
    // allocate the message digest handle
    unsigned int flags = 0;
    if (hmac)
        flags |= GCRY_MD_FLAG_HMAC;
    gcry_md_hd_t handle = 0;
    gcry_error_t err = gcry_md_open(&handle, algo, flags);
    if (err)
        fatal_error("gcry_md_open: %s", gcry_strerror(err));

    // traverse the memory
    memory_walker::pointer w = memory_walker_gcrypt::create(handle);
    input.walk(w);

    // generate the result
    const unsigned char *data = gcry_md_read(handle, algo);
    size_t data_size = gcry_md_get_algo_dlen(algo);
    output = record(record::type_data, address, data, data_size);

    // free the message digest handle
    gcry_md_close(handle);
#else
    (void)input;
    fatal_error("not compiled with libgcrypt support");
    output =
        record
        (
            record::type_data,
            address,
            (const unsigned char *)"oops",
            4
        );
#endif
}


const char *
srecord::input_filter_message_gcrypt::get_algorithm_name()
    const
{
#ifdef HAVE_LIBGCRYPT
    return gcry_md_algo_name(algo);
#else
    return "libgcrypt not available";
#endif
}


// vim: set ts=8 sw=4 et :
