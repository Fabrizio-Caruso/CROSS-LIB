


# BUILD INSTRUCTIONS

If you just want to build the game only for a specific system without choosing a specific version, 
you can build the default target for a given system with

`make [system_name] -f Makefile.[game name]`

where current possible game names are `chase`, `shoot`, `bomber`. For *Cross Chase* you can always omit `-f Makefile.chase`.

For the list of systems either run `make list` or look at 
https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/SYSTEMS.md

The following sections are meant for *Cross Chase* and so the commands can omit `-f Makefile.[game name]`.
For the other games the same target names are used but fewer targets exist.

## Compile a specific target
For this you may need to look inside the Makefile file to have the exact target name to use with the "make" command. 
The name of the default target for a given system is given by `[system name]`. For the list of systems either run `make list` or look at 
https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/SYSTEMS.md

Examples: 
- `make c64` builds a Commodore 64 binary.
- `make nes` builds a binary for the Nintendo NES videogame console.
- `make ti83` builds a binary for the Texas Instrument TI 83 scientific calculator.

The name of the target is given by the `[system name]_[optional attribute(s)]`.
The most common attributes are:
- memory (e.g., `exp`, `3k`, `8k`, `16k`, `32k`, etc.);
- target video mode (e.g., `40col`, `80col`, `wrx`, `adm3a`, `vt52`, `vt100`, etc.);
- display mode (e.g., `buffered`, `udg`, etc.);
- CPU used for models with more than one CPU architecture (e.g., for the Commodore 128 you may have `z80` or `8502`);
- game mode (`turnbased`);
- input mode (e.g., `qaopm`, `ijkl`, `joystick`, etc.)
- binary format (e.g., `wav`, `crt`, `cmd`, etc.)

Examples: 
- `make gcc_buffered` builds a GCC buffered and turn-based version for the host console.
- `make pc8086` builds a binary for the PC 8086/8088 (a floppy disk image to be run on a Intel 8086/8088 PC).
- `make vic20_exp_8k` builds a binary for the Commodore Vic 20 with an 8k memory expansion (`exp` means that the expansion brings 8k of "additional" memory)


## Compile all targets for a given system 
Run `make [system name]_targets`, where [system name] is one of the systems listed in link in the section "SUPPORTED TARGETS " of this document.

Examples:
- `make vic20_targets` builds all Commodore Vic 20 binaries (different versions of the game for different memory configurations).
- `make spectrum_targets` builds all Sinclair Spectrum binaries (different versions of the game for different memory configurations).
- `make zx81_targets` builds all ZX81 binaries (different versions of the game for different memory configurations and different graphics configurations such as WRX hi-res).



## Compile all targets for a given cross-compiler
Run `make [compiler name]_targets` where the [compiler name] is of one the supported compilers and dev-kits as shown in: https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/docs/COMPILER_NAMES.md

Examples: 
- `make cmoc_targets` builds all targets that are built with the CMOC cross-compiler for the Motorola 6809 systems.
- `make gcc_targets` builds all targets by using GCC for the host console (e.g., CYGWIN, Linux, etc. console).
- `make cc65_targets` builds all targets that are built with the CC65 cross-compiler for the MOS 6502 systems.
- `make z88dk_targets` [EXTREMELY SLOW] builds all targets that are built with the SCCZ80 and ZSDCC cross-compilers of the Z88DK dev-kit for Zilog 80 and Intel 8080 systems.


## Special cases
Some targets, e.g., CP/M and Commodore 128, can be built by two different compilers because they can run different CPU architectures or because we may want to have multiple versions.
In such cases we can specify the version or architecture:

Examples:
- `make c128_targets` builds all Commodore 128 targets for both the MOS 8502 and for the more exotic Zilog 80 non-CP/M mode.
- `make c128_8502_targets` builds all Commodore 128 targets for the MOS 8502 architecture in both 40 and 80 column mode.
- `make c128_z80_targets` builds all Commodore 128 targets for the Zilog 80 non-CP/M mode in both 40 and 80 column mode.
- `make cpm_targets` builds all "generic" CP/M targets for both the Intel 8080 and Zilog 80 architecture.
- `make cpm_8080_targets` builds all "generic" CP/M targets for the Intel 8080 architecture (compatible with Zilog 80 systems).
- `make cpm_z80_targets` builds all "generic" CP/M targets for both the Zilog 80 architecture (not compatible with Intel 8080 systems).
