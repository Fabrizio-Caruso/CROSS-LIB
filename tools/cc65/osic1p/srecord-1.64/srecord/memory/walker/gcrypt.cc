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

#include <srecord/memory/walker/gcrypt.h>


srecord::memory_walker_gcrypt::~memory_walker_gcrypt()
{
}


srecord::memory_walker_gcrypt::memory_walker_gcrypt(gcry_md_hd_t a_handle) :
    handle(a_handle)
{
}


srecord::memory_walker::pointer
srecord::memory_walker_gcrypt::create(gcry_md_hd_t a_handle)
{
    return pointer(new srecord::memory_walker_gcrypt(a_handle));
}


void
srecord::memory_walker_gcrypt::observe(unsigned long, const void *data,
    int length)
{
#ifdef HAVE_LIBGCRYPT
    gcry_md_write(handle, data, length);
#else
    (void)data;
    (void)length;
#endif
}


// vim: set ts=8 sw=4 et :
