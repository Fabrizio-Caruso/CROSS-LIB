/*
 * mangle.c:
 *
 * Converts filenames into ABC syntax (mangled and plain)
 *
 * Returns 0 for OK, -1 for failure (filename cannot be converted)
 */

#include "abcdisk.h"

static const wchar_t srcset[] =
    L"0123456789_."
    L"ÉABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÅÜÆØ"
    L"éabcdefghijklmnopqrstuvwxyzäöåüæø";
static const unsigned char dstset[] =
    "0123456789_."
    "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^[\\"
    "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^[\\";

/* Produces a mangled name, without dot */
int mangle_name(unsigned char *dst, const char *src)
{
    unsigned char *d = dst;
    const char *s, *p;
    wchar_t sc;
    const wchar_t *scp;
    char dc;
    int n;

    if ((p = strrchr(src, '/')))
        s = p + 1;
    else
        s = src;

    memset(dst, ' ', 11);
    dst[11] = '\0';

    mbtowc(NULL, NULL, 0);      /* Reset the shift state */

    while (d < dst + 11 && (n = mbtowc(&sc, s, (size_t) ~ 0)) > 0) {
        s += n;

        if ((scp = wcschr(srcset, sc))) {
            dc = dstset[scp - srcset];
        } else {
            dc = '_';
        }

        if (dc == '.')
            d = dst + 8;
        else
            *d++ = dc;
    }

    return *s ? -1 : 0;
}

/*
 * Produces an unformatted filename in a buffer padded with CR.
 */
int embed_name(unsigned char *dst, size_t len, const char *src)
{
    unsigned char *d = dst;
    const char *s = src;
    wchar_t sc;
    const wchar_t *scp;
    char dc;
    int n;

    memset(dst, '\r', len);

    mbtowc(NULL, NULL, 0);      /* Reset the shift state */

    while (d < dst + len && (n = mbtowc(&sc, s, (size_t) ~ 0)) > 0) {
        s += n;

        if ((scp = wcschr(srcset, sc))) {
            dc = dstset[scp - srcset];
        } else if (sc == L':') {
	    dc = ':';
	} else {
	    return -1;
	}
	*d++ = dc;
    }

    return *s ? -1 : 0;
}
