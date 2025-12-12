/*
 *  Copyright (C) 2016 Daniel Serpell
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "darray.h"
#include "msg.h"
#include <stdio.h>

void darray_fill_ptr(void *arr, size_t sz, size_t init)
{
    darray(char) *ret = arr;
    if( !ret )
        memory_error();
    ret->data = check_malloc(sz * init);
    ret->len  = 0;
    ret->size = init;
}

void *darray_alloc(size_t sz, size_t init)
{
    darray(char) *ret = check_malloc(sizeof(darray(char)));
    darray_fill_ptr(ret, sz, init);
    return ret;
}

void darray_grow(void *arr, size_t sz, size_t newsize)
{
    darray(char) *p = arr;
    while( newsize > p->size )
    {
        p->size *= 2;
        if( !p->size || !(p->data = check_realloc(p->data, sz * p->size)) )
            memory_error();
    }
}

void darray_free(void *arr)
{
    darray(char) *p = arr;
    free(p->data);
    free(p);
}

void darray_delete_ptr(void *arr)
{
    darray(char) *p = arr;
    free(p->data);
}
