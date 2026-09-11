#ifndef MAP_H
#define MAP_H

#include "config.h"
#include "npc.h"

void map_init(mapstate current_mapstate);
void map_draw(void);

int map_is_wall(int y, int x);
int map_is_door(int y, int x);
int map_is_enemy(int y, int x);
int map_is_item(int y, int x);

npc* position_of_enemy_array(void);
char value_of_part_of_map(int y, int x);
int* position_of_room_counter(void);

void map_remove_enemy_at(int y, int x);

void map_remove_item_at(int y, int x);
#endif