10 PRINT TAB(50);CHR$(164);" Derik van Z\";CHR$(34);"utphen (jun94)"
20 PRINT "=== Dumping CP/M Tracks to a file ==="
30 PRINT 
40 SYMBOL AFTER 256
50 MEMORY &4000
60 DEFINT a-z
70 sector = &4000
80 track = &4001
90 buf = &4002
100 errno = &4004
101 'far address = &4005
102 'rsx name = &4008
110 readsec = &4100
120 POKE &4008,&84 ' RSX-Command for READ-SECTOR
130 READ progsize
140 FOR addr=readsec TO readsec+progsize-1
150 READ code
160 POKE addr,code
170 NEXT
180 PRINT "Insert a CP/M Diskette and press a key"
190 WHILE INKEY$="":WEND
200 ptr=&5000
210 FOR t = 0 TO 1
220 FOR s = &41 TO &49 
230 PRINT USING "Track=#   Sector=\\"+CHR$(13);t;HEX$(s);
240 POKE sector,s
250 POKE track,t
260 POKE buf,ptr MOD 256
270 POKE buf+1,ptr \ 256
280 CALL readsec
290 IF PEEK(errno)>0 THEN PRINT "Something went wrong! ... Aborted!":END
300 ptr = ptr + 512
310 NEXT
320 NEXT
330 PRINT "Insert a Diskette to save the CP/M image and press a key"
340 WHILE INKEY$="":WEND
350 PRINT "Writing to SYSTEM.CPM"
360 SAVE "system.cpm",b,&5000,18*512
370 DATA 46 
380 DATA &21,&08,&40,&cd,&d4,&bc,&d2,&28,&40,&22,&05,&40,&79,&32,&07,&40
390 DATA &1e,&00,&21,&01,&40,&56,&21,&00,&40,&4e,&2a,&02,&40,&df,&05,&40
400 DATA &d2,&28,&40,&af,&32,&04,&40,&c9,&3e,&01,&32,&04,&40,&c9
405 '
410 ' The MC program is the following:
420 ' 
430 ' org       &4000
440 ' sector:   db      0
450 ' track:    db      0
460 ' mem:      dw      0
470 ' errno:    db      0
480 ' faraddr:  dw      0
490 '           db      0
500 ' name:     db      &84
510 ' 
520 ' kl-find-command = &BCD4
530 ' 
540 ' org       &4100
550 ' 21 08 40  ld      hl,name
560 ' CD D4 BC  call    kl-find-command
570 ' D2 28 40  jp      nc,error
580 ' 22 05 40  ld      (faraddr),hl
590 ' 79        ld      a,c
600 ' 32 07 40  ld      (faraddr+2),a
610 ' 1E 00     ld      e,0
620 ' 21 01 40  ld      hl,track
630 ' 56        ld      d,(hl)
640 ' 21 00 40  ld      hl,sector
650 ' 4E        ld      c,(hl)
660 ' 2A 02 40  ld      hl,(mem)
670 ' DF        rst     3
680 ' 05 40     dw      faraddr
690 ' D2 28 40  jp      nc,error
700 ' AF        xor     a
710 ' 32 04 40  ld      (errno),a
720 ' C9        ret
730 ' error:
740 ' 3E 01     ld      a,1
750 ' 32 04 40  ld      (errno),a
760 ' C9        ret 
