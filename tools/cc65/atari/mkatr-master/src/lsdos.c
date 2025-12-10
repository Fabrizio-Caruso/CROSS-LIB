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
 * Reads a DOS 2 or DOS 2.5 file-system.
 */
#define _GNU_SOURCE
#include "lsdos.h"
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
struct lsdos
{
    struct atr_image *atr;
    int atari_list;
    int lower_case;
    int extract_files;
    int dir_size;
    int ldos_csize;
    int fix_bibo;
};

//---------------------------------------------------------------------
static uint16_t read16(const uint8_t *p)
{
    return p[0] | (p[1] << 8);
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
        if( (c >= 0xB0 && c <= 0xB9) || (c >= 0xC1 && c <= 0xDA) ||
            (c >= 0xE1 && c <= 0xFA) )
            c = c & 0x7F;
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

// Read up to size bytes from file at given map sector
static unsigned read_file(struct atr_image *atr, unsigned sect, unsigned size,
                          uint8_t *data, int dos2, int mdos)
{
    // To avoid circular references, keep a bitmap with all the sectors already used
    uint8_t *visited = check_calloc(65536, 1);
    unsigned lst     = atr->sec_size - 3;
    unsigned pos     = 0;
    while( sect )
    {
        const uint8_t *m = atr_data(atr, sect);
        if( sect < 2 || !m )
        {
            show_msg("invalid sector link");
            break;
        }
        unsigned len  = m[lst + 2];
        unsigned link = (m[lst] << 8) | m[lst + 1];

        // DOS 1.0, only last sector has a size field
        if( !dos2 && !mdos )
        {
            len  = (len & 0x80) ? len & 0x7F : lst;
            link = (len & 0x80) ? 0 : link;
        }
        // Only MyDOS stores full sector number
        if( !mdos || atr->sec_count < 1023 )
            link = link & 0x3FF;

        if( visited[sect & 0xFFFF] )
        {
            show_msg("loop in sector link at sector %d", sect);
            break;
        }
        else
            visited[sect & 0xFFFF] = 1;

        if( size > pos )
        {
            unsigned rem = size - pos > len ? len : size - pos;
            memcpy(data + pos, m, rem);
        }
        pos += len;
        sect = link;
    }
    free(visited);
    return pos;
}

static const uint8_t *dir_data(struct lsdos *ls, unsigned dir, unsigned fn)
{
    if( ls->ldos_csize )
    {
        int cluster = dir / ls->ldos_csize;
        int pos     = (dir + fn / 8) % ls->ldos_csize;
        int sector  = cluster * ls->ldos_csize + pos;
        return atr_data(ls->atr, sector);
    }
    else if( ls->fix_bibo )
    {
        const uint8_t *data = atr_data(ls->atr, dir + fn / 16);
        return fn & 8 ? data + 0x80 : data;
    }
    else
        return atr_data(ls->atr, dir + fn / 8);
}

static void read_dir(struct lsdos *ls, unsigned dir, const char *name)
{
    unsigned ssize = ls->atr->sec_size;

    if( ls->atari_list )
        printf("Directory of %s\n\n", *name ? name : "/");

    for( int fn = 0; fn < ls->dir_size; fn++ )
    {
        const uint8_t *data = dir_data(ls, dir, fn);
        if( !data )
            break;
        const uint8_t *entry = data + (fn & 7) * 16;
        int flags            = entry[0];
        unsigned size        = read16(entry + 1);
        unsigned sect        = read16(entry + 3);
        if( !flags ) // End of directory
            break;
        if( flags & 0x80 ) // Deleted
            continue;
        char fname[32], aname[32];
        if( !get_name(fname, aname, entry + 5, 11, ls->lower_case) || !*fname )
        {
            show_msg("%s: invalid file name, skip", name);
            continue;
        }
        if( sect < 2 )
        {
            show_msg("%s: invalid file sector, skip", name);
            continue;
        }
        char *new_name;
        asprintf(&new_name, "%s/%s", name, fname);

        if( flags == 0x10 )
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
                read_dir(ls, sect, new_name);
            }
            else if( ls->atari_list )
            {
                // Print entry, but don´t recurse
                printf("%-12s  <DIR>\n", aname);
            }
            else
            {
                printf("%8u\t\t%s/\n", size * ssize, new_name);
                read_dir(ls, sect, new_name);
            }
        }
        else if( 0 != (flags & 0x41) )
        {
            int mdos       = flags & 0x04;
            int dos2       = flags & 0x02;
            int max_size   = size * ssize;
            uint8_t *fdata = check_malloc(max_size);
            unsigned fsize = 0;
            // Skip files of size 0
            if( max_size > 0 )
            {
                fsize = read_file(ls->atr, sect, max_size, fdata, dos2, mdos);
                if( fsize > max_size )
                    show_msg("%s: file too long in disk", new_name);
            }
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
            }
            else if( ls->atari_list )
                printf("%-12s %7u\n", aname, fsize);
            else
                printf("%8u\t\t%s\n", fsize, new_name);
            free(fdata);
        }
        else
        {
            show_msg("%s: invalid file type %02x", new_name, flags);
        }
        free(new_name);
    }
    // traverse dir again if listing in Atari format, to show sub directories
    if( ls->atari_list )
    {
        printf("\n");
        for( int fn = 0; fn < ls->dir_size; fn++ )
        {
            const uint8_t *data = dir_data(ls, dir, fn);
            if( !data )
                break;
            const uint8_t *entry = data + (fn & 7) * 16;
            int flags            = entry[0];
            unsigned sect        = read16(entry + 3);
            if( !flags )
                break;          // End of directory
            if( flags != 0x10 ) // Not directory
                continue;

            char fname[32], aname[32];
            if( !get_name(fname, aname, entry + 5, 11, ls->lower_case) || !*fname )
                continue;
            char *new_name;
            asprintf(&new_name, "%s/%s", name, fname);
            read_dir(ls, sect, new_name);
            free(new_name);
        }
    }
}

// Detect Bibo-DOS directory format, it uses the full sector in DD for the
// directory data, instead of the first 128 bytes of MyDOS and DOS 2.0D.
// Note that Bibo-DOS 7.0 fixes this, so it will be detected ad MyDOS.
int detect_bibo(struct atr_image *atr, int dir_sect)
{
    const uint8_t *data = atr_data(atr, dir_sect);
    if( !data )
        return 0;

    // Check if this directory has any entries above 128 bytes
    for( int i = 0; i < 16; i++ )
    {
        int flags = data[i * 16];
        // End of directory?
        if( !flags )
            break;
        // Invalid entry?
        if( 0 == (flags & 0xC1) )
            continue;
        if( flags == 0xFF )
            continue;
        int siz = read16(data + i * 16 + 1);
        int sec = read16(data + i * 16 + 3);
        if( sec < 1 || sec > atr->sec_count || siz < 1 || siz + 12 > atr->sec_count )
            continue;
        // Valid and more than 7, it is Bibo-DOS
        if( i > 7 )
            return 1;
    }

    // Bibo-DOS does not support sub-directories, so we only need to examine
    // the main directory.
    return 0;
}

int dos_read(struct atr_image *atr, const char *atr_name, int atari_list, int lower_case,
             int extract_files)
{
    // Check DOS filesystem
    // Read VTOC
    const uint8_t *vtoc = atr_data(atr, 360);
    if( !vtoc )
        return 1;

    unsigned signature  = vtoc[0];
    unsigned alloc_sect = read16(vtoc + 1);
    unsigned free_sect  = read16(vtoc + 3);
    unsigned bitmap_0   = vtoc[10]; // Bitmap for sectors 0 to 7
    unsigned bitmap_360 = vtoc[55]; // Bitmap for sectors 360 to 367
    unsigned dir_size   = 64;       // Entries per directory
    unsigned ldos_csize = 0;        // LiteDOS cluster size
    unsigned fix_bibo   = 0;        // Fix Bibo-DOS DD directories.
    char *bad_sig       = "";       // Message for corrected bad signature

    // Calculate signature for MyDOS image format:
    unsigned mydos_sig = 2;
    if( (atr->sec_size == 128 && atr->sec_count > 943) ||
        (atr->sec_size == 256 && atr->sec_count > 1023) )
        mydos_sig = 2 + (atr->sec_count / 8 + 266) / 256;

    // Check for LiteDOS
    if( signature & 0x80 )
    {
        // Check rest of signature
        if( memcmp(vtoc + 3, "LiteDOS", 7) )
            return 1;
        free_sect  = read16(vtoc + 0x71);
        ldos_csize = 1 + (signature & 0x7F);
        dir_size   = 8 * ldos_csize - 8;
    }
    else if( 0x40 == (signature & 0xC0) )
    {
        // Check rest of signature
        if( memcmp(vtoc + 5, "\0\0\0\0\0", 5) )
            return 1;
        if( bitmap_0 & 0x80 )
            return 1;
        ldos_csize = 2 + 2 * (signature & 0x3F);
        dir_size   = 8 * ldos_csize - 8;
    }
    else
    {
        // Check for malformed signatures
        if( !signature )
        {
            if( 0 != (bitmap_0 & 0xC0) || 0 != (bitmap_360 & 0xC0) )
                return 1;
            if( memcmp(vtoc + 5, "\0\0\0\0\0", 5) )
                return 1;
            if( atr->sec_count > 720 || free_sect > alloc_sect ||
                (alloc_sect < 707 || alloc_sect > 709) )
                return 1;
            // Assume DOS 2
            signature = alloc_sect != 709 ? 2 : 1;
            bad_sig   = " (with bad signature)";
        }
        // Detect MyDOS images with bad signature, this happens in 720 DD images
        if( signature == 3 && mydos_sig == 2 &&
            ((alloc_sect == 707 && atr->sec_size == 128) ||
             (alloc_sect == 708 && atr->sec_size == 256)) )
        {
            // Detect as MyDOS signature to 3
            mydos_sig = 3;
            bad_sig   = " (with bad signature)";
        }
        // Patch bad VTOC
        if( !*bad_sig && signature == 2 && (alloc_sect == 707 || alloc_sect == 708) &&
            alloc_sect >= free_sect && 0 != (bitmap_0 & 0xF0) )
        {
            bitmap_0 &= 0x0F;
            bitmap_360 = 0x7F;
            bad_sig    = " (with bad VTOC)";
        }

        if( (signature == 1 && atr->sec_size != 128) ||  // DOS 1 only supports SD,
            (signature == 1 && atr->sec_count > 720) ||  // 720 sectors of 128 bytes.
            (signature > 2 && signature != mydos_sig) || // MyDOS signature for >944 secs
            (signature == 2 && atr->sec_count > 1120) || // DOS 2.x only up to 1040
                                                         // sectors, but there are images
                                                         // with 1120 sectors.
            0 != (bitmap_0 & 0x80) )                     // Sector 0 allocated
        {
            return 1;
        }
        // DOS 1 bitmap should reserve sector 1, DOS 2 reserves 1, 2 and 3
        if( 0 != (bitmap_0 & 0xC0) || (signature == 2 && 0 != (bitmap_0 & 0xF0)) ||
            0 != (bitmap_360 & 0x80) )
        {
            show_msg("%s: invalid DOS file system, bitmap not ok.", atr_name);
            return 1;
        }
    }

    if( alloc_sect > atr->sec_count )
        show_msg("%s: DOS sectors (%d) more than ATR image (%d).", atr_name, alloc_sect,
                 atr->sec_count);
    if( free_sect > alloc_sect )
        show_msg("%s: DOS free sectors more than allocated.", atr_name);

    const char *dosver = "DOS 1";
    if( signature == 2 && atr->sec_count > 943 )
        dosver = "DOS 2.5";
    else if( signature == 2 && atr->sec_size == 128 )
        dosver = "DOS 2.0s";
    else if( signature == 2 )
        dosver = "DOS 2.0D";
    else if( signature & 0x80 )
        dosver = "LiteDOS";
    else if( signature & 0x40 )
        dosver = "LiteDOS-SE";
    else if( signature > 2 )
        dosver = "MyDOS";

    if( atr->sec_size == 256 && !ldos_csize && detect_bibo(atr, 361) )
    {
        dosver   = "Bibo-DOS";
        fix_bibo = 1;
    }

    if( atari_list )
        printf("ATR image: %s\n"
               "Image size: %u sectors of %u bytes\n"
               "DOS size: %u sectors free of %u total\n"
               "Volume: %s%s\n",
               atr_name, atr->sec_count, atr->sec_size, free_sect, alloc_sect, dosver,
               bad_sig);
    else
        printf("%s: %u sectors of %u bytes, %s%s, %d sectors free of %d total.\n",
               atr_name, atr->sec_count, atr->sec_size, dosver, bad_sig, free_sect,
               alloc_sect);

    struct lsdos *ls  = check_malloc(sizeof(struct lsdos));
    ls->atr           = atr;
    ls->atari_list    = atari_list;
    ls->lower_case    = lower_case;
    ls->extract_files = extract_files;
    ls->dir_size      = dir_size;
    ls->ldos_csize    = ldos_csize;
    ls->fix_bibo      = fix_bibo;
    read_dir(ls, 361, "");

    free(ls);
    return 0;
}
