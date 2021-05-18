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
    mov bx, sp
    push es
    xor ax, ax
    movb al, 8(bx)      ! y
    mov di, ax
    shl ax, 1
    shl ax, 1
    add ax, di          ! ax = y*5
    movb cl, 6
    shl ax, cl
    xor ax, ax
    movb al, 10(bx)     ! ax = x
    add ax, di          ! ax = y*320+x
    movb cl, 3
    shl ax, cl          ! ax = (y*8)*320 + x*8
    mov di, ax
    mov si, 2(bx)       ! tile base pointer
    xor ax, ax
    movb al, 6(bx)      ! tile index
    shl ax, cl          ! cl was still 3, ax = idx*8 - 8 bytes per tile
    mov si, ax
    mov ax, 0xa000
    push ax
    pop es
    xor ax, ax
    movb al, 4(bx)      ! ah = 0, al = color
    mov cx, 8           ! vertical loop
  outer_loop:
    push cx
    mov cx, 8           ! horizontal loop
    movb bl, (si)
  inner_loop:
    shlb bl, 1
    jc write_label
    eseg movb (di), ah  ! write 0
    inc di
    loop inner_loop     ! loop until cx = 0
    jmp out_inner_loop
  write_label:
    stosb               ! write color and increment di
    loop inner_loop     ! loop until cx = 0
  out_inner_loop:
    pop cx
    add di, 312         ! next line
    inc si              ! next byte in input
    loop outer_loop
    pop es
    ret
