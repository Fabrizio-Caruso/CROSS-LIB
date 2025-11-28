.sect .text
.sect .rom
.sect .data
.sect .bss

.sect .text

.define __cls
__cls:
    mov ax, 0x0013   ! VGA 320x200x256
    int 0x10          ! BIOS sets the mode and clears the screen
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

.define __wait_for_key
__wait_for_key:
    movb ah, 0x00    ! function: wait for key
    int 0x16         ! returns key in AX
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
!    push    ax
!    push    bx
!    push    cx
!    push    dx
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
!    pop     dx
!    pop     cx
!    pop     bx
!    pop     ax
    pop     bp
    ret


.define __kb_poll
__kb_poll:
    inb 0x60
    ret

! int kb_poll_buffer() returns ASCII code or 0 if none
.define __kb_poll_buffer
__kb_poll_buffer:

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



