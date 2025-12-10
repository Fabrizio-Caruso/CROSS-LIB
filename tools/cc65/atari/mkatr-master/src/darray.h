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
#pragma once

#include <stdlib.h>
// This code is inspired by "darray.h" by Joseph Adams <joeyadams3.14159@gmail.com>

// Define a dynamic array with struct name, of the given element type.
#define darray_struct(type, name)                                                        \
    struct name                                                                          \
    {                                                                                    \
        type *data;                                                                      \
        size_t len;                                                                      \
        size_t size;                                                                     \
    }

// Define a dynamic array as unnamed struct, with the given element type.
#define darray(type) darray_struct(type, )

// Internal interface - don't call those directly.
void darray_fill_ptr(void *, size_t, size_t);
void *darray_alloc(size_t, size_t);
void darray_grow(void *, size_t, size_t);
void darray_delete_ptr(void *);

// Allocate a new dynamic array of the given type with the given initial size
#define darray_new(type, init_size) darray_alloc(sizeof(type), init_size)
// Frees a dynamic array allocated with darray_new()
void darray_free(void *);

// Initialize an already allocated dynamic array with the given initial size
#define darray_init(arr, init_size)                                                      \
    darray_fill_ptr(&(arr), sizeof((arr).data[0]), init_size)
// Deallocates memory for a dynamic array initialized with darray_init()
#define darray_delete(arr) darray_delete_ptr(&(arr))

// Adds one element "val" to the dynamic array "arr".
#define darray_add(arr, val)                                                             \
    do                                                                                   \
    {                                                                                    \
        darray_grow((arr), sizeof((arr)->data[0]), (arr)->len + 1);                      \
        (arr)->data[(arr)->len] = val;                                                   \
        (arr)->len++;                                                                    \
    } while( 0 )

// Returns the current length (number of elements) of the dynamic array.
#define darray_len(arr) ((arr)->len)

// Access the i-eth element of the dynamic array.
#define darray_i(arr, i) ((arr)->data[i])

// Traverses all elements in the array
#define darray_foreach(itm, arr)                                                         \
    for( (itm) = &darray_i(arr, 0); (itm) < &darray_i(arr, darray_len(arr)); (itm)++ )
