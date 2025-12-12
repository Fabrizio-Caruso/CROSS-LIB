/*
 *  Copyright (C) 2023 Daniel Serpell
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program.  If not, see <http://www.gnu.org/licenses/>
 */
/*
 * Loads an ATR with a SpartaDOS file-system and list contents.
 */
#include "atr.h"
#include "compat.h"
#include "lsdos.h"
#include "lsextra.h"
#include "lshowfen.h"
#include "lssfs.h"
#include "msg.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//---------------------------------------------------------------------
static void show_usage(void)
{
    printf("Usage: %s [options] <atr_image_file>\n"
           "Options:\n"
           "\t-a\tShow listing in Atari instead of UNIX format.\n"
           "\t-l\tConvert filenames to lower-case.\n"
           "\t-x\tExtract listed files to current path.\n"
           "\t-X path\tExtract listed files to given path.\n"
           "\t-h\tShow this help.\n"
           "\t-v\tShow version information.\n",
           prog_name);
    exit(EXIT_SUCCESS);
}

//---------------------------------------------------------------------
int main(int argc, char **argv)
{
    const char *atr_name = 0;
    const char *ext_path = 0;
    int lower_case       = 0;
    int atari_list       = 0;
    int extract_files    = 0;
    prog_name            = argv[0];
    for( int i = 1; i < argc; i++ )
    {
        char *arg = argv[i];
        if( arg[0] == '-' )
        {
            char op;
            while( 0 != (op = *++arg) )
            {
                if( op == 'h' || op == '?' )
                    show_usage();
                else if( op == 'l' )
                    lower_case = 1;
                else if( op == 'a' )
                    atari_list = 1;
                else if( op == 'x' )
                    extract_files = 1;
                else if( op == 'X' )
                {
                    if( i + 1 >= argc )
                        show_opt_error("option '-X' needs an argument");
                    i++;
                    extract_files = 1;
                    ext_path      = argv[i];
                }
                else if( op == 'v' )
                    show_version();
                else
                    show_opt_error("invalid command line option '-%c'", op);
            }
        }
        else if( !atr_name )
            atr_name = arg;
        else
            show_opt_error("multiple ATR files in command line");
    }
    if( !atr_name )
        show_opt_error("ATR file name expected");

    if( extract_files && atari_list )
        show_opt_error("options '-x' and '-a' not compatible");

    // Load ATR image file
    struct atr_image *atr = load_atr_image(atr_name);

    // Open target directory
    if( ext_path && chdir(ext_path) )
    {
        // If path does not exists, check if we can make it
        if( errno == ENOENT )
        {
            // Try to create the path
            show_msg("creating output path '%s'.", ext_path);
            if( compat_mkdir(ext_path) || chdir(ext_path) )
                show_error("can't create path, '%s': %s", ext_path, strerror(errno));
        }
        else
            show_error("%s: invalid extract path, %s", ext_path, strerror(errno));
    }

    int e = sfs_read(atr, atr_name, atari_list, lower_case, extract_files);
    if( e )
        e = howfen_read(atr, atr_name, atari_list, lower_case, extract_files);
    if( e )
        e = dos_read(atr, atr_name, atari_list, lower_case, extract_files);
    if( e )
        e = extra_read(atr, atr_name, atari_list, lower_case, extract_files);
    if( e )
        show_msg("%s: ATR image format not supported.", atr_name);
    atr_free(atr);

    return e;
}
