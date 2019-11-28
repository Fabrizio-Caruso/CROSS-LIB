
#include "shared/_ansi_console.h"

extern int optie;

/* Prototypes: */
void use_option(void);
void show_version_copyright(void);
void show_help(void);
void use(void);
void show_options(void);


/* Functions: */
void show_help() {
	printf(
		TEKST_NL(
"\nbbcim, versie "BBCIM_VERSION", "BBCIM_COPYRIGHT" (" CONSOLE_GRAY "\http://wouter.bbcmicro.net/" CONSOLE_RESTORE ")\n"
"Optie overzicht:\n"
"  -c : toon bestanden in een diskbeeld\n"
"  -e : haal bestanden uit een diskbeeld\n"
"  -a : plaats bestanden in een diskbeeld\n"
"  -d : verwijder bestanden uit een diskbeeld\n"
"  -40|-80|-max : wijzig grootte van een Acorn DFS diskbeeld\n"
"  -min : minimaliseer een Acorn DFS diskbeeld\n"
"  -crc : bereken CRC's van bestanden in een diskbeeld\n"
"  -boot : wijzig boot-optie van de gegeven zijde van een diskbeeld\n"
"  -id : probeer een diskbeeld type te identificeren\n"
"  -inf_clean : archief bestanden bijwerken (naam en crc)\n"
"  OUD -inf_crc : controleer CRC's van archief bestanden\n"
"  -s : splits een tekst bestand in .inf bestanden\n"
"  -interss : combineer enkelzijdige diskbeelden\n"
"  -splitds : splits een dubbelzijdig diskbeeld\n"
"  -ddos_dfs : splits een enkelzijdig DDOS diskbeeld in Acorn DFS diskbeelden\n"
"  -w62_dfs : splits een watford 62-bestanden diskbeeld in Acorn DFS diskbeelden\n"
"  -help : geeft meer informatie over gebruik van een optie\n"
"  -H : toon deze help tekst\n"
"  -V : toon het bbcim versie nummer\n"
"Zie de 'man' pagina of  bbcim -help <optie>  voor nog meer informatie.\n"
			)

		TEKST_ENG(
"\nbbcim, version "BBCIM_VERSION", "BBCIM_COPYRIGHT" (" CONSOLE_GRAY "http://wouter.bbcmicro.net/" CONSOLE_RESTORE "\n"
"Overview of options:\n"
"  -c : show files in disk image\n"
"  -e : extract files from disk image\n"
"  -a : add files to disk image\n"
"  -d : delete files from a disk image\n"
"  -40|-80|-max : change the size of a DFS disk image\n"
"  -min : minimize a DFS disk image\n"
"  -crc : calculate CRC's of files in a disk image\n"
"  -inf_clean : cleanup archive file information (name and crc)\n"
"  OLD -inf_crc : check CRC's of archive files\n"
"  -s : split a text file with file attributes into .inf files\n"
"  -interss : combine 2 single sided disimages\n"
"  -splitds : split a double sided disk image\n"
"  -ddos_dfs : split single sided DDOS disk image into Acorn DFS disk images\n"
"  -w62_dfs : split a watford 62-file disk image into Acorn DFS disk images\n"
"  -boot : set boot option of the given side of a disk image\n"
"  -id : try to identify the disk image type\n"
"  -help : gives more information on how to use an option\n"
"  -H : show this help text\n"
"  -V : show bbcim version number\n"
"See the 'man' page or  bbcim -help <option>  for even more information.\n"
			)
		);
}


void use_option() {
	printf("Use: bbcim ");
	switch(optie) {
	case VERSIE:
		printf("-V\n");
		break;

	case HELP:
		printf("-H\n");
		break;

    case HELP2:
		printf("-help <option>\n");
		break;

	case INTERSS:
		printf("-interss <disk1> <disk2> <new disk>\n");
		break;

	case SPLITDS:
		printf("-splitds <disk>\n");
		break;

	case DDOS_DFS:
		printf("-ddos_dfs <disk>\n");
		break;

	case W62_DFS:
		printf("-w62_dfs <disk>\n");
		break;

	case SPLIT_CAT:
		printf("-s[@] <text file>\n");
		break;

	case EXTRACT:
		printf("-e[s|d,@,b,r] <disk> [<file 1>...<file n>]\n");
		break;

	case ADD:
		printf("-a[b,r] [<diskoptions>] <disk> [-side <side>] {list of files}\n");
		break;

	case DEL:
		printf("bbcim -d <disk> [-side <side>] {list of files}\n");
		break;

	case CAT:
		printf("-c <disk> [-side <side>]\n");
		break;

	case CRC:
		printf("-crc <disk> [-side <side>]\n");
		break;

	case ARCHIVE_CLEANUP:
		printf("bbcim -inf_clean {list}\n");
		break;

	case ARCHIVE_CRC:
		printf("-inf_crc {list}\n");
		break;

	case DISKSIZE40:
		printf("-40 <disk>\n");
		break;

	case DISKSIZE80:
		printf("-80 <disk>\n");
		break;

	case MINIMISE:
		printf("-min <disk>\n");
		break;

	case MAXIMISE:
		printf("-max <disk>\n");
		break;

	case NEW_DISK:
		printf("-new <disk>\n");
		break;

	case BOOT:
		printf("-boot <disk> {NONE|LOAD|RUN|EXEC}\n");
		break;

	case ID:
		printf("-id <disk>\n");
		break;

	default:
		printf("Error\n");
		break;
	}
}


void use() {
	show_version_copyright();
	show_options();
}


void show_version_copyright() {
	printf("\n"
		TEKST_NL("bbcim, versie "BBCIM_VERSION", "BBCIM_COPYRIGHT" (" CONSOLE_GRAY "\http://wouter.bbcmicro.net/" CONSOLE_RESTORE ")\n")
		TEKST_ENG("bbcim, version "BBCIM_VERSION", "BBCIM_COPYRIGHT" (" CONSOLE_GRAY "http://wouter.bbcmicro.net/" CONSOLE_RESTORE ")\n")
		);
}


void show_options() {
	printf(
		TEKST_NL("Opties:\n")
		TEKST_ENG("Options:\n")
		"        -e[s|d,@,b,r] -a[b,r] -d -c -crc -min -40 -80 -max -id -boot\n"
		"        -inf_clean -inf_crc -s[@]\n"
		"        -splitds -interss -ddos_dfs -w62_dfs\n"
		"        -V -H -help\n"
		TEKST_NL("Probeer opties -H of -help <optie> voor meer informatie\n")
		TEKST_ENG("For more information try option -H or -help <option>\n")
		);
}
