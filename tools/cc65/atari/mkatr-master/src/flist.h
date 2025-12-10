/*
 *  Copyright (C) 2016 Daniel Serpell
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
 * List of files to add.
 */
#pragma once

#include "darray.h"

/* File attributes */
enum fattr
{
    at_protected = 1,
    at_hidden    = 2,
    at_archived  = 4
};

/* One file or directory */
struct afile
{
    const char *fname;
    const char *aname;
    const char *pname; // Full path name
    char *data;
    size_t size;
    struct afile *dir; // Parent directory
    int level;         // Level inside directory structure, 0 = root
    int is_dir;
    enum fattr attribs;
    int boot_file;
    int map_sect;
    char date[3];
    char time[3];
};

/* Max directory size in bytes, 32kb */
#define SFS_MAX_DIR_SIZE 32768

typedef darray(struct afile *) file_list;

void flist_add_main_dir(file_list *flist);
void flist_add_file(file_list *flist, const char *fname, int boot_file,
                    enum fattr attribs);
