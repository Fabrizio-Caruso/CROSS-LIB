## CROSSLIB

For each target, at least one of the following modes has to be implemented:
1. memory-mapped graphics (display can be implemented by writing bytes into video memory)
2. conio-like (display is performed by either direct conio APIs or a wrapper that provides conio APIs, e.g., ADM3/A, VT52, VT100, etc.)
3. TGI (CC65-specific graphics APIs)
4. Z88DK Sprites (Z88DK-specific sprite APIs)
5. CPCRSLIB (Amstrad CPC-specific graphics APIs)
6. bit-mapped (display is implemented by writing into video memory and each bytes corresponds to one or more pixels)
7. buffered (stdlib-only "graphics" for targets with just stdlib support)

A graphic mode may implement the concrete graphics by either calling low level graphics instructions (e.g., writing into video-memory) or by calling higher level instructions (e.g., using control codes).

For example for a CPM target with ADM3/A control codes, we use
1. CrossLib APIs that call
2. conio.h APIs that call
3. ADM3/A control codes

For a target with direct or indirect video memory (e.g., Commodore 64, MSX, etc.), one may use:
1. CrossLib APIs that implement
2. Direct (or indirect) writes into video memory

Remark: For some targets multiple implementations are possible (C64 may use conio APIs as provided by C65, memory-mapped graphics, TGI APIs)
