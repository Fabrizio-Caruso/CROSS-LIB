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
 * Sparta FS
 */
#include "spartafs.h"
#include "asm/boot128.h"
#include "asm/boot256.h"
#include "crc32.h"
#include "msg.h"
#include <stdint.h>
#include <string.h>

static char hex(int x)
{
    return ("0123456789ABCDEF")[x & 0x0F];
}

struct sfs
{
    uint8_t *data;
    int nsec;
    int bmap;
    int nbmp;
    int csec;
    int boot_map;
    int sec_size;
};

static uint8_t *sfs_ptr(struct sfs *sfs, int sec)
{
    return sfs->data + sfs->sec_size * (sec - 1);
}

static void sfs_free_sec(struct sfs *sfs, int sec)
{
    sfs_ptr(sfs, sfs->bmap)[sec >> 3] |= (128 >> (sec & 7));
}

static int sfs_used(struct sfs *sfs, int sec)
{
    return 0 == (sfs_ptr(sfs, sfs->bmap)[sec >> 3] & (128 >> (sec & 7)));
}

static int sfs_alloc(struct sfs *sfs)
{
    int sec = sfs->csec;
    while( sec <= sfs->nsec && sfs_used(sfs, sec) )
        sec++;
    if( sec <= sfs->nsec )
    {
        sfs_ptr(sfs, sfs->bmap)[sec >> 3] &= ~(128 >> (sec & 7));
        sfs->csec = sec + 1;
        return sec;
    }
    else
        return -1;
}

static int get_word(const uint8_t *data)
{
    return (data[0] & 0xFF) + ((data[1] & 0xFF) << 8);
}

static int sfs_patch_byte(struct sfs *sfs, int smap, int pos, int byte)
{
    int mpos = 4;
    while( pos >= sfs->sec_size )
    {
        pos -= sfs->sec_size;
        mpos += 2;
        if( mpos >= sfs->sec_size )
        {
            mpos = 4;
            smap = get_word(sfs_ptr(sfs, smap));
            if( smap < 1 || smap > sfs->nsec )
                return -1;
        }
    }
    int sect = get_word(sfs_ptr(sfs, smap) + mpos);
    if( sect < 1 || sect > sfs->nsec )
        return -1;
    uint8_t *data = sfs_ptr(sfs, sect);
    data[pos]     = byte;
    return 0;
}

static int sfs_add_data(struct sfs *sfs, char *data, int size)
{
    int sec_size = sfs->sec_size;
    int last = 0, first = 0;
    uint8_t *pmap = 0;
    do
    {
        // Alloc a sector map
        int smap = sfs_alloc(sfs);
        if( smap < 0 )
            return smap;

        if( pmap )
        {
            pmap[0] = smap & 0xFF;
            pmap[1] = smap >> 8;
        }
        else
            first = smap;

        pmap    = sfs_ptr(sfs, smap);
        pmap[2] = last & 0xFF;
        pmap[3] = last >> 8;
        // Copy data
        int i;
        for( i = 4; i < sec_size && size > 0; i += 2 )
        {
            int num = size > sec_size ? sec_size : size;
            int sec = sfs_alloc(sfs);
            if( sec < 0 )
                return sec;
            pmap[i]     = sec & 0xFF;
            pmap[i + 1] = sec >> 8;
            memcpy(sfs_ptr(sfs, sec), data, num);
            size -= num;
            data += num;
        }
        last = smap;
    } while( size );
    return first;
}

// Write the boot sectors, relocated to the given address
static void write_boot(struct sfs *sfs, int address)
{
    unsigned char data[384];
    unsigned char *boot;
    unsigned *reloc;
    unsigned rsize, i;

    if( sfs->nsec < 3 )
        return;

    if( sfs->sec_size == 128 )
    {
        boot  = boot128_bin;
        reloc = boot128_reloc;
        rsize = sizeof(boot128_reloc) / sizeof(boot128_reloc[0]);
    }
    else if( sfs->sec_size == 256 )
    {
        boot  = boot256_bin;
        reloc = boot256_reloc;
        rsize = sizeof(boot256_reloc) / sizeof(boot256_reloc[0]);
    }
    else
        return;

    // Relocate code using reloc table:
    memcpy(data, boot, 384);
    for( i = 0; i < rsize; i++ )
        data[reloc[i] - 1] = data[reloc[i] - 1] + address - 16;

    // Copy boot sectors, always 128 byte size:
    for( i = 0; i < 3; i++ )
        memcpy(sfs->data + sfs->sec_size * i, data + 128 * i, 128);
}

// Sorting function: sort by level, then directories first and last by file name
static int compare_level(const void *a, const void *b)
{
    const struct afile *fa = *(const struct afile *const *)a;
    const struct afile *fb = *(const struct afile *const *)b;
    if( fb->level != fa->level )
        return fb->level - fa->level;
    if( fb->is_dir != fa->is_dir )
        return fb->is_dir - fa->is_dir;
    return strncmp(fa->aname, fb->aname, 11);
}

struct sfs *build_spartafs(int sector_size, int num_sectors, unsigned boot_addr,
                           file_list *flist)
{
    struct sfs *sfs = check_malloc(sizeof(struct sfs));
    sfs->data       = check_calloc(sector_size, num_sectors);
    sfs->nsec       = num_sectors;
    sfs->bmap       = 4;
    sfs->nbmp       = ((num_sectors + 8) / 8 + sector_size - 1) / sector_size;
    sfs->csec       = 4 + sfs->nbmp;
    sfs->sec_size   = sector_size;

    write_boot(sfs, boot_addr);

    int i;
    for( i = sfs->csec; i <= sfs->nsec; i++ )
        sfs_free_sec(sfs, i);

    // Sort the entries by the level - higher level first
    qsort(&darray_i(flist, 0), darray_len(flist), sizeof(darray_i(flist, 0)),
          compare_level);

    // Cleanup all directories
    struct afile **ptr;
    darray_foreach(ptr, flist)
    {
        struct afile *af = *ptr;
        if( af->is_dir )
        {
            af->size = 23;
            memset(af->data, 0, 23);
        }
    }

    // Add each file
    int dsec = -1;
    darray_foreach(ptr, flist)
    {
        struct afile *af = *ptr;
        if( af->is_dir )
        {
            int dsize   = af->size;
            af->data[0] = 0x28;
            af->data[1] = 0; // Parent dir map,
            af->data[2] = 0; // written later
            af->data[3] = dsize & 0xFF;
            af->data[4] = (dsize >> 8) & 0xFF;
            af->data[5] = dsize >> 16;
            memcpy(&af->data[6], af->aname, 11);
            memcpy(&af->data[17], &af->date, 3);
            memcpy(&af->data[20], &af->time, 3);
        }
        // Add data
        int msec = sfs_add_data(sfs, af->data, af->size);
        if( msec < 0 )
        {
            sfs_free(sfs);
            return 0;
        }
        // Set map sector
        af->map_sect = msec;
        // Add to directory
        struct afile *dir = af->dir;
        if( dir )
        {
            char *cdir = dir->data + dir->size;

            cdir[0] = 0x08 | (af->is_dir ? 0x20 : 0x00) | af->attribs;
            cdir[1] = msec & 0xFF;
            cdir[2] = msec >> 8;
            cdir[3] = af->size & 0xFF;
            cdir[4] = (af->size >> 8) & 0xFF;
            cdir[5] = af->size >> 16;
            memcpy(&cdir[6], af->aname, 11);
            memcpy(&cdir[17], &af->date, 3);
            memcpy(&cdir[20], &af->time, 3);

            dir->size += 23;
            if( dir->size > SFS_MAX_DIR_SIZE )
                show_error("too many files in directory %s.", dir->pname);
        }
        else
            // This is the main directory, remember location
            dsec = msec;

        if( af->boot_file )
            sfs->boot_map = msec;
    }

    // Set the "parent" directory to all sub-directories
    darray_foreach(ptr, flist)
    {
        struct afile *af = *ptr;
        if( af->is_dir && af->dir )
        {
            int parent = af->dir->map_sect;
            sfs_patch_byte(sfs, af->map_sect, 1, parent & 0xFF);
            sfs_patch_byte(sfs, af->map_sect, 2, parent >> 8);
        }
    }

    // Check main directory
    if( dsec < 0 )
        show_error("internal error - no main directory.");

    // Get's CRC32 of current data
    unsigned crc = crc32(0, sfs->data, sfs->sec_size * sfs->nsec);

    sfs->data[1]  = 0x03;
    sfs->data[7]  = 0x80;
    sfs->data[9]  = dsec & 0xFF;
    sfs->data[10] = dsec >> 8;
    sfs->data[11] = sfs->nsec & 0xFF;
    sfs->data[12] = sfs->nsec >> 8;
    sfs->data[13] = (sfs->nsec - sfs->csec + 1) & 0xFF;
    sfs->data[14] = (sfs->nsec - sfs->csec + 1) >> 8;
    sfs->data[15] = sfs->nbmp;
    sfs->data[16] = sfs->bmap & 0xFF;
    sfs->data[17] = sfs->bmap >> 8;
    sfs->data[18] = sfs->csec & 0xFF;
    sfs->data[19] = sfs->csec >> 8;
    sfs->data[20] = sfs->csec & 0xFF;
    sfs->data[21] = sfs->csec >> 8;
    sfs->data[22] = 'D';
    sfs->data[23] = 'S';
    sfs->data[24] = 'K';
    sfs->data[25] = '_';
    sfs->data[26] = hex(crc >> 8);
    sfs->data[27] = hex(crc >> 12);
    sfs->data[28] = hex(crc >> 16);
    sfs->data[29] = hex(crc >> 20);
    sfs->data[30] = 0x28;
    sfs->data[31] = sfs->sec_size > 128 ? 0 : 128;
    sfs->data[32] = 0x20;
    sfs->data[39] = crc & 0xFF;
    sfs->data[40] = sfs->boot_map & 0xFF;
    sfs->data[41] = sfs->boot_map >> 8;

    return sfs;
}

uint8_t *sfs_get_data(const struct sfs *sfs)
{
    return sfs->data;
}

int sfs_get_num_sectors(const struct sfs *sfs)
{
    return sfs->nsec;
}

int sfs_get_sector_size(const struct sfs *sfs)
{
    return sfs->sec_size;
}

int sfs_get_free_sectors(const struct sfs *sfs)
{
    return sfs->nsec - sfs->csec + 1;
}

void sfs_free(struct sfs *sfs)
{
    if( sfs )
    {
        free(sfs->data);
        free(sfs);
    }
}
