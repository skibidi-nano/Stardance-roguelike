#ifndef ITEM_H
#define ITEM_H

#include <stdbool.h>
#include "config.h"
#include "npc.h"


typedef struct
{
    int item_y;
    int item_x;
    int number;
    bool active;
}
item;

typedef enum
{
    HEAL,
    EXTRA_STRENGTH,
    EXTRA_HP,
    EMPTY
}
items;

item item_init(int map_width, int map_height, int number, bool state, npc enemies[], int enemy_amnt);
int items_per_room(int room_width, int room_height);
items random_item(void);

void init_item_screen(void);
void item_screen_draw(items item);

void heal_sprite(void);
void extra_hp_sprite(void);
void extra_strength_sprite(void);

#endif