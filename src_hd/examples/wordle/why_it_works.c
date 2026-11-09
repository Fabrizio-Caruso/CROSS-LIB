## Why this works well

### 1. Compression

A normal C string array for 5-letter words is often:

```c
char words[N][6];
```

That uses `6N` bytes.

The packed representation uses:

```c
unsigned char dict[3 * N];
```

So it is about half the memory of null-terminated strings.

For lowercase words:

```text
26^5 = 11,881,376 < 2^24
```

Therefore 3 bytes are enough.

### 2. Random selection

Because each word is one fixed 3-byte element, random selection is easy:

```c
idx = random() % N;
word = decode(dict[idx]);
```

That is `O(1)`.

### 3. Quick membership test

Because the base-26 code preserves lexicographic order for fixed-length words, you can sort the packed codes and use binary search.

For example:

```text
"aaaaa" < "aaaab" < "aaabz" < "baaaa"
```

Binary search is `O(log N)`.

The two-letter index makes it faster by first selecting the bucket for the first two letters. There are only 676 possible buckets, so the binary search
range is much smaller.

## If you need even faster membership: use a bitset

If memory is not extremely tight, you can add a bitset of all possible 5-letter lowercase words.

Number of possible words:

```text
26^5 = 11,881,376
```

Required bits:

```text
11,881,376 bits
```

Required bytes:

```text
11,881,376 / 8 = 1,485,172 bytes
```

That is about **1.48 MB**.

If you have that much RAM, membership testing becomes `O(1)`:

```c
#define MAX_5_CODES (26UL * 26UL * 26UL * 26UL * 26UL)
#define BITSET_BYTES (MAX_5_CODES / 8UL + 1UL)

static unsigned char *bitset = NULL;

int bitset_init(void)
{
    bitset = (unsigned char *)calloc(BITSET_BYTES, 1UL);
    return bitset != NULL;
}

void bitset_free(void)
{
    if (bitset != NULL) {
        free(bitset);
        bitset = NULL;
    }
}

int bitset_set_code(unsigned long code)
{
    if (bitset == NULL) {
        return 0;
    }

    bitset[code >> 3UL] |= (unsigned char)(1UL << (code & 7UL));
    return 1;
}

int bitset_contains_word(const char *w)
{
    unsigned long code;

    if (bitset == NULL || !valid_5(w)) {
        return 0;
    }

    code = encode5(w);

    return (int)((bitset[code >> 3UL] >> (code & 7UL)) & 1UL);
}
```

Then lookup becomes:

```c
if (bitset_contains_word(word)) {
    /* word is in dictionary */
}
```

You can still keep the packed 3-byte array for random word selection.

## Recommended choices

### Lowest memory

Use:

```c
packed 3-byte sorted array + binary search
```

Memory:

```text
3N bytes
```

Lookup:

```text
O(log N)
```

Random:

```text
O(1)
```

This is probably the best default for a slow machine with limited RAM.

### Faster lookup, still low memory

Use:

```c
packed 3-byte sorted array + two-letter index
```

Extra memory:

```text
676 * 2 * sizeof(size_t)
```

Usually a few KB.

Lookup:

```text
O(log(bucket_size))
```

This is often the best balance for a slow machine.

### Fastest lookup, fixed 1.5 MB overhead

Use:

```c
packed 3-byte array + bitset
```

Memory:

```text
3N + 1,485,172 bytes
```

Lookup:

```text
O(1)
```

Random:

```text
O(1)
```

Use this if you have at least about 1.5 MB extra RAM and membership testing is very frequent.

## If your words are not lowercase

### Case-insensitive

Convert to lowercase before encoding.

### Case-sensitive a-z and A-Z

Use base 52:

```text
52^5 = 380,201,632 < 2^29
```

Store 4 bytes per word.

### Arbitrary 5 ASCII characters

Store 5 bytes per word.

You can still sort and binary search, but use `memcmp` on 5-byte blocks instead of 24-bit integer comparison.

## If the dictionary is static

For the best performance on a slow machine, do not build and sort the dictionary at runtime if you can avoid it.

Generate the packed array once on a fast machine, then store it in your program as:

```c
static const unsigned char dict[] = {
    0x00, 0x00, 0x00,
    0x01, 0x00, 0x00,
    /* ... */
};

#define DICT_N (sizeof dict / 3)
```

Then the slow machine only does:

- random index selection
- 3-byte reads
- integer comparisons
- optional bit tests

No sorting, no `malloc`, no string handling.

## Summary

For 5-letter lowercase words in ANSI C:

```text
Encode each word as a 24-bit base-26 number.
Store as 3 bytes per word.
Sort the packed array.
Random word: pick random index, decode.
Membership: binary search, or two-letter indexed binary search, or bitset.
```

This gives you:

- compressed storage,
- fast random selection,
- fast exact membership testing,
- low CPU cost,
- low memory cost.