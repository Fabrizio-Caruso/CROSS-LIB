/* $Id: rwprotos.h,v 1.5 2006/04/05 21:19:19 chris Exp $
 *
 * Olivetti M20 PCOS diskette access utility: image write services functions
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

#ifndef __RWPROTOS_H__
#define __RWPROTOS_H__

extern void do_new(FILE **,int,char **);
extern void do_rm(FILE **,int,char **);
extern void do_put(FILE **,int,char **);
extern void do_mput(FILE **,int,char **);

#endif /* #ifndef __RWPROTOS_H__ */
/* Local Variables: */
/* c-file-style: "cpg" */
/* c-basic-offset: 4 */
/* End: */
