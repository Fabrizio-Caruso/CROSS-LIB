#!/usr/bin/env python

# 	../tools/generic/add_BASIC_header.py a40.bin a40.prg
#	../tools/generic/CC1541/cc1541.exe -f xl -w a40.prg X$(GAME_NAME)_$@.d64

import sys

c128_header = b'\x01\x1c\x0B\x1c\x13\x02\x9E\x37\x31\x38\x31\x00\x00\x00'   # 2 byte start-address + 12-byte header


def add_header(raw_file, full_file, header):
    with open(full_file, 'wb') as out, open(raw_file, 'rb') as inp:
        # inp.read(3)            # skip 3 bytes from source file
        out.write(header)      # write header
        out.write(inp.read())  # copy the source file
        

if __name__ == "__main__":
    params = sys.argv
    raw_file = params[1]
    full_file = params[2]
    print("raw_file: " + raw_file)
    print("full_file: " + full_file)
    add_header(raw_file, full_file, c128_header)