/* $Id: main.c,v 1.15 2006/10/19 23:41:25 chris Exp $
 *
 * Olivetti M20 PCOS diskette access utility
 * Copyright (C) 2000,2001,2004,2006 by Christian Groessler
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

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "version.h"
#include "cpgtypes.h"
#include "cpgdefs.h"
#include "fhdr.h"
#include "pcosdir.h"
#include "cmdopts.h"
#include "roprotos.h"
#include "rwprotos.h"
#include "imgopen.h"

static char issue[] = "m20 v" VERSION_ASCII
                      "\n" COPYRIGHT_STRING "\n";

int main(int argc,char **argv)
{
    int option,lsl=FALSE;
    FILE *image = NULL;

#ifdef DEBUG /* until I know all entries of the disk structures, I may need this code.... */
    printf("sizeof struct file_header: %ld\n",(long)sizeof(struct file_header));
    printf("sizeof struct dir_sector:  %ld\n",(long)sizeof(struct dir_sector));
#endif

    if (argc <=2) {
        if (argc == 2 && !strcmp(*(argv+1),"version")) goto version;
      errline:
        fprintf(stderr,issue);
        fprintf(stderr,"usage: %s <image file> <option>\n",*argv);
        fprintf(stderr,"       option can be one of\n");
        fprintf(stderr,"          ls      - list files on image\n");
        fprintf(stderr,"          lsl     - verbosely list files on image\n");
        fprintf(stderr,"          cat     - show file from image\n");
        fprintf(stderr,"          get     - get file from image\n");
        fprintf(stderr,"          mget    - get multiple files from image\n");
        fprintf(stderr,"          put     - put file onto image\n");
        fprintf(stderr,"          mput    - put multiple files onto image\n");
        fprintf(stderr,"          rm      - delete files from image\n");
        fprintf(stderr,"          new     - create a new empty image\n");
        fprintf(stderr,"          version - display program version\n");
        return(1);
    }

    if (!strcmp(*(argv+2),"lsl") || !strcmp(*(argv+2),"dir"))
        option = LSL_OPTION;
    else if (!strcmp(*(argv+2),"ls"))
        option = LS_OPTION;
    else if (!strcmp(*(argv+2),"cat") || !strcmp(*(argv+2),"type"))
        option = CAT_OPTION;
    else if (!strcmp(*(argv+2),"rm") || !strcmp(*(argv+2),"del"))
        option = DEL_OPTION;
    else if (!strcmp(*(argv+2),"get"))
        option = GET_OPTION;
    else if (!strcmp(*(argv+2),"mget"))
        option = MGET_OPTION;
    else if (!strcmp(*(argv+2),"put"))
        option = PUT_OPTION;
    else if (!strcmp(*(argv+2),"mput"))
        option = MPUT_OPTION;
    else if (!strcmp(*(argv+2),"new"))
        option = NEW_OPTION;
    else if (!strcmp(*(argv+2),"version"))
        option = VERSION_OPTION;
    else goto errline;

    switch(option) {
        case LSL_OPTION:
            lsl = TRUE;
            /* fall thru */
        case LS_OPTION:
            if (open_img_ro(*(argv+1),&image)) {
              open_file_err:
                fprintf(stderr,"%s: cannot open image file: %s: %s\n",*argv,*(argv+1),strerror(errno));
                return(option);
            }
            do_ls(&image,argc-2,argv+2,lsl);
            break;
        case CAT_OPTION:
            if (open_img_ro(*(argv+1),&image)) {
                goto open_file_err;
            }
            do_cat(&image,argc-2,argv+2);
            break;
        case GET_OPTION:
            if (open_img_ro(*(argv+1),&image)) {
                goto open_file_err;
            }
            do_get(&image,argc-2,argv+2);
            break;
        case MGET_OPTION:
            if (open_img_ro(*(argv+1),&image)) {
                goto open_file_err;
            }
            do_mget(&image,argc-2,argv+2);
            break;
        case PUT_OPTION:
            if (open_img_rw(*(argv+1),&image)) {
                goto open_file_err;
            }
            do_put(&image,argc-2,argv+2);
            break;
        case MPUT_OPTION:
            if (open_img_rw(*(argv+1),&image)) {
                goto open_file_err;
            }
            do_mput(&image,argc-2,argv+2);
            break;
        case NEW_OPTION:
            if (open_img_new(*(argv+1),&image)) {
                goto open_file_err;
            }
            do_new(&image,argc-2,argv+2);
            break;
        case DEL_OPTION:
            if (open_img_rw(*(argv+1),&image)) {
                goto open_file_err;
            }
            do_rm(&image,argc-2,argv+2);
            break;
        case VERSION_OPTION:
      version:
            printf("%s: %d.%02d\n",*argv,MAJOR_VERSION,MINOR_VERSION);
    }

#ifdef DOS
    if ((unsigned int)image > 'Z' - 'A') fclose(image);  /* DOS version may record the floppy drive there */
#else
    if (image) fclose(image);
#endif
    return(0);
}

/* Local Variables: */
/* c-file-style: "cpg" */
/* c-basic-offset: 4 */
/* End: */
