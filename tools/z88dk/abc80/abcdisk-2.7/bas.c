#include "compiler.h"
#include "bin2abc.h"

#define MAXLINE 79

int writebas(FILE *out, struct bindata *in)
{
    const uint8_t *data;
    const bool *isset;
    int nextaddr = 0;
    int addr;
    char buf[MAXLINE+1];
    int buf_len = 0;
    int line = in->firstline;

    apply_relocs(in);

    data = in->data;
    isset = in->isset;

    for (addr = 0; addr < MAX_ADDR; addr++) {
        if (isset[addr]) {
            if (buf_len) {
                if (addr != nextaddr || buf_len > MAXLINE-5) {
		    buf[buf_len++] = '\n';
		    fwrite(buf, 1, buf_len, out);
		    buf_len = 0;
		}
	    }

	    if (!buf_len) {
		buf_len = snprintf(buf, sizeof buf, "%d POKE %d%%",
				   line++, addr);
	    }

	    buf_len += snprintf(buf + buf_len, sizeof buf - buf_len,
				",%d%%", data[addr]);
	    nextaddr = addr + 1;
	}
    }

    if (buf_len) {
	buf[buf_len++] = '\n';
	fwrite(buf, 1, buf_len, out);
    }

    if (in->subroutine) {
	fprintf(out, "%d RETURN\n", line++);
    } else {
	if (in->entrypoint >= 0)
	    fprintf(out, "%d Z%%=CALL(%d%%)\n", line++, in->entrypoint);

	switch (in->chain[0]) {
	case ':':
	    fprintf(out, "%d END\n", line++);
	    break;
	case '\0':
	case ' ':
	    /* Make it work on both ABC80 and 800 */
	    fprintf(out, "%d ON ERROR GOTO %d\n", line, line+2);
	    fprintf(out, "%d CHAIN \"\"\n", line+1); /* ABC80 */
	    fprintf(out, "%d CHAIN \"NUL:\"\n", line+2); /* ABC800 */
	    line += 3;
	    break;
	default:
	    fprintf(out, "%d CHAIN \"%s\"\n", line++, in->chain);
	    break;
	}
    }

    return 0;
}
