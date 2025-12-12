/*
 *  Copyright (C) 2024 Daniel Serpell
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
 * Reads a HOWFEN DOS menu disk.
 */
#define _GNU_SOURCE
#include "lshowfen.h"

#include "atr.h"
#include "compat.h"
#include "msg.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// Read data and write a UNIX filename and an "Atari" filename.
static unsigned get_name(char *name, char *aname, const uint8_t *data, int max,
                         int lower_case)
{
    unsigned len = 0, last = 0;
    memset(aname, ' ', max + 1);
    for( int i = 0; i < max; i++ )
    {
        uint8_t c = data[i];
        if( (c & 0x40) == 0 )
            c = c + 0x20;
        else if( (c & 0x60) == 0x40 )
            c = c - 0x40;
        if( (c >= 0xB0 && c <= 0xB9) || (c >= 0xC1 && c <= 0xDA) ||
            (c >= 0xE1 && c <= 0xFA) )
            c = c & 0x7F;
        if( c >= 'A' && c <= 'Z' && lower_case )
            c = c - 'A' + 'a';
        if( c < ' ' || c == '/' || c == '.' || c == '?' || c == '\\' || c == 96 ||
            c > 'z' )
            c = '_';
        name[len++] = c;
        aname[i]    = c;
        if( c != ' ' )
            last = len;
    }
    name[last]  = 0;
    aname[last] = 0;
    return last;
}

// Decode decimal length
int get_len(const uint8_t *p)
{
    int len = 0;
    for( int i = 0; i < 4; i++ )
    {
        if( p[i] > 0x0F && p[i] < 0x1A )
            len = len * 10 + p[i] - 0x10;
    }
    return len;
}

int howfen_read(struct atr_image *atr, const char *atr_name, int atari_list,
                int lower_case, int extract_files)
{
    const uint8_t *sec1 = atr_data(atr, 1);
    if( !sec1 )
        return 1;
    // Minimal number of sectors is 10
    if( atr->sec_count < 10 )
        return 1;

    // Check signature: ' HOWFEN DOS ', in internal codes
    if( memcmp(sec1 + 0x58, "\x80\x28\x2f\x37\x26\x25\x2e\x00\x24\x2f\x33\x00", 12) )
        return 1;

    // Get menu version
    char ver[6];
    memcpy(ver, sec1 + 0x64, 5);
    if( ver[0] == 0x36 )
    {
        for( int i = 0; i < 5; i++ )
            ver[i] = (ver[i] & 0x3F) + 0x20;
        ver[5] = 0;
    }
    else
    {
        ver[0] = 0;
    }

    if( atari_list )
        printf("ATR image: %s\n"
               "Image size: %u sectors of %u bytes\n"
               "Volume: HOWFEN DOS %s\n",
               atr_name, atr->sec_count, atr->sec_size, ver);
    else
        printf("%s: %u sectors of %u bytes, HOWFEN DOS %s.\n", atr_name, atr->sec_count,
               atr->sec_size, ver);

    // This is the actual tables in the loader:
    // $89 + N*$20 : line with letter, name and size
    // $329     : number of files in the menu
    // $32A + N : LSB of starting sector of file N
    // $33E + N : MSB of starting sector of file N
    //
    // Files are stored ass boot sectors:
    //   $00 : if 1, high part of number of sectors, ignored otherwise.
    //   $01 : number of sectors
    // $02/$03 : Load address
    // $04/$05 : Address copied to DOSINI
    // $06 ... : Run data

    // Read directory
    char fname[32];
    char aname[32];
    for( int i = 0; i < 20; i++ )
    {
        const uint8_t *pos = sec1 + 0x8A + i * 0x20;
        if( *pos == (0x21 + i) )
        {
            // Get file size
            int slen = get_len(pos + 0x1B);
            // Check filename
            if( get_name(fname, aname, pos + 2, 25, lower_case) )
            {
                // Get sector number
                uint16_t snum = sec1[0x32A + i] + (sec1[0x33E + i] << 8);
                // Data is contiguos on disk, just read
                const uint8_t *fdata = atr_data(atr, snum);
                if( fdata )
                {
                    int ilen = fdata[1] + (fdata[0] == 1 ? 0x100 : 0);
                    if( ilen != slen )
                        show_msg("length does not match");
                    // Check that we have all the data
                    if( snum + slen - 1 > atr->sec_count )
                    {
                        show_msg("truncated file");
                        slen = atr->sec_count - snum + 1;
                    }
                }
                else
                {
                    show_msg("invalid sector number");
                    slen = 0;
                }
                unsigned fsize = slen * atr->sec_size;
                if( extract_files )
                {
                    struct stat st;
                    fprintf(stderr, "%s\n", fname);
                    // Check if file already exists:
                    if( 0 == stat(fname, &st) )
                        show_error("%s: file already exists.", fname);
                    // Create new file
                    int fd = creat(fname, 0666);
                    if( fd == -1 )
                        show_error("%s: can´t create file, %s", fname, strerror(errno));
                    if( fsize != write(fd, fdata, fsize) )
                        show_error("%s: can´t write file, %s", fname, strerror(errno));
                    if( close(fd) )
                        show_error("%s: can´t write file, %s", fname, strerror(errno));
                }
                else if( atari_list )
                    printf("%-20s %7u\n", aname, fsize);
                else
                    printf("%8u\t\t/%s\n", fsize, fname);
            }
        }
        else
            show_msg("invalid entry at pos %c", 'A' + i);
    }

    return 0;
}
