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
    

.define _write_tile
_write_tile:
    push bp
    mov bp, sp
    push es
    xor ax, ax
    mov al, [bp+8] ; y
    mov di, ax
    shl ax, 1
    shl ax, 1
    add ax, di ; ax = y*5
    mov cl, 6
    shl ax, cl ; ax = y*320
    xor ax, ax
    mov al, [bp+10]  ; ax = x
    add ax, di ; ax = y*320+x
    mov cl, 3
    shl ax, cl ; ax = (y*8)*320 + x*8
    mov di, ax
    mov si, [bp+2] ; tile base pointer
    xor ax, ax
    mov al, [bp+6]
    shl ax, cl ; cl was still 3, ax = idx*8 - 8 bytes per tile
    mov si, ax
    mov ax, 0a000h
    push ax
    pop es
    xor ax, ax
    mov al, [bp+4] ; ah = 0, al = color
    mov cx, 8 ; vertical loop
  outer_loop:
    push cx
    mov cx, 8 ; horizontal loop
    mov bl, [si]
  inner_loop:
    shl bl, 1
    jc write_label
    mov [es:di], ah ; write 0
    inc di
    loop inner_loop ; loop until cx = 0
    jmp out_inner_loop
  write_label:
    stosb ; write color and increment di
    loop inner_loop ; loop until cx = 0
  out_inner_loop:
    pop cx
    add di, 312 ; next line
    inc si ; next byte in input
    loop outer_loop
    pop es
    pop bp
    ret
