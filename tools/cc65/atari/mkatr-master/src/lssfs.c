/*
 *  Copyright (C) 2023 Daniel Serpell
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
 * Reads a SpartaDOS file-system.
 */
#define _GNU_SOURCE
#include "lssfs.h"
#include "atr.h"
#include "compat.h"
#include "msg.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <utime.h>

//---------------------------------------------------------------------
// Global state
struct lssfs
{
    struct atr_image *atr;
    int atari_list;
    int lower_case;
    int extract_files;
};

//---------------------------------------------------------------------
static uint16_t read16(const uint8_t *p)
{
    return p[0] | (p[1] << 8);
}

static unsigned read24(const uint8_t *p)
{
    return p[0] | (p[1] << 8) | (p[2] << 16);
}

// Get maximum size of file at given map sector
static unsigned file_msize(struct atr_image *atr, unsigned map)
{
    if( map < 2 || map > atr->sec_count )
    {
        show_msg("invalid sector map");
        return 0;
    }
    unsigned size = 0;
    const uint8_t *m;
    while( 0 != (m = atr_data(atr, map)) )
    {
        // Iterate all sectors of map
        for( unsigned s = 4; s < atr->sec_size; s += 2 )
            if( read16(m + s) )
                size += atr->sec_size;
        // next map
        map = read16(m);
    }
    return size;
}

// Read up to size bytes from file at given map sector
static unsigned read_file(struct atr_image *atr, unsigned map, unsigned size,
                          uint8_t *data)
{
    const uint8_t *m = atr_data(atr, map);
    unsigned s       = 4;
    unsigned pos     = 0;
    if( map < 2 || !m )
    {
        show_msg("invalid sector map");
        return 0;
    }

    while( size )
    {
        if( s >= atr->sec_size )
        {
            // next map
            map = read16(m);
            if( !map )
                return pos;
            m = atr_data(atr, map);
            s = 4;
            if( map < 2 || !m )
            {
                show_msg("invalid next sector map");
                return pos;
            }
        }
        unsigned rem = size > atr->sec_size ? atr->sec_size : size;
        unsigned sec = read16(m + s);
        s += 2;
        if( !sec )
            memset(data + pos, 0, rem);
        else if( sec < 2 || sec > atr->sec_count )
        {
            show_msg("invalid data sector");
            return pos;
        }
        else
            memcpy(data + pos, atr_data(atr, sec), rem);
        pos += rem;
        size -= rem;
    }
    return pos;
}

// Read data and write a UNIX filename and an "Atari" filename.
static unsigned get_name(char *name, char *aname, const uint8_t *data, int max,
                         int lower_case)
{
    unsigned l = 0;
    int dot    = 0;
    memset(aname, ' ', max + 1);
    aname[max + 1] = 0;
    for( int i = 0; i < max; i++ )
    {
        uint8_t c = data[i];
        if( c >= 'A' && c <= 'Z' && lower_case )
            c = c - 'A' + 'a';
        if( c < ' ' || c == '/' || c == '.' || c == '?' || c == '\\' || c == 96 ||
            c > 'z' )
            c = '_';
        else if( c == ' ' )
            continue;
        if( i > 7 && !dot )
        {
            dot       = 1;
            name[l++] = '.';
        }
        name[l++]      = c;
        aname[i + dot] = c;
    }
    name[l] = 0;
    return l;
}

static void set_times(const char *path, int d_day, int d_mon, int d_yea, int t_hh,
                      int t_mm, int t_ss)
{
    struct utimbuf tb;
    struct tm t;
    memset(&t, 0, sizeof(t));
    t.tm_sec   = t_ss;
    t.tm_min   = t_mm;
    t.tm_hour  = t_hh;
    t.tm_mday  = d_day;
    t.tm_mon   = d_mon;
    t.tm_year  = d_yea > 83 ? d_yea : d_yea + 100;
    t.tm_isdst = -1;
    tb.actime = tb.modtime = mktime(&t);
    utime(path, &tb);
}

static void read_dir(struct lssfs *ls, unsigned map, const char *name)
{
    if( ls->atari_list )
        printf("Directory of %s\n\n", *name ? name : "/");

    uint8_t *data = check_malloc(65536); // max directory size (2848 entries)
    unsigned len  = read_file(ls->atr, map, 65536, data);
    if( !len )
    {
        show_msg("%s: can´t get directory data", name);
        free(data);
        return;
    }
    else if( len == 65536 )
        show_msg("%s: directory too big", name);

    // traverse dir
    for( unsigned i = 23; i < len; i += 23 )
    {
        unsigned flags = data[i];
        if( !flags )
            break; // no more entries
        if( 0 == (flags & 0x08) )
            continue; // unused
        if( 0x10 == (flags & 0x10) )
            continue; // erased
        int is_dir     = flags & 0x20;
        unsigned fmap  = read16(data + i + 1);
        unsigned fsize = read24(data + i + 3);
        int fd_day     = data[i + 17];
        int fd_mon     = data[i + 18];
        int fd_yea     = data[i + 19];
        int ft_hh      = data[i + 20];
        int ft_mm      = data[i + 21];
        int ft_ss      = data[i + 22];
        char fname[32], aname[32];
        if( !get_name(fname, aname, data + i + 6, 11, ls->lower_case) || !*fname )
        {
            show_msg("%s: invalid file name, skip", name);
            continue;
        }
        char *new_name;
        asprintf(&new_name, "%s/%s", name, fname);
        if( is_dir )
        {
            if( ls->extract_files )
            {
                struct stat st;
                const char *path = new_name + 1;
                fprintf(stderr, "%s/\n", path);
                // Check if directory already exists:
                if( stat(path, &st) || !S_ISDIR(st.st_mode) )
                {
                    // Create new directory
                    if( compat_mkdir(path) )
                        show_error("%s: can´t create directory, %s", path,
                                   strerror(errno));
                }
                // Extract files inside
                read_dir(ls, fmap, new_name);
                // Set time/date
                set_times(path, fd_day, fd_mon, fd_yea, ft_hh, ft_mm, ft_ss);
            }
            else if( ls->atari_list )
            {
                // Print entry, but don´t recurse
                printf("%-12s  <DIR>  %02d-%02d-%02d %02d:%02d\n", aname, fd_day, fd_mon,
                       fd_yea, ft_hh, ft_mm);
            }
            else
            {
                unsigned dirsz = file_msize(ls->atr, fmap);
                printf("%8u\t%02d-%02d-%02d %02d:%02d:%02d\t%s/\n", dirsz, fd_day, fd_mon,
                       fd_yea, ft_hh, ft_mm, ft_ss, new_name);
                read_dir(ls, fmap, new_name);
            }
        }
        else
        {
            uint8_t *fdata = check_malloc(fsize);
            unsigned r     = read_file(ls->atr, fmap, fsize, fdata);
            if( r != fsize )
                show_msg("%s: short file in disk", new_name);
            if( ls->extract_files )
            {
                struct stat st;
                const char *path = new_name + 1;
                fprintf(stderr, "%s\n", path);
                // Check if file already exists:
                if( 0 == stat(path, &st) )
                    show_error("%s: file already exists.", path);
                // Create new file
                int fd = creat(path, 0666);
                if( fd == -1 )
                    show_error("%s: can´t create file, %s", path, strerror(errno));
                if( fsize != write(fd, fdata, fsize) )
                    show_error("%s: can´t write file, %s", path, strerror(errno));
                if( close(fd) )
                    show_error("%s: can´t write file, %s", path, strerror(errno));
                // Set time/date
                set_times(path, fd_day, fd_mon, fd_yea, ft_hh, ft_mm, ft_ss);
            }
            else if( ls->atari_list )
                printf("%-12s %7u %02d-%02d-%02d %02d:%02d\n", aname, fsize, fd_day,
                       fd_mon, fd_yea, ft_hh, ft_mm);
            else
                printf("%8u\t%02d-%02d-%02d %02d:%02d:%02d\t%s\n", fsize, fd_day, fd_mon,
                       fd_yea, ft_hh, ft_mm, ft_ss, new_name);
            free(fdata);
        }
        free(new_name);
    }
    // traverse dir again if listing in Atari format, to show sub directories
    if( ls->atari_list )
    {
        printf("\n");
        for( unsigned i = 23; i < len; i += 23 )
        {
            unsigned flags = data[i];
            if( !flags )
                break; // no more entries
            if( 0 == (flags & 0x08) )
                continue; // unused
            if( 0x10 == (flags & 0x10) )
                continue; // erased
            if( 0 == (flags & 0x20) )
                continue; // not directory
            unsigned fmap = read16(data + i + 1);
            char fname[32], aname[32];
            if( !get_name(fname, aname, data + i + 6, 11, ls->lower_case) )
                continue;
            char *new_name;
            asprintf(&new_name, "%s/%s", name, fname);
            read_dir(ls, fmap, new_name);
            free(new_name);
        }
    }
    free(data);
}

int sfs_read(struct atr_image *atr, const char *atr_name, int atari_list, int lower_case,
             int extract_files)
{
    // Check SFS filesystem
    // Read superblock
    const uint8_t *boot  = atr_data(atr, 1);
    unsigned signature   = boot[7];
    unsigned rootdir_map = read16(boot + 9);
    unsigned num_sect    = read16(boot + 11);
    unsigned bitmap_sect = read16(boot + 16);
    unsigned sector_size = boot[31] ? boot[31] : 256;
    char vol_name[32], aname[32];
    if( !get_name(vol_name, aname, boot + 22, 8, lower_case) )
        vol_name[0] = 0;

    if( signature != 0x80 )
        return 1;
    if( sector_size != atr->sec_size )
    {
        show_msg("%s: invalid SpartaDOS file system, mismatch sector sizes (%d!=%d).",
                 atr_name, sector_size, atr->sec_size);
        return 1;
    }
    if( num_sect < atr->sec_count )
        show_msg("%s: ATR image is bigger than file system.", atr_name);
    if( num_sect > atr->sec_count )
        show_msg("%s: WARNING: ATR image is smaller than file system.", atr_name);
    if( rootdir_map < 2 || rootdir_map > atr->sec_count )
    {
        show_msg("%s: invalid SpartaDOS file system, root dir map outside disk.",
                 atr_name);
        return 1;
    }
    if( bitmap_sect < 2 || bitmap_sect > atr->sec_count )
    {
        show_msg("%s: invalid SpartaDOS file system, bitmap outside disk.", atr_name);
        return 1;
    }

    if( atr->sec_count < 6 )
    {
        show_msg("%s: ATR image with too few sectors.", atr_name);
        return 1;
    }

    if( atari_list )
        printf("ATR image: %s\n"
               "Image size: %u sectors of %u bytes\n"
               "Volume Name: %s\n",
               atr_name, atr->sec_count, atr->sec_size, *vol_name ? vol_name : "NONE");
    else
        printf("%s: %u sectors of %u bytes, volume name '%s'.\n", atr_name,
               atr->sec_count, atr->sec_size, vol_name);

    struct lssfs *ls  = check_malloc(sizeof(struct lssfs));
    ls->atr           = atr;
    ls->atari_list    = atari_list;
    ls->lower_case    = lower_case;
    ls->extract_files = extract_files;
    read_dir(ls, rootdir_map, "");

    free(ls);
    return 0;
}
