#ifndef ALREADY_INCLUDED_PC
#define ALREADY_INCLUDED_PC
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Copyright (c) 2004 Magnus Lind.
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

#include "expr.h"

void pc_dump(int level);
void pc_set(int pc);
void pc_set_expr(struct expr *pc);
struct expr *pc_get(void);
void pc_add(int offset);
void pc_add_expr(struct expr *pc);
void pc_unset(void);

#ifdef __cplusplus
}
#endif
#endif
