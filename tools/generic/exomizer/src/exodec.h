#ifndef ALREADY_INCLUDED_EXODEC
#define ALREADY_INCLUDED_EXODEC
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Copyright (c) 2005 Magnus Lind.
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

#include "buf.h"
#include "flags.h"

struct dec_table
{
    unsigned char table_bit[8];
    unsigned char table_off[8];
    unsigned char table_bi[100];
    unsigned char table_lo[100];
    unsigned char table_hi[100];
};

struct dec_ctx
{
    int inpos;
    int inend;
    unsigned char *inbuf;
    struct buf *outbuf;
    unsigned char bitbuf;
    /* dep_table */
    struct dec_table t;
    int bits_read;
    int flags_proto;
};

/* returns the encoding */
void
dec_ctx_init(struct dec_ctx *ctx,
             struct buf *enc_in, /* optional */
             struct buf *inbuf, struct buf *outbuf, int flags_proto);

void
dec_ctx_table_dump(struct dec_ctx *ctx, struct buf *enc_out);

void
dec_ctx_free(struct dec_ctx *ctx);

void dec_ctx_decrunch(struct dec_ctx *ctx);

#ifdef __cplusplus
}
#endif
#endif
