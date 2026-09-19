#ifndef NPC_H
#define NPC_H

#include "entity.h"

typedef struct
{
    int npc_y;
    int npc_x;
    int number;
    type npc_type;
    bool active;
    bool direction;
}
npc;

npc npc_init(int map_width, int map_height, type enemy_type, int number, bool state);
npc boss_init(void);
entity standard_enemy_stats(int max, int current, int power);

void address_enemy_sprite(void);
void modulo_enemy_sprite(void);
void boss_sprite(void);

#endif