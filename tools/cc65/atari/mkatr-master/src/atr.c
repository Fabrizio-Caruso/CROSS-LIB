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
 * Load ATR files.
 */

#include "atr.h"
#include "msg.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Load disk image from file
struct atr_image *load_atr_image(const char *file_name)
{
    FILE *f = fopen(file_name, "rb");
    if( !f )
    {
        show_error("can´t open disk image '%s': %s", file_name, strerror(errno));
        return 0;
    }

    // Get header
    uint8_t hdr[16];
    if( 1 != fread(hdr, 16, 1, f) )
    {
        show_error("%s: can´t read ATR header", file_name);
        fclose(f);
        return 0;
    }
    if( hdr[0] != 0x96 || hdr[1] != 0x02 )
    {
        // Check if we can open as a raw SS/SD or SD/ED image
        uint8_t *data = check_calloc(1, 128 * 1040 + 16);
        // Move header
        memcpy(data, hdr, 16);
        // Read the rest of the file
        size_t num = 16 + fread(data + 16, 1, 1040 * 128, f);
        // Accept only exact sizes
        if( num != 720 * 128 && num != 1040 * 128 )
        {
            show_error("%s: not an ATR image", file_name);
            fclose(f);
            free(data);
            return 0;
        }
        fclose(f);
        struct atr_image *atr = check_malloc(sizeof(struct atr_image));
        atr->data             = data;
        atr->sec_size         = 128;
        atr->sec_count        = num / 128;
        return atr;
    }
    unsigned ssz = hdr[4] | (hdr[5] << 8);
    if( ssz != 128 && ssz != 256 )
    {
        show_error("%s: unsupported ATR sector size (%d)", file_name, ssz);
        fclose(f);
        return 0;
    }
    unsigned isz = (hdr[2] << 4) | (hdr[3] << 12) | (hdr[6] << 20);
    // Some images store full size fo the first 3 sectors, others store
    // 128 bytes for those:
    unsigned pad_size    = isz % ssz == 0 ? 0 : (ssz - 128) * 3;
    unsigned num_sectors = (isz + pad_size) / ssz;
    if( isz >= 0x1000000 || num_sectors * ssz - pad_size != isz )
    {
        // If image size is invalid, assume sector padding:
        pad_size    = 3 * (ssz - 128);
        num_sectors = (isz + pad_size) / ssz;
        if( num_sectors > 65535 )
            num_sectors = 65535;
        if( num_sectors < 3 )
            show_error("%s: invalid ATR image size (%d), too small.", file_name, isz);
        show_msg("%s: invalid ATR image size (%d), rounding down to (%d)", file_name, isz,
                 num_sectors * ssz - pad_size);
    }
    // Allocate new storage
    uint8_t *data = check_calloc(ssz, num_sectors);
    // Read 3 first sectors
    for( unsigned i = 0; i < num_sectors; i++ )
    {
        if( 1 != fread(data + ssz * i, (i < 3 && pad_size) ? 128 : ssz, 1, f) )
        {
            show_msg("%s: ATR file too short at sector %d", file_name, i + 1);
            break;
        }
    }
    // Check that sector paddings are 0
    if( ssz == 256 && num_sectors > 3 )
    {
        int chk = 0;
        for( unsigned i = 0; i < 3; i++ )
        {
            chk = 0;
            for( unsigned j = 0; j < 128; j++ )
                chk += data[i * 256 + j + 128];
            if( chk != 0 )
            {
                show_msg("%s: ATR suspect - sector %d has data over 128 bytes, fixing.",
                         file_name, i + 1);
                break;
            }
        }
        if( chk )
        {
            // Try to detect and fixing bad image
            // Check zeros at end:
            int chk1 = 0;
            for( unsigned i = 0; i < 384; i++ )
                chk1 += data[ssz * num_sectors - 384 + i];
            // Check zeros after first 3 sectors:
            int chk2 = 0;
            for( unsigned i = 0; i < 384; i++ )
                chk2 += data[384 + i];

            if( !chk1 && chk2 )
            {
                memmove(data + 3 * 256, data + 3 * 128, ssz * (num_sectors - 3));
                memmove(data + 2 * 256, data + 2 * 128, 128);
                memmove(data + 1 * 256, data + 1 * 128, 128);
            }
            else if( !chk2 )
            {
                memmove(data + 3 * 256, data + 3 * 128, 128);
                memmove(data + 2 * 256, data + 2 * 128, 128);
                memmove(data + 1 * 256, data + 1 * 128, 128);
            }
            // Clear remaining space
            memset(data + 2 * 256 + 128, 0, 128);
            memset(data + 1 * 256 + 128, 0, 128);
            memset(data + 0 * 256 + 128, 0, 128);
        }
    }
    fclose(f);
    // Ok, copy to image
    struct atr_image *atr = check_malloc(sizeof(struct atr_image));
    atr->data             = data;
    atr->sec_size         = ssz;
    atr->sec_count        = num_sectors;
    return atr;
}

void atr_free(struct atr_image *atr)
{
    if( atr->data )
        free((uint8_t *)(atr->data));
    free(atr);
}

const uint8_t *atr_data(const struct atr_image *atr, unsigned sector)
{
    if( sector < 1 || sector > atr->sec_count )
        return 0;
    else
        return atr->data + (sector - 1) * atr->sec_size;
}
