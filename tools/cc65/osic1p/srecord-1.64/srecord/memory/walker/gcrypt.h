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

#ifndef SRECORD_MEMORY_WALKER_GCRYPT_H
#define SRECORD_MEMORY_WALKER_GCRYPT_H

#include <srecord/config.h>
#ifdef HAVE_LIBGCRYPT
#include <gcrypt.h>
#else
typedef void *gcry_md_hd_t;
#endif

#include <srecord/memory/walker.h>

namespace srecord
{

/**
  * The srecord::memory_walker_gcrypt class is used to represent walking
  * memory and passing it to the gcrypt library in order to calculate a
  * message digest.
  */
class memory_walker_gcrypt:
    public memory_walker
{
public:
    /**
      * The destructor.
      */
    virtual ~memory_walker_gcrypt();

private:
    /**
      * The constructor.
      * It is private on purpose, use the #create class method instead.
      *
      * @param handle
      *     used to access the libgcrypt handle to the message digest
      *     being calculated.
      */
    memory_walker_gcrypt(gcry_md_hd_t handle);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param handle
      *     used to access the libgcrypt handle to the message digest
      *     being calculated.
      */
    static pointer create(gcry_md_hd_t handle);

protected:
    // See base class for documentation.
    void observe(unsigned long, const void *, int);

private:
    /**
      * The handle instance variable is used to access the libgcrypt
      * handle to the message digest being calculated.
      */
    gcry_md_hd_t handle;

    /**
      * The default constructor.  Do not use.
      */
    memory_walker_gcrypt();

    /**
      * The copy constructor.  Do not use.
      */
    memory_walker_gcrypt(const memory_walker_gcrypt &);

    /**
      * The assignment operator.  Do not use.
      */
    memory_walker_gcrypt &operator=(const memory_walker_gcrypt &);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_MEMORY_WALKER_GCRYPT_H
