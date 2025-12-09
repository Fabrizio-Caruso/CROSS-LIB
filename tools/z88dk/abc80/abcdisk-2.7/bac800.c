#include "abcdisk.h"
#include "bin2abc.h"
#include "asmsrc/bac800.h"
#include "asmsrc/bac800d.h"
#include "asmsrc/bac800a.h"

/* This trailer simply means "this program has no long variable names */
static const uint8_t trailer[] = {
    0x00, 0x00, 0x00, 0x00, 0xff, 0xfb, 0xff
};

int writebac800(FILE *out, struct bindata *in)
{
    uint8_t *loader;
    int loaderlen, reloclen;
    int padding;
    const char *chain;

    if (!in->relocs && !in->origin) {
	/*
	 * Make this a warning, because right now we can't tell the case
	 * where there zero relocations...
	 */
        fprintf(stderr,
		"%s: warning: ABC800 .bac files should be relocatable\n",
                program_name);
    }

    if (in->origin) {
	loader = bac800a_bin;
        loaderlen = sizeof bac800a_bin;
	reloclen = 0;
	set16(loader + bac800a_SYM_ENTRY, in->entrypoint);
        set16(loader + bac800a_SYM_ADDRESS, in->origin);
	set16(loader + bac800a_SYM_LENGTH, in->datalen);
    } else {
	int relocoffset;
	
	if (in->driver) {
	    loader = bac800d_bin;
	    loaderlen = sizeof bac800d_bin;
	    relocoffset = 0;
	    set16(loader + bac800d_SYM_ENTRY, in->entrypoint);
	} else {
	    loader = bac800_bin;
	    loaderlen = sizeof bac800_bin;
	    relocoffset = bac800_SYM__end - bac800_SYM_LAST_RELOC;
	    set16(loader + bac800_SYM_ENTRY, in->entrypoint);
	}

	reloclen = pack_relocs(in, relocoffset, !in->driver);
	if (reloclen < 0)
	    return -1;

	if (in->driver) {
	    set16(loader + bac800d_SYM_DATA_OFFSET,
		  loaderlen + reloclen - bac800d_SYM__start);
	    set16(loader + bac800d_SYM_COMSZ, in->datalen);
	} else {
	    set16(loader + bac800_SYM_NEGDATAPTR,
		  -(loaderlen + reloclen - bac800_SYM__start));
	}
    }

    /* These fields are common to all */
    set16(loader + bac800_SYM_PRGSZ,
	  loaderlen + reloclen + in->datalen + sizeof trailer);

    chain = in->chain;
    if (!chain[0])
	chain = "NUL:";

    embed_name(loader + bac800_SYM_b_chainstr, CHAIN_LEN, chain);

    /* The loader already includes the header and BASIC prefix */
    fwrite(loader, 1, loaderlen, out);
    if (reloclen)
	fwrite(in->packed_relocs, 1, reloclen, out);
    fwrite(in->data + in->origin, 1, in->datalen, out);
    fwrite(trailer, 1, sizeof trailer, out);

    /* Pad to the end of a 253-byte sector */
    padding = 252 - (loaderlen + reloclen + in->datalen +
		     sizeof trailer - 1) % 253;
    while (padding--)
        putc(0, out);

    return 0;
}
