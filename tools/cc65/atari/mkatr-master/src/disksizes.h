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

// Standard disk sizes
static struct
{
    int size;
    int num;
} sectors[] = {{128, 720},   //   720 sectors of 128 bytes, 90k  (SD)
               {128, 1040},  //  1040 sectors of 128 bytes, 130k (ED)
               {256, 720},   //   720 sectors of 256 bytes, 180k (DD)
               {256, 1440},  //  1440 sectors of 256 bytes, 360k (HD)
               {256, 2048},  //  2048 sectors of 256 bytes, 512k (hard disk)
               {256, 4096},  //  4096 sectors of 256 bytes,   1M (hard disk)
               {256, 8192},  //  8192 sectors of 256 bytes,   2M (hard disk)
               {256, 16384}, // 16384 sectors of 256 bytes,   4M (hard disk)
               {256, 32768}, // 32768 sectors of 256 bytes,   8M (hard disk)
               {256, 65535}, // 65536 sectors of 256 bytes,  16M (biggest possible image)
               {0, 0}};
