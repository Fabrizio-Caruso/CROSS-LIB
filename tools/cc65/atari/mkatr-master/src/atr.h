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
#pragma once
#include <stdint.h>

struct atr_image
{
    const uint8_t *data;
    unsigned sec_size;
    unsigned sec_count;
};

struct atr_image *load_atr_image(const char *file_name);
void atr_free(struct atr_image *atr);
const uint8_t *atr_data(const struct atr_image *atr, unsigned sector);
