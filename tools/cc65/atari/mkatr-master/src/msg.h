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
#pragma once
#include <stddef.h>

extern const char *prog_name;

void show_error(const char *format, ...) __attribute__((noreturn, format(printf, 1, 2)));
void show_opt_error(const char *format, ...)
    __attribute__((noreturn, format(printf, 1, 2)));
void show_msg(const char *format, ...) __attribute__((format(printf, 1, 2)));
void show_version(void);
void memory_error(void) __attribute__((noreturn));
void *check_malloc(size_t size);
void *check_calloc(size_t nmemb, size_t size);
void *check_realloc(void *ptr, size_t size);
