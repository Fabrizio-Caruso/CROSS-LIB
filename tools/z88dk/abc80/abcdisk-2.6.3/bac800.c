#include "abcdisk.h"
#include "bin2abc.h"
#include "asmsrc/bac800.h"
#include "asmsrc/bac800d.h"

/* This trailer simply means "this program has no long variable names */
static const uint8_t trailer[] = {
    0x00, 0x00, 0x00, 0x00, 0xff, 0xfb, 0xff
};

int writebac800(FILE *out, struct bindata *in)
{
    uint8_t *loader;
    int loaderlen, reloclen, relocoffset;
    int padding;
    const char *chain;

    if (!in->relocs) {
        fprintf(stderr, "%s: ABC800 .bac files must be relocatable\n",
                program_name);
        return -1;
    }

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

    /* These fields are common to both bac800 and bac800d */
    set16(loader + bac800_SYM_PRGSZ,
          loaderlen + reloclen + in->datalen + sizeof trailer);

    chain = in->chain;
    if (!chain[0])
	chain = "NUL:";

    embed_name(loader + bac800_SYM_b_chainstr, CHAIN_LEN, chain);

    if (in->driver) {
        set16(loader + bac800d_SYM_DATA_OFFSET,
              loaderlen + reloclen - bac800d_SYM__start);
        set16(loader + bac800d_SYM_COMSZ, in->datalen);
    } else {
	set16(loader + bac800_SYM_NEGDATAPTR,
	      -(loaderlen + reloclen - bac800_SYM__start));
    }

    /* The loader already includes the header and BASIC prefix */
    fwrite(loader, 1, loaderlen, out);
    fwrite(in->packed_relocs, 1, reloclen, out);
    fwrite(in->data, 1, in->datalen, out);
    fwrite(trailer, 1, sizeof trailer, out);

    /* Pad to the end of a 253-byte sector */
    padding = 252 - (loaderlen + reloclen + in->datalen +
		     sizeof trailer - 1) % 253;
    while (padding--)
        putc(0, out);

    return 0;
}
