#include <stdlib.h>
#include "config.h"
#include "generation.h"

int get_random_int(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

//Generates the info for random rooms (width, height and corner coordinates)
room random_room_gen(void)
{
    //initialize the room variables
    room rndm_room;
    rndm_room.x = 0;
    rndm_room.y = 0;

    //calculate room size
    rndm_room.height = (rand() % (MAX_HEIGHT - MIN_HEIGHT + 1)) + MIN_HEIGHT;
    rndm_room.width = (rand() % (MAX_WIDTH - MIN_WIDTH + 1)) + MIN_WIDTH;

    //choose random wall (0 for north wall, 1 for south wall, 2 for west wall, 3 for east wall)
    int wall_side = get_random_int(0, 3);
    
    //Door position selection
    switch (wall_side)
    {
        case 0: //North wall
            rndm_room.door_y = 0;
            rndm_room.door_x = get_random_int(1, rndm_room.width - 2);
            break;

        case 1: //South wall
            rndm_room.door_y = rndm_room.height - 1;
            rndm_room.door_x = get_random_int(1, rndm_room.width - 2);
            break;

        case 2: //West wall
            rndm_room.door_x = 0;

            rndm_room.door_y = get_random_int(1, rndm_room.height - 2);
            break;

        case 3: //East wall
            rndm_room.door_x = rndm_room.width - 1;
            rndm_room.door_y = get_random_int(1, rndm_room.height - 2);
            break;
    }
    return rndm_room;
}
