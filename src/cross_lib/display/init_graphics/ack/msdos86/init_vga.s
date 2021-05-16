.sect .text
.sect .rom
.sect .data
.sect .bss

.sect .text


.define _init_vga
_init_vga:
    mov ax, 0x0013
    int 0x10
    ret

