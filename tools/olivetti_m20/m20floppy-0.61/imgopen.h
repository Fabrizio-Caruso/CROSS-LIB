/* $Id: imgopen.h,v 1.4 2006/04/05 18:55:01 chris Exp $
 *
 * Olivetti M20 PCOS diskette access utility: image open routines prototypes
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

#ifndef __IMGOPEN_H__
#define __IMGOPEN_H__

extern int open_img_rw(char *,FILE **);
extern int open_img_ro(char *,FILE **);
extern int open_img_new(char *,FILE **);

#endif /* #ifndef __IMGOPEN_H__ */
/* Local Variables: */
/* c-file-style: "cpg" */
/* c-basic-offset: 4 */
/* End: */
