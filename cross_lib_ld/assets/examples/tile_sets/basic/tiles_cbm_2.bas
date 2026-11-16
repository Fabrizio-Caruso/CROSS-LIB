1 goto800
2 poke53280,6:poke53281,1
5 s=1304:ex=1:l=3:sc=0:so=54272
10 pokeso+24,15:pokeso+5,17:pokeso+6,16:pokeso,100
20 tc=27:poke251,112:poke831,0:poke832,6:poke829,20
30 tb(1)=112
40 tb(2)=197
50 ss$="{right*19}{space*3}{down}{left*4}{space*5}{down}{left*3} "
60 sc$="{right*19}{green}###{down}{left*4}{blue}#####{down}{left*3}$"
70 poke53272,(peek(53272)and240)+12
80 t(1)=38:t(2)=40
90 print"{clear}{down*2}{red}########################################";
91 print"{green}###################{space*3}##################";
92 print"{blue}##################{space*5}#################";
100 fori=16to1step-1
110 print"{home}{down*3}";:fort=1toi:print"{down}";:next:printsc$
120 fory=1to75:next
130 print"{home}{down*3}";:fort=1toi:print"{down}";:next:printss$
140 next
150 print"{clear}{down*2}{red}########################################";
151 print"{green}########################################";
152 print"{blue}########################################";
160 print"{home}{down}++++++++++++++++++++++++++++++++++++++++"
170 print"{home}{down*23},,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,";
180 tt=1:d=-1:ao=40:mp=1244:mc=55516:tp=19:ba=121:z=0:poke834,tp
185 w=int(rnd(1)*39)+1104
190 poke251,tb(tt):sys49152
200 print"{home}{down*16}-------{space*26}-------"
210 print"{home}{space*6}punti=";sc:print"{home}{right*25}clown=";l
220 pokemp,36:pokemc,0
230 gosub750:iffbthen240
235 goto230
240 u=int(rnd(1)*10)
244 ifz=0andu>8thenz=7:goto248
246 ifz=7andu>7thenz=0:goto248
248 ifpeek(w)=35thenpokew+so,z
290 poke251,tb(tt):sys49434:tp=peek(834)
310 ifd=-1then330
320 ifd=1then410
325 goto310
330 op=mp:mp=mp+ao:mc=mp+so:pm=peek(mp)
335 if((op-1023)/40=int((op-1023)/40))andao=41thenmp=mp-40:pm=peek(mp):mc=mp+so
340 ifpm=32thengoto406
350 ifpm=35thengosub560:goto406
370 ifpm=t(tt)then460
380 ifpm=43orpm=45thenmp=op:d=1:ao=int(rnd(1)*3)+39:goto240
400 l=l-1:ifl<=0then630
404 goto180
406 pokeop,32:pokemp,36:pokemc,0:goto240
410 op=mp:mp=mp-ao:mc=mp+so:pm=peek(mp)
415 if((op-1024)/40=int((op-1024)/40))andao=41thenmp=mp+40:pm=peek(mp):mc=mp+so
420 ifpm=32then450
430 ifpm=35thend=-1:gosub560:goto450
435 ifpm=45thenpokeop,32:mp=mp-ao:mc=mp+so
440 ifpm=43thenmp=op:d=-1:ao=int(rnd(1)*3)+39:goto240
450 pokeop,32:pokemp,36:pokemc,0:goto240
460 sc=sc+5:pokeop,32
480 pokeso+1,10:pokeso+4,33
490 poke251,tc:sys49152
500 pokeso+4,32:iftt=1thenmp=1787+tp:mc=mp+so
510 iftt=2thenmp=1785+tp:mc=mp+so
520 tt=tt+1:iftt>2thentt=1
530 d=1
540 poke251,tb(tt):sys49152
550 ao=int(rnd(1)*3)+39
555 print"{home}{space*6}punti=";sc:print"{home}{right*25}clown=";l:goto240
560 pokeso+1,10:pokeso+4,129:fori=1to10:next:pokeso+4,128
563 ifmp=wandc=0thenl=l-1:goto180
566 ifmp=wandc=7thensc=sc+250:goto600
570 ifmp>1103andmp<1144thensc=sc+100
580 ifmp>1143andmp<1184thensc=sc+75
590 ifmp>1183andmp<1224thensc=sc+50
600 ifsc>ex*2000thenl=l+1:ex=ex+1
605 ba=ba-1:ifba=1then180
610 ao=int(rnd(1)*3)+39
620 print"{home}{space*6}punti=";sc:print"{home}{right*25}clown=";l:return
630 g$=" p a r t i t a   c o n c l u s a "
640 fori=2to34step2
650 print"{home}{down*9}";tab(i+2);mid$(g$,i,1)
660 fort=1to100:next:next
670 fori=1to250:next
680 print"{space*2}premi il pulsante per giocare ancora"
690 print"{space*3}spingi il joystick verso il basso"
695 print"{space*4}per terminare"
700 gosub750
710 iffbthenclr:goto2
720 ifj1thensys2048:end
730 goto700
750 p=peek(56320)and15
760 j1=-(p=13)
770 fb=-((peek(56320)and16)=0):return
800 poke53280,6:poke53281,1
810 print"{clear}{down*12}{space*17}{red}c{cyan}i{purple}r{green}c{blue}o{black}":c=0
820 print"{down}{space*12}attendere prego...":goto1310
830 gosub10000:poke53272,(peek(53272)and240)+12
840 print"{clear}{down*12}{right*17}{black}circo"
850 print"{home}{down*10}{right*15}#########"
855 print"{down*3}{right*15}#########"
860 print"{home}{down*11}{right*15}#{down}{left}#{down}{left}#{up*2}{right*7}#{down}{left}#{down}{left}#"
880 fort=1to3:forp=1to7:s=p
890 fori=55711to55720:pokei,s
900 s=s+1:ifs=1thens=2
910 ifs>7thens=0
920 next
930 fori=55760to55840step40:pokei,s
940 s=s+1:ifs=1thens=2
950 ifs>7thens=0
960 next
970 fori=55880to55871step-1:pokei,s
980 s=s+1:ifs=1thens=2
990 ifs>7thens=0
1000 next
1010 fori=55831to55751step-40:pokei,s
1020 s=s+1:ifs-1thens=2
1030 ifs>7thens=0
1040 next
1050 nextp,t
1060 poke53272,21:print"{clear}{down*12}";
1070 print"{space*5}vuoi le istruzioni? (s/n)"
1080 geta$:ifa$=""then1080
1090 ifa$="s"then1120
1100 ifa$="n"then1500
1110 goto1080
1120 printchr$(14);"{clear}{down}questo e' il gioco del circo."
1130 print"{down}lo scopo del gioco consiste nel far"
1135 print"scoppiare tutti i palloncini."
1140 print"{down}cio' e' possibile muovendo il joystick"
1145 print"a sinistra e a destra in modo da pren-"
1150 print"dere al volo il clown"
1153 print"{down}all'inizio premere il pulsarte di sparo"
1155 print"per dare il via al gioco."
1160 print"{home}{down*20} premi qualsiasi tasto"
1170 geta$:ifa$=""then1170
1180 print"{clear}{down}tutti i blocchi a scacchi fanno rimbal- zare il vostro giocatore"
1190 print"{down}ogni 2000 punti verra' concesso un uomo in piu'."
1200 print"{down}anche il blocco mobile permettera' al {space*3}vostro giocatore di";
1205 print" rimbalzare."
1210 print"{home}{down*22} premi qualsiasi tasto"
1220 geta$:ifa$=""then1220
1230 print"{clear}{down}dovete far attenzione al palloncino"
1235 print"di diverso colore della riga superiore."
1240 print"{down}quando il palloncino e' nero, scoppiera'"
1250 print"{up}in faccia al vostro giocatore, quando e'"
1260 print"{up}giallo vi dara' 250 punti"
1270 print"{down*2}buona fortuna!!!"
1280 print"{home}{down*22} premi qualsiasi tasto"
1290 geta$:ifa$=""then1290
1300 goto1500
1310 poke52,48:poke56,48:clr
1320 cs=12288:poke56334,peek(56334)and254:poke1,peek(1)and251
1330 fori=cstocs+511:pokei,peek(i+40960):next
1340 poke1,peek(1)or4:poke56334,peek(56334)or1
1360 fori=cs+35*8tocs+46*8-1:readj:pokei,j:a=a+j:next
1365 ifa<>6897thenprint"errore nei dati alle righe 1380-1480":end
1370 goto830
1380 data28,62,47,63,63,126,96,0
1390 data58,58,18,124,16,56,68,68
1400 data128,64,32,16,24,28,38,37
1410 data128,64,32,16,8,4,2,1
1420 data1,2,4,8,24,56,100,164
1430 data1,2,4,8,16,32,64,128
1440 data0,0,0,255,24,24,36,36
1450 data0,0,0,255,0,0,0,0
1460 data170,85,170,85,170,85,170,85
1470 data255,255,255,255,255,255,255,255
1480 data170,85,170,85,170,85,170,85
1500 print"{clear}":printchr$(17):goto2
10000 i=49152
10010 reada:ifa=256thengoto10025
10020 pokei,a:i=i+1:c=c+a:goto10010
10025 ifc<>21810thenprint"errore nei dati alle righe 10030-10580":end
10028 return
10030 data172,66,3,174,61,3,24
10040 data32,240,255,169,192,133,252
10050 data160,0,177,251,32,210,255
10060 data200,192,85,208,246,96,32
10070 data36,32,36,32,32,32,32
10080 data32,32,32,32,32,32,32
10090 data32,32,32,32,32,32,32
