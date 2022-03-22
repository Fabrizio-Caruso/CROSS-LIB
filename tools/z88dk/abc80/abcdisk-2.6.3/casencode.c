/*
 * Encode ABC80 cassette files
 *
 * Uses libsndfile to create the desired output format.
 */

#include "abcdisk.h"
#include <sndfile.h>
#include <math.h>
#include <ctype.h>

#define MAX_CHANNELS		32 /* Need to put a limit somewhere... */
static double fbuf[MAX_CHANNELS];  /* Single sample frame buffer */

/*
 * Cassette I/O
 */
struct modulator_state {
    int32_t parity;		/* Last carrier state (+1/-1) */
    uint32_t fraction;		/* Fraction of last sample left (out of 2^32) */
};

/* Cassette block data format */
struct cas_block {
    uint8_t leadin[32];         /* 0x00 */
    uint8_t sync[3];            /* 0x16 */
    uint8_t stx;                /* 0x02 */
    uint8_t blktype;            /* 0x00 for data, 0xff for filename */
    uint8_t blkno[2];           /* Block number (littleendian) */
    uint8_t data[253];          /* Actual data */
    uint8_t etx;                /* 0x03 */
    uint8_t csum[2];            /* Checksum */
    uint8_t leadout[1];		/* 0x00 */
};

/* Cassette block + metadata */
struct cas_block_info {
    struct cas_block blk;
    const uint8_t *data;	/* Input data to be written */
    const char *filename;	/* For error messages */
    unsigned char mangled_name[12];
    int block_nr;		/* Note: -1 is valid here */
    unsigned int data_blocks;
    unsigned int offset;	/* Offset in input data */
    unsigned int baudrate;
    unsigned int divisor;
    struct modulator_state ms;
};

static inline bool is_sndformat(int format)
{
    /* The major format must be != 0 */
    return format & SF_FORMAT_TYPEMASK;
}

/* Unencoded binary formats */
#define FORMAT_BINARY	1	/* Includes framing data */
#define FORMAT_BLOCK	2	/* Just the blocks */

static void version(void)
{
    fprintf(stderr, "abcdisk %s\n", VERSION);
}

static const char *_progname;

static void usage(int err)
{
    version();
    fprintf(stderr, "Usage: %s [options] inputfile[=outname]...\n"
	    "Options:\n"
	    "-o outputfile  write to a file instead of standard output\n"
	    "-s samplerate  sample rate of output file (default 11025)\n"
	    "-c channels    channels in output file (default 1)\n"
	    "-b baudrate    output baud rate (default %d, ABC800 can use %d)\n"
	    "-S             ABC80 baud rate (%d)\n"
	    "-F             ABC800 baud rate (%d)\n"
	    "-f format      output sound file format (default wav)\n"
	    "-f help        list available output formats\n"
	    "-C             equivalent to -f cas\n"
	    "-B             equivalent to -f blk\n"
	    "-v             print verbose status messages\n"
	    "-q             print no status messages\n"
	    "-V             print version and exit\n"
	    "-h             print this help text and exit\n"
	    , _progname,
	    CAS_BAUDRATE_ABC80, CAS_BAUDRATE_ABC800,
	    CAS_BAUDRATE_ABC80, CAS_BAUDRATE_ABC800);
    exit(err);
}

static const char *getstrarg(char ***argp)
{
    const char *arg;

    arg = *++(*argp);
    if (!arg)
	usage(1);
    return arg;
}

static unsigned long getnumarg(char ***argp)
{
    const char *arg = getstrarg(argp);
    char *ep;
    unsigned long rv;

    errno = 0;
    rv = strtoul(arg, &ep, 0);
    if (errno || *ep) {
	usage(1);
	return ULONG_MAX;
    }
    return rv;
}

static void list_formats(FILE *f)
{
    int fmt_count, i;

    sf_command(NULL, SFC_GET_FORMAT_MAJOR_COUNT, &fmt_count, sizeof fmt_count);

    fprintf(f, "%-7s %s\n", "cas", "unmodulated cassette data");
    fprintf(f, "%-7s %s\n", "blk", "cassette block data without framing");

    for (i = 0; i < fmt_count; i++) {
	SF_FORMAT_INFO sfi;
	sfi.format = i;
	sf_command(NULL, SFC_GET_FORMAT_MAJOR, &sfi, sizeof sfi);
	fprintf(f, "%-7s %s\n", sfi.extension, sfi.name);
    }
}

/* Pick the preferred subtype */
static int refine_format(SF_INFO *sfinfo)
{
    static const int priorities[] = {
	[SF_FORMAT_DPCM_16] = 1,
	[SF_FORMAT_PCM_16]  = 2,
	[SF_FORMAT_DPCM_8]  = 3,
	[SF_FORMAT_PCM_S8]  = 4,
	[SF_FORMAT_PCM_U8]  = 5,
	[SF_FORMAT_ULAW]    = 6,
	[SF_FORMAT_ALAW]    = 7,
	[SF_FORMAT_PCM_24]  = 8,
	[SF_FORMAT_PCM_32]  = 9,
	[SF_FORMAT_FLOAT]   = 10,
	[SF_FORMAT_DOUBLE]  = 11
    };
    int fmt_count, i;
    int best_format   = sfinfo->format;
    int best_priority = INT_MAX;

    if (sfinfo->format & SF_FORMAT_SUBMASK) {
	if (sf_format_check(sfinfo))
	    return 0;		/* Already have a valid subtype */
    }

    sf_command(NULL, SFC_GET_FORMAT_SUBTYPE_COUNT, &fmt_count, sizeof fmt_count);

    for (i = 0; i < fmt_count; i++) {
	SF_FORMAT_INFO sfi;
	int priority;

	sfi.format = i;
	sf_command(NULL, SFC_GET_FORMAT_SUBTYPE, &sfi, sizeof sfi);
	if (!sfi.format)
	    continue;
	sfinfo->format = (sfinfo->format & ~SF_FORMAT_SUBMASK) |
	    (sfi.format & SF_FORMAT_SUBMASK);
	if (!sf_format_check(sfinfo))
	    continue;		/* Not supported for this major format */

	priority = sfi.format + 100;

	if ((size_t)sfi.format < sizeof(priorities)/sizeof(priorities[0]) &&
	    priorities[sfi.format]) {
	    priority = priorities[sfi.format];
	}

	if (priority < best_priority) {
	    best_format   = sfinfo->format;
	    best_priority = priority;
	}
    }

    sfinfo->format = best_format;
    return 0;
}


static int getformatarg(char ***argp)
{
    const char *arg = getstrarg(argp);
    int fmt_count, i;

    if (!strcmp(arg, "cas") || !strcmp(arg, "bin") || !strcmp(arg, "binary"))
	return FORMAT_BINARY;

    if (!strcmp(arg, "blk") || !strcmp(arg, "block") || !strcmp(arg, "blocks"))
	return FORMAT_BLOCK;

    sf_command(NULL, SFC_GET_FORMAT_MAJOR_COUNT, &fmt_count, sizeof fmt_count);

    if (!arg[0] || !strcmp(arg, "help") || !strcmp(arg, "list")) {
	list_formats(stdout);
	exit(0);
    }

    for (i = 0; i < fmt_count; i++) {
	SF_FORMAT_INFO sfi;
	sfi.format = i;
	sf_command(NULL, SFC_GET_FORMAT_MAJOR, &sfi, sizeof sfi);
	if (!strcmp(arg, sfi.extension))
	    return sfi.format;
    }

    /* Not found, output list */
    fprintf(stderr, "%s: unknown output format: %s\n"
	    "Known formats are:\n",
	    _progname, arg);
    list_formats(stderr);
    exit(1);
}

/*
 * Fudge the baud rate so it seems to match what ABC800 produces;
 * returns the divisor byte to be put in the header.
 */
static unsigned int fudge_baudrate(unsigned int *baudrate)
{
	unsigned int divisor;

	divisor = DIV_ROUND(CAS_SIO_CLOCK, *baudrate);
	*baudrate = DIV_ROUND(CAS_SIO_CLOCK, divisor);
	return divisor == CAS_DIVISOR_ABC80 ? 0 : divisor;
}

static void cas_format_block(struct cas_block_info *cbi)
{
    const uint8_t *csumptr;
    uint16_t csum;
    int i;

    memset(cbi->blk.leadin, 0, sizeof cbi->blk.leadin);
    memset(cbi->blk.sync, 0x16, sizeof cbi->blk.sync);
    cbi->blk.stx      = 0x02;
    cbi->blk.blktype  = -(cbi->block_nr < 0);
    cbi->blk.blkno[0] = cbi->block_nr;
    cbi->blk.blkno[1] = cbi->block_nr >> 8;
    cbi->blk.etx      = 0x03;

    csumptr = &cbi->blk.blktype;
    csum = 0;
    for (i = 0; i < 257; i++)
        csum += *csumptr++;

    cbi->blk.csum[0] = csum;
    cbi->blk.csum[1] = csum >> 8;
}

static void cas_format_header(struct cas_block_info *cbi)
{
    cbi->ms.parity   = -1;
    cbi->ms.fraction =  0;
    cbi->block_nr = -1;
    memset(cbi->blk.data, 0, sizeof cbi->blk.data);
    memcpy(cbi->blk.data, cbi->mangled_name, 11);

    /* ABC800 puts a code for the divisor in the header */
    cbi->blk.data[12]  = cbi->divisor;

    /* ABC-klubben standard: block count encoded in the header */
    cbi->blk.data[251] = cbi->data_blocks;
    cbi->blk.data[252] = cbi->data_blocks >> 8;

    cas_format_block(cbi);
}

static bool is_stdio(const char *filename)
{
    return !filename || !filename[0] || (filename[0] == '-' && !filename[1]);
}

static SNDFILE *create_sndfile(const char *filename, SF_INFO *sfinfo)
{
    SNDFILE *sf = NULL;
    const char *errname;

    errno = 0;

    if (is_stdio(filename)) {
	filename = NULL;
	errname = "stdout";
    }

    if (is_sndformat(sfinfo->format)) {
	refine_format(sfinfo);
	if (!sf_format_check(sfinfo)) {
	    fprintf(stderr, "%s: %s: invalid parameters for data format\n",
		    _progname, errname);
	    exit(1);
	}
	if (!filename)
	    sf = sf_open_fd(fileno(stdout), SFM_WRITE, sfinfo, 0);
	else
	    sf = sf_open(filename, SFM_WRITE, sfinfo);
    } else {
	if (!filename) {
	    sf = (SNDFILE *)stdout;
	} else {
	    sf = (SNDFILE *)fopen(filename, "wb");
	}
    }

    if (!sf) {
	fprintf(stderr, "%s: %s: %s\n", _progname, errname,
		is_sndformat(sfinfo->format)
		? sf_strerror(NULL) : strerror(errno));
	exit(1);
    }

    return sf;
}

static FILE *open_input(char *filename, unsigned char mangled[12])
{
    FILE *f;
    char *outname = strrchr(filename, '\0');

    while (--outname >= filename) {
	if (*outname == '=') {
	    *outname++ = '\0';
	} else if (*outname != '.' && !isalnum(*outname)) {
	    outname++;
	    break;
	} else if (outname == filename) {
	    break;
	}
    }

    if (is_stdio(filename))
	filename = NULL;

    if (!*outname) {
	outname = filename;
	if (!outname) {
	    outname = "stdin";
	}
    }

    if (mangle_name(mangled, outname)) {
	fprintf(stderr, "%s: %s: invalid ABC output filename: %s\n",
		_progname, filename, outname);
	exit(1);
    }
    if (is_stdio(filename))
	filename = NULL;

    if (!filename)
	f = stdin;
    else
	f = fopen(filename, "rb");

    if (!f) {
	fprintf(stderr, "%s: %s: %s\n", _progname,
		filename ? filename : "stdin", strerror(errno));
	exit(1);
    }

    return f;
}

static int write_samples(SNDFILE *sf, SF_INFO *sfinfo, int64_t mdata,
			 unsigned int count)
{
    int channels = sfinfo->channels;
    int c;
    double fdata;

    assert(mdata >= -((int64_t)1 << 32) &&
	   mdata <=  ((int64_t)1 << 32));

    fdata = ldexp(mdata, -32);
    for (c = 0; c < channels; c++)
	fbuf[c] = fdata;

    while (count--)
	if (sf_writef_double(sf, fbuf, 1) != 1)
	    return -1;

    return 0;
}

/* Encode a block with FM modulation */
static int modulate(SNDFILE *sf, SF_INFO *sfinfo, struct cas_block_info *cbi)
{
    const uint8_t *p = (const uint8_t *)&cbi->blk;
    size_t bytes = sizeof cbi->blk;
    uint64_t sle;		/* samples/half bit * 2^32 */

    sle = (((uint64_t)sfinfo->samplerate << 31) +
	   ((uint64_t)cbi->baudrate << 30))
	/ cbi->baudrate;

    assert(sle >> 32);

    while (bytes--) {
	int bits = 8;
	uint8_t d = *p++;
	while (bits--) {
	    int h;
	    int bit = d & 1;
	    d >>= 1;

	    for (h = 1; h >= 0; h--) {
		int old_parity = cbi->ms.parity;
		uint32_t fraction = cbi->ms.fraction;
		uint32_t samples  = (sle + fraction) >> 32;
		int new_parity = (h | bit) ? -old_parity : old_parity;
		int64_t mdata;

		mdata = old_parity * (int64_t)fraction +
		  new_parity * (((int64_t)1 << 32) - fraction);
		write_samples(sf, sfinfo, mdata, 1);

		mdata = (int64_t)new_parity << 32;
		write_samples(sf, sfinfo, mdata, samples-1);

		cbi->ms.parity   = new_parity;
		cbi->ms.fraction = fraction + (uint32_t)sle;
	    }
	}
    }

    return 0;
}

/* Write out any remaining stuff to round out the file */
static int encode_remains(SNDFILE *sf, SF_INFO *sfinfo, struct cas_block_info *cbi)
{
    if (is_sndformat(sfinfo->format) && cbi->ms.fraction)
	write_samples(sf, sfinfo, (int64_t)cbi->ms.fraction * cbi->ms.parity, 1);

    return 0;
}

/* Encode or write a block */
static int encode_block(SNDFILE *sf, SF_INFO *sfinfo, struct cas_block_info *cbi)
{
    if (!is_sndformat(sfinfo->format)) {
	FILE *f = (FILE *)sf;
	size_t bytes = 0;
	const void *data = NULL;

	switch (sfinfo->format) {
	case FORMAT_BINARY:
	    data  = &cbi->blk;
	    bytes = sizeof cbi->blk;
	    break;
	case FORMAT_BLOCK:
	    data  = &cbi->blk.blktype;
	    bytes = 256;	/* blktype+blkno+data */
	    break;
	default:
	    break;
	}
	return fwrite(data, 1, bytes, f) != bytes || ferror(f);
    } else {
	return modulate(sf, sfinfo, cbi);
    }
}

static void set_title(SNDFILE *sf, struct cas_block_info *cbi)
{
    char unmangled_name[128];

    unmangle_filename(unmangled_name, cbi->mangled_name);
    sf_set_string(sf, SF_STR_TITLE, unmangled_name);
    sf_set_string(sf, SF_STR_COMMENT,
		  cbi->divisor ? "ABC800 cassette data" :
		  "ABC80 cassette data");
}

static int insert_silence(SNDFILE *sf, SF_INFO *sfinfo, int sec)
{
    return write_samples(sf, sfinfo, 0, sfinfo->samplerate * sec);
}

static int process_input_file(SNDFILE *sf, SF_INFO *sfinfo, char *filename,
			      unsigned int baudrate, bool first)
{
    FILE *in;
    struct cas_block_info cbi;
    int bytes, blocks;

    memset(&cbi, 0, sizeof cbi);
    in = open_input(filename, cbi.mangled_name);

    cbi.filename = filename;
    cbi.data = make_abcfile(in, &bytes); /* Also closes the input file */
    cbi.data_blocks = blocks = bytes / 253;

    if (is_sndformat(sfinfo->format)) {
	if (first) {
	    cbi.baudrate = baudrate;
	    cbi.divisor  = fudge_baudrate(&cbi.baudrate);
	    set_title(sf, &cbi);
	} else {
	    insert_silence(sf, sfinfo, 5);
	}
    }

    /* The header block is always encoded at ABC80 speed */
    cbi.baudrate = CAS_BAUDRATE_ABC80;
    cbi.divisor = fudge_baudrate(&cbi.baudrate);

    cas_format_header(&cbi);
    if (encode_block(sf, sfinfo, &cbi))
	return -1;

    /* Now we can switch to a higher speed */
    cbi.baudrate = baudrate;
    cbi.divisor  = fudge_baudrate(&cbi.baudrate);

    while (blocks--) {
	cbi.block_nr++;

	memcpy(cbi.blk.data, cbi.data + cbi.offset, 253);
	cas_format_block(&cbi);
	if (encode_block(sf, sfinfo, &cbi))
	    return -1;

	cbi.offset += 253;
    }

    encode_remains(sf, sfinfo, &cbi);
    return 0;
}

static void close_output(SNDFILE *sf, SF_INFO *sfinfo)
{
    if (is_sndformat(sfinfo->format)) {
	sf_close(sf);
    } else if ((FILE *)sf != stdout) {
	fclose((FILE *)sf);
    }
}

int main(int argc, char *argv[])
{
    SNDFILE *sf;
    unsigned int baudrate = CAS_BAUDRATE_ABC80;
    const char *outputfile = NULL;
    char *inputfile;
    const char *arg;
    bool first;
    char **argp = &argv[0];
    SF_INFO sfinfo = {
	.samplerate = 11025,
	.channels   = 1,
	.format     = SF_FORMAT_WAV
    };

    (void)argc;

    _progname = argv[0];

    setlocale(LC_ALL, "");

    while ((arg = *++argp) && arg[0] == '-' && arg[1]) {
	char opt;
	while ((opt = *++arg)) {
	    switch (opt) {
	    case 'f':
		sfinfo.format = getformatarg(&argp);
		break;
	    case 's':
		sfinfo.samplerate = getnumarg(&argp);
		break;
	    case 'c':
		sfinfo.channels = getnumarg(&argp);
		break;
	    case 'r':
		baudrate = getnumarg(&argp);
		break;
	    case 'F':
		baudrate = CAS_BAUDRATE_ABC800;
		break;
	    case 'S':
		baudrate = CAS_BAUDRATE_ABC80;
		break;
	    case 'C':
		sfinfo.format = FORMAT_BINARY; /* Unencoded output */
		break;
	    case 'B':
		sfinfo.format = FORMAT_BLOCK; /* Block data output */
		break;
	    case 'v':
		/* Reserved for verbose output */
		break;
	    case 'V':
		version();
		exit(0);
		break;
	    case 'h':
		usage(0);
		break;
	    case 'q':
		/* Reserved for quiet output */
		break;
	    case 'o':
		outputfile = getstrarg(&argp);
		break;
	    default:
		fprintf(stderr, "%s: unknown option: -%c\n", _progname, opt);
		usage(1);
		break;
	    }
	}
    }

    if (!*argp) {
	fprintf(stderr, "%s: no input files!\n", _progname);
	exit(1);
    }

    if (sfinfo.channels < 1 || sfinfo.channels > MAX_CHANNELS) {
	fprintf(stderr, "%s: %s: invalid number of channels: %d\n",
		_progname, is_stdio(outputfile) ? "stdout" : outputfile,
		sfinfo.channels);
	exit(1);
    }
    if (sfinfo.samplerate < (int)(4*baudrate)) {
	fprintf(stderr, "%s: %s: sample rate %d too low for baud rate %u\n",
		_progname, is_stdio(outputfile) ? "stdout" : outputfile,
		sfinfo.samplerate, baudrate);
	exit(1);
    }

    sf = create_sndfile(outputfile, &sfinfo);

    first = true;
    while ((inputfile = *argp++)) {
	process_input_file(sf, &sfinfo, inputfile, baudrate, first);
	first = false;
    }

    close_output(sf, &sfinfo);

    return 0;
}
