.sect .text
.sect .rom
.sect .data
.sect .bss

.sect .text

.define _cls
_cls:
    push es
    mov ax,0xA000
    mov es,ax
    mov di,0x0000
    sub ax,ax
    mov cx,32000
    rep stosw
    pop es
    ret

.define _init_vga
_init_vga:
    mov ax, 0x13
    int 0x10
    ret
    
.define _video_mode
_video_mode:
    mov ax, 19
    int 16
    ret


.define _text_mode
_text_mode:
    mov ax, 3
    int 16
    ret

.define _test
_test:
    mov ax, 0x0b800
    mov ds, ax         
    movb cl, 65        
    movb ch, 223        
    mov bx, 0x15E      
    mov (bx), cx       
    ret                

! PlotPixel_STOSB:
!   CX = x (0..319)
!   DX = y (0..199)
!   AL = color (0..255)
! Uses STOSB to write the pixel
! Registers preserved.
.define _plot
_plot:
    push    bp
    mov     bp, sp
    push    ax
    push    bx
    push    cx
    push    dx
    push    di
    
    ! mov bx, sp
    mov cx, 4(bp)
    mov dx, 6(bp)


    ! Set ES = A000h (VGA memory)
    mov     ax, 0xA000
    mov     es, ax


    ! Compute offset = y*320 + x
    mov     ax, dx       ! AX = y
    mov     bx, 320
    mul     bx           ! AX = y*320   (fits in 16 bits)
    add     ax, cx       ! AX = y*320 + x
    mov     di, ax       ! DI = offset

    movb al, 8(bp)       ! color

    ! AL already contains color
    ! Write pixel using STOSB
    stosb                 ! [ES:DI] = AL, increments DI

    pop     di
    pop     dx
    pop     cx
    pop     bx
    pop     ax
    pop     bp
    ret


.define _write_tile
_write_tile:
    mov bp, sp
    push es
    xor ax, ax
    movb al, 6(bp)      ! y
    mov di, ax
    shl ax, 1
    shl ax, 1
    add ax, di          ! ax = y*5
    movb cl, 6
    shl ax, cl          ! ax = y*320
    mov di, ax
    xor ax, ax
    movb al, 4(bp)     ! ax = x
    add ax, di          ! ax = y*320+x
    movb cl, 3
    shl ax, cl          ! ax = (y*8)*320 + x*8
    mov di, ax
    mov si, 12(bp)       ! tile base pointer
    xor ax, ax
    movb al, 8(bp)      ! tile index
    shl ax, cl          ! cl was still 3, ax = idx*8 - 8 bytes per tile
    mov si, ax
    mov ax, 0xa000
    push ax
    pop es
    xor ax, ax
    movb al, 10(bp)      ! ah = 0, al = color
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
    ! movb al, 0x1F
    ! eseg movb (es), ah
    ! eseg movb (si), al
    stosb               ! write color and increment di
    loop inner_loop     ! loop until cx = 0
  out_inner_loop:
    pop cx
    add di, 312         ! next line
    inc si              ! next byte in input
    loop outer_loop
    pop es
    ret
