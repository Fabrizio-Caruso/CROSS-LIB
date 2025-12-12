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
 * Manages the list of files & directories
 */
#include "flist.h"
#include "msg.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

static char *read_file(const char *fname, size_t size)
{
    char *data;
    FILE *f = fopen(fname, "rb");
    if( !f )
        show_error("can't open file '%s': %s", fname, strerror(errno));
    data = check_malloc(size);
    if( size != fread(data, 1, size, f) )
        show_error("error reading file '%s': %s", fname, strerror(errno));
    fclose(f);
    return data;
}

// Checks if given character is a PATH separator
static int is_separator(char c)
{
#ifdef _WIN32
    return c == '/' || c == '\\' || c == ':';
#else
    return c == '/';
#endif
}

static char *atari_name(const char *fname)
{
    // Convert to 8+3 filename
    char *out = strdup("           ");
    if( !out )
        memory_error();

    // Search last part of filename (similar to "basename")
    const char *in, *p;
    in = p = fname;
    while( p[0] && p[1] )
    {
        while( is_separator(p[0]) && p[1] )
            p++;
        if( p[0] && !is_separator(p[0]) )
            in = p;
        while( p[0] && !is_separator(p[0]) )
            p++;
    }

    // Copy up to 8 characters
    int pos = 0, dot = 0;
    while( *in )
    {
        char c = *in++;
        if( !c )
            break;
        else if( c == '.' && !dot )
        {
            pos = 8;
            dot = 1;
            continue;
        }
        else if( pos > 7 && !dot )
            continue;
        else if( pos > 10 )
            break;
        else if( is_separator(c) )
            break;
        else if( c >= 'a' && c <= 'z' )
            out[pos] = c - 'a' + 'A';
        else if( (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') )
            out[pos] = c;
        else
            out[pos] = '_';
        pos++;
    }
    return out;
}

static char *path_name(const char *dir, const char *name)
{
    size_t n  = strlen(dir);
    char *ret = check_malloc(n + 14);
    strcpy(ret, dir);
    ret[n++] = '>';
    int i;
    for( i = 0; i < 11; i++ )
    {
        if( name[i] != ' ' )
        {
            if( i == 8 )
                ret[n++] = '.';
            ret[n++] = name[i];
        }
    }
    ret[n] = 0;
    return ret;
}

void flist_add_main_dir(file_list *flist)
{
    // Creates MAIN directory
    struct afile *dir = check_malloc(sizeof(struct afile));
    // Convert time to broken time
    time_t ttim    = time(0);
    struct tm *tim = localtime(&ttim);

    dir->date[0]   = tim->tm_mday;
    dir->date[1]   = tim->tm_mon + 1;
    dir->date[2]   = tim->tm_year % 100;
    dir->time[0]   = tim->tm_hour;
    dir->time[1]   = tim->tm_min;
    dir->time[2]   = tim->tm_sec;
    dir->fname     = "";
    dir->aname     = "MAIN       ";
    dir->pname     = "";
    dir->dir       = 0;
    dir->size      = 23;
    dir->is_dir    = 1;
    dir->boot_file = 0;
    dir->data      = check_malloc(SFS_MAX_DIR_SIZE);
    dir->level     = 0;

    darray_add(flist, dir);
}

void flist_add_file(file_list *flist, const char *fname, int boot_file,
                    enum fattr attribs)
{
    struct stat st;

    if( 0 != stat(fname, &st) )
        show_error("reading input file '%s': %s", fname, strerror(errno));

    if( S_ISREG(st.st_mode) || S_ISDIR(st.st_mode) )
    {
        struct afile *f = check_malloc(sizeof(struct afile));

        // Search in the file list if the path is inside an added directory
        struct afile *dir = 0, **ptr;
        darray_foreach(ptr, flist)
        {
            struct afile *af = *ptr;
            if( af->is_dir && fname == strstr(fname, af->fname) &&
                (!dir || strlen(dir->fname) < strlen(af->fname)) )
                dir = af;
        }

        if( !dir )
            show_error("internal error - no main directory");

        // Convert time to broken time
        struct tm *tim = localtime(&st.st_mtime);

        f->date[0] = tim->tm_mday;
        f->date[1] = tim->tm_mon + 1;
        f->date[2] = tim->tm_year % 100;
        f->time[0] = tim->tm_hour;
        f->time[1] = tim->tm_min;
        f->time[2] = tim->tm_sec;
        f->fname   = fname;
        f->aname   = atari_name(fname);
        f->pname   = path_name(dir->pname, f->aname);
        f->dir     = dir;
        f->level   = dir->level + 1;
        f->attribs = attribs;

        if( !f->aname || !strcmp(f->aname, "           ") )
            show_error("can't add file/directory named '%s'", fname);

        // Search for repeated files
        darray_foreach(ptr, flist)
        {
            struct afile *af = *ptr;
            if( af->dir == f->dir && !strncmp(af->aname, f->aname, 11) )
                show_error("repeated file/directory named '%s'", f->pname);
        }

        if( S_ISDIR(st.st_mode) )
        {
            f->size      = 23;
            f->is_dir    = 1;
            f->boot_file = 0;
            f->data      = check_malloc(SFS_MAX_DIR_SIZE);

            show_msg("added dir  '%-20s', from '%s'.", f->pname, f->fname);
        }
        else
        {
            if( st.st_size > 0x1000000 )
                show_error("file size too big '%s'", fname);

            f->size      = st.st_size;
            f->is_dir    = 0;
            f->boot_file = boot_file;
            f->data      = read_file(f->fname, f->size);

            show_msg("added file '%-20s', %5ld bytes, from '%s'%s%s%s%s.", f->pname,
                     (long)f->size, f->fname, attribs & at_protected ? ", +p" : "",
                     attribs & at_hidden ? ", +h" : "",
                     attribs & at_archived ? ", +a" : "", boot_file ? ", (boot)" : "");
        }
        darray_add(flist, f);
    }
    else
        show_error("invalid file type '%s'", fname);
}
