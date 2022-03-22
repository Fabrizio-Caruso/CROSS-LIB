/*
 * unmangle.c
 *
 * Convert a filename in ABC encoding (without dot) to a proper filename
 * in host encoding.
 */

#include "abcdisk.h"

size_t unmangle_filename(char *outbuf, const unsigned char *in)
{
    static const wchar_t my_tolower[256] =
        L"\000\001\002\003\004\005\006\007\010\011\012\013\014\015\016\017"
        L"\020\021\022\023\024\025\026\027\030\031\032\033\034\035\036\037"
        L" !\"#¤%&'()*+,-./0123456789:;<=>?"
        L"éabcdefghijklmnopqrstuvwxyzäöåü_"
        L"éabcdefghijklmnopqrstuvwxyzäöåü\377"
        L"\200\201\202\203\204\205\206\207\210\211\212\213\214\215\216\217"
        L"\220\221\222\223\224\225\226\227\230\231\232\233\234\235\236\237"
        L"\240\241\242\243\244\245\246\247\250\251\252\253\254\255\256\257"
        L"\260\261\262\263\264\265\266\267\270\271\272\273\274\275\276\277"
        L"\300\301\302\303\304\305\306\307\310\311\312\313\314\315\316\317"
        L"\320\321\322\323\324\325\326\327\330\331\332\333\334\335\336\337"
        L"\340\341\342\343\344\345\346\347\350\351\352\353\354\355\356\357"
        L"\360\361\362\363\364\365\366\367\370\371\372\373\374\375\376\377";
    int i;
    char *out = outbuf;

    wctomb(NULL, 0);

    for (i = 0; i < 8; i++) {
        if (*in != ' ')
            out += wctomb(out, my_tolower[*in]);
        in++;
    }

    if (memcmp(in, "   ", 3) && memcmp(in, "Ufd", 3)) {
        out += wctomb(out, L'.');
        for (i = 0; i < 3; i++) {
            if (*in != ' ')
                out += wctomb(out, my_tolower[*in]);
            in++;
        }
    }

    *out = '\0';

    return out - outbuf;
}
