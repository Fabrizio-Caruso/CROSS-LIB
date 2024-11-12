#include "bbc.h"

// typedef struct {
  // int channel;
  // int amplitude;
  // int pitch;
  // int duration;
// } bbc_sound;

bbc_sound ping_snd =      {1,-15,100,2};

bbc_sound tick_snd =      {1,-15,50,1};
bbc_sound tock_snd =      {1,-15,30,1};

bbc_sound shoot_snd =     {0,-15,40,1};
bbc_sound explosion_snd = {0,-15,20,2};