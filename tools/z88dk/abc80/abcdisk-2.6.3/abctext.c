#include "compiler.h"
#include "patchlevel.h"

/*
 * Convert an ABC text file in binary form to more normal form,
 * optionally with character set conversion.
 */

#ifndef MB_LEN_MAX
# define MB_LEN_MAX 256		/* Excessive but should be safe */
#endif

static const wchar_t abcchartbl[256] =
    L"\000\001\002\003\004\005\006\007\010\011\012\013\014\015\016\017"
    L"\020\021\022\023\024\025\026\027\030\031\032\033\034\035\036\037"
    L" !\"#¤%&'()*+,-./0123456789:;<=>?"
    L"ÉABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÅÜ_"
    L"éabcdefghijklmnopqrstuvwxyzäöåü\x25fc"

    /* Any better idea what to do with the high half? */
    L"\200\201\202\203\204\205\206\207\210\211\212\213\214\215\216\217"
    L"\220\221\222\223\224\225\226\227\230\231\232\233\234\235\236\237"
    L"\240\241\242\243\244\245\246\247\250\251\252\253\254\255\256\257"
    L"\260\261\262\263\264\265\266\267\270\271\272\273\274\275\276\277"
    L"\300\301\302\303\304\305\306\307\310\311\312\313\314\315\316\317"
    L"\320\321\322\323\324\325\326\327\330\331\332\333\334\335\336\337"
    L"\340\341\342\343\344\345\346\347\350\351\352\353\354\355\356\357"
    L"\360\361\362\363\364\365\366\367\370\371\372\373\374\375\376\377";

static int fputc_convert(int c, FILE *f)
{
    unsigned char abcchar = c;

    fputwc(abcchartbl[abcchar], f);
    return ferror(f) ? EOF : abcchar;
}

const char *program;

static void usage(void)
{
    fprintf(stderr, "abcdisk %s\n", VERSION);
    fprintf(stderr, "Usage: %s [-c] [abcfile [textfile]]\n", program);
    fprintf(stderr, "       -c : convert to host (multi)byte encoding\n");
    exit(1);
}

int main(int argc, char *argv[])
{
    FILE *f = stdin;
    FILE *out = stdout;
    char buf[253], *p;
    int n, i;
    char lastbyte;
    int (*fput_char)(int, FILE *) = fputc;
    char * const *arg = &argv[1];

    setlocale(LC_ALL, "");

    (void)argc;
    program = argv[0];

    if (*arg && (*arg)[0] == '-' && (*arg)[1]) {
	switch ((*arg)[1]) {
	case 'c':
	    fput_char = fputc_convert;
	    break;
	default:
	    usage();
	    break;
	}
	arg++;
    }

    if (*arg && strcmp(*arg, "-")) {
        f = fopen(*arg, "rb");
	if (!f) {
	    perror(*arg);
	    exit(1);
	}
    }

    if (*arg)
	arg++;

    if (*arg && strcmp(*arg, "-")) {
	out = fopen(*arg, "wt");
	if (!out) {
	    perror(*arg);
	    exit(1);
	}
    }

    lastbyte = 0;

    while ((n = fread(buf, 1, 253, f)) > 0) {
        if (!memcmp(buf, "\0\0\0\0\0\0", 6))
            break;              /* End of file */

        p = buf;
        while (n--) {
            if (lastbyte == '\t') {
                /* TAB + count is used for compressed spaces */
                i = *p;
                while (i--)
                    fput_char(' ', out);
            } else {
                switch (*p) {
                case '\t':
                    break;	/* Skip for now, process on next iteration */
                case '\3':
                    n = 0;      /* ETX, jump to next block */
                    break;
		case '\n':
		  if (lastbyte != '\r')
		    fput_char('\n', out);
		  break;
                default:
                    fput_char(*p, out);
                    break;
                }
            }
            lastbyte = *p++;
        }
    }

    fflush(out);

    /* Exit closes files as appropriate */
    return 0;
}
