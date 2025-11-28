.sect .text
.sect .rom
.sect .data
.sect .bss

.sect .text

!.define __cls
!__cls:
!    push bp
!    push di
!    mov ax, 0x0013   ! VGA 320x200x256
!    int 0x10          ! BIOS sets the mode and clears the screen
!    pop di
!    pop bp
!    ret

.define __init_vga
__init_vga:
    push bp
    push di
    mov ax, 0x13
    int 0x10
    pop di
    pop bp
    ret
    
.define __video_mode
__video_mode:
    push bp
    push di
    mov ax, 19
    int 16
    pop di
    pop bp
    ret


!.define _text_mode
!_text_mode:
!    mov ax, 3
!    int 16
!    ret

.define __wait_for_key
__wait_for_key:
    push bp
    push di
    movb ah, 0x00    ! function: wait for key
    int 0x16         ! returns key in AX
    pop di
    pop bp
    ret
   

.define __delete_vga_pixel
__delete_vga_pixel:
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

    ! movb al, 0       ! color
    xor ax,ax

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

.define __delete_vga_segment
__delete_vga_segment:
    push    bp
    mov     bp, sp
!    push    ax
!    push    bx
    push    cx
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

    movb al, 0       ! color

    ! AL already contains color
    ! Write pixel using STOSB
    mov cx, 8
    rep stosb                 ! [ES:DI] = AL, increments DI
    
!    mov ax, di
!    add ax, 312
!    mov di, ax
!    movb al, 0
!    mov cx, 8
!    rep stosb                 ! [ES:DI] = AL, increments DI

!    mov ax, di
!    add ax, 312
!    mov di, ax
!    movb al, 0
!    mov cx, 8
!    rep stosb                 ! [ES:DI] = AL, increments DI

!    mov ax, di
!    add ax, 312
!    mov di, ax
!    movb al, 0
!    mov cx, 8
!    rep stosb                 ! [ES:DI] = AL, increments DI

!    mov ax, di
!    add ax, 312
!    mov di, ax
!    movb al, 0
!    mov cx, 8
!    rep stosb                 ! [ES:DI] = AL, increments DI

!    mov ax, di
!    add ax, 312
!    mov di, ax
!    movb al, 0
!    mov cx, 8
!    rep stosb                 ! [ES:DI] = AL, increments DI

!    mov ax, di
!    add ax, 312
!    mov di, ax
!    movb al, 0
!    mov cx, 8
!    rep stosb                 ! [ES:DI] = AL, increments DI

!    mov ax, di
!    add ax, 312
!    mov di, ax
!    movb al, 0
!    mov cx, 8
!    rep stosb                 ! [ES:DI] = AL, increments DI

    
!    stosb
!    stosb
!    stosb
!    stosb
!    stosb
!    stosb
!    stosb

    
    
    pop     di
!    pop     dx
    pop     cx
!    pop     bx
!    pop     ax
    pop     bp
    ret


! TODO: Fix this because it is still broken
.define __delete_vga_tile
__delete_vga_tile:
    push    bp
    mov     bp, sp
!    push    ax
!    push    bx
    push    cx
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

    movb al, 0       ! color

    ! AL already contains color
    ! Write pixel using STOSB
    mov cx, 8
    rep stosb                 ! [ES:DI] = AL, increments DI
   
    mov ax, di
    add ax, 312
    mov di, ax
    movb al, 0
    mov cx, 8
    rep stosb                 ! [ES:DI] = AL, increments DI

    mov ax, di
    add ax, 312
    mov di, ax
    movb al, 0
    mov cx, 8
    rep stosb                 ! [ES:DI] = AL, increments DI

    mov ax, di
    add ax, 312
    mov di, ax
    movb al, 0
    mov cx, 8
    rep stosb                 ! [ES:DI] = AL, increments DI

    mov ax, di
    add ax, 312
    mov di, ax
    movb al, 0
    mov cx, 8
    rep stosb                 ! [ES:DI] = AL, increments DI

    mov ax, di
    add ax, 312
    mov di, ax
    movb al, 0
    mov cx, 8
    rep stosb                 ! [ES:DI] = AL, increments DI

    mov ax, di
    add ax, 312
    mov di, ax
    movb al, 0
    mov cx, 8
    rep stosb                 ! [ES:DI] = AL, increments DI

    mov ax, di
    add ax, 312
    mov di, ax
    movb al, 0
    mov cx, 8
    rep stosb                 ! [ES:DI] = AL, increments DI

    
    
    pop     di
!    pop     dx
    pop     cx
!    pop     bx
!    pop     ax
    pop     bp
    ret


! PlotPixel_STOSB:
!   CX = x (0..319)
!   DX = y (0..199)
!   AL = color (0..255)
! Uses STOSB to write the pixel
! Registers preserved.
.define __plot_vga
__plot_vga:
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
    push di
    push bx

    movb bl, 0
CheckLoop:
    movb ah, 0x01
    int 0x16
    jz Done
    
    movb ah, 0x00
    int 0x16
    movb bl, al
    jmp CheckLoop
Done:
    movb ah, 0
    movb al, bl
    pop bx
    pop di
    pop bp
    ret




!    push bp

!    movb  ah, 0x01      ! check for key
!    int  0x16
!    jz   no_key         ! ZF=1  no key

!    movb  ah, 0x00      ! read key (returns ASCII in AL)
!    int  0x16
!    movb  ah, 0         ! zero-extend
!    jmp  done


!no_key:
!    xor  ax, ax         ! return 0

!done:
!    pop  bp
!    ret


.define __speaker_beep
__speaker_beep:
    movb     al, 0xB6       ! Channel 2, lobyte/hibyte, mode 3 (square wave)
    outb     0x43

    mov      ax, 1193
    outb     0x42        ! Low byte
    movb     al, ah
    outb     0x42        ! High byte

    ! Enable speaker (bits 0 and 1 of port 61h)
    inb      0x61
    orb      al, 0x03
    outb     0x61

    ! ---- Delay loop ----
    mov cx,  0xFFFF
.delay:
    loop .delay

    ! Disable speaker
    inb      0x61
    andb     al, 0xFC       ! clear bits 0 and 1
    outb     0x61
    ret
