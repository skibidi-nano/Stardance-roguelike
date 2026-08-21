#ifndef MAP_H
#define MAP_H

#include "config.h"

void map_init(void);
void map_draw(void);

int map_is_wall(int y, int x);
int map_is_door(int y, int x);
#endif