#ifndef ALREADY_INCLUDED_CHUNKPOOL
#define ALREADY_INCLUDED_CHUNKPOOL
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Copyright (c) 2003 - 2005, 2015 Magnus Lind.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not
 *   claim that you wrote the original software. If you use this software in a
 *   product, an acknowledgment in the product documentation would be
 *   appreciated but is not required.
 *
 *   2. Altered source versions must be plainly marked as such, and must not
 *   be misrepresented as being the original software.
 *
 *   3. This notice may not be removed or altered from any source distribution.
 *
 */

#include "callback.h"
#include "vec.h"

struct chunkpool {
    int item_size;
    int item_pos;
    int item_end;
    void *current_chunk;
    struct vec used_chunks;
    int alloc_count;
};

void
chunkpool_init(struct chunkpool *ctx, int item_size);

void
chunkpool_free(struct chunkpool *ctx);

void
chunkpool_free2(struct chunkpool *ctx, cb_free *f);

void *
chunkpool_malloc(struct chunkpool *ctx);

void *
chunkpool_calloc(struct chunkpool *ctx);

#ifdef __cplusplus
}
#endif
#endif
