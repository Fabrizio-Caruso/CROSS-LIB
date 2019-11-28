/* BBCIM: Acorn BBC computer disk image manipulation
 *
 * disk_catalogue_extract.c:
 * NEDERLANDS: toon catalogus van bestanden etc in diskbeeld (DFS/ADFS).
 * ENGLISH:    Show catalogue & extract files & calculate CRC's (Acorn DFS)
 *
 * Copyright (C) W.H.Scholten 1996,1997,1999,2000,2001,2003,2004, 2009
 */



int  _bbcim__disk_dfscat_extract_crc(struct bbc_archive__diskimage *image, int side, struct bbc_archive__file* beeb_file, int flags, int verbose);



int  bbcim_disk_cat_extract_crc (struct bbc_archive__diskimage *image, int side, struct bbc_archive__file* beeb_file, int flags, int verbose) {
	int i;


//	struct bbc_archive__file_attr *attr;
//	if (beeb_file) attr = beeb_file->attr;


	if (bbcim_diskimage_sanitycheck(image, verbose)) return bbcim_errno;

	if (side < 0 || (side >= image->attr->sides))
		return bbcim_print_error_bad_side("bbcim_cat_extract_crc", image, side, verbose);

	if (bbcim_debug) { printf("Cat/extr flags:\n"); for (i=0; i<16; i++) { if (flags & (1<<i)) printf("Flag %d\n", 1<<i); } }

	bbcim_errno = 0;

	if (verbose && (flags & BBCIM_EXTRACT_FILES)) flags |= BBCIM_SHOW_FILES;

	if (image->attr->type == BBC_ARCHIVE__DFS_DISK) {
		/* int  st =*/ _bbcim__disk_dfscat_extract_crc(image, side, beeb_file, flags, verbose);
	}
	else if (image->attr->type == BBC_ARCHIVE__ADFS_DISK) {
		return bbcim_print_error_unsupported_disktype("bbcim_cat_extract_crc", image, verbose);
	}
	else {
		return bbcim_print_error_unsupported_disktype("bbcim_cat_extract_crc", image, verbose);
	}

	return bbcim_errno;
}

#include "disk_catalogue_extract__dfs.c"

