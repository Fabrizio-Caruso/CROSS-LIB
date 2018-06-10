#include <coco.h>

int main()
{
  unsigned char i;

  unsigned short points = 0;

  for(i=0;i<20;++i)
  {
    points += 200u;

    printf("points: %u\n", points);

    sleep(1);
  }
  return 0;
}
