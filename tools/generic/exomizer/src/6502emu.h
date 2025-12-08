#ifndef ALREADY_INCLUDED_6502EMU
#define ALREADY_INCLUDED_6502EMU
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Copyright (c) 2007 - 2008 Magnus Lind.
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

#include "int.h"

#define MEM_ACCESS_READ(THIS,ADDR) (THIS)->read((THIS),(ADDR))
#define MEM_ACCESS_WRITE(THIS,ADDR,VALUE) (THIS)->write((THIS),(ADDR),(VALUE))

struct mem_access
{
    void *ctx;
    u8 (*read)(struct mem_access *this, u16 address);
    void (*write)(struct mem_access *this, u16 address, u8 value);
};

u16 mem_access_read_u16le(struct mem_access *this, u16 address);


struct cpu_ctx
{
    struct mem_access mem;
    u32 cycles;
    u16 pc;
    u8 sp;
    u8 flags;
    u8 a;
    u8 x;
    u8 y;
};

void next_inst(struct cpu_ctx *r);

#ifdef __cplusplus
}
#endif
#endif
