#include "npc.h"
#include "generation.h"
#include <stdlib.h>

npc npc_init(int map_width, int map_height)
{
    npc enemy;
    
    // npc creation (everywhere but 1|1)
    do {
        enemy.npc_x = get_random_int(1, map_width - 2);
        enemy.npc_y = get_random_int(1, map_height - 2);
    } while (enemy.npc_x == 1 && enemy.npc_y == 1);

    return enemy;
}