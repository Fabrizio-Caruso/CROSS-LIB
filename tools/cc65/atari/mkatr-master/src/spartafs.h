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
#pragma once
#include "flist.h"
#include <stdint.h>

struct sfs;

struct sfs *build_spartafs(int sector_size, int num_sectors, unsigned boot_addr,
                           file_list *flist);

uint8_t *sfs_get_data(const struct sfs *);
int sfs_get_num_sectors(const struct sfs *);
int sfs_get_sector_size(const struct sfs *);
int sfs_get_free_sectors(const struct sfs *);
void sfs_free(struct sfs *);
