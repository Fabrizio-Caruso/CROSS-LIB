/* $Id: roprotos.h,v 1.5 2006/07/11 22:22:31 chris Exp $
 *
 * Olivetti M20 PCOS diskette access utility: image readonly services functions
 * Copyright (C) 2000,2006 by Christian Groessler
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write
 * to the Free Software Foundation, 51 Franklin Street - Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __ROPROTOS_H__
#define __ROPROTOS_H__

extern void do_ls(FILE **,int,char **,int);
extern void do_cat(FILE **,int,char **);
extern void do_get(FILE **,int,char **);
extern void do_mget(FILE **,int,char **);

#endif /* #ifndef __ROPROTOS_H__ */
/* Local Variables: */
/* c-file-style: "cpg" */
/* c-basic-offset: 4 */
/* End: */
