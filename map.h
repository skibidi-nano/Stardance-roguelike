#ifndef MAP_H
#define MAP_H

#include "config.h"
#include "npc.h"

typedef struct
{
    int dir_x;
    int dir_y;
} 
direction;

void map_init(mapstate current_mapstate);
void map_draw(mapstate current_mapstate);
void visible_map_init(int player_y, int player_x);
void map_refresh(int player_refresh_y, int player_refresh_x, mapstate current_mapstate);

int map_is_wall(int y, int x);
int map_is_door(int y, int x, mapstate *current_mapstate);
int map_is_enemy(int y, int x);
int map_is_item(int y, int x);

void map_remove_enemy_at(int y, int x);
void map_remove_item_at(int y, int x);

void enemy_pursuit(int player_x, int player_y, mapstate current_mapstate);
int signum(int input);

void boss_item_creation(void);

npc* position_of_enemy_array(void);
int* position_of_room_counter(void);
int* position_of_boss_counter(void);
int value_of_boss_counter(void);
char value_of_part_of_map(int y, int x);
#endif