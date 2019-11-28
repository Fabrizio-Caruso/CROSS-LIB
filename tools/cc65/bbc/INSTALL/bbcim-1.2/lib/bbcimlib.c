/* BBCIM: bbcimlib.c: bbc disk image manipulation functies.

   Copyright (C) W.H. Scholten 1996, 1997, 1999, 2000, 2001, 2003
  
   Permission to use, copy, modify, distribute, and sell this software
   and its documentation for any purpose is hereby granted without fee,
   provided that the above copyright notice appear in all copies and
   that both that copyright notice and this permission notice appear in
   supporting documentation, and that the name of the copyright holder
   not be used in advertising or publicity pertaining to distribution
   of the software without specific, written prior permission. The
   copyright holder makes no representations about the suitability of
   this software for any purpose. It is provided "as is" without express
   or implied warranty.
  
   THE COPYRIGHT HOLDER DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS
   SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
   FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
   SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
   RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
   CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
   CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
  
  
   Send comments and bug-reports to 
  
      swhs-web@xs4all.nl
  
  
*/


/*__________________INCLUDES___________________*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "_config.h"

/* mkdir etc.: */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


//#include <bbcim_types.h>
#include <bbcim.h>
#include "bbcim_internal.h"

int   bbcim_errno = 0;
char *bbcim_error = NULL; /* points to the last error */

int   bbcim_debug = 0;

#include "overschrijf.c"

#include "utils.c"

//#include "catalogue_to_inf.c"
//#include "archive_crc_check.c"
//#include "archive_cleanup.c"

#include "_bbc_crc.c"
#include "_bbc_filenames.c"

#include "archive_format/_bbc_archive_write_attributes.c"
#include "archive_format/_bbc_archive_read_attributes.c"


#include "disk_convert_ddos_to_dfs.c"

#include "disk_catalogue_extract.c"
#include "disk_add_file.c"
#include "disk_delete_file.c"
#include "disk_expand.c"
#include "disk_minimise.c"
#include "disk_set_boot.c"
#include "disk_new.c"

#include "disk_combine_ss.c"
#include "disk_split_ds.c"

#include "disk_convert_w62_to_dfs.c"

#include "disk_id.c"

#include "error.c"

