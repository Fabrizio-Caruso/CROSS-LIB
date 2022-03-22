/*
 * readfile.c
 *
 * Read a host file and if necessary convert it to ABC format.
 */

#include "abcdisk.h"

/*
 * Get the contents of a file, and guess whether or not it's a text file;
 * then convert it to ABC format (not including block headers, so only
 * 253 bytes of data.)
 */

unsigned char *make_abcfile(FILE *f, int *sizep)
{
    unsigned char buffer[256];
    int i, n;
    int size = 0;
    int binary = 0;
    unsigned char *data, *dp;
    int blocks;

    if (!f)
        return NULL;

    while ((n = fread(buffer, 1, sizeof buffer, f))) {
        if (!binary)
            for (i = 0; i < n; i++) {
                if ((buffer[i] & 0x80) || buffer[i] == 0x00
                    || buffer[i] == 0x03) {
                    binary = 1;
                    break;
                }
            }

        size += n;
    }

    rewind(f);

    if (binary) {
        blocks = (size + 252) / 253;
        data = xmalloc(blocks * 253);
        fread(data, size, 1, f);
        memset(data + size, 0, blocks * 253 - size);
        size = blocks * 253;
    } else {
        int blocks = (size + 251) / 252 + 1;
        data = xmalloc(blocks * 253);

        dp = data;
        while (memset(buffer, 0, 253), (n = fread(buffer, 1, 252, f))) {
            for (i = 0; i < n; i++) {
                if (buffer[i] == '\n')
                    buffer[i] = '\r';
            }
            buffer[n] = '\x03';

            memcpy(dp, buffer, 253);
            dp += 253;
        }

        /* Ending block */
        memset(buffer, 0, 6);
        buffer[6] = '\x03';
        memcpy(dp, buffer, 253);
        dp += 253;

        size = dp - data;
    }

    fclose(f);

    *sizep = size;

    return data;
}
