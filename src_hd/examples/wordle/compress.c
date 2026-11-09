#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CODE_BYTES 3
#define BASE 26UL

#define POW26_1 26UL
#define POW26_2 (26UL * 26UL)
#define POW26_3 (26UL * 26UL * 26UL)
#define POW26_4 (26UL * 26UL * 26UL * 26UL)

#define BUCKET_COUNT 676 /* 26 * 26 */

typedef struct {
    size_t start[BUCKET_COUNT];
    size_t count[BUCKET_COUNT];
} TwoLetterIndex;

/*
 * Valid word: exactly 5 lowercase letters, null-terminated.
 */
static int valid_5(const char *w)
{
    int i;

    for (i = 0; i < 5; i++) {
        if (w[i] < 'a' || w[i] > 'z') {
            return 0;
        }
    }

    return w[5] == '\0';
}

/*
 * Encode "abcde" as:
 *
 *   a * 26^4 + b * 26^3 + c * 26^2 + d * 26 + e
 *
 * The result is 0 .. 26^5 - 1, which fits in 24 bits.
 */
static unsigned long encode5(const char *w)
{
    return (unsigned long)(w[0] - 'a') * POW26_4
         + (unsigned long)(w[1] - 'a') * POW26_3
         + (unsigned long)(w[2] - 'a') * POW26_2
         + (unsigned long)(w[3] - 'a') * POW26_1
         + (unsigned long)(w[4] - 'a');
}

/*
 * Decode a 24-bit base-26 code into a null-terminated 5-letter word.
 */
static void decode5(unsigned long v, char *out)
{
    int i;

    for (i = 4; i >= 0; i--) {
        out[i] = (char)('a' + (v % BASE));
        v = v / BASE;
    }

    out[5] = '\0';
}

static void pack_code(unsigned long v, unsigned char *p)
{
    p[0] = (unsigned char)(v & 0xFFUL);
    p[1] = (unsigned char)((v >> 8) & 0xFFUL);
    p[2] = (unsigned char)((v >> 16) & 0xFFUL);
}

static unsigned long code_at(const unsigned char *p)
{
    return (unsigned long)p[0]
         | ((unsigned long)p[1] << 8)
         | ((unsigned long)p[2] << 16);
}

/*
 * Comparator for qsort on packed 3-byte codes.
 */
static int cmp_packed(const void *a, const void *b)
{
    const unsigned char *x = (const unsigned char *)a;
    const unsigned char *y = (const unsigned char *)b;

    if (x[0] != y[0]) return (int)x[0] - (int)y[0];
    if (x[1] != y[1]) return (int)x[1] - (int)y[1];
    return (int)x[2] - (int)y[2];
}

/*
 * Build a packed sorted dictionary.

 * words: array of C strings, each exactly 5 lowercase letters.
 * n: number of words.
 * out_count: receives the number of packed words stored.
 * remove_duplicates: 1 if duplicate words should be removed.
 */
unsigned char *build_dictionary(const char **words, size_t n,
                                size_t *out_count,
                                int remove_duplicates)
{
    unsigned char *p;
    size_t i;
    size_t m = 0UL;

    if (out_count == NULL) {
        return NULL;
    }

    *out_count = 0UL;

    if (n == 0UL) {
        return NULL;
    }

    p = (unsigned char *)malloc(n * CODE_BYTES);
    if (p == NULL) {
        return NULL;
    }

    for (i = 0UL; i < n; i++) {
        if (valid_5(words[i])) {
            pack_code(encode5(words[i]), &p[m * CODE_BYTES]);
            m++;
        }
    }

    if (m == 0UL) {
        free(p);
        return NULL;
    }

    qsort(p, m, CODE_BYTES, cmp_packed);

    if (remove_duplicates) {
        size_t w = 0UL;

        for (i = 1UL; i < m; i++) {
            if (memcmp(&p[i * CODE_BYTES],
                       &p[(i - 1UL) * CODE_BYTES],
                       CODE_BYTES) != 0) {
                if (w != i) {
                    memmove(&p[w * CODE_BYTES],
                            &p[i * CODE_BYTES],
                            CODE_BYTES);
                }
                w++;
            }
        }

        m = w;
    }

    *out_count = m;
    return p;
}

/*
 * Simple portable RNG state.
 * For a slow machine, this is much cheaper than repeatedly calling rand().
 */
static unsigned long rng_state = 0x12345679UL;

void rng_seed(unsigned long seed)
{
    if (seed == 0UL) {
        seed = 1UL;
    }

    rng_state = seed;
}

static unsigned long rng_next(void)
{
    unsigned long x = rng_state;

    x ^= (x << 13);
    x ^= (x >> 17);
    x ^= (x << 5);

    rng_state = x;
    return x;
}

/*
 * Randomly select one word from the packed dictionary.
 *
 * This chooses uniformly over the entries currently stored.
 * If you removed duplicates, it chooses uniformly over unique words.
 * If you kept duplicates, it chooses uniformly over original entries.
 */
int random_word(const unsigned char *dict, size_t n, char *out)
{
    size_t idx;

    if (dict == NULL || out == NULL || n == 0UL) {
        return 0;
    }

    idx = (size_t)(rng_next() % (unsigned long)n);

    decode5(code_at(&dict[idx * CODE_BYTES]), out);
    return 1;
}

/*
 * Plain binary search membership test.
 *
 * O(log N) comparisons. Each comparison is a 24-bit integer comparison.
 */
int word_in_dict(const unsigned char *dict, size_t n, const char *w)
{
    unsigned long key;
    size_t lo, hi, mid;
    const unsigned char *p;
    unsigned long v;

    if (dict == NULL || n == 0UL || !valid_5(w)) {
        return 0;
    }

    key = encode5(w);

    lo = 0UL;
    hi = n;

    while (lo < hi) {
        mid = lo + (hi - lo) / 2UL;
        p = &dict[mid * CODE_BYTES];
        v = code_at(p);

        if (v < key) {
            lo = mid + 1UL;
        } else if (v > key) {
            hi = mid;
        } else {
            return 1;
        }
    }

    return 0;
}

/*
 * Optional small index for faster lookup.
 *
 * Bucket by the first two letters. There are only 26*26 = 676 buckets.
 * This reduces the binary search range significantly.
 *
 * Build this after the dictionary is sorted.
 */
int build_two_letter_index(const unsigned char *dict, size_t n,
                           TwoLetterIndex *t)
{
    size_t i;
    unsigned long v;
    unsigned long b;

    if (dict == NULL || t == NULL) {
        return 0;
    }

    for (i = 0UL; i < BUCKET_COUNT; i++) {
        t->start[i] = 0UL;
        t->count[i] = 0UL;
    }

    for (i = 0UL; i < n; i++) {
        v = code_at(&dict[i * CODE_BYTES]);

        /*
         * code = a*26^4 + b*26^3 + c*26^2 + d*26 + e
         *
         * code / 26^3 = a*26 + b
         */
        b = v / POW26_3;

        if (t->count[b] == 0UL) {
            t->start[b] = i;
        }

        t->count[b]++;
    }

    return 1;
}

/*
 * Faster membership test using the two-letter index.
 */
int word_in_dict_indexed(const unsigned char *dict, size_t n,
                         const TwoLetterIndex *t,
                         const char *w)
{
    unsigned long key;
    unsigned long b;
    size_t lo, hi, mid;
    const unsigned char *p;
    unsigned long v;

    if (dict == NULL || t == NULL || n == 0UL || !valid_5(w)) {
        return 0;
    }

    key = encode5(w);
    b = key / POW26_3;

    if (b >= BUCKET_COUNT) {
        return 0;
    }

    if (t->count[b] == 0UL) {
        return 0;
    }

    lo = t->start[b];
    hi = lo + t->count[b];

    while (lo < hi) {
        mid = lo + (hi - lo) / 2UL;
        p = &dict[mid * CODE_BYTES];
        v = code_at(p);

        if (v < key) {
            lo = mid + 1UL;
        } else if (v > key) {
            hi = mid;
        } else {
            return 1;
        }
    }

    return 0;
}

int main(void)
{
    const char *words[] = {
        "hello",
        "world",
        "apple",
        "world",
        "grape"
    };

    size_t n = sizeof(words) / sizeof(words[0]);
    size_t count = 0UL;
    unsigned char *dict;
    TwoLetterIndex idx;
    char out[6];
    int i;

    dict = build_dictionary(words, n, &count, 1);
    if (dict == NULL) {
        return 1;
    }

    build_two_letter_index(dict, count, &idx);

    for (i = 0; i < 5; i++) {
        if (random_word(dict, count, out)) {
            printf("%s\n", out);
        }
    }

    printf("apple=%d zebra=%d\n",
           word_in_dict_indexed(dict, count, &idx, "apple"),
           word_in_dict_indexed(dict, count, &idx, "zebra"));

    free(dict);
    return 0;
}