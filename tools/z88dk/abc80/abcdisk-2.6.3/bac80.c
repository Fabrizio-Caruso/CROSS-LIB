#include "abcdisk.h"
#include "bin2abc.h"
#include "asmsrc/bac80.h"
#include "asmsrc/bac80r.h"

#define BLKSIZE	253

struct basic_block {
    FILE *out;
    int bytes;                  /* Bytes in buffer */
    int outbytes;               /* Bytes already written to file */
    uint16_t lineno;
    uint8_t *stringptr;        /* Accumulating string data, pointer to len */
    uint8_t data[BLKSIZE];
};

#define LINEHDR 3		/* Bytes for the BASIC line header */
#define LINEEND 1		/* Trailing overhead for BASIC line */
#define STRHDR  (LINEHDR+3)	/* Leading overhead bytes for string data */
#define STREND  (LINEEND+1)	/* Trailing overhead bytes for string data */
#define BLKFREE (BLKSIZE-1)	/* Usable bytes in a block (1 byte for EOB) */

/* Finalize and output a data block */
static void writeblock(struct basic_block *blk, bool final)
{
    int bytes = blk->bytes;

    if (!bytes && !final)
        return;

    assert(bytes < BLKSIZE);

    blk->data[bytes++] = final;
    memset(blk->data + bytes, 0, BLKSIZE - bytes);
    fwrite(blk->data, 1, BLKSIZE, blk->out);

    blk->outbytes += BLKSIZE;
    blk->bytes = 0;
    blk->stringptr = NULL;
}

/*
 * Close a BASIC string being emitted.
 */
static void closestring(struct basic_block *blk)
{
    uint8_t *p = blk->data + blk->bytes;

    if (!blk->stringptr)
        return;

    *p++ = 0xbb;		/* End string expression */
    *p++ = 0x0d;                /* Terminating CR */

    blk->bytes += STREND;
    blk->stringptr = NULL;
}

/*
 * Emit data encapsulated in BASIC strings.  Multiple chunks can be
 * merged into common strings and strings can be broken up between
 * blocks.
 */
static void stringdata(struct basic_block *blk, const uint8_t *data, int len)
{
    while (len) {
        int copylen;
        uint8_t *p = blk->data + blk->bytes;

        if (!blk->stringptr) {
	    if (blk->bytes >= BLKFREE - (STRHDR+STREND)) {
                /* Not enough space for any string */
                writeblock(blk, false);
                continue;
            }
            blk->stringptr = p;
            *p++ = STRHDR + STREND;	/* Overhead per statement */
            p = set16(p, ++blk->lineno);
            *p++ = 0xcb;		/* String */
	    *p++ = '"';			/* Deliminator (unused) */
	    *p++ = 0;			/* Length of string */
            blk->bytes += STRHDR;
        }

        copylen = (BLKFREE - STREND) - blk->bytes;
        if (copylen > len)
            copylen = len;

        if (copylen <= 0) {
            /* Not enough space left in this block for even one byte */
            closestring(blk);
            continue;
        }

        memcpy(p, data, copylen);

        p = blk->stringptr;
        p[0] += copylen;
        p[5] += copylen;

	data += copylen;
        blk->bytes += copylen;
        len -= copylen;
    }
}

/*
 * Emit a standalone BASIC statement, without encapsulating it in a string.
 */
static void bacstmt(struct basic_block *blk, const uint8_t * data, int len)
{
    uint8_t *p;

    if (blk->stringptr)
	closestring(blk);	/* Close any open string data */

    if (len > BLKFREE - (LINEHDR+LINEEND) - blk->bytes)
        writeblock(blk, false);

    p = &blk->data[blk->bytes];
    *p++ = len + (LINEHDR+LINEEND);	/* Length of statement */
    p = set16(p, ++blk->lineno);        /* Line number */
    p = mempcpy(p, data, len);		/* Actual bytecode data */
    *p++ = 0x0d;			/* Terminating CR */

    blk->bytes = p - blk->data;
}

/*
 * BASIC statement for the original RUN with CHAIN option
 *
 * PRINT CALL(PEEK(65054%)+SWAP%(65055%)-xxxx%) ":CHAINFILE:HERE:"+""
 *
 * 65054 = EOFA, since we have the loader at the end.
 * The +"" forces BASIC to copy the string to the stack instead of
 * pointing into the P-code.
 */
static uint8_t run_prefix[] = {
    0x84, 0xc7, 0x1e, 0xfe, 0xce, 0x36, 0xc7, 0x1f,
    0xfe, 0xce, 0x36, 0xce, 0x34, 0xf5, 0xc7,
    0, 0,                       /* = loader length */
    0xf8, 0xce, 0x3a, 0xba,
    0xcb, '"', 16,		/* String expression */
    ':', 'C', 'H', 'A', 'I', 'N', 'F', 'I',
    'L', 'E', ':', 'H', 'E', 'R', 'E', ':',
    0xcb, '"', 0, 0xf6,		/* +"" */
    0xbb			/* End string expression */
};
#define RUN_CALL_OFFSET 15
#define RUN_CHAIN_OFFSET (RUN_CALL_OFFSET+9)

/*
 * This is the BASIC statement:
 *  1 IF CALL(yyyy%)
 * followed by an end of program marker (1 byte) (which also = END).
 *
 * This includes the BASIC statement encapsulation, since this is
 * part of the payload.
 *
 * The address yyyy% is set to the entry point via a normal relocation.
 */
static uint8_t restart_prefix[] = {
    11,				/*  0 - line len */
    0x01, 0x00,			/*  1 - line no */
    0x88,			/*  3 - IF */
    0xc7, 0, 0,			/*  4 - address */
    0xce, 0x3a,			/*  7 - CALL() */
    0xba,			/*  8 - end integer */
    0x87, 0x01,			/* 10 - skip one byte if false */
    0x0d,			/* 12 - end of line */
    0x01			/* 13 - End of BASIC program */
};
#define RESTART_CALL_OFFSET 5

/*
 * STOP statement, used as end-of-data marker.  Technically,
 * any BASIC statement of 2 bytes or less would work here.
 */
static const uint8_t stop_stmt[] = {
    0x86, 0x89			/* BASIC bytecode for STOP */
};

/*
 * Main routine
 */
int writebac80(FILE *out, struct bindata *in)
{
    struct basic_block blk;
    uint8_t *loader;
    int reloclen = 0;
    int loader_len;
    int loader_offset = 1;      /* 1 for the statement terminal CR */
    int pfxlen = 0;

    memset(&blk, 0, sizeof blk);
    blk.data[0] = 0x82;         /* Program start marker, not in memory */
    blk.bytes = 1;
    blk.out = out;

    if (in->relocs) {
        /* This is a relocatable binary, load it at BOFA */
        loader = bac80r_bin;
        loader_len = bac80r_SYM__end;
        loader_offset += (loader_len - bac80r_SYM__start);

        if (!in->driver) {
            /* Application, add a restart prefix to payload */
            pfxlen = sizeof restart_prefix;
            set16(restart_prefix + RESTART_CALL_OFFSET, in->entrypoint);
            push_reloc(in, RESTART_CALL_OFFSET - pfxlen);
        }
        loader[bac80r_SYM_DELTA] = pfxlen;
        set16(loader + bac80r_SYM_ENTRY, in->entrypoint + pfxlen);

        reloclen = pack_relocs(in, pfxlen, false);
        if (reloclen < 0)
            return -1;

        set16(loader + bac80r_SYM_NEG_RELOC_LEN, -(reloclen+1));
    } else {
        /* This is to be loaded at a fixed address */
        loader = bac80_bin;
        loader_len = bac80_SYM__end;
        loader_offset += (loader_len - bac80_SYM__start);

        set16(loader + bac80_SYM_ENTRY, in->entrypoint);
        set16(loader + bac80_SYM_LOAD, in->origin);
    }

    set16(run_prefix + RUN_CALL_OFFSET, loader_offset);
    embed_name(run_prefix + RUN_CHAIN_OFFSET, CHAIN_LEN, in->chain);

    /* Emit the RUN call prefix */
    bacstmt(&blk, run_prefix, sizeof run_prefix);

    /* Emit the restart prefix, if applicable */
    stringdata(&blk, restart_prefix, pfxlen);

    /* Emit the actual binary data */
    stringdata(&blk, in->data + in->origin, in->datalen);

    /* Emit relocation information */
    if (in->relocs) {
        stringdata(&blk, (const uint8_t *)"\1", 1);     /* EOFA marker */
        stringdata(&blk, in->packed_relocs, in->packed_relocs_len);
    }

    /* Emit STOP statement to indicate end of data blocks */
    bacstmt(&blk, stop_stmt, sizeof stop_stmt);

    /* Emit loader */
    bacstmt(&blk, loader, loader_len);

    /* Write the final block */
    writeblock(&blk, true);

    return 0;
}
