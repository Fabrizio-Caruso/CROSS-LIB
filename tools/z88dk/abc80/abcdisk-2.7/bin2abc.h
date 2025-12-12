#ifndef BIN2ABC_H
#define BIN2ABC_H

#include "compiler.h"

/*
 * Input available to output formats. Not all of them are relevant to
 * all output formats.
 */
struct bindata {
    uint8_t *data;	 	/* The actual data buffer */
    bool *isset;		/* Bytes actually written to */
    uint16_t *relocs;		/* Relocations (NULL for non-relocatable) */
    uint8_t *packed_relocs;	/* Packed relocations buffer */
    int origin;			/* Load address */
    int datalen;		/* Set bytes past origin */
    int entrypoint;		/* Entry point address (possibly relative) */
    int firstline;		/* Line number to start at */
    int nrelocs;		/* Number of relocations */
    int packed_relocs_len;	/* Length of packed relocations */
    bool subroutine;		/* Generate a subroutine for GOSUB */
    bool driver;		/* Options suitable for a driver? */
    const char *chain;		/* Filename for chaining */
};

#define MAX_ADDR 65536

static inline bool is_stdio(const char *filename)
{
    return !filename || !filename[0] || (filename[0] == '-' && !filename[1]);
}

static inline uint8_t *set16(uint8_t *data, uint16_t val)
{
    *data++ = val;
    *data++ = val >> 8;
    return data;
}

static inline uint16_t get16(const uint8_t *data)
{
    return data[0] + (data[1] << 8);
}

static inline uint16_t letohs(uint16_t v)
{
#ifdef WORDS_LITTEENDIAN
    return v;
#else
    return (uint16_t) ((v >> 8) | (v << 8));
#endif
}

FILE *open_file(const char *filename, const char *mode);
int close_file(FILE * f);
size_t read_file(void *buf, size_t buflen, const char *filename);
int readihex(struct bindata *in, const char *filename);

void read_reloc_file(struct bindata *in, const char *filename);
void push_reloc(struct bindata *in, uint16_t reloc);
int pack_relocs(struct bindata *in, int delta, bool adjust);
void apply_relocs(struct bindata *in);

#define CHAIN_LEN 16

int writeabs(FILE *out, struct bindata *in);
int writebac80(FILE *out, struct bindata *in);
int writebac800(FILE *out, struct bindata *in);
int writebas(FILE *out, struct bindata *in);

extern const char *program_name;

#endif /* BIN2ABC_H */
