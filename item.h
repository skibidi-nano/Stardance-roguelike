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
    DAMAGE,
    POISON,
    EXTRA_STRENGTH,
    EXTRA_HP,
    BOSS_ITEM,
    EMPTY
}
items;

typedef enum
{
    ENTER_ESC,
    ENTER_LEGAL,
    ENTER_ILLEGAL
}
pass;

item item_init(int map_width, int map_height, int number, bool state, npc enemies[], int enemy_amnt);
int items_per_room(int room_width, int room_height);
items random_item(void);

void init_item_screen(void);
void item_screen_draw(items item, bool inventory_full);


void heal_potion(void);
void damage_potion(void);

void poison_potion(void);
void poison_call(int turn_counter);
void poison_enabler(bool input);


void heal_sprite(void);
void damage_sprite(void);
void poison_sprite(void);
void extra_hp_sprite(void);
void extra_strength_sprite(void);
void boss_item_sprite(void);

#endif