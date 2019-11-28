/* Prototypes: */
void help2_diskoptions(void);
void help2_bootoption(void);
void help2(char *optstring);


/* Functions: */
void help2_diskoptions() {
	printf("\n"
		   " <disk options>: [-type <DFS | W62 | DDOS | ADFS | HDFS>] [-bytes <no>]  [-sectors <no>] [-tracks <no>] [-sides <no>]\n");

}

void help2_bootoption() {
	printf("\n <boot option>: [ none | LOAD | RUN | EXEC ]\n");
}

void help2(char *optstring) {
	int optie;
	int i;

	if (optstring[0] == '-') optstring = optstring+1;

	optie = -1;

	for (i=0; i<_NUM_TYPES_; i++) {
		if (!strcmp(optstring, optie_strings[i])) {
			optie = i;
			break;
		}
	}

	if (optie == -1) {
		fprintf(stderr, 
				TEKST_NL("Er is alleen hulp beschikbaar voor:\n")
				TEKST_ENG("Help can only be provided for:\n")
			);
		show_options();
		exit(1);
	}

	switch (optie) {
	case HELP2:
		printf(
			TEKST_NL(" bbcim -help <optie>:\n"
					 "              Geeft een  uittreksel  uit de man  pagina  over het\n"
					 "              gebruik van optie <optie>.\n")
			TEKST_ENG(" bbcim -help <option>:\n"
					  "              Gives an extract of the man page pertaining to option <option>\n")
				);
		break;
	case VERSIE:
		printf(" bbcim -V:\n"
"              print bbcim version.\n"
			);
		break;
	case HELP:
		printf(" bbcim -H:\n"
"              gives general help (command overview)."
"\n"
			);
		break;
	case INTERSS:
		printf(
" bbcim -interss  [<diskoptions  0>]  <disk0>  [<diskoptions   1>]\n"
"       <disk1> <disk2>\n"
"	      interleaves  two single  sided disk  images  <disk0> and\n"
"	      <disk1> to <disk2>.\n"
			);
		break;
	case SPLITDS:
		printf(
" bbcim -splitds [<diskoptions>] <disk>\n"
"	      splits an interleaved disk  image <disk> to <disk>.0 and\n"
"	      <disk>.1.\n"
			);
		break;
	case DDOS_DFS:
		printf(
" bbcim -ddos_dfs [<diskoptions>] <disk>\n"
"	      splits a  single sided DDOS disk image  into several DFS\n"
"	      disk images.  Eg. myfiles > myfiles.A,..., myfiles.H.\n"
			);
		break;
	case W62_DFS:
		printf(
" bbcim -w62_dfs [<diskoptions>] <disk>\n"
"	      splits a  single sided watford  62 file disk  image into\n"
"	      two DFS disk images (one image if there are less than 32\n"
"	      files on the image) <disk>.1 and <disk>.2.\n"
			);
		break;
	case SPLIT_CAT:
		printf(
" bbcim -s[@] <file>\n"
"	      splits  a text  file with  file  information (catalogue)\n"
"	      into .inf  files. Lines not  starting with a  BBC (Acorn\n"
"	      DFS) file name (i.e.  dir.name) are skipped. This can be\n"
"	      used  for example to  make archive  files from  an xbeeb\n"
"	      directory file (__CATALOG__).\n"
"\n"
"	      In DFS  use, one  is usually in  directoy '$'  (the root\n"
"	      dir), and  the current directory is  not displayed. This\n"
"	      means, it's  useful to remove  the root dir '$'  in host\n"
"	      file names. If you don't want this, use option '@' which\n"
"	      keeps the root directory $.\n"
			);
		break;
	case EXTRACT:
		printf(
" bbcim -e[s|d,@,b,r]  [<diskoptions>]  <disk>  [-side   <side>]\n"
"       [{list}]\n"
"	      extracts files. Without d,s or @ the option, -e extracts\n"
"	      files from a disk  image 'imagefile' with filenames such\n"
"	      as  'imagefile.D.DATA'  but  acorn  DFS  root  dirs  are\n"
"	      removed,       so      '$.!BOOT'       is      extracted\n"
"	      to  'imagefile.!BOOT'. With every  'bare' file,  an info\n"
"	      file is  created containing in particular  the load- and\n"
"	      execution  addresses (i.e.   the  archive format).   The\n"
"	      suboptions produce the following results:\n"
"\n"
"	      d extracts the files in a directory named after the disk\n"
"	      image (appending \".d\"): eg. 'imagefile.d/$.ELITE'.\n"
"\n"
"	      s gives short filenames  (no leading disk image name nor\n"
"	      is a directory created to  put the files in).  Note that\n"
"	      d and s are mutually exclusive.\n"
"\n"
"	      @  keeps the  Acorn  DFS root  directory  $, so  $.ELITE\n"
"	      doesn't become ELITE.\n"
"\n"
"	      b only  the bare  files are created,  and no  info files\n"
"	      (with file attributes).\n"
"\n"
"	      r NOT  IMPLEMENTED: remove the extracted  files from the\n"
"	      disk image\n"
"\n"
"	      When a  list of file names  is given, only  the files in\n"
"	      the list are extracted from the disk image.\n"
			);
		help2_diskoptions();
		break;
	case ADD:
		printf(
" bbcim -a[b,r] [<diskoptions>] <disk> [-side <side>]\n"
"       {list}\n"
"	      add  files  (in  archive  format)  to  the  disk  image:\n"
"	      Automatically skips  '.inf' files  so bbcim -a  <disk> *\n"
"	      (on unix)  gives no problems.  If the  disk image didn't\n"
"	      exist a new one will be created.  The option 'r' removes\n"
"	      the archive  files after adding them to  the disk image.\n"
"	      The  option 'b'  adds files  without .inf  file (setting\n"
"	      load/exec addresses to 0).\n"
			);
		help2_diskoptions();
		break;
	case DEL:
		printf(
" bbcim -d  [<diskoptions>]  <disk>  [-side  <side>]\n"
"       {list}\n"
"	      delete  the list of  files from  the disk  image.  NOTE:\n"
"	      files without a BBC (Acorn DFS) directory are assumed to\n"
"	      have the  root directory $,  eg. ELITE is assumed  to be\n"
"	      $.ELITE\n"
			);
		help2_diskoptions();
		break;
	case CAT:
		printf(
" bbcim -c[R] [<diskoptions>] <disk> [-side <side>]\n"
"	      show catalogue  (files in image). R  is for hierarchical\n"
"	      filing system images, recursively showing all files from\n"
"	      the root or given directory.\n"
			);
		help2_diskoptions();
		break;
	case CRC:
		printf(
" bbcim -crc [<diskoptions>] <disk> [-side <side>]\n"
"	      calculates crc's for all the files in the disk image.\n"
			);
		break;

	case NEW_DISK:
		printf(
" bbcim -new [<diskoptions>] <disk>\n"
"	      makes  a new  (empty)  disk image  (for  Acorn DFS:  512\n"
"	      bytes, boot option EXEC).\n"
			);
		break;

	case ARCHIVE_CLEANUP:
		printf(
" bbcim -inf_clean {list}\n"
"	      renames  archive/info  files   to  standard  format  (as\n"
"	      produced  with '#'  in -e  (XXX update  this as  '#' was\n"
"	      removed XXX)), add's crc's if not found, checks crc's if\n"
"	      found.\n"
			);
		break;
	case ARCHIVE_CRC:
		printf(
" bbcim -inf_crc {list}"
"	      check crc's of the given archive files.\n"
			);
		break;
	case DISKSIZE40:
		printf(
" bbcim -40 [<diskoptions>] <disk>\n"
"	      expand a  disk image  to 40 tracks  (if current  size is\n"
"	      smaller than  40 tracks).   This changes the  disk image\n"
"	      size, but  more importantly  also the DFS  filing system\n"
"	      size  (the  number with  the  number  of sectors  that's\n"
"	      stored  in the disk  image).  So  shrinking an  80 track\n"
"	      image to 40 track is  possible by first using -min, then\n"
"	      -40 (if the files fit on 40 tracks).\n"
			);
		break;
	case DISKSIZE80:
		printf(
" bbcim -80 [<diskoptions>] <disk>\n"
"	      expand a  disk image  to 80 tracks  (if current  size is\n"
"	      smaller than  80 tracks).   This changes the  disk image\n"
"	      size, but  more importantly  also the DFS  filing system\n"
"	      size (see above).\n"
			);
		break;
	case MINIMISE:
		printf(
" bbcim -min [<diskoptions>] <disk>\n"
"	      minimise a disk image (equal to *COMPACT and cutting off\n"
"	      after  the last  used sector). In  case of double  sided\n"
"         images, both sides are minimised.\n"
			);
		break;
	case MAXIMISE:
		printf(
" bbcim -max [<diskoptions>] <disk>\n"
"	      expand a single or double sided DFS disk image to almost\n"
"	      256K  per side. This  changes the  disk image  size, but\n"
"	      more importantly  also the  DFS filing system  size (see\n"
"	      above).\n"
			);
		break;
	case BOOT:
		printf(
" bbcim -boot [<diskoptions>] <disk> [-side <side>] <boot option>\n"
"	      Sets the boot option of the disk image: none, LOAD, RUN,\n"
"	      EXEC.\n"
			);
		help2_diskoptions();
		help2_bootoption();
		break;
	case ID:
		printf(
" bbcim -id <disk>\n"
"              tries to identify a disk image (type/sides).\n"
			);
		break;
	default:
		fprintf(stderr, "hmm, shouldn't get here\n");
		exit(1);
		break;
	}
}
