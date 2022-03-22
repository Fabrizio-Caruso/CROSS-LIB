#include "compiler.h"
#include "bin2abc.h"

/*
 * Make sure we have a relocation buffer; also marks the output as relocatable
 */
static void do_make_relocatable(struct bindata *in)
{
    if (likely(in->relocs))
	return;

    in->relocs = calloc(MAX_ADDR+1, sizeof(uint16_t));
    in->nrelocs = 0;
}

/*
 * Load a file of relocations
 */
void read_reloc_file(struct bindata *in, const char *filename)
{
    size_t n, i;

    do_make_relocatable(in);

    n = read_file(in->relocs + in->nrelocs, 2 * (MAX_ADDR - in->nrelocs),
		  filename);
    n >>= 1;

#ifdef WORDS_LITTLEENDIAN
    (void)i;			/* Already loaded */
#else
    /* Host byte order */
    for (i = 0; i < n; i++) {
        in->relocs[in->nrelocs + i] = letohs(in->relocs[in->nrelocs + i]);
    }
#endif
    in->nrelocs += n;
}

/*
 * Push additional relocations onto the list
 */
void push_reloc(struct bindata *in, uint16_t r)
{
    do_make_relocatable(in);
    in->relocs[in->nrelocs++] = r;
}

/*
 * Pack relocations; this is done in place.
 * The value delta is added to the first replocation skip.
 *
 * If adjust is set then the first relocation is adjusted to
 * add the size of the relocation data itself to it.
 */
static int sort_base;

/*
 * Sort addresses, keeping in mind the sort_base starting point
 * Correct wraparound behavior here is really important!
 */
static int sort_addrs(const void *p1, const void *p2)
{
    uint16_t a1 = *(const uint16_t *)p1 - sort_base;
    uint16_t a2 = *(const uint16_t *)p2 - sort_base;

    return a1 - a2;
}

int pack_relocs(struct bindata *in, int delta, bool adjust)
{
    uint8_t *p, *rbuf;
    const uint16_t *rp;
    uint16_t r, r0;
    unsigned int rd;
    int last;
    int len;
    int nrelocs = in->nrelocs;

    sort_base = -delta;
    qsort(in->relocs, nrelocs, sizeof *(in->relocs), sort_addrs);

    /* +2 bytes for end marker and possibly adjustment */
    in->packed_relocs = calloc(nrelocs+1, 2);

    p = rbuf = in->packed_relocs + 1;  /* +1 in case we need adjust */

    rp = in->relocs;
    r0 = *rp + delta;		/* Save the first relocation */

    last = 0;
    while (nrelocs--) {
	r = *rp++ + delta;
	rd = r - last;
        if (rd == 0) {
            continue;           /* Duplicated relocation */
        } else if (rd < 0x80) {
            *p++ = rd;
        } else if (rd < 0x7eff) {
            *p++ = (rd >> 8) + 0x81;
            *p++ = rd;
        } else {
            fprintf(stderr,
                    "%s: impossibly large relocation jump: 0x%x bytes\n",
                    program_name, r);
            return -1;
        }
        last = r + 2;
    }
    *p++ = 0x80;                /* End markerxs */
    len = p - rbuf;

    /* If len == 1 then all we have is the end marker, nothing to adjust */
    if (len > 1 && adjust) {
	uint16_t r1 = r0 + len;

        if (r1 > 0x7f) {
            if (r0 <= 0x7f) {
                rbuf--;         /* We saved a byte for this just in case */
                r1++;           /* Add one more byte to the reloc length */
                len++;
            }

            rbuf[0] = (r1 >> 8) + 0x81;
            rbuf[1] = r1;
        } else {
            rbuf[0] = r1;
        }
    }

    /* XXX: this loses the buffer for free(). */
    in->packed_relocs = rbuf;
    return in->packed_relocs_len = len;
}

/*
 * Apply relocations; this is the only option when dealing with
 * absolute file formats like .abs.
 */
void apply_relocs(struct bindata *in)
{
    int origin = in->origin;
    uint8_t *data = in->data;
    const uint16_t *rp = in->relocs;
    int i;

    if (!rp)
	return;

    for (i = 0; i < in->nrelocs; i++) {
	uint16_t r = *rp++ + origin;

	set16(data+r, get16(data+r) + origin);
    }
}
