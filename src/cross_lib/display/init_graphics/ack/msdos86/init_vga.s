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


! int kb_poll() returns ASCII code or 0 if none
.define _kb_poll
_kb_poll:

    push bp
    mov  bp, sp

    movb  ah, 0x01      ! check for key
    int  0x16
    jz   no_key         ! ZF=1  no key

    movb  ah, 0x00      ! read key (returns ASCII in AL)
    int  0x16
    movb  ah, 0         ! zero-extend
    jmp  done

no_key:
    xor  ax, ax         ! return 0

done:
    pop  bp
    ret



