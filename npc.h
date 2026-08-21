#ifndef NPC_H
#define NPC_H

typedef struct
{
    int npc_y;
    int npc_x;
}
npc;

npc npc_init(int map_width, int map_height);

#endif