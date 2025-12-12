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
 * Extracts various simple boot formats.
 */
#define _GNU_SOURCE
#include "crc32.h"
#include "lsextra.h"
#include "msg.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static unsigned get_name(char *name, char *aname, const uint8_t *data, int max,
                         int lower_case)
{
    unsigned l = 0;
    unsigned a = 0;
    int dot    = 0;
    memset(aname, ' ', max + 1);
    aname[max + 1] = 0;
    for( int i = 0; i < max; i++ )
    {
        uint8_t c = data[i];
        if( c >= 'A' && c <= 'Z' && lower_case )
            c = c - 'A' + 'a';
        if( c == '.' )
        {
            // Store dot
            dot       = 1;
            name[l++] = c;
            while( a < 8 )
                aname[a++] = ' ';
            continue;
        }
        if( c < ' ' || c == '/' || c == '.' || c == '?' || c == '\\' || c == 96 ||
            c > 'z' )
            c = '_';
        else if( c == ' ' )
            continue;
        if( i > 7 && !dot )
        {
            dot        = 1;
            name[l++]  = '.';
            aname[a++] = ' ';
        }
        name[l++]  = c;
        aname[a++] = c;
    }
    name[l] = 0;
    return l;
}

static uint16_t read16(const uint8_t *p)
{
    return (p == NULL) ? 0 : (p[0] | (p[1] << 8));
}

static int check_bas2boot(struct atr_image *atr)
{
    // BAS2BOOT only supports 128 bytes per sector, and we must have
    // at least one data sector
    if( atr->sec_size != 128 || atr->sec_count < 2 )
        return 0;

    const uint8_t *sec1 = atr_data(atr, 1);
    const uint8_t *sec2 = atr_data(atr, 2);

    // Check boot count == 2 and load address == $700
    if( read16(sec1) != 0x200 || read16(sec1 + 2) != 0x700 )
        return 0;

    // Check ID
    if( memcmp(sec2 + 0x3C, "BAS2BOOT", 8) )
        return 0;

    // Ok, we have a BAS2BOOT file
    return 1;
}

static void extract_bas2boot(struct atr_image *atr, int atari_list, int lower_case,
                             int extract_files)
{
    // Get headers
    const uint8_t *sec1 = atr_data(atr, 1);
    const uint8_t *sec2 = atr_data(atr, 2);
    // Get filename
    char path[32], aname[32];
    if( !get_name(path, aname, sec2 + 0x60, 12, lower_case) || !*path )
    {
        strcpy(path, "noname.bas");
        strcpy(aname, "NONAME  BAS");
    }
    // Adds '.BAS' if no extension is present
    if( !strchr(path, '.') )
    {
        const char *ext = lower_case ? ".bas" : ".BAS";
        strcat(path, ext);
        memcpy(aname + 9, ext + 1, 3);
    }

    // Get data and length
    unsigned fsize = read16(sec1 + 8);
    uint8_t *fdata = check_malloc(fsize < 16 ? 16 : fsize);

    // Read header and undo bad conversion for certain files
    memcpy(fdata, sec2 + 0x72, 14);
    for( int i = 2; i < 14; i += 2 )
    {
        int x        = fdata[0] + fdata[1] * 256;
        int y        = fdata[i] + fdata[i + 1] * 256 + x;
        fdata[i]     = y & 0xFF;
        fdata[i + 1] = y >> 8;
    }
    // Read rest of file
    unsigned secnum = 3;
    for( unsigned pos = 14; pos < fsize; pos += 128, secnum++ )
    {
        unsigned len     = fsize - pos < 128 ? fsize - pos : 128;
        const uint8_t *s = atr_data(atr, secnum);
        if( !s )
            memset(fdata + pos, 0, len);
        else
            memcpy(fdata + pos, s, len);
    }

    if( extract_files )
    {
        struct stat st;
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
    else if( atari_list )
        printf("%-12s %7u\n", aname, fsize);
    else
        printf("%8u\t\t%s\n", fsize, path);

    free(fdata);
}

static int check_kboot(struct atr_image *atr)
{
    // K-File stucture:
    // 0000 : $00 / $03 / $00 / $07 / $14 / $07 / $4c / $14 / $07
    // 0009 : LSB / MSB / HSB (24 bits) of file length
    // 000C : 00
    // 000D : a9 46 8d c6 02 d0 fe ...

    // Only supports 128 bytes per sector, and the first 3 sectors are the boot
    // loader:
    if( atr->sec_size != 128 || atr->sec_count < 3 )
        return 0;

    const uint8_t *sec = atr_data(atr, 1);

    // Check "signature"
    return 0 == memcmp("\x00\x03\x00\x07\x14\x07\x4c\x14\x07", sec, 9);
}

static void extract_kboot(struct atr_image *atr, const char *atr_name, int atari_list,
                          int lower_case, int extract_files)
{

    const uint8_t *sec = atr_data(atr, 1);
    unsigned fsize     = sec[9] + (sec[10] << 8) + (sec[11] << 16);
    if( !fsize )
    {
        if( atari_list )
            printf("<EMPTY>\n");
        else
            printf("%8u\t\t/\n", 0);
        return;
    }
    // Get file data
    unsigned max_len     = atr->sec_count * 128 - 3 * 128;
    const uint8_t *fdata = atr_data(atr, 4);
    if( !fdata )
    {
        show_error("%s: missing file data", atr_name);
        return;
    }
    if( max_len < fsize )
    {
        show_msg("%s: data shorter than expected, truncating", atr_name);
        fsize = max_len;
    }

    unsigned crc = crc32(0, fdata, fsize);
    if( extract_files )
    {
        char *path;
        asprintf(&path, "kboot-%08x.xex", crc);
        struct stat st;
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
        free(path);
    }
    else if( atari_list )
        printf("%08X COM %7u\n", crc, fsize);
    else
    {
        printf("%8u\t\t/kboot-%08x.xex\n", fsize, crc);
    }
}

static void show_header(struct atr_image *atr, const char *atr_name, int atari_list,
                        const char *volname)
{
    if( atari_list )
        printf("ATR image: %s\n"
               "Image size: %u sectors of %u bytes\n"
               "Volume: %s\n",
               atr_name, atr->sec_count, atr->sec_size, volname);
    else
        printf("%s: %u sectors of %u bytes, %s.\n", atr_name, atr->sec_count,
               atr->sec_size, volname);
}

int extra_read(struct atr_image *atr, const char *atr_name, int atari_list,
               int lower_case, int extract_files)
{
    // Check BAS2BOOT
    if( check_bas2boot(atr) )
    {
        show_header(atr, atr_name, atari_list, "BAS2BOOT");
        extract_bas2boot(atr, atari_list, lower_case, extract_files);
        return 0;
    }
    else if( check_kboot(atr) )
    {
        show_header(atr, atr_name, atari_list, "K-BOOT");
        extract_kboot(atr, atr_name, atari_list, lower_case, extract_files);
        return 0;
    }

    return 1;
}
