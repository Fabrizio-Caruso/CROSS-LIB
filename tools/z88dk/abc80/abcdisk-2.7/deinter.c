/*
 * deinter.c
 *
 * Undo software interleaving
 */

#include "abcdisk.h"

int main(int argc, char *argv[])
{
    int sectsize;
    int ileave;
    int ilsize;
    FILE *f;
    char *buf;
    int i, j;

    if (argc < 2 || (argv[1][0] == '-' && !argv[1][1])) {
	fprintf(stderr, "abcdisk %s\n", VERSION);
        fprintf(stderr, "Usage: %s file [ileave [ilsize [sector]]]\n", argv[0]);
        exit(1);
    }

    f = fopen(argv[1], "rb");
    if (!f) {
        perror(argv[1]);
        exit(1);
    }

    ileave = (argc > 2) ? atoi(argv[2]) : 7;
    ilsize = (argc > 3) ? atoi(argv[3]) : 16;
    sectsize = (argc > 4) ? atoi(argv[4]) : 256;

    buf = xmalloc(sectsize * ilsize);

    while (fread(buf, 1, sectsize * ilsize, f))
        for (i = 0, j = 0; i < ilsize; i++, j = (j + ileave) % ilsize)
            fwrite(buf + j * sectsize, 1, sectsize, stdout);

    fclose(f);

    return 0;
}
