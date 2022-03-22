/* $Id: packstrt.h,v 1.4 2006/04/05 18:55:02 chris Exp $
 *
 * start structure packing for different compilers
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

/* packing start */
#if !defined(_NEWGNUC_) && !defined(__WATCOMC__) && !defined(_NEWMSC_)
#  pragma pack(1)
#endif
#ifdef _NEWMSC_
#  pragma pack(push,_cpg_label_,1)
#endif

/* Local Variables: */
/* c-file-style: "cpg" */
/* c-basic-offset: 4 */
/* End: */
