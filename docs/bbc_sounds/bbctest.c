#include <stdio.h>

#include "bbc.h"

bbc_sound snd = {1,-15,200,20};

main()
{
  unsigned int i=0;
  puts("playing sound");
  OSSOUND(&snd);
  puts("done");
  while(1){
    printf("k=%03u js=%05u %05u %05u %05u\n",OSSCANKEY16(),OSADVAL(1),OSADVAL(2),OSADVAL(3),OSADVAL(4));
  }
}

