/*
 * bin2abc.c
 *
 * Convert a binary file either in binary or ith format to an ABC80/800
 * .bac, .abs, or .bas (POKE) file.
 */

#include "abcdisk.h"
#include "bin2abc.h"

FILE *open_file(const char *filename, const char *mode)
{
    FILE *f;

    if (is_stdio(filename)) {
        f = (mode[0] == 'w') ? stdout : stdin;
    } else {
        f = fopen(filename, mode);
        if (!f) {
            fprintf(stderr, "%s: %s: %s\n", program_name, filename,
                    strerror(errno));
            return NULL;
        }
    }

    clearerr(f);
    return f;
}

int close_file(FILE * f)
{
    int err;

    if (!f)
	return 0;

    err = ferror(f);

    if (f != stdin && f != stdout && f != stderr)
        err |= fclose(f);

    return err;
}

size_t read_file(void *buf, size_t buflen, const char *filename)
{
    FILE *f;
    size_t n;

    f = open_file(filename, "rb");
    if (!f)
        return 0;

    n = fread(buf, 1, buflen, f);

    close_file(f);
    return n;
}

static int read_binary(struct bindata *in, const char *filename,
		       uint16_t offset)
{
    size_t i, n;

    n = read_file(in->data + offset, MAX_ADDR - offset, filename);
    if (!n)
        return -1;

    for (i = offset; i < offset + n; i++)
        in->isset[i] = true;

    return offset + n;
}

#define CHAIN_LEN 16
static const char no_chain[] = ":CHAINFILE:HERE:";

/*
 * Search for a autochain filename in the header of a .bac file
 * and change it if we can find one.
 */
static void patch_chain(const char *filename, const char *chain)
{
    uint8_t block[253];
    FILE *f;
    const uint8_t *searchfor;
    static const uint8_t search80[4]  = {0xba, 0xcb, '"', CHAIN_LEN};
    static const uint8_t search800[4] = {0x86, 0x0b, 0xcb, CHAIN_LEN};
    uint8_t *p, endbyte;
    int limit, i;

    errno = 0;

    f = open_file(filename, "r+b");
    if (!f)
	return;

    /* Should definitely be in the first file block */
    if (fread(block, 1, sizeof block, f) != sizeof block)
	goto failure;

    if ((block[0] & 0xf0) != 0x80) {
	/* Definitely not a .bac file */
	goto failure;
    }

    if (block[0] & 0x08) {
	/* Looks like an ABC800 .bac file */
	p = block + 0x14;
	limit = sizeof block - 0x14;
	searchfor = search800;
	endbyte = 0xb9;
	if (!chain[0]) {
	    /* The wipeout name is NUL: on ABC800, not an empty string */
	    chain = "NUL:";
	}
    } else {
	/* Looks like an ABC80 .bac file */
	p = block + 4;
	limit = block[1] - 3 - 1; /* 3 for line header, 1 for final CR */
	searchfor = search80;
	endbyte = 0xbb;
    }

    /* Point p to the actual string */
    p += 4;

    /* The subtract is because we must able to fit the string+prefix+endbyte */
    for (limit -= CHAIN_LEN+5; limit >= 0; p++, limit--) {
	if (memcmp(p-4, searchfor, 4) || p[CHAIN_LEN] != endbyte)
	    continue;

	/*
	 * Looks plausible. Now make sure the field looks like an
	 * embedded filename field.
	 */
	for (i = 0; i < CHAIN_LEN; i++) {
	    uint8_t c = p[i];
	    if (c != '\r' && !(c == '.') && !(c == ':') &&
		!(c >= '0' && c <= '9') &&
		!(c >= 'A' && c < '_'))
		break;		/* Not possible */
	}

	if (i == CHAIN_LEN)
	    break;		/* Found it! */
    }

    if (limit < 0)
	goto failure;

    /* We got it! */
    embed_name(p, CHAIN_LEN, chain);
    fseek(f, p - block, SEEK_SET);
    if ((fwrite(p, 1, CHAIN_LEN, f) != CHAIN_LEN) || fflush(f))
	goto failure;

    goto success;

failure:
    fprintf(stderr, "%s: %s: %s\n", program_name, filename,
	    errno ? strerror(errno)
	    : "unable to find chain file name to patch");

success:
    fclose(f);
    return;
}

const char *program_name;

static no_return usage(void)
{
    fprintf(stderr, "abcdisk %s\n", VERSION);
    fprintf(stderr, "Type %s --help for usage instructions\n", program_name);
    exit(1);
}

/* Helper functions that error out on a missing argument */
static char *short_arg(char opt, char *arg)
{
    if (!arg) {
        fprintf(stderr, "%s: the -%c option requires an argument\n",
                program_name, opt);
        usage();
    }
    return arg;
}

static char *long_arg(const char *opt, char *arg)
{
    if (!arg) {
        fprintf(stderr, "%s: the --%s option requires an argument\n",
                program_name, opt);
        usage();
    }
    return arg;
}

#define SHORT_ARG()	short_arg(optchr, *option++)
#define LONG_ARG()	long_arg(optstr,  *option++)

static unsigned long intarg(const char *arg)
{
    char *ep;
    unsigned long v;

    v = strtoul(arg, &ep, 0);
    if (!ep[0])
        return v;

    fprintf(stderr, "%s: invalid integer argument: %s\n", program_name, arg);
    usage();
}

/*
 * If filename is non-NULL return filename, otherwise either
 * no_chain or "" depending on "driver"
 */
static const char *chainfile(const char *filename, bool driver)
{
    if (filename)
	return filename;

    return driver ? "" : no_chain;
}

static no_return help(void)
{
    printf("abcdisk %s\n", VERSION);
    printf("Usage: %s [options...] inputfiles...\n"
           "Options:\n"
           "  -1, --bac80       generate a .bac file for ABC80\n"
           "  -2, --bac800      generate a .bac file for ABC800\n"
	   "  -t, --bas         generate a .bas file\n"
	   "      --firstline # first line number in .bas file\n"
	   "      --subroutine  .bas file ends in RETURN\n"
           "  -a, --abs         generate an .abs file\n"
           "  -o, --output ...  send output to a file instead of stdout\n"
           "  -r, --reloc ...   read a relocation file (.reloc from z80asm)\n"
           "  -b, --binary      input files are binary\n"
           "  -i, --ihex        input files are in Intel hex format\n"
           "  -d, --driver      generate .bac files suitable for device drivers\n"
           "  -D, --app         generate .bac files suitable for applications\n"
	   "  -c, --chain       run another program after termination\n"
	   "  -n, --new         erase program after execution\n"
	   "  -C, --no-chain    do not chain another program, normal END\n"
	   "  -p, --patch       patch in a new chain target in an existing file\n"
           "  -s, --origin #    specify the load address for a binary file\n"
           "  -e, --entry #     specify the entry point of the program\n"
           "  -z, --bss #       pad the output with # bytes of zero\n"
           "  -h, --help        show this help text\n"
	   "      --version     print the program version\n"
	   , program_name);
    exit(0);
}

enum output {
    OUT_BAC80,
    OUT_BAC800,
    OUT_ABS,
    OUT_BAS
};

int main(int argc, char *argv[])
{
    bool ihex = false;
    char **option, *optstr, optchr;
    enum output output = OUT_BAC80;
    int bss = 0;
    const char *outputfile = NULL;
    bool patch_mode = false;
    FILE *out;
    int nextloadaddr;
    int firstaddr, lastaddr, i;
    int err = 0;
    struct bindata *in;

    program_name = argv[0];
    (void)argc;

    in = calloc(sizeof *in, 1);
    in->entrypoint = -1;
    in->firstline = 1;
    in->subroutine = false;
    in->driver = false;
    in->origin = -1;
    nextloadaddr = 0;

    /* Aren't modern computers nice? */
    in->data = calloc(MAX_ADDR, 1);
    in->isset = calloc(MAX_ADDR, sizeof(bool));

    option = &argv[1];
    while ((optstr = *option++) != NULL) {
        if (optstr[0] == '-') {
            optstr++;
            optchr = *optstr++;

            if (optchr == '-') {
                if (!strcmp(optstr, "abc80") || !strcmp(optstr, "bac80")) {
                    output = OUT_BAC80;
                } else if (!strcmp(optstr, "abc800") ||
                           !strcmp(optstr, "bac800")) {
                    output = OUT_BAC800;
                } else if (!strcmp(optstr, "abs")) {
                    output = OUT_ABS;
		} else if (!strcmp(optstr, "bas")) {
		    output = OUT_BAS;
                } else if (!strcmp(optstr, "reloc")) {
		    read_reloc_file(in, LONG_ARG());
                } else if (!strcmp(optstr, "ihex") ||
                           !strcmp(optstr, "ith") ||
                           !strcmp(optstr, "intel") || !strcmp(optstr, "hex")) {
                    ihex = true;
                } else if (!strcmp(optstr, "binary")) {
                    ihex = false;
                } else if (!strcmp(optstr, "entry")) {
                    in->entrypoint = intarg(LONG_ARG());
                } else if (!strcmp(optstr, "driver") ||
                           !strcmp(optstr, "no-app")) {
                    in->driver = true;
                } else if (!strcmp(optstr, "app") ||
                           !strcmp(optstr, "no-driver")) {
                    in->driver = false;
		} else if (!strcmp(optstr, "chain")) {
		    in->chain = LONG_ARG();
		} else if (!strcmp(optstr, "new")) {
		    in->chain = "";
		} else if (!strcmp(optstr, "no-chain") ||
			   !strcmp(optstr, "end")) {
		    in->chain = no_chain;
		} else if (!strcmp(optstr, "patch")) {
		    patch_mode = true;
                } else if (!strcmp(optstr, "output")) {
                    outputfile = LONG_ARG();
                } else if (!strcmp(optstr, "start") ||
                           !strcmp(optstr, "origin")) {
		    nextloadaddr = intarg(LONG_ARG());
		    if (in->origin == -1 || nextloadaddr < in->origin)
			in->origin = nextloadaddr;
                } else if (!strcmp(optstr, "entry") ||
                           !strcmp(optstr, "entrypoint")) {
                    in->entrypoint = intarg(LONG_ARG());
                } else if (!strcmp(optstr, "bss") || !strcmp(optstr, "zero")) {
                    bss += intarg(LONG_ARG());
		} else if (!strcmp(optstr, "firstline")) {
		    in->firstline = intarg(LONG_ARG());
		} else if (!strcmp(optstr, "subroutine")) {
		    in->subroutine = true;
		} else if (!strcmp(optstr, "version")) {
		    printf("abcdisk %s\n", VERSION);
		    exit(0);
                } else if (!strcmp(optstr, "help")) {
                    help();
                } else {
                    fprintf(stderr, "%s: unknown option --%s\n",
                            program_name, optstr);
                    usage();
                }
            } else {
                /* Short option */
                while (optchr) {
                    switch (optchr) {
                    case '1':  /* For "BASIC I" */
                        output = OUT_BAC80;
                        break;
                    case '2':  /* For "BASIC II" */
                        output = OUT_BAC800;
                        break;
                    case 'a':
                        output = OUT_ABS;
                        break;
		    case 't':
			output = OUT_BAS;
			break;
                    case 'i':
                        ihex = true;
                        break;
                    case 'b':
                        ihex = false;
                        break;
                    case 'r':
                        read_reloc_file(in, SHORT_ARG());
                        break;
                    case 'e':
                        in->entrypoint = intarg(SHORT_ARG());
                        break;
                    case 's':
                        nextloadaddr = intarg(SHORT_ARG());
			if (in->origin == -1 || nextloadaddr < in->origin)
			    in->origin = nextloadaddr;
                        break;
                    case 'o':
                        outputfile = SHORT_ARG();
                        break;
                    case 'd':
                        in->driver = true;
                        break;
                    case 'D':
                        in->driver = false;
                        break;
		    case 'c':
			in->chain = SHORT_ARG();
			break;
		    case 'n':
			in->chain = "";
			break;
		    case 'C':
			in->chain = no_chain;
			break;
		    case 'p':
			patch_mode = true;
			break;
                    case 'z':
                        bss += intarg(SHORT_ARG());
                        break;
                    case 'h':
                        help();
                        break;
                    default:
                        fprintf(stderr, "%s: unknown option -%c\n",
                                program_name, optchr);
                        usage();
                        break;
                    }
                    optchr = *optstr++;
                }
            }
        } else {
            /* Not an option, input file */
	    if (patch_mode) {
		patch_chain(optstr, chainfile(in->chain, in->driver));
	    } else {
		/* Read input file */
		if (ihex)
		    nextloadaddr = readihex(in, optstr);
		else
		    nextloadaddr = read_binary(in, optstr, nextloadaddr);
	    }
        }
    }

    if (patch_mode)
	return 0;

    in->chain = chainfile(in->chain, in->driver);

    /* Find the first and last addresses in the data array */
    firstaddr = -1;
    lastaddr = -1;
    for (i = 0; i < MAX_ADDR; i++) {
        if (in->isset[i]) {
            lastaddr = i;
            if (firstaddr < 0)
                firstaddr = i;
        }
    }

    if (firstaddr < 0 && !bss) {
        fprintf(stderr, "%s: no input data!\n", program_name);
	usage();
    }

    if (in->origin == -1 || firstaddr < in->origin)
	in->origin = firstaddr;

    if (in->entrypoint == -1)
	in->entrypoint = in->origin;

    /* Add a zero-filled section after the last data */
    if (bss) {
        uint8_t *dp = in->data + lastaddr;
        bool *ip = in->isset + lastaddr;

        lastaddr += bss;

        memset(dp, 0, bss);
        while (bss--)
            *ip++ = true;
    }

    in->datalen = lastaddr - in->origin + 1;

    out = open_file(outputfile, "wb");
    if (!out) {
        fprintf(stderr, "%s: %s: %s\n", program_name, outputfile,
                strerror(errno));
        exit(1);
    }

    switch (output) {
    case OUT_BAC80:
        err = writebac80(out, in);
        break;

    case OUT_BAC800:
        err = writebac800(out, in);
        break;

    case OUT_ABS:
	err = writeabs(out, in);
        break;

    case OUT_BAS:
	err = writebas(out, in);
	break;
    }

    err |= close_file(out);

    if (err && !is_stdio(outputfile)) {
	if (errno)
	    fprintf(stderr, "%s: %s: %s\n", program_name, outputfile,
		    strerror(errno));
        remove(outputfile);
    }

    return !!err;
}
