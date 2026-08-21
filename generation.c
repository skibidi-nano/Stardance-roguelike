#include <stdlib.h>
#include "config.h"
#include "generation.h"


room random_room_gen(void)
{
    room rndm_room;
    rndm_room.x = 0;
    rndm_room.y = 0;
    rndm_room.height = (rand() % (MAX_HEIGHT - MIN_HEIGHT + 1)) + MIN_HEIGHT;
    rndm_room.width = (rand() % (MAX_WIDTH - MIN_WIDTH + 1)) + MIN_WIDTH;

    return rndm_room;
}