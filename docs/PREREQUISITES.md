## Prerequisites

- [Non-UNIX only] Install a POSIX environment if necessary, e.g., Cygwin.
- Install `make` in your environment.
- Install the required cross-compilers (see list of supported cross-compilers above).
- [Only some compilers] Install the additional requirements for a given cross-compiler, e.g., CMOC and LCC1802 require a separate linker, some GCC targets require "ncurses".
- [Only some compilers] Make sure they are either installed as described in the Makefile variables: `Z88DK_PATH`, `CC65_PATH`, `LCC1802_PATH`, etc. or set these variables accordingly.
- [Z88DK-only] Select the optimization level in Makefile by setting the variables: `SCZZ80_OPTS` (`-O3` for highest optimizations), `ZSDCC_OPTS` (`-SO3` for high optimizations; `-SO3 --max-allocs-per-node200000` for highest and extremely slow optimizations).
