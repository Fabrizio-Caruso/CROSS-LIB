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
/*
 * Shows error messages.
 */
#include "msg.h"
#include "version.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

const char *prog_name;

void show_error(const char *format, ...)
{
    va_list ap;
    fprintf(stderr, "%s: Error, ", prog_name);
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}

void show_msg(const char *format, ...)
{
    va_list ap;
    fprintf(stderr, "%s: ", prog_name);
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    fprintf(stderr, "\n");
}

void show_opt_error(const char *format, ...)
{
    va_list ap;
    fprintf(stderr, "%s: Error, ", prog_name);
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    fprintf(stderr, ". Try '%s -h' for help.\n", prog_name);
    exit(EXIT_FAILURE);
}

void show_version(void)
{
    printf("mkatr version %s, Copyright (C) 2016-2023 Daniel Serpell\n\n"
           "This is free software under the GNU GENERAL PUBLIC LICENSE.\n"
           "There is NO warranty; not even for MERCHANTABILITY or FITNESS\n"
           "FOR A PARTICULAR PURPOSE.\n",
           prog_version);
    exit(EXIT_SUCCESS);
}

void memory_error(void)
{
    show_error("memory error!\n");
}

void *check_malloc(size_t size)
{
    void *ret = malloc(size);
    if( !ret )
        memory_error();
    return ret;
}

void *check_calloc(size_t nmemb, size_t size)
{
    void *ret = calloc(nmemb, size);
    if( !ret )
        memory_error();
    return ret;
}

void *check_realloc(void *ptr, size_t size)
{
    void *ret = realloc(ptr, size);
    if( !ret )
        memory_error();
    return ret;
}
