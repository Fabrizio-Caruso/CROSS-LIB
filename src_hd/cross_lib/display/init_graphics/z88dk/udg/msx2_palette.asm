MSX1palette:
        ld      bc,$99

        xor     a               ; Set color 0 ...
        di
        out     (c),a
        ld      a,80h+16        ; ...into register 16 (+80h)
        out     (c),a

        inc     c               ; C= CPU port connected to the VDP writing port #2
        ld      b,32
        ld      hl,MSX1paletteData
        otir
        ei
        ret

MSX1paletteData:
        db      00h,0   ; Color 0
        db      00h,0   ; Color 1
        db      11h,5   ; Color 2
        db      33h,6   ; Color 3
        db      26h,2   ; Color 4
        db      37h,3   ; Color 5
        db      52h,2   ; Color 6
        db      27h,6   ; Color 7
        db      62h,2   ; Color 8
        db      63h,3   ; Color 9
        db      52h,5   ; Color A
        db      63h,6   ; Color B
        db      11h,4   ; Color C
        db      55h,2   ; Color D
        db      55h,5   ; Color E
        db      77h,7   ; Color F



; MSX1paletteData:
        ; db      00h,0   ; Color 0
        ; db      00h,0   ; Color 1
        ; db      11h,5   ; Color 2
        ; db      33h,6   ; Color 3
        ; db      26h,2   ; Color 4
        ; db      37h,3   ; Color 5
        ; db      52h,2   ; Color 6
        ; db      27h,6   ; Color 7
        ; db      62h,2   ; Color 8
        ; db      63h,3   ; Color 9
        ; db      52h,5   ; Color A
        ; db      63h,6   ; Color B
        ; db      11h,4   ; Color C
        ; db      55h,2   ; Color D
        ; db      55h,5   ; Color E
        ; db      77h,7   ; Color F
