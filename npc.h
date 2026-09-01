#ifndef NPC_H
#define NPC_H

#include "entity.h"

typedef struct
{
    int npc_y;
    int npc_x;
    int number;
    bool active;
}
npc;

npc npc_init(int map_width, int map_height, int number, bool state);
npc npc_kill(npc enemy, int current_enemy_count);
entity standard_enemy_stats(int max, int current, int power);
void standard_enemy_sprite(void);

#endif