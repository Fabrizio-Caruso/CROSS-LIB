#include "compiler.h"
#include "bin2abc.h"

struct abs_data_hdr {
    uint8_t type;               /* 0 for data */
    uint8_t data_bytes;
    uint8_t zero;               /* Reserved */
    uint8_t addrh;
    uint8_t addrhx;
    uint8_t addrl;
    uint8_t addrlx;
};

/*
 * This is the minimum space needed in a record to be able to fit
 * at least one byte worth of data: header + csum + 2 byte end of record
 */
#define MINSPACE ((int)sizeof(struct abs_data_hdr)+3+1)

static void
write_buffer(FILE * out, const uint8_t * buf, int buf_len, int buf_address,
             int *spaceinrecord_ptr)
{
    int spaceinrecord = *spaceinrecord_ptr;
    struct abs_data_hdr hdr;
    uint8_t csum;
    int i;

    memset(&hdr, 0, sizeof hdr);

    csum = 0;
    for (i = 0; i < buf_len; i++)
        csum += buf[i];

    hdr.data_bytes = buf_len;
    hdr.addrh = buf_address >> 8;
    hdr.addrhx = ~hdr.addrh;
    hdr.addrl = buf_address;
    hdr.addrlx = ~hdr.addrl;

    fwrite(&hdr, 1, sizeof hdr, out);
    fwrite(buf, 1, buf_len, out);
    putc(csum, out);

    spaceinrecord -= (buf_len + sizeof hdr + 1);

    if (spaceinrecord < MINSPACE) {
        putc(0xff, out);
        spaceinrecord--;
        putc(0x00, out);
        spaceinrecord--;

        while (spaceinrecord--)
            putc(0, out);

        spaceinrecord = 253;
    }

    *spaceinrecord_ptr = spaceinrecord;
}

int writeabs(FILE *out, struct bindata *in)
{
    const uint8_t *data;
    const bool *isset;
    int spaceinrecord = 253;
    int nextaddr = 0;
    int addr;
    uint8_t buf[256];
    int buf_address = 0;
    int buf_len = 0;

    apply_relocs(in);

    data = in->data;
    isset = in->isset;

    for (addr = 0; addr < MAX_ADDR; addr++) {
	if (isset[addr]) {
            bool newheader = false;

            if (buf_len) {
                if (addr != nextaddr) {
                    int padding = addr - nextaddr;
                    if (padding <= (int)sizeof(struct abs_data_hdr) + 1 &&
                        padding <= spaceinrecord - MINSPACE + 1) {
                        /* Short stretch of nothing. Just convert it to zero. */
                        memset(buf + buf_len, 0, padding);
                        buf_len += padding;
                    } else {
                        newheader = true;
                    }
                }

                if (spaceinrecord < buf_len - MINSPACE)
                    newheader = true;   /* Can't fit at least one more byte */

                if (newheader) {
                    write_buffer(out, buf, buf_len, buf_address,
                                 &spaceinrecord);
                    buf_len = 0;
                    buf_address = addr;
                }
            } else {
		buf_address = addr;
	    }

            buf[buf_len++] = data[addr];
            nextaddr = addr + 1;
        }
    }

    if (buf_len)
        write_buffer(out, buf, buf_len, buf_address, &spaceinrecord);

    /* Write entry point */
    write_buffer(out, NULL, 0, in->entrypoint, &spaceinrecord);

    /* Pad to end of record */
    while (spaceinrecord--)
        putc(0, out);

    return 0;
}
