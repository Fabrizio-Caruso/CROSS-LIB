/* BBCIM: bbcim.c: Frontend to the bbcim library

   Copyright (C) W.H.Scholten 1996, 1997, 1999, 2001, 2003, 2009, 2016

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
//#include <bbcim_types.h>
#include <bbcim.h>

//#include "../lib/bbcim_internal.h" /* temp */


/* From utils.c: */
extern int  set_disktype(struct bbc_archive__diskimage *image, char *arg, int verbose);
extern void set_diskimage_defaults(struct bbc_archive__diskimage *image);
extern int  copy_disktype(struct bbc_archive__diskimage *from_image1, struct bbc_archive__diskimage *to_image2);
extern int  side_from_args(char *argv, int argc);
extern int  get_disk_type_name_side(char **argv, int argc, struct bbc_archive__diskimage *image, int *side);

enum optie_type { VERSIE, HELP, INTERSS, SPLITDS, DDOS_DFS, W62_DFS, SPLIT_CAT, EXTRACT, ADD, DEL, CAT, CRC, ARCHIVE_CLEANUP, ARCHIVE_CRC, DISKSIZE40, DISKSIZE80, MINIMISE, MAXIMISE, BOOT, NEW_DISK, ID, HELP2, _NUM_TYPES_ };

char *optie_strings[] = {
	"V", "H", "interss", "splitds", "ddos_dfs", "w62_dfs", "s", "e", "a", "d", "c", "crc", "inf_clean", "inf_crc", "40", "80", "min", "max", "boot", "new", "id", "help"
};

char subopties[] = { 0,0,0,0,0,0, 1,4,2,0,0, 0,0,0,0,0,0,0,0,0,0,0 };

//char min_args[]  = { 0,0,4,2,1,1, 1,1,2,2,1, 1,1,1,1,1,1,1,1,1,1,0 };


#include "_help.c"
#include "_help_man.c"
#include "../lib/overschrijf.h"
#include "functions/archive_cleanup.c"
#include "functions/archive_crc_check.c"
#include "functions/catalogue_to_inf.c"



/*__________________PROGRAMMA___________________*/
/* Prototypes: */
void argc_check(int argc, int options, int required);



/* Functions: */
void argc_check(int argc, int options, int required) {
	if (argc-options < required) {
		fprintf(stderr,
				TEKST_NL("Niet genoeg parameters\n\n")
				TEKST_ENG("Missing parameters\n\n"));
		use_option();
		printf(TEKST_NL("Gebruik -help <optie> voor meer informatie\n\n")
			   TEKST_ENG("For more information use -help <option>\n\n"));
		exit(1);
	}
}


	int  optie;
int main(int argc, char *argv[]) {
	FILE *fpdisk;
	char optstring[21];
	int  bad_option;
	int  options = 1;

	int  flags;

	char cat_naam[256];

	struct bbc_archive__diskimage image;
	struct bbc_archive__diskimage_attr image_attr;
	char disk[256];

	struct bbc_archive__file file; /* dummy argument to general functions */
	struct bbc_archive__file_attr file_attr;

	int  side = 0;

	struct bbc_archive__diskimage image1, image2;
	struct bbc_archive__diskimage_attr image_attr1, image_attr2;
	char side0[256],  side1[256];

	int  i;


	bbcim_debug = 0; /* libbbcim & frontend */

	image.attr = &image_attr;
	image.filename = disk;

	file.attr = &file_attr;

	image1.attr = &image_attr1;
	image1.filename = side0;
	image2.attr = &image_attr2;
	image2.filename = side1;

	set_diskimage_defaults(&image);
	set_diskimage_defaults(&image1);
	set_diskimage_defaults(&image2);


	bbcim_errno = 0;


	/* ------------------------------------ */
	/* Process options and arguments        */
	if (argc <= 1) { use(); exit(1); }

	strncpy(optstring, argv[1], 20);
	if (bbcim_debug) printf("optstring=%s\n", optstring);


	/*  for (;;) options = saved_options; */
	/* to check all options that completely/partially resemble? */
	/* i.e. if -extr is checked as for -e it gives bad option, but -extr could be another command */

	optie = -1;
	if (optstring[0] == '-') {
		int  i;

		/* First check exact fit with options that have no suboptions */
		for (i=0; i<_NUM_TYPES_; i++) {
			if (!subopties[i]) {
				if (!strcmp(optstring+1, optie_strings[i])) {
					optie = i; options++;
					break;
				}
			}
		}
		if (bbcim_debug) printf("optie=%d, options=%d\n", optie, options);

		if (optie == -1) {
			for (i=0; i<_NUM_TYPES_; i++) {
				if (subopties[i]) {
					if (!strncmp(optstring+1, optie_strings[i], strlen(optie_strings[i]))) {
						optie = i; options++;
						break;
					}
				}
			}
		}

		//if (optie == -1) { use(); exit(1); }
	} else use();


	bad_option = 0;

	if (bbcim_debug) printf("optie=%d (\"%s\"), 'options'=%d\n", optie, optie_strings[optie], options);




	/* --------------------------------------------------------- */
	/* The option should now be known, so execute the command */
	switch (optie) {
	case VERSIE:
		printf("bbcim "BBCIM_VERSION"\n");
		break;

	case HELP:
		show_help();
		break;

    case HELP2:
		//argc_check(argc, options, 1);
		if (argc-options < 1) { use(); exit(1); }
		help2(argv[2]);
		break;

	case INTERSS:
		options += get_disk_type_name_side(argv+options, argc-options, &image1, &side);

		options += get_disk_type_name_side(argv+options, argc-options, &image2, &side);

		argc_check(argc, options, 1);
		strcpy(disk, argv [ options ]); options++;

		bbcim_interleave_singlesided(&image1, &image2, disk, 1);
		break;

	case SPLITDS:
		options += get_disk_type_name_side(argv+options, argc-options, &image, &side);

		bbcim_split_doublesided(&image, 1);
		break;

	case DDOS_DFS:
		options += get_disk_type_name_side(argv+options, argc-options, &image, &side);
		bbcim_ddos_to_dfs(&image, 1);
		break;

	case W62_DFS:
		options += get_disk_type_name_side(argv+options, argc-options, &image, &side);
		bbcim_w62_to_dfs(&image, 1);
		break;

	case SPLIT_CAT:
		argc_check(argc, options, 1);
		strcpy(cat_naam, argv [ options ]); options++;

		flags = BBCIM__NAME__KEEP_ROOT_DIR | BBCIM__NAME__KEEP_NONROOT_DIR;
		for (i=/*2*/strlen(optie_strings [ optie ])+1; i<strlen(optstring); i++) {
			switch (optstring[i]) {
			case '@' : flags &= ~BBCIM__NAME__KEEP_ROOT_DIR; break;
//			case '#' : flags &= ~(BBCIM__NAME__KEEP_NONROOT_DIR | BBCIM__NAME__KEEP_ROOT_DIR); break;
			default  : bad_option = 1;
				break;
			}
		}
		if (bad_option == 1) { break; }

		bbcim_catalogue_to_inf(cat_naam, flags, 1);
		break;

	case EXTRACT:
		options += get_disk_type_name_side(argv+options, argc-options, &image, &side);

		flags = BBCIM_EXTRACT_FILES | BBCIM_DISK_INFO | BBCIM__NAME__KEEP_ROOT_DIR | BBCIM__NAME__KEEP_NONROOT_DIR;

		for (i=/*2*/strlen(optie_strings [ optie ])+1; i<strlen(optstring); i++) {
			switch (optstring[i]) {
			case 'd' : flags |=  BBCIM_EXTRACT_DIR; break;
			case 's' : flags |=  BBCIM_SHORT_NAME; break;
			case 'b' : flags &= ~BBCIM_DISK_INFO; break;
			case '@' : flags &= ~BBCIM__NAME__KEEP_ROOT_DIR; break;
//			case 'r' : flags |=  BBCIM_REMOVE_FILE; break;
			default  : bad_option = 1;
				break;
			}
			printf("flags=0x%x\n", flags);
			if ((flags & BBCIM_SHORT_NAME) && (flags & BBCIM_EXTRACT_DIR)) {
				bad_option = 1;
				break;
			}
		}
		if (!bad_option) {
			int extract_given = 0;
			for (i=options; i<argc; i++) {
				file.attr->name = argv[i];
				bbcim_disk_cat_extract_crc(&image, side, &file, flags, 1);
				extract_given++;
			}
			if (extract_given == 0) {
				if (bbcim_debug) printf("DEBUG: extract all\n");
				bbcim_disk_cat_extract_crc(&image, side, NULL, flags, 1);
			}
		}
		break;

	case ADD:
		options += get_disk_type_name_side(argv+options, argc-options, &image, &side);

		flags = BBCIM_DISK_INFO;
		for (i=2; i<strlen(optstring); i++) {
			switch (optstring[i]) {
			case 'r' : flags |=  BBCIM_REMOVE_FILE; break;
			case 'b' : flags &= ~BBCIM_DISK_INFO;   break;
			default  : bad_option = 1;
				break;
			}
		}
		/* Het volgende moet naar add_to_image() code (of NIET?). */
		fpdisk = fopen(disk, "rb");
		if (fpdisk == NULL) {
			image.attr->tracks = 80;
			bbcim_disk_new(&image, 1);
			printf(TEKST_NL("Nieuw diskbeeld gemaakt\n")
				   TEKST_ENG("New disk image made\n"));
		}
		else fclose(fpdisk);

		for (i=options; i<argc; i++) {
			if (!bbcim_disk_add_file(&image, side, argv[i], &file, flags, 1))
				printf("%s: added\n", argv[i]);
		}
		break;

	case DEL:
		options += get_disk_type_name_side(argv+options, argc-options, &image, &side);

		flags = 0;
		argc_check(argc, options, 1); /* at least one! */
		for (i=options; i<argc; i++) {
			bbcim_disk_del_file(&image, side, argv[i], flags, 1);
		}
		break;

	case CAT:
		options += get_disk_type_name_side(argv+options, argc-options, &image, &side);

		flags = BBCIM_SHOW_FILES;
		{
			int extract_given = 0;
			for (i=options; i<argc; i++) {
				file.attr->name = argv[i];
				bbcim_disk_cat_extract_crc(&image, side, &file, flags, 1);
				extract_given++;
			}
			if (extract_given == 0) {
				if (bbcim_debug) printf("DEBUG: extract all\n");
				bbcim_disk_cat_extract_crc(&image, side, NULL, flags, 1);
			}
		}
		break;

	case CRC:
		options += get_disk_type_name_side(argv+options, argc-options, &image, &side);

		flags = BBCIM_CALC_CRC | BBCIM_SHOW_FILES;
		for (i=options; i<argc; i++) {
			file.attr->name = argv[i];
			bbcim_disk_cat_extract_crc(&image, side, &file, flags, 1);
		}
		if (options == argc)
			bbcim_disk_cat_extract_crc(&image, side, NULL, flags, 1);
		break;

	case ARCHIVE_CLEANUP:
		argc_check(argc, options, 1); /* at least one! */
		for (i=options; i<argc; i++) {
			bbcim_archive_cleanup(argv[i], 1);
		}
		break;

	case ARCHIVE_CRC:
		argc_check(argc, options, 1); /* at least one! */
		for (i=options; i<argc; i++) {
			bbcim_archive_crc_check(argv[i], 1);
		}
		break;

	case DISKSIZE40:
		options += get_disk_type_name_side(argv+options, argc-options, &image, &side);
		bbcim_disk_expand(&image, 40*10, 1);
		break;

	case DISKSIZE80:
		options += get_disk_type_name_side(argv+options, argc-options, &image, &side);
		bbcim_disk_expand(&image, 80*10, 1);
		break;

	case MINIMISE:
		options += get_disk_type_name_side(argv+options, argc-options, &image, &side);
		bbcim_disk_minimise(&image, 1);
		break;

	case MAXIMISE:
		options += get_disk_type_name_side(argv+options, argc-options, &image, &side);
		bbcim_disk_expand(&image, 1023, 1);
		break;

	case NEW_DISK:
		options += get_disk_type_name_side(argv+options, argc-options, &image, &side);
		bbcim_disk_new(&image, 1);
		break;

	case BOOT:
	{	int bootoption = 0;
		options += get_disk_type_name_side(argv+options, argc-options, &image, &side);

		argc_check(argc, options, 1);
		if (!strcmp(argv[options], "none")) bootoption = BBCIM_DFS_BOOTOPTION_NONE;
		else if (!strcmp(argv[options], "EXEC")) bootoption = BBCIM_DFS_BOOTOPTION_EXEC;
		else if (!strcmp(argv[options], "RUN")) bootoption = BBCIM_DFS_BOOTOPTION_RUN;
		else if (!strcmp(argv[options], "LOAD")) bootoption = BBCIM_DFS_BOOTOPTION_LOAD;
		else { bad_option = 1; break; }

		bbcim_disk_set_boot(&image, side, bootoption, 1);
		break;
	}

	case ID:
	{	int type_side0;
		argc_check(argc, options, 1);
		bbcim_disk_identify(argv[options], &type_side0, 1);
	}
		break;

	default: bad_option = 1;
		break;
	}

	if (bad_option) {
		/* Don't do this again buddy! */
		fprintf(stderr,
				TEKST_NL("Foutieve optie %s\n")
				TEKST_ENG("Bad option %s\n"), argv[1]);
		use();
	}

	return bbcim_errno;
}
