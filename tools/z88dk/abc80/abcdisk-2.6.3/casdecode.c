/*
 * Decode ABC80 cassette files
 *
 * Uses libsndfile to parse the input file format.
 */

#include "abcdisk.h"
#include <sndfile.h>
#include <math.h>

static unsigned int xtime;

static void output_block(uint8_t * buf, uint8_t * csum)
{
    uint16_t csum1, csum2;
    int i;
    static FILE *f;
    static uint16_t block;
    static char filename[64];
    int err = 0;

    if (!buf) {
        err = 1;                /* Notification of a bad block */
    } else {
        csum2 = csum[0] + (csum[1] << 8);

        csum1 = 0x03;           /* Checksum includes ETX */
        for (i = 0; i < 256; i++)
            csum1 += buf[i];

        if (csum1 == csum2) {
            if (!memcmp(buf, "\xff\xff\xff", 3)) {
                if (f)
                    fclose(f);

                unmangle_filename(filename, buf + 3);
                f = fopen(filename, "wb");
                if (!f) {
                    perror(filename);
                    exit(1);
                }
                block = 0;
                printf("File: %s\n", filename);
            } else if (!f || buf[0] != 0x00 ||
                       (buf[1] + (buf[2] << 8)) != block) {
                printf("Bad block ID: %02x %02x %02x\n",
                       buf[0], buf[1], buf[2]);
                err = 1;
            } else {
                block++;
                fwrite(buf + 3, 1, 253, f);
            }
        } else {
            printf("Checksum error: got %04x data %04x\n", csum2, csum1);
            err = 1;
        }
    }

    if (err && f) {
        fclose(f);
        remove(filename);
        f = NULL;
    }
}

static void output_bit(unsigned int bit)
{
    enum state {
        st_leader,
        st_sync,
        st_stx,
        st_data,
        st_etx,
        st_csum
    };
    static enum state st = st_leader;
    static unsigned int ctr = 0;
    static unsigned int bits = 0;
    static uint8_t data = 0;
    static uint8_t buf[256];
    static uint8_t csum[2];

    data = (data >> 1) + (bit << 7);    /* Bigendian bit order... */
    bits++;

    switch (st) {
    case st_leader:
        ctr++;
        if (bit == 1)
            ctr = 0;
        if (ctr > 128)          /* After 128 zeroes start looking for sync */
            st = st_sync;
        break;

    case st_sync:
        if (data == 0x16) {
            st = st_stx;        /* Sync acquired */
            bits = 0;
        }
        break;

    case st_stx:
        if (bits < 8)
            break;
        bits = 0;
        ctr = 0;
        if (data == 0x02) {
            st = st_data;
        } else if (data == 0x16) {
            /* Got another SYNC */
        } else {
            printf("Got %02x when expecting SYNC or STX\n", data);
            output_block(NULL, NULL);
            st = st_leader;     /* ERROR */
        }
        break;

    case st_data:
        if (bits < 8)
            break;
        bits = 0;
        buf[ctr++] = data;
        if (ctr == 256)
            st = st_etx;
        break;

    case st_etx:
        if (bits < 8)
            break;
        bits = 0;
        ctr = 0;
        if (data == 0x03)
            st = st_csum;
        else {
            printf("Got %02x when expecting ETX\n", data);
            output_block(NULL, NULL);
            st = st_leader;     /* ERROR */
        }
        break;

    case st_csum:
        if (bits < 8)
            break;
        bits = 0;
        csum[ctr++] = data;
        if (ctr == 2) {
            output_block(buf, csum);
            ctr = 0;
            st = st_leader;
        }
        break;
    }
}

static void process_flank(unsigned int time)
{
    static unsigned int since_start = 0;
    static unsigned int captured = 0;

    since_start += time;

    if (since_start < xtime / 4) {
        printf("Quick flank: %u:%u\n", since_start, xtime);
    } else if (since_start < xtime * 3 / 4) {
        captured = 1;
    } else if (since_start < xtime * 5 / 4) {
        output_bit(captured);
        since_start = captured = 0;
    } else {
        printf("Slow flank: %u:%u\n", since_start, xtime);
        since_start = 0;
    }
}

int main(int argc, char *argv[])
{
    SNDFILE *f;
    double samp, osamp, scale;
    double v, k, cutoff, thresh;
    double *frame;
    unsigned int state;
    unsigned int time, ptime;
    int i;
    SF_INFO sfinfo;

    setlocale(LC_ALL, "");

    if (argc < 2 || (argv[1][0] == '-' && !argv[1][1])) {
	fprintf(stderr, "abcdisk %s\n", VERSION);
	fprintf(stderr, "Usage: %s inputfile [threshold_hz [cutoff]]\n", argv[0]);
	exit(1);
    }

    memset(&sfinfo, 0, sizeof sfinfo);
    f = sf_open(argv[1], SFM_READ, &sfinfo);
    if (!f) {
	fprintf(stderr, "%s: %s: %s\n", argv[0], argv[1], sf_strerror(NULL));
	exit(1);
    }

    thresh = ((argc > 2) ? atof(argv[2]) : CAS_BAUDRATE_ABC800*3)/32768.0;
    cutoff = (argc > 3) ? atof(argv[3]) : 0.0;

    k = exp(-cutoff / sfinfo.samplerate);
    xtime = sfinfo.samplerate / CAS_BAUDRATE_ABC80;

    state = time = ptime = 0;
    samp = osamp = 0.0;
    v = 0.0;

    frame = calloc(sizeof *frame, sfinfo.channels);
    if (!frame) {
	perror(argv[0]);
	exit(1);
    }
    scale = 1.0/sfinfo.channels;

    while (sf_readf_double(f, frame, 1) == 1) {
	/* Average between channels */
	samp = 0.0;
	for (i = 0; i < sfinfo.channels; i++)
	    samp += frame[i];
	samp *= scale;

	/* samp is now a normalized value in the range +/- 1.0 */

        /* Emulate high pass filter in ABC80 */
        v = (v * k) + (samp - osamp);
        osamp = samp;

        if (v >= thresh) {
            if (!state) {
                state = 1;
                process_flank(time - ptime);
                ptime = time;
            }
        } else if (v <= -thresh) {
            if (state) {
                state = 0;
                process_flank(time - ptime);
                ptime = time;
            }
        }
        time++;
    }

    sf_close(f);

    return 0;
}
