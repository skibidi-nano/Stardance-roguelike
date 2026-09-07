#ifndef ENTITY_H
#define ENTITY_H

#include "entity.h"

typedef enum 
{
    ADDRESS,
    MODULO,
}
type;

typedef struct
{
    int max_hp;
    int current_hp;
    int attack_power;
    int type;
} 
entity;

void draw_health_bar(int current_hp, int max_hp, int y);

void draw_current_strength(int current_strength, int y);
void draw_current_hp(int current_hp, int y);

#endif