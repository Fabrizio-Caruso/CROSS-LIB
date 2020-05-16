
# SUPPORTED COMPILERS

The program is written in ANSI C and currently compiled with 
- CC65 for all MOS 6502 targets (http://www.cc65.org/), 
- SCCZ80 (from Z88DK) for most Zilog 80 and Intel 8080 targets (https://www.z88dk.org/), 
- ZSDCC (from Z88DK) for some Zilog 80 and Intel 8080 targets (https://www.z88dk.org/),
- CMOC for Motorola 6809 targets (https://perso.b2b2c.ca/~sarrazip/dev/cmoc.html),
- LCC1802 for COSMAC RCA 1802 targets (https://sites.google.com/site/lcc1802/downloads),
- ACK for PC 8088/8086, CP/M Intel 8080, Linux 386/68K/PPC/MIPS, PDP11 (https://github.com/davidgiven/ack),
- XTC68 (C68) for the Motorola 68008-based Sinclair QL (https://github.com/stronnag/xtc68),
- Z8KGCC for the Zilog Z8000-based Olivetti M20 series (http://www.z80ne.com/m20/sections/download/z8kgcc/z8kgcc.html),
- "GCC for Atari ST" (http://vincent.riviere.free.fr/soft/m68k-atari-mint/),
- "GCC for Amiga" (https://github.com/bebbo/amiga-gcc),
- "GCC for TI" for the Texas TI99/4a (http://atariage.com/forums/topic/164295-gcc-for-the-ti/page-6),
- GCC for modern 32/64-bit PC under any operating systems 

Previous versions used 
- WinCMOC for 6809 targets  (https://sourceforge.net/projects/cmoc-win32/).
WinCMOC has been abandoned because it uses an old CMOC version that has broken pointer to functions and because it is no longer maintained by its author. 

For other targets, the following compilers and dev-kits may be included in future versions of the tool-chain:
- QDOS-GCC for the Sinclair QL (http://morloch.hd.free.fr/qdos/qdosgcc.html)
- SDCC for Z80 and other targets (http://sdcc.sourceforge.net/),
- PVSNESLIB for the Super Nintendo Entertainment System (https://github.com/alekmaul/pvsneslib),
- amiga-os-toolchain for all Amigas (https://github.com/cahirwpz/amigaos-cross-toolchain),
- GCC6809 for other 6809 targets (https://sourceforge.net/projects/freewpc/files/GCC-6809/),
- TIGCC for 68k-based TI calculators (http://tigcc.ticalc.org/).

The tool-chain also uses some third-party target-specific tools to create usable cassette/disk/rom images.
This is documented in https://github.com/Fabrizio-Caruso/CROSS-CHASE/blob/master/tools/README.txt