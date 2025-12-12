/*
 * Copyright (c) 2002 - 2018 Magnus Lind.
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

#include "../src/log.h"
#include "../src/exo_util.h"
#include "../src/6502emu.h"
#include "../src/areatrace.h"
#include "../src/int.h"
#include "../src/buf_io.h"
#include "../src/perf.h"

#include <stdlib.h>

struct mem_ctx
{
    u8 *mem;
    struct areatrace at;
};

static void mem_access_write(struct mem_access *this, u16 address, u8 value)
{
    struct mem_ctx *ctx = this->ctx;
    ctx->mem[address] = value;
    areatrace_access(&ctx->at, address);
}

static u8 mem_access_read(struct mem_access *this, u16 address)
{
    struct mem_ctx *ctx = this->ctx;
    return ctx->mem[address];
}

void save_single(const char *in_name, struct buf *mem, int start, int end)
{
    struct buf mb_name;
    const char *out_name;

    buf_init(&mb_name);

    buf_printf(&mb_name, "%s.out", in_name);
    out_name = (const char *)buf_data(&mb_name);

    buf_remove(mem, end, -1);
    buf_remove(mem, 0, start);

    write_file(out_name, mem);

    buf_free(&mb_name);
}

void test_single(const char *prg_name, const char *data_name,
                 int *cyclesp, int *in_lenp, int *out_lenp)
{
    struct cpu_ctx r;
    struct load_info prg_info;
    struct load_info data_info;
    struct buf mem_mb;
    u8 *mem;
    struct mem_ctx mem_ctx;
    int start;
    int end;

    buf_init(&mem_mb);
    buf_append(&mem_mb, NULL, 65536);
    mem = buf_data(&mem_mb);
    memset(mem, 0, 65536);
    mem_ctx.mem = mem;

    areatrace_init(&mem_ctx.at);

    prg_info.basic_txt_start = 0x0801;
    data_info.basic_txt_start = 0x0801;
    load_located(prg_name, mem, &prg_info);

    /* no start address from load*/
    if(prg_info.run == -1)
    {
        /* look for sys line */
        prg_info.run = find_sys(mem + prg_info.basic_txt_start, 0x9e, NULL);
    }
    if(prg_info.run == -1)
    {
        LOG(LOG_ERROR, ("Error, can't find entry point.\n"));
        exit(1);
    }

    LOG(LOG_DEBUG, ("run %04x\n", prg_info.run));

    load_located(data_name, mem, &data_info);

    /* communicate the data region to the prg */
    mem[2] = data_info.start & 255;
    mem[3] = data_info.start >> 8;
    mem[4] = data_info.end & 255;
    mem[5] = data_info.end >> 8;

    r.cycles = 0;
    r.mem.ctx = &mem_ctx;
    r.mem.read = mem_access_read;
    r.mem.write = mem_access_write;
    r.pc = prg_info.run;
    r.sp = '\xff';
    r.flags = 0;

    /* setting up decrunch */
    while(r.sp >= 0x10)
    {
        next_inst(&r);
    }

    /* save traced area */
    areatrace_merge_overlapping(&mem_ctx.at);
    areatrace_get_largest(&mem_ctx.at, &start, &end);

    save_single(data_name, &mem_mb, start, end);

    areatrace_free(&mem_ctx.at);
    buf_free(&mem_mb);

    if (cyclesp != NULL)
    {
        *cyclesp = r.cycles;
    }
    if (in_lenp != NULL)
    {
        *in_lenp = data_info.end - data_info.start;
    }
    if (out_lenp != NULL)
    {
        *out_lenp = end - start;
    }
}

int main(int argc, char *argv[])
{
    struct perf_ctx perf;
    struct buf buf;
    int i;
    int cycles;
    int inlen;
    int outlen;
    int cycles_sum = 0;
    int inlen_sum = 0;
    int outlen_sum = 0;

    /* init logging */
    LOG_INIT_CONSOLE(LOG_TERSE);

    perf_init(&perf);

    for (i = 1; i < argc; i += 2)
    {
        test_single(argv[i], argv[i + 1], &cycles, &inlen, &outlen);
        perf_add(&perf, argv[i + 1], inlen, outlen, cycles);
        cycles_sum += cycles;
        inlen_sum += inlen;
        outlen_sum += outlen;
    }
    if (argc > 3)
    {
        perf_add(&perf, "Total", inlen_sum, outlen_sum, cycles_sum);
    }

    buf_init(&buf);
    perf_buf_print(&perf, &buf);
    LOG(LOG_TERSE, ("%s", (char*)buf_data(&buf)));

    buf_free(&buf);
    perf_free(&perf);

    return 0;
}
