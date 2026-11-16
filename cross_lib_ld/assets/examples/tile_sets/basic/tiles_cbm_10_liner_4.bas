0v=36878:t=30720:pokev+1,110:b=7816:c=b+12:z=65:q=.:g=160:dimc(g):pokev-9,255:data17,85
1fori=1to16:reade:poke7167+i,255-e:next:s$="{home}{down*3}{reverse on}        {white}step{yellow}":data17,,254,,,70,136
2s=3:fori=.to3:p(i)=1:p(6-i)=0:next:c(0)=3:c(1)=5:print"{clear}{reverse on}  {cyan}{reverse off}@{green}a{reverse on} {white}vic {green}fruzzle {cyan}{reverse off}@{green}a"
3h=42:q=.:p(s)=g:print"{down*18}{reverse on}{yellow}    use {white}abcdefg{down}"spc(11)"{yellow}reset with {white}x"
4e=22:fori=.to6:poke8036+i*2,i+129:forj=1to8:pokeb+e*j+i*2,230:pokeb+e*j+i*2+t,7:next:next
5fori=.to6:pokeb+t+i*2,c(p(i)):pokeb+i*2,p(i):next:prints$;q;:d$="{down}done!":data170,136,,127,,
6geta$:on-(a$="")goto6:f=asc(a$)-z:on-(f=23)goto2:on-(f<.orf>6orf=sors<.ors>6)goto6:data98
7x=peek(b+f*2):on-((x=1)and(((s-f)>2)or(f>s)))goto6:on-((x=0)and(((f-s)>2)or(s>f)))goto6
8pokeb+f*2,g:q=q+1:j=b+f*2-22-sgn(f-s):pokej+t,c(x):pokej,x:pokev,9:pokev,0:p(s)=x:p(f)=g
9k=tan(9.9):s=f:pokej,g:on-(p(4)*p(5)*p(6)<>1orp(3)<2)goto5:print"{home}"spc(9)d$:pokev+1,h:goto5