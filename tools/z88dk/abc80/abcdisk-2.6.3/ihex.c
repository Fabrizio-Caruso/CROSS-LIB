#include "bin2abc.h"

#include <ctype.h>

#define MAXLINE BUFSIZ

static int hexval(unsigned char c)
{
    if (!isxdigit(c))
        return -1;

    c &= 0x4f;
    if (c > 9)
        c -= ('A' - 10);
    return c;
}

static long gethex(char **pp, uint8_t * csum, int bytes)
{
    long v = 0;
    int sh = 0;
    const char *p = *pp;

    while (bytes--) {
        int c1, c2;

        c1 = hexval(*p++);
        if (c1 < 0) {
            v = -1L;
            break;
        }
        c2 = hexval(*p++);
        if (c2 < 0) {
            v = -1L;
            break;
        }
        c2 += (c1 << 4);
        if (csum)
            *csum += c2;

        v += (unsigned long)c2 << sh;
        sh += 8;
    }

    *pp = (char *)p;
    return v;
}

static int parse_line(char *p, uint8_t *data, bool *isset, int *entrypt)
{
    uint8_t csum;
    int databytes, type, checksum;
    int d, n, i;
    int address;
    uint8_t buf[256];
    int myentry;
    int rv = 0;

    csum = 0;

    if ((databytes = gethex(&p, &csum, 1)) < 0)
        goto fail;

    if ((address = gethex(&p, &csum, 2)) < 0)
        goto fail;

    if ((type = gethex(&p, &csum, 1)) < 0)
        goto fail;

    myentry = *entrypt;

    switch (type) {
    case 0:
        /* Data record */
        if (myentry < 0)
            myentry = address;
        for (n = 0; n < databytes; n++) {
            if ((d = gethex(&p, &csum, 1)) < 0)
                goto fail;

            buf[n] = d;
        }
        break;

    case 1:
        /* End of file/entry point record */
        if (databytes != 0)
            goto fail;

        rv = -2;
        if (address > 0)        /* Unofficial but common extension */
            myentry = address;
        break;

    case 5:
        /* Linear start address (entrypoint) */
        if (databytes != 4)
            goto fail;

        myentry = gethex(&p, &csum, 4);
        break;

    default:
        goto fail;              /* Unknow record */
    }

    if ((checksum = gethex(&p, NULL, 1)) < 0)
        goto fail;

    if (checksum != (uint8_t) - csum)
        goto fail;

    if (type == 0) {
        memcpy(data + address, buf, databytes);
        for (i = 0; i < databytes; i++)
            isset[address + i] = true;
    }

    if (((unsigned int)myentry & ~0xffffU) == 0)
        *entrypt = myentry;

    return rv;

fail:
    errno = EINVAL;
    return -1;
}

int readihex(struct bindata *in, const char *filename)
{
    char buffer[MAXLINE];
    FILE *f;
    unsigned int line = 0;
    int err = 0;
    int entry = -1;

    f = open_file(filename, "rt");
    if (!f)
        return -1;

    while (fgets(buffer, sizeof buffer, f)) {
        line++;

        if (buffer[0] != ':')
            continue;

        err = parse_line(buffer + 1, in->data, in->isset, &entry);

        if (err == -2) {
            err = 0;            /* Not actually an error */
            break;              /* End of file */
        } else if (err < 0) {
            fprintf(stderr, "%s: %s:%u: invalid Intel hex line format\n",
                    program_name, filename, line);
	    break;
        }
    }
    err |= close_file(f);

    if (!err && in->entrypoint < 0)
	in->entrypoint = entry;

    return err;
}
